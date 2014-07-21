#include <common.h>
#include <malloc.h>
#include "rk3036_hdmi.h"
#include <asm-generic/errno.h>
#include <asm/arch/rkplat.h>

#include "rk_hdmi.h"

static int rk3036_hdmi_show_reg(struct hdmi_dev *hdmi_dev)
{
	int i = 0;
	u32 val = 0;

	printf("\n>>>rk3036_ctl reg");
	for (i = 0; i < 16; i++)
		printf(" %2x", i); 

	printf("\n-----------------------------------------------------------------");

	for (i = 0; i <= PHY_PRE_DIV_RATIO; i++) {
		hdmi_readl(hdmi_dev, i, &val);
		if (i % 16 == 0)
			printf("\n>>>rk3036_ctl %2x:", i); 
		printf(" %02x", val);
	}   
	printf("\n-----------------------------------------------------------------\n");

	return 0;
}

int hdmi_init_video_para(struct hdmi_dev *hdmi_dev)
{
	struct hdmi_video_para *video = &hdmi_dev->vpara;

	memset(video, 0, sizeof(struct hdmi_video_para));
	video->vic         = hdmi_dev->video.vic;
	video->input_mode  = VIDEO_INPUT_RGB_YCBCR_444;
	video->input_color = VIDEO_INPUT_COLOR_RGB;
	video->output_mode = OUTPUT_HDMI;
	video->format_3d   = HDMI_3D_NONE;	/* TODO modify according to EDID if need */
	//video->pixel_repet = 0;
	/* 0:IT Video Format  1:CE Video Format
	 * TODO modify according to EDID
	 */
	video->color_limit_range = 1;

#ifdef SOURCE_ABOVE_10BIT
	if (hdmi_dev->driver.edid.deepcolor & HDMI_COLOR_DEPTH_16BIT)
		video->color_depth = HDMI_COLOR_DEPTH_16BIT;
	else if (hdmi_dev->driver.edid.deepcolor & HDMI_COLOR_DEPTH_12BIT)
		video->color_depth = HDMI_COLOR_DEPTH_12BIT;
	else
#endif
	if (hdmi_dev->driver.edid.deepcolor & HDMI_COLOR_DEPTH_10BIT)
		video->color_depth = HDMI_COLOR_DEPTH_10BIT;
	else
		video->color_depth = HDMI_COLOR_DEPTH_8BIT;

	if (hdmi_dev->driver.edid.ycbcr444)
		video->output_color = VIDEO_OUTPUT_YCBCR444;
	else if (hdmi_dev->driver.edid.ycbcr422)
		video->output_color = VIDEO_OUTPUT_YCBCR422;
	else
		video->output_color = VIDEO_OUTPUT_RGB444;

	/*For DVI, output RGB */
	if (hdmi_dev->driver.edid.sink_hdmi == 0)
		video->output_color = VIDEO_OUTPUT_RGB444;

	return 0;
}

static inline void delay100us(void)
{
	mdelay(1);
}

static void rk3036_hdmi_av_mute(struct hdmi_dev *hdmi_dev, bool enable)
{
	hdmi_writel(hdmi_dev, AV_MUTE,
		    v_AUDIO_MUTE(enable) | v_VIDEO_MUTE(enable));
}

static void rk3036_hdmi_sys_power(struct hdmi_dev *hdmi_dev, bool enable)
{
	if (enable)
		hdmi_msk_reg(hdmi_dev, SYS_CTRL, m_POWER, v_PWR_ON);
	else
		hdmi_msk_reg(hdmi_dev, SYS_CTRL, m_POWER, v_PWR_OFF);
}

static void rk3036_hdmi_set_pwr_mode(struct hdmi_dev *hdmi_dev, int mode)
{

	if (hdmi_dev->driver.pwr_mode == mode)
		return;

	switch (mode) {
	case NORMAL:
		HDMIDBG("%s change pwr_mode NORMAL pwr_mode = %d, mode = %d\n",
			 __func__, hdmi_dev->driver.pwr_mode, mode);
		rk3036_hdmi_sys_power(hdmi_dev, false);
		hdmi_writel(hdmi_dev, PHY_DRIVER, 0xaa);
		hdmi_writel(hdmi_dev, PHY_PRE_EMPHASIS, 0x6f);
		hdmi_writel(hdmi_dev, PHY_SYS_CTL,0x15);
		hdmi_writel(hdmi_dev, PHY_SYS_CTL,0x14);
		hdmi_writel(hdmi_dev, PHY_SYS_CTL,0x10);
		hdmi_writel(hdmi_dev, PHY_CHG_PWR, 0x0f);
		hdmi_writel(hdmi_dev, 0xce, 0x00);
		hdmi_writel(hdmi_dev, 0xce, 0x01);
		rk3036_hdmi_av_mute(hdmi_dev, 1);
		rk3036_hdmi_sys_power(hdmi_dev, true);
		break;
	case LOWER_PWR:
		HDMIDBG("%s change pwr_mode LOWER_PWR pwr_mode = %d, mode = %d\n",
			 __func__, hdmi_dev->driver.pwr_mode, mode);
		rk3036_hdmi_av_mute(hdmi_dev, 0);
		rk3036_hdmi_sys_power(hdmi_dev, false);
		hdmi_writel(hdmi_dev, PHY_DRIVER, 0x00);
		hdmi_writel(hdmi_dev, PHY_PRE_EMPHASIS, 0x00);
		hdmi_writel(hdmi_dev, PHY_CHG_PWR, 0x00);
		hdmi_writel(hdmi_dev, PHY_SYS_CTL,0x17);
		break;
	default:
		HDMIDBG("unkown rk3036 hdmi pwr mode %d\n",
			 mode);
	}

	hdmi_dev->driver.pwr_mode = mode;
}

static int hdmi_detect_hotplug(struct hdmi_dev *hdmi_dev)
{
	u32 value = 0;

	hdmi_readl(hdmi_dev, HDMI_STATUS, &value);
	HDMIDBG("[%s] value %02x\n", __func__, value);
	value &= m_HOTPLUG;
	HDMIDBG("[%s] value %02x\n", __func__, value);
	if (value == m_HOTPLUG)
		return HDMI_HPD_ACTIVED;
	else if (value)
		return HDMI_HPD_INSERT;
	else
		return HDMI_HPD_REMOVED;
}

int rk3036_hdmi_insert(struct hdmi_dev *hdmi_dev)
{
	rk3036_hdmi_set_pwr_mode(hdmi_dev, NORMAL);
	return 0;
}

static int hdmi_dev_read_edid(struct hdmi_dev *hdmi_dev, int block, u8 *buf)
{
	u32 c = 0;
	u8 segment = 0;
	u8 offset = 0;
	int ret = -1;
	int i, j;
	int ddc_bus_freq;
	int trytime;
	int checksum = 0;

	if (block % 2)
		offset = HDMI_EDID_BLOCK_SIZE;

	if (block / 2)
		segment = 1;

	ddc_bus_freq = (HDMI_SYS_FREG_CLK >> 2) / HDMI_SCL_RATE;
	hdmi_writel(hdmi_dev, DDC_BUS_FREQ_L, ddc_bus_freq & 0xFF);
	hdmi_writel(hdmi_dev, DDC_BUS_FREQ_H, (ddc_bus_freq >> 8) & 0xFF);

	HDMIDBG("EDID DATA (Segment = %d Block = %d Offset = %d):\n",
		 (int)segment, (int)block, (int)offset);

	/* Enable edid interrupt */
	hdmi_writel(hdmi_dev, INTERRUPT_MASK1,
		    m_INT_HOTPLUG | m_INT_EDID_READY);

	for (trytime = 0; trytime < 10; trytime++) {
		hdmi_writel(hdmi_dev, INTERRUPT_STATUS1, 0x04);

		/* Set edid fifo first addr */
		hdmi_writel(hdmi_dev, EDID_FIFO_OFFSET, 0x00);

		/* Set edid word address 0x00/0x80 */
		hdmi_writel(hdmi_dev, EDID_WORD_ADDR, offset);

		/* Set edid segment pointer */
		hdmi_writel(hdmi_dev, EDID_SEGMENT_POINTER, segment);

		for (i = 0; i < 10; i++) {
			/* Wait edid interrupt */
			mdelay(10);
			c = 0x00;
			hdmi_readl(hdmi_dev, INTERRUPT_STATUS1, &c);

			if (c & m_INT_EDID_READY)
				break;
		}

		if (c & m_INT_EDID_READY) {
			for (j = 0; j < HDMI_EDID_BLOCK_SIZE; j++) {
				c = 0;
				hdmi_readl(hdmi_dev, 0x50, &c);
				buf[j] = c;
				checksum += c;
#ifdef HDMIDEBUG
				if (j % 16 == 0)
					printf("\n>>>0x%02x: ", j);
				printf("0x%02x ", c);
#endif
			}

			/* clear EDID interrupt reg */
			hdmi_writel(hdmi_dev, INTERRUPT_STATUS1,
				    m_INT_EDID_READY);

			if ((checksum & 0xff) == 0) {
				ret = 0;
				HDMIDBG("[%s] edid read sucess\n", __func__);
				break;
			}
		}
	}
	//close edid irq
	hdmi_writel(hdmi_dev, INTERRUPT_MASK1, 0);

	return ret;
}

static void rk3036_hdmi_config_avi(struct hdmi_dev *hdmi_dev,
				  unsigned char vic, unsigned char output_color)
{
	int i;
	char info[SIZE_AVI_INFOFRAME];

	memset(info, 0, SIZE_AVI_INFOFRAME);
	hdmi_writel(hdmi_dev, CONTROL_PACKET_BUF_INDEX, INFOFRAME_AVI);
	info[0] = 0x82;
	info[1] = 0x02;
	info[2] = 0x0D;
	info[3] = info[0] + info[1] + info[2];
	info[4] = (AVI_COLOR_MODE_RGB << 5);
	info[5] =
	    (AVI_COLORIMETRY_NO_DATA << 6) | (AVI_CODED_FRAME_ASPECT_NO_DATA <<
					      4) |
	    ACTIVE_ASPECT_RATE_SAME_AS_CODED_FRAME;
	info[6] = 0;
	info[7] = vic;
	if ((vic == HDMI_720X480I_60HZ_VIC) || (vic == HDMI_720X576I_50HZ_VIC))
		info[8] = 1;
	else
		info[8] = 0;

	/* Calculate AVI InfoFrame ChecKsum */
	for (i = 4; i < SIZE_AVI_INFOFRAME; i++)
		info[3] += info[i];

	info[3] = 0x100 - info[3];

	for (i = 0; i < SIZE_AVI_INFOFRAME; i++)
		hdmi_writel(hdmi_dev, CONTROL_PACKET_ADDR + i, info[i]);
}

static int rk3036_hdmi_config_video(struct hdmi_dev *hdmi_dev)
{
	int value;
	struct fb_videomode *mode = NULL;
	struct hdmi_video_timing *timing = NULL;
	struct hdmi_video_para *vpara = &hdmi_dev->vpara;

	/* Output RGB as default */
	vpara->output_color = VIDEO_OUTPUT_RGB444;
	if (hdmi_dev->driver.pwr_mode == LOWER_PWR)
		rk3036_hdmi_set_pwr_mode(hdmi_dev, NORMAL);

	/* Disable video and audio output */
	hdmi_writel(hdmi_dev, AV_MUTE, v_AUDIO_MUTE(1) | v_VIDEO_MUTE(1));

	/* Input video mode is SDR RGB24bit, Data enable signal from external */
	hdmi_writel(hdmi_dev, VIDEO_CONTRL1,
		    v_VIDEO_INPUT_FORMAT(VIDEO_INPUT_SDR_RGB444) |
		    v_DE_EXTERNAL);
	hdmi_writel(hdmi_dev, VIDEO_CONTRL2,
		    v_VIDEO_INPUT_BITS(VIDEO_INPUT_8BITS) |
		    v_VIDEO_OUTPUT_FORMAT(vpara->output_color & 0xFF));

	/* Set HDMI Mode */
	hdmi_writel(hdmi_dev, HDCP_CTRL, v_HDMI_DVI(vpara->output_mode));

	/* Enable or disalbe color space convert */
	if (vpara->input_color != vpara->output_color)
		value = v_SOF_DISABLE | v_CSC_ENABLE;
	else
		value = v_SOF_DISABLE;
	hdmi_writel(hdmi_dev, VIDEO_CONTRL3, value);

	/* Set ext video timing */
#if 1
	hdmi_writel(hdmi_dev, VIDEO_TIMING_CTL, 0);
	//mode = (struct fb_videomode *)hdmi_vic_to_videomode(vpara->vic);
	timing = (struct hdmi_video_timing *)hdmi_vic2timing(hdmi_dev, vpara->vic);
	if(timing == NULL) {
		printf("[%s] not found vic %d\n", __FUNCTION__, vpara->vic);
		return -1;
	}
	mode = &(timing->mode);
	if (mode == NULL) {
		printf("[%s] not found vic %d\n", __func__,
			 vpara->vic);
		return -ENOENT;
	}
	hdmi_dev->tmdsclk = mode->pixclock;
#else
	value = v_EXTERANL_VIDEO(1) | v_INETLACE(mode->vmode);
	if (mode->sync & FB_SYNC_HOR_HIGH_ACT)
		value |= v_HSYNC_POLARITY(1);
	if (mode->sync & FB_SYNC_VERT_HIGH_ACT)
		value |= v_VSYNC_POLARITY(1);
	hdmi_writel(hdmi_dev, VIDEO_TIMING_CTL, value);

	value = mode->left_margin + mode->xres + mode->right_margin +
	    mode->hsync_len;
	hdmi_writel(hdmi_dev, VIDEO_EXT_HTOTAL_L, value & 0xFF);
	hdmi_writel(hdmi_dev, VIDEO_EXT_HTOTAL_H, (value >> 8) & 0xFF);

	value = mode->left_margin + mode->right_margin + mode->hsync_len;
	hdmi_writel(hdmi_dev, VIDEO_EXT_HBLANK_L, value & 0xFF);
	hdmi_writel(hdmi_dev, VIDEO_EXT_HBLANK_H, (value >> 8) & 0xFF);

	value = mode->left_margin + mode->hsync_len;
	hdmi_writel(hdmi_dev, VIDEO_EXT_HDELAY_L, value & 0xFF);
	hdmi_writel(hdmi_dev, VIDEO_EXT_HDELAY_H, (value >> 8) & 0xFF);

	value = mode->hsync_len;
	hdmi_writel(hdmi_dev, VIDEO_EXT_HDURATION_L, value & 0xFF);
	hdmi_writel(hdmi_dev, VIDEO_EXT_HDURATION_H, (value >> 8) & 0xFF);

	value = mode->upper_margin + mode->yres + mode->lower_margin +
	    mode->vsync_len;
	hdmi_writel(hdmi_dev, VIDEO_EXT_VTOTAL_L, value & 0xFF);
	hdmi_writel(hdmi_dev, VIDEO_EXT_VTOTAL_H, (value >> 8) & 0xFF);

	value = mode->upper_margin + mode->vsync_len + mode->lower_margin;
	hdmi_writel(hdmi_dev, VIDEO_EXT_VBLANK, value & 0xFF);

	if (vpara->vic == HDMI_720x480p_60Hz_4_3
	    || vpara->vic == HDMI_720x480p_60Hz_16_9)
		value = 42;
	else
		value = mode->upper_margin + mode->vsync_len;

	hdmi_writel(hdmi_dev, VIDEO_EXT_VDELAY, value & 0xFF);

	value = mode->vsync_len;
	hdmi_writel(hdmi_dev, VIDEO_EXT_VDURATION, value & 0xFF);
#endif

	if (vpara->output_mode == OUTPUT_HDMI) {
		rk3036_hdmi_config_avi(hdmi_dev, vpara->vic,
				      vpara->output_color);
		HDMIDBG("[%s] sucess output HDMI.\n", __func__);
	} else {
		HDMIDBG("[%s] sucess output DVI.\n", __func__);
	}

	/* rk3028a */
	hdmi_writel(hdmi_dev, PHY_PRE_DIV_RATIO, 0x1e);
	hdmi_writel(hdmi_dev, PHY_FEEDBACK_DIV_RATIO_LOW, 0x2c);
	hdmi_writel(hdmi_dev, PHY_FEEDBACK_DIV_RATIO_HIGH, 0x01);
	
	return 0;
}

static void rk3036_hdmi_config_aai(struct hdmi_dev *hdmi_dev)
{
	int i;
	char info[SIZE_AUDIO_INFOFRAME];

	memset(info, 0, SIZE_AUDIO_INFOFRAME);

	info[0] = 0x84;
	info[1] = 0x01;
	info[2] = 0x0A;

	info[3] = info[0] + info[1] + info[2];
	for (i = 4; i < SIZE_AUDIO_INFOFRAME; i++)
		info[3] += info[i];

	info[3] = 0x100 - info[3];

	hdmi_writel(hdmi_dev, CONTROL_PACKET_BUF_INDEX, INFOFRAME_AAI);
	for (i = 0; i < SIZE_AUDIO_INFOFRAME; i++)
		hdmi_writel(hdmi_dev, CONTROL_PACKET_ADDR + i, info[i]);
}

static int rk3036_hdmi_config_audio(struct hdmi_dev *hdmi_dev,
				   struct hdmi_audio *audio)
{
	int rate, N, channel, mclk_fs;

	if (audio->channel < 3)
		channel = I2S_CHANNEL_1_2;
	else if (audio->channel < 5)
		channel = I2S_CHANNEL_3_4;
	else if (audio->channel < 7)
		channel = I2S_CHANNEL_5_6;
	else
		channel = I2S_CHANNEL_7_8;

	switch (audio->rate) {
	case HDMI_AUDIO_FS_32000:
		rate = AUDIO_32K;
		N = N_32K;
		mclk_fs = MCLK_384FS;
		break;
	case HDMI_AUDIO_FS_44100:
		rate = AUDIO_441K;
		N = N_441K;
		mclk_fs = MCLK_256FS;
		break;
	case HDMI_AUDIO_FS_48000:
		rate = AUDIO_48K;
		N = N_48K;
		mclk_fs = MCLK_256FS;
		break;
	case HDMI_AUDIO_FS_88200:
		rate = AUDIO_882K;
		N = N_882K;
		mclk_fs = MCLK_128FS;
		break;
	case HDMI_AUDIO_FS_96000:
		rate = AUDIO_96K;
		N = N_96K;
		mclk_fs = MCLK_128FS;
		break;
	case HDMI_AUDIO_FS_176400:
		rate = AUDIO_1764K;
		N = N_1764K;
		mclk_fs = MCLK_128FS;
		break;
	case HDMI_AUDIO_FS_192000:
		rate = AUDIO_192K;
		N = N_192K;
		mclk_fs = MCLK_128FS;
		break;
	default:
		printf("[%s] not support such sample rate %d\n",
			__func__, audio->rate);
		return -ENOENT;
	}

	/* set_audio source I2S */
	if (HDMI_CODEC_SOURCE_SELECT == INPUT_IIS) {
		hdmi_writel(hdmi_dev, AUDIO_CTRL1, 0x00);
		hdmi_writel(hdmi_dev, AUDIO_SAMPLE_RATE, rate);
		hdmi_writel(hdmi_dev, AUDIO_I2S_MODE,
			    v_I2S_MODE(I2S_STANDARD) | v_I2S_CHANNEL(channel));
		hdmi_writel(hdmi_dev, AUDIO_I2S_MAP, 0x00);
		/* no swap */
		hdmi_writel(hdmi_dev, AUDIO_I2S_SWAPS_SPDIF, 0);
	} else {
		hdmi_writel(hdmi_dev, AUDIO_CTRL1, 0x08);
		/* no swap */
		hdmi_writel(hdmi_dev, AUDIO_I2S_SWAPS_SPDIF, 0);
	}

	/* Set N value */
	hdmi_writel(hdmi_dev, AUDIO_N_H, (N >> 16) & 0x0F);
	hdmi_writel(hdmi_dev, AUDIO_N_M, (N >> 8) & 0xFF);
	hdmi_writel(hdmi_dev, AUDIO_N_L, N & 0xFF);
	rk3036_hdmi_config_aai(hdmi_dev);

	return 0;
}

#if 0
int rk3036_hdmi_removed(struct hdmi *hdmi_drv)
{

	dev_info(hdmi_drv->dev, "Removed.\n");
	if (hdmi_drv->hdcp_power_off_cb)
		hdmi_drv->hdcp_power_off_cb();
	rk3036_hdmi_set_pwr_mode(hdmi_drv, LOWER_PWR);

	return HDMI_ERROR_SUCESS;
}

void rk3036_hdmi_work(struct hdmi *hdmi_drv)
{
	u32 interrupt = 0;
	struct hdmi_dev *hdmi_dev = /*container_of(hdmi_drv,
						       struct hdmi_dev,
						       driver)*/hdmi;

#ifndef SOC_CONFIG_RK3036
        hdmi_readl(hdmi_dev, INTERRUPT_STATUS1,&interrupt);
        if(interrupt){
                hdmi_writel(hdmi_dev, INTERRUPT_STATUS1, interrupt);
        }
#else
	hdmi_readl(hdmi_dev, HDMI_STATUS,&interrupt);
	if(interrupt){
		hdmi_writel(hdmi_dev, HDMI_STATUS, interrupt);
	}
#endif
	if (interrupt & m_HOTPLUG) {
		if (hdmi_drv->state == HDMI_SLEEP)
			hdmi_drv->state = WAIT_HOTPLUG;

		queue_delayed_work(hdmi_drv->workqueue, &hdmi_drv->delay_work,
				   msecs_to_jiffies(40));

	}

	if (hdmi_drv->hdcp_irq_cb)
		hdmi_drv->hdcp_irq_cb(0);
}
#endif

void rk3036_hdmi_control_output(struct hdmi_dev *hdmi_dev, int enable)
{
	u32 mutestatus = 0;

	if (enable) {
		if (hdmi_dev->driver.pwr_mode == LOWER_PWR)
			rk3036_hdmi_set_pwr_mode(hdmi_dev, NORMAL);
		hdmi_readl(hdmi_dev, AV_MUTE, &mutestatus);
		if (mutestatus && (m_AUDIO_MUTE | m_VIDEO_BLACK)) {
			hdmi_writel(hdmi_dev, AV_MUTE,
				    v_AUDIO_MUTE(0) | v_VIDEO_MUTE(0));
		}
		rk3036_hdmi_sys_power(hdmi_dev, true);
		rk3036_hdmi_sys_power(hdmi_dev, false);
		rk3036_hdmi_sys_power(hdmi_dev, true);
		hdmi_writel(hdmi_dev, 0xce, 0x00);
		delay100us();
		hdmi_writel(hdmi_dev, 0xce, 0x01);
	} else {
		hdmi_writel(hdmi_dev, AV_MUTE,
			    v_AUDIO_MUTE(1) | v_VIDEO_MUTE(1));
	}
}

static void rk3036_hdmi_reset(struct hdmi_dev *hdmi_dev)
{
	u32 val = 0;
	u32 msk = 0;

	hdmi_msk_reg(hdmi_dev, SYS_CTRL, m_RST_DIGITAL, v_NOT_RST_DIGITAL);
	delay100us();
	hdmi_msk_reg(hdmi_dev, SYS_CTRL, m_RST_ANALOG, v_NOT_RST_ANALOG);
	delay100us();
	msk = m_REG_CLK_INV | m_REG_CLK_SOURCE | m_POWER | m_INT_POL;
	val = v_REG_CLK_INV | v_REG_CLK_SOURCE_SYS | v_PWR_ON | v_INT_POL_HIGH;
	hdmi_msk_reg(hdmi_dev, SYS_CTRL, msk, val);

	//rk3036_hdmi_set_pwr_mode(hdmi_dev, LOWER_PWR);
	rk3036_hdmi_set_pwr_mode(hdmi_dev, NORMAL);
}

static int rk3036_hdmi_hardware_init(struct hdmi_dev *hdmi_dev)
{
	int i = 5, ret = -1, val = 0;

	if (!hdmi_dev)
		return ret;


    rk3036_hdmi_reset_pclk();
    rk3036_hdmi_reset(hdmi_dev);
	mdelay(400);

	hdmi_dev->read_edid = hdmi_dev_read_edid;
	hdmi_dev->driver.pwr_mode = NORMAL;

	if (hdmi_detect_hotplug(hdmi_dev) == HDMI_HPD_ACTIVED) {
		hdmi_parse_edid(hdmi_dev);
		hdmi_find_best_mode(hdmi_dev);
		hdmi_init_video_para(hdmi_dev);
		rk3036_hdmi_config_video(hdmi_dev);
		rk3036_hdmi_config_audio(hdmi_dev, &hdmi_dev->driver.audio);
		rk3036_hdmi_control_output(hdmi_dev, 1);

		ret = 0;
	}else {
		printf("Hdmi Devices Not Exist.\n");
	}

	return ret;
}

void rk3036_hdmi_probe(vidinfo_t *panel)
{
	int val = 0;
	struct hdmi_dev *hdmi_dev = NULL;

	//iomux scl/ada/hpd/cec
	val = (0x55 | (0x55 << 16));
	grf_writel(val, GRF_GPIO1B_IOMUX); 

	hdmi_dev = malloc(sizeof(struct hdmi_dev));
	if (hdmi_dev != NULL && panel != NULL) {
		memset(hdmi_dev, 0, sizeof(struct hdmi_dev));
		hdmi_dev->regbase = (void *)RKIO_HDMI_PHYS;
		hdmi_dev->hd_init = rk3036_hdmi_hardware_init;
		hdmi_dev->read_edid = hdmi_dev_read_edid;

        hdmi_dev->driver.vic = HDMI_VIDEO_DEFAULT_MODE;
		//audio
        hdmi_dev->driver.audio.channel = HDMI_AUDIO_DEFAULT_CHANNEL;
        hdmi_dev->driver.audio.rate = HDMI_AUDIO_DEFAULT_RATE;
        hdmi_dev->driver.audio.word_length = HDMI_AUDIO_DEFAULT_WORD_LENGTH;

		rk_hdmi_register(hdmi_dev, panel);
	}else {
		printf("%s: hdmi_dev %#x  panel %#x\n", __func__, hdmi_dev, panel);
	}

	rk3036_hdmi_show_reg(hdmi_dev);
	free(hdmi_dev);
}
