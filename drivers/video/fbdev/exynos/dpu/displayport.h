/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Header file for Samsung EXYNOS SoC DisplayPort driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef _DISPLAYPORT_H_
#define _DISPLAYPORT_H_

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <media/v4l2-subdev.h>
//#include <linux/switch.h>
//#include <linux/wakelock.h>
#if defined(CONFIG_CCIC_NOTIFIER)
#include <linux/usb/manager/usb_typec_manager_notifier.h>
#include <linux/notifier.h>
#include <linux/ccic/ccic_notifier.h>
#endif

#include "regs-displayport.h"
#include "./panels/decon_lcd.h"

extern int displayport_log_level;

#define DISPLAYPORT_MODULE_NAME "exynos-displayport"

#define displayport_err(fmt, ...)						\
	do {									\
		if (displayport_log_level >= 3) {				\
			pr_err("Displayport: " pr_fmt(fmt), ##__VA_ARGS__);			\
		}								\
	} while (0)

#define displayport_warn(fmt, ...)						\
	do {									\
		if (displayport_log_level >= 4) {				\
			pr_warn("Displayport: " pr_fmt(fmt), ##__VA_ARGS__);			\
		}								\
	} while (0)

#define displayport_info(fmt, ...)						\
	do {									\
		if (displayport_log_level >= 6)					\
			pr_info("Displayport: " pr_fmt(fmt), ##__VA_ARGS__);			\
	} while (0)

#define displayport_dbg(fmt, ...)						\
	do {									\
		if (displayport_log_level >= 7)					\
			pr_info("Displayport: " pr_fmt(fmt), ##__VA_ARGS__);			\
	} while (0)

extern struct displayport_device *displayport_drvdata;

enum displayport_state {
	DISPLAYPORT_STATE_INIT,
	DISPLAYPORT_STATE_ON,
	DISPLAYPORT_STATE_OFF
};

enum displayport_dynamic_range_type {
	VESA_RANGE = 0,   /* (0 ~ 255) */
	CEA_RANGE = 1,    /* (16 ~ 235) */
};

struct displayport_resources {
	int aux_ch_mux_gpio;
	int irq;
	void __iomem *regs;
};

enum displayport_aux_ch_command_type {
	I2C_WRITE = 0x4,
	I2C_READ = 0x5,
	DPCD_WRITE = 0x8,
	DPCD_READ = 0x9,
};

typedef enum {
	NORAMAL_DATA = 0,
	TRAINING_PATTERN_1 = 1,
	TRAINING_PATTERN_2 = 2,
	TRAINING_PATTERN_3 = 3,
} displayport_training_pattern;

typedef enum {
	DISABLE_PATTEN = 0,
	D10_2_PATTERN = 1,
	SERP_PATTERN = 2,
	PRBS7 = 3,
	CUSTOM_80BIT = 4,
	HBR2_COMPLIANCE = 5,
} displayport_qual_pattern;

typedef enum {
	ENABLE_SCRAM = 0,
	DISABLE_SCRAM = 1,
}displayport_scrambling;

enum displayport_interrupt_mask {
	VSYNC_DET_INT_MASK,
	PLL_LOCK_CHG_INT_MASK,
	VID_FORMAT_CHG_INT_MASK,
	VID_CLK_CHG_INT_MASK,
	HOTPLUG_CHG_INT_MASK,
	HPD_LOST_INT_MASK,
	PLUG_INT_MASK,
	INT_HPD_INT_MASK,
	RPLY_RECEIV_INT_MASK,
	AUX_ERR_INT_MASK,
	HDCP_LINK_CHECK_INT_MASK,
	HDCP_LINK_FAIL_INT_MASK,
	HW_HDCP_DONE_INT_MASK,
	HW_AUTH_CHG_INT_MASK,
	HDCP_R0_READY_INT_MASK,
/*
	AUDIO_FIFO_UNDER_RUN_INT_MASK,
	AUDIO_FIFO_OVER_RUN_INT_MASK,
*/
	ALL_INT_MASK
};

#define MAX_LANE_CNT 4
#define DPCD_BUF_SIZE 12

#define FB_AUDIO_LPCM	1

#define FB_AUDIO_192KHZ	(1 << 6)
#define FB_AUDIO_176KHZ	(1 << 5)
#define FB_AUDIO_96KHZ	(1 << 4)
#define FB_AUDIO_88KHZ	(1 << 3)
#define FB_AUDIO_48KHZ	(1 << 2)
#define FB_AUDIO_44KHZ	(1 << 1)
#define FB_AUDIO_32KHZ	(1 << 0)

#define FB_AUDIO_24BIT	(1 << 2)
#define FB_AUDIO_20BIT	(1 << 1)
#define FB_AUDIO_16BIT	(1 << 0)

struct fb_audio {
	u8 format;
	u8 channel_count;
	u8 sample_rates;
	u8 bit_rates;
	u8 speaker;
};

struct fb_vendor {
	u8 vic_len;
	u8 vic_data[16];
};

#define MAX_REACHED_CNT 3
#define MAX_PRE_EMPHASIS_REACHED_CNT 2
#define MAX_SWING_REACHED_BIT_POS 2
#define MAX_PRE_EMPHASIS_REACHED_BIT_POS 5
#define DPCP_LINK_SINK_STATUS_FIELD_LENGTH 8

#define DPCD_ADD_REVISION_NUMBER 0x00000
#define DPCD_ADD_MAX_LINK_RATE 0x00001
#define LINK_RATE_1_62Gbps 0x06
#define LINK_RATE_2_7Gbps 0x0A
#define LINK_RATE_5_4Gbps 0x14

#define DPCD_ADD_MAX_LANE_COUNT 0x00002
#define MAX_LANE_COUNT (0x1F << 0)
#define TPS3_SUPPORTED (1 << 6)
#define ENHANCED_FRAME_CAP (1 << 7)

#define DPCD_ADD_MAX_DOWNSPREAD 0x00003
#define NO_AUX_HANDSHAKE_LINK_TRANING (1 << 6)

#define DPCD_ADD_DOWN_STREAM_PORT_PRESENT 0x00005
#define BIT_DFP_TYPE 0x6
#define DFP_TYPE_DP 0x00
#define DFP_TYPE_VGA 0x01 /* analog video */
#define DFP_TYPE_HDMI 0x2
#define DFP_TYPE_OTHERS 0x3 /* not have EDID like composite and Svideo port */

#define DPCD_ADD_I2C_SPEED_CONTROL_CAPABILITES 0x0000C
#define I2C_1Kbps 0x01
#define I2C_5Kbps 0x02
#define I2C_10Kbps 0x04
#define I2C_100Kbps 0x08
#define I2C_400Kbps 0x10
#define I2C_1Mbps 0x20

#define DPCD_ADD_TRAINING_AUX_RD_INTERVAL 0x0000E
#define TRANING_AUX_RD_INTERVAL_400us 0x00
#define TRANING_AUX_RD_INTERVAL_4ms 0x01
#define TRANING_AUX_RD_INTERVAL_8ms 0x02
#define TRANING_AUX_RD_INTERVAL_12ms 0x03
#define TRANING_AUX_RD_INTERVAL_16ms 0x04

#define DPCD_ADD_LINK_BW_SET 0x00100

#define DPCD_ADD_LANE_COUNT_SET 0x00101

#define DPCD_ADD_TRANING_PATTERN_SET 0x00102
#define TRAINING_PTTERN_SELECT (3 << 0)
#define RECOVERED_CLOCK_OUT_EN (1 << 4)
#define DPCD_SCRAMBLING_DISABLE (1 << 5)
#define SYMBOL_ERROR_COUNT_SEL (3 << 6)

#define DPCD_ADD_TRANING_LANE0_SET 0x00103
#define DPCD_ADD_TRANING_LANE1_SET 0x00104
#define DPCD_ADD_TRANING_LANE2_SET 0x00105
#define DPCD_ADD_TRANING_LANE3_SET 0x00106
#define VOLTAGE_SWING_SET (3 << 0)
#define MAX_SWING_REACHED (1 << 2)
#define PRE_EMPHASIS_SWING_SET (3 << 3)
#define MAX_PRE_EMPHASIS_REACHED (1 << 5)

#define DPCD_ADD_I2C_SPEED_CONTROL_STATUS 0x00109

#define DPCD_ADD_LINK_QUAL_LANE0_SET 0x0010B
#define DPCD_ADD_LINK_QUAL_LANE1_SET 0x0010C
#define DPCD_ADD_LINK_QUAL_LANE2_SET 0x0010D
#define DPCD_ADD_LINK_QUAL_LANE3_SET 0x0010E
#define DPCD_LINK_QUAL_PATTERN_SET (7 << 0)

#define DPCD_ADD_SINK_COUNT 0x00200
#define SINK_COUNT2 (1 << 7)
#define CP_READY (1 << 6)
#define SINK_COUNT1 (0x3F << 0)

#define DPCD_ADD_DEVICE_SERVICE_IRQ_VECTOR 0x00201
#define AUTOMATED_TEST_REQUEST (1 << 1)
#define CP_IRQ (1 << 2)
#define MCCS_IRQ (1 << 3)
#define DOWN_REP_MSG_RDY (1 << 4)
#define UP_REQ_MSG_RDY (1 << 5)
#define SINK_SPECIFIC_IRQ (1 << 6)

#define DPCD_ADD_LANE0_1_STATUS 0x00202
#define LANE0_CR_DONE (1 << 0)
#define LANE0_CHANNEL_EQ_DONE (1 << 1)
#define LANE0_SYMBOL_LOCKED (1 << 2)
#define LANE1_CR_DONE (1 << 4)
#define LANE1_CHANNEL_EQ_DONE (1 << 5)
#define LANE1_SYMBOL_LOCKED (1 << 6)

#define DPCD_ADD_LANE2_3_STATUS 0x00203
#define LANE2_CR_DONE (1 << 0)
#define LANE2_CHANNEL_EQ_DONE (1 << 1)
#define LANE2_SYMBOL_LOCKED (1 << 2)
#define LANE3_CR_DONE (1 << 4)
#define LANE3_CHANNEL_EQ_DONE (1 << 5)
#define LANE3_SYMBOL_LOCKED (1 << 6)

#define DPCD_ADD_LANE_ALIGN_STATUS_UPDATE 0x00204
#define INTERLANE_ALIGN_DONE (1 << 0)
#define DOWNSTREAM_PORT_STATUS_CHANGED (1 << 6)
#define LINK_STATUS_UPDATE (1 << 7)

#define DPCD_ADD_SINK_STATUS 0x00205
#define RECEIVE_PORT_0_STATUS (1 << 0)
#define RECEIVE_PORT_1_STATUS (1 << 1)

#define DPCD_ADD_ADJUST_REQUEST_LANE0_1 0x00206
#define VOLTAGE_SWING_LANE0 (3 << 0)
#define PRE_EMPHASIS_LANE0 (3 << 2)
#define VOLTAGE_SWING_LANE1 (3 << 4)
#define PRE_EMPHASIS_LANE1 (3 << 6)

#define DPCD_ADD_ADJUST_REQUEST_LANE2_3 0x00207
#define VOLTAGE_SWING_LANE2 (3 << 0)
#define PRE_EMPHASIS_LANE2 (3 << 2)
#define VOLTAGE_SWING_LANE3 (3 << 4)
#define PRE_EMPHASIS_LANE3 (3 << 6)

#define DPCD_TEST_REQUEST 0x00218
#define TEST_LINK_TRAINING (1 << 0)
#define TEST_VIDEO_PATTERN (1 << 1)
#define TEST_EDID_READ (1 << 2)
#define TEST_PHY_TEST_PATTERN (1 << 3)
#define TEST_FAUX_TEST_PATTERN (1<<4)
#define TEST_AUDIO_PATTERN (1<<5)
#define TEST_AUDIO_DISABLED_VIDEO (1<<6)

#define DPCD_TEST_LINK_RATE 0x00219
#define TEST_LINK_RATE (0xFF << 0)

#define DPCD_TEST_LANE_COUNT 0x00220
#define TEST_LANE_COUNT (0x1F << 0)

#define DPCD_TEST_PATTERN 0x00221

#define DPCD_TEST_H_TOTAL_1 0x00222	//[15:8]
#define DPCD_TEST_H_TOTAL_2 0x00223	//[7:0]
#define DPCD_TEST_V_TOTAL_1 0x00224	//[15:8]
#define DPCD_TEST_V_TOTAL_2 0x00225	//[7:0]

#define DPCD_TEST_H_START_1 0x00226	//[15:8]
#define DPCD_TEST_H_START_2 0x00227	//[7:0]
#define DPCD_TEST_V_START_1 0x00228	//[15:8]
#define DPCD_TEST_V_START_2 0x00229	//[7:0]

#define DPCD_TEST_H_SYNC_1 0x0022A	//[15:8]
#define DPCD_TEST_H_SYNC_2 0x0022B	//[7:0]
#define DPCD_TEST_V_SYNC_1 0x0022C	//[15:8]
#define DPCD_TEST_V_SYNC_2 0x0022D	//[7:0]

#define DPCD_TEST_H_WIDTH_1 0x0022E	//[15:8]
#define DPCD_TEST_H_WIDTH_2 0x0022F	//[7:0]
#define DPCD_TEST_V_HEIGHT_1 0x00230	//[15:8]
#define DPCD_TEST_V_HEIGHT_2 0x00231	//[7:0]

#define DPCD_TEST_MISC_1 0x00232
#define TEST_SYNCHRONOUS_CLOCK (1 << 0)
#define TEST_COLOR_FORMAT (3 << 1)
#define TEST_DYNAMIC_RANGE (1 << 3)
#define TEST_YCBCR_COEFFICIENTS (1 << 4)
#define TEST_BIT_DEPTH (7 << 5)

#define DPCD_TEST_MISC_2 0x00233
#define TEST_REFRESH_DENOMINATOR (1 << 0)
#define TEST_INTERLACED (1 << 1)

#define DPCD_TEST_REFRESH_RATE_NUMERATOR 0x00234

#define DCDP_ADD_PHY_TEST_PATTERN 0x00248
#define PHY_TEST_PATTERN_SEL (3 << 0)

#define DPCD_TEST_RESPONSE 0x00260
#define TEST_ACK (1 << 0)
#define TEST_NAK (1 << 1)
#define TEST_EDID_CHECKSUM_WRITE (1 << 2)

#define DPCD_TEST_EDID_CHECKSUM 0x00261

#define DPCD_TEST_AUDIO_MODE 0x00271
#define TEST_AUDIO_SAMPLING_RATE (0x0F << 0)
#define TEST_AUDIO_CHANNEL_COUNT (0xF0 << 0)

#define DPCD_TEST_AUDIO_PATTERN_TYPE 0x00272

#define DPCD_BRANCH_HW_REVISION	0x509
#define DPCD_BRANCH_SW_REVISION_MAJOR	0x50A
#define DPCD_BRANCH_SW_REVISION_MINOR	0x50B

#define DPCD_ADD_SET_POWER 0x00600
#define SET_POWER_STATE (3 << 0)
#define SET_POWER_DOWN 0x02
#define SET_POWER_NORMAL 0x01

#define DPCD_HDCP22_RX_CAPS 0x6921D
#define VERSION (0xFF << 16)
#define HDCP_CAPABLE (1 << 1)

#define DPCD_HDCP22_RX_INFO 0x69330

#define DPCD_HDCP22_RX_CAPS_LENGTH 3
#define DPCD_HDCP_VERSION_BIT_POSITION 16

#define DPCD_HDCP22_RXSTATUS_READY			(0x1 << 0)
#define DPCD_HDCP22_RXSTATUS_HPRIME_AVAILABLE		(0x1 << 1)
#define DPCD_HDCP22_RXSTATUS_PAIRING_AVAILABLE		(0x1 << 2)
#define DPCD_HDCP22_RXSTATUS_REAUTH_REQ			(0x1 << 3)
#define DPCD_HDCP22_RXSTATUS_LINK_INTEGRITY_FAIL	(0x1 << 4)

#define HDCP_VERSION_1_3 0x13
#define HDCP_VERSION_2_2 0x02

#define SYNC_POSITIVE 0
#define SYNC_NEGATIVE 1

typedef enum {
	PIXEL_CLOCK_25_200,
	PIXEL_CLOCK_27_000,
	PIXEL_CLOCK_27_027,
	PIXEL_CLOCK_33_750,
	PIXEL_CLOCK_71_000,
	PIXEL_CLOCK_74_250,
	PIXEL_CLOCK_108_000,
	PIXEL_CLOCK_148_500,
	PIXEL_CLOCK_234_000,
	PIXEL_CLOCK_241_500,
	PIXEL_CLOCK_297_000,
	PIXEL_CLOCK_312_000,
	PIXEL_CLOCK_533_000,
	PIXEL_CLOCK_594_000,
} pixelclock;

typedef struct {
	u32 p;
	u32 m;
	u32 s;
	u32 divide;
} pms_info;

typedef enum {
	v640x480p_60Hz,
	v720x480p_60Hz,
	v720x576p_50Hz,
	v1280x720p_50Hz,
	v1280x720p_60Hz,
	v1280x800p_RB_60Hz,
	v1280x1024p_60Hz,
	v1920x1080p_24Hz,
	v1920x1080p_25Hz,
	v1920x1080p_30Hz,
	v1920x1080p_50Hz,
	v1920x1080p_60Hz,
	v1920x1440p_60Hz,
	v2560x1440p_59Hz,
	v2560x1440p_60Hz,
	v3840x2160p_24Hz,
	v3840x2160p_25Hz,
	v3840x2160p_30Hz,
	v3840x2160p_50Hz,
	v3840x2160p_RB_59Hz,
	v3840x2160p_60Hz,
	v4096x2160p_24Hz,
	v4096x2160p_25Hz,
	v4096x2160p_30Hz,
	v4096x2160p_50Hz,
	v4096x2160p_60Hz,
	sa_crc_640x10_60Hz,
} videoformat;

typedef struct {
	videoformat video_format;
	u32 total_pixel;
	u32 active_pixel;
	u32 v_f_porch;
	u32 v_sync;
	u32 v_b_porch;
	u32 total_line;
	u32 active_line;
	u32 h_f_porch;
	u32 h_sync;
	u32 h_b_porch;
	u32 fps;
	pixelclock pixel_clock;
	u8 vic;
	u8 v_sync_pol;
	u8 h_sync_pol;
} videoformat_info;

typedef enum{
	ASYNC_MODE = 0,
	SYNC_MODE,
} audio_sync_mode;

enum audio_sampling_frequency {
	FS_32KHZ	= 0,
	FS_44KHZ	= 1,
	FS_48KHZ	= 2,
	FS_88KHZ	= 3,
	FS_96KHZ	= 4,
	FS_176KHZ	= 5,
	FS_192KHZ	= 6,
};

enum audio_bit_per_channel {
	AUDIO_16_BIT = 0,
	AUDIO_20_BIT,
	AUDIO_24_BIT,
};

enum audio_16bit_dma_mode {
	NORMAL_MODE = 0,
	PACKED_MODE = 1,
	PACKED_MODE2 = 2,
};

enum audio_dma_word_length {
	WORD_LENGTH_1 = 0,
	WORD_LENGTH_2,
	WORD_LENGTH_3,
	WORD_LENGTH_4,
	WORD_LENGTH_5,
	WORD_LENGTH_6,
	WORD_LENGTH_7,
	WORD_LENGTH_8,
};

enum audio_clock_accuracy {
	Level2 = 0,
	Level1 = 1,
	Level3 = 2,
	NOT_MATCH = 3,
};

enum bit_depth{
	BPC_6 = 0,
	BPC_8,
	BPC_10,
	BPC_12,
	BPC_16,
};

enum test_pattern{
	COLOR_BAR = 0,
	WGB_BAR,
	MW_BAR,
};

enum hotplug_state{
	HPD_UNPLUG = 0,
	HPD_PLUG,
	HPD_IRQ,
};

struct displayport_device {
	enum displayport_state state;
	struct device *dev;
	struct displayport_resources res;

	unsigned int data_lane;
	u32 data_lane_cnt;
	struct phy *phy;
	spinlock_t slock;

	struct dsim_lcd_driver *panel_ops;
	struct decon_lcd lcd_info;

	struct v4l2_subdev sd;
	struct v4l2_dv_timings cur_timings;

	struct workqueue_struct *dp_wq;
	struct workqueue_struct *hdcp2_wq;
	struct delayed_work hpd_plug_work;
	struct delayed_work hpd_unplug_work;
	struct delayed_work hpd_irq_work;
	//struct switch_dev hpd_switch;
	//struct switch_dev audio_switch;

	struct delayed_work hdcp13_work;
	struct delayed_work hdcp22_work;
	struct delayed_work hdcp13_integrity_check_work;
	int hdcp_ver;

	struct mutex cmd_lock;
	struct mutex hpd_lock;
	struct mutex aux_lock;
	struct mutex training_lock;
	wait_queue_head_t dp_wait;
#if defined(CONFIG_CCIC_NOTIFIER)
	struct notifier_block dp_typec_nb;
	ccic_notifier_dp_pinconf_t ccic_notify_dp_conf;
#endif
	int hpd_current_state;
	enum hotplug_state hpd_state;
	//struct wake_lock dp_wake_lock;
	int dp_sw_sel;
	int gpio_sw_oe;
	int gpio_sw_sel;
	int gpio_usb_dir;
	int dfp_type;
	const char *aux_vdd;

	int auto_test_mode;
	enum bit_depth bpc;
	u8 bist_used;
	enum test_pattern bist_type;
	enum displayport_dynamic_range_type dyn_range;
};

struct displayport_debug_param {
	u8 param_used;
	u8 link_rate;
	u8 lane_cnt;
};

extern videoformat_info videoformat_parameters[];
extern pms_info pms_parameters[];
extern videoformat g_displayport_videoformat;

/* EDID functions */
/* default preset configured on probe */
#define EDID_DEFAULT_TIMINGS_IDX (0) /* 640x480@60Hz */

#define EDID_ADDRESS 0x50
#define AUX_DATA_BUF_COUNT 16
#define EDID_BUF_COUNT 256
#define AUX_RETRY_COUNT 3
#define AUX_TIMEOUT_1800us 0x03

#define DATA_BLOCK_TAG_CODE_MASK 0xE0
#define DATA_BLOCK_LENGTH_MASK 0x1F
#define DATA_BLOCK_TAG_CODE_BIT_POSITION 5

#define VSDB_TAG_CODE 3
#define VSDB_LATENCY_FILEDS_PRESETNT_MASK 0x80
#define VSDB_I_LATENCY_FILEDS_PRESETNT_MASK 0x40
#define VSDB_HDMI_VIDEO_PRESETNT_MASK 0x20
#define VSDB_VIC_FIELD_OFFSET 14
#define VSDB_VIC_LENGTH_MASK 0xE0
#define VSDB_VIC_LENGTH_BIT_POSITION 5
#define IEEE_REGISTRATION_IDENTIFIER_0 0x03
#define IEEE_REGISTRATION_IDENTIFIER_1 0x0C
#define IEEE_REGISTRATION_IDENTIFIER_2 0x00

#define DETAILED_TIMING_DESCRIPTION_SIZE 18
#define AUDIO_DATA_BLOCK 1
#define VIDEO_DATA_BLOCK 2
#define SPEAKER_DATA_BLOCK 4
#define SVD_VIC_MASK 0x7F

struct displayport_supported_preset {
	struct v4l2_dv_timings dv_timings;
	u16 xres;
	u16 yres;
	u16 refresh;
	u32 vmode;
	u8 vic;
	char *name;
	bool edid_support_match;
};

#define V4L2_DV_BT_CVT_3840X2160P59_ADDED { \
	.type = V4L2_DV_BT_656_1120, \
	V4L2_INIT_BT_TIMINGS(3840, 2160, 0, V4L2_DV_HSYNC_POS_POL, \
		533250000, 48, 32, 80, 3, 5, 54, 0, 0, 0, \
		V4L2_DV_BT_STD_DMT | V4L2_DV_BT_STD_CVT, \
		V4L2_DV_FL_REDUCED_BLANKING) \
}

#define V4L2_DV_BT_CVT_2560X1440P60_ADDED { \
	.type = V4L2_DV_BT_656_1120, \
	V4L2_INIT_BT_TIMINGS(2560, 1440, 0, V4L2_DV_HSYNC_POS_POL, \
		312250000, 192, 272, 464, 3, 5, 45, 0, 0, 0, \
		V4L2_DV_BT_STD_DMT | V4L2_DV_BT_STD_CVT, 0) \
}

#define V4L2_DV_BT_CVT_2560X1440P59_ADDED { \
	.type = V4L2_DV_BT_656_1120, \
	V4L2_INIT_BT_TIMINGS(2560, 1440, 0, V4L2_DV_HSYNC_POS_POL, \
		241500000, 48, 32, 80, 3, 5, 33, 0, 0, 0, \
		V4L2_DV_BT_STD_DMT | V4L2_DV_BT_STD_CVT, 0) \
}

extern const int displayport_pre_cnt;
extern struct displayport_supported_preset displayport_supported_presets[];
extern const int videoformat_parameters_cnt;

struct exynos_displayport_data {
	enum {
		EXYNOS_DISPLAYPORT_STATE_PRESET = 0,
		EXYNOS_DISPLAYPORT_STATE_ENUM_PRESET,
		EXYNOS_DISPLAYPORT_STATE_RECONNECTION,
		EXYNOS_DISPLAYPORT_STATE_HDCP,
		EXYNOS_DISPLAYPORT_STATE_AUDIO,
	} state;
	struct	v4l2_dv_timings timings;
	struct	v4l2_enum_dv_timings etimings;
	__u32	audio_info;
	int	hdcp;
};

struct displayport_audio_config_data {
	u32 audio_enable;
	u32 audio_channel_cnt;
	enum audio_sampling_frequency audio_fs;
	enum audio_bit_per_channel audio_bit;
	enum audio_16bit_dma_mode audio_packed_mode;
	enum audio_dma_word_length audio_word_length;
};

/* InfoFrame */
#define	INFOFRAME_PACKET_TYPE_AVI 0x82		/** Auxiliary Video information InfoFrame */
#define	INFOFRAME_PACKET_TYPE_AUDIO 0x84	/** Audio information InfoFrame */
#define MAX_INFOFRAME_LENGTH 27

#define AVI_INFOFRAME_VERSION 0x02
#define AVI_INFOFRAME_LENGTH 0x0D
#define ACTIVE_FORMAT_INFOMATION_PRESENT (1 << 4)	/* No Active Format Infomation */
#define ACITVE_PORTION_ASPECT_RATIO (0x8 << 0)		/* Same as Picture Aspect Ratio */

#define AUDIO_INFOFRAME_VERSION 0x01
#define AUDIO_INFOFRAME_LENGTH 0x0A
#define AUDIO_INFOFRAME_PCM (1 << 4)
#define AUDIO_INFOFRAME_SF_BIT_POSITION 2

struct infoframe {
	u8 type_code;
	u8 version_number;
	u8 length;
	u8 data[MAX_INFOFRAME_LENGTH];
};

typedef struct{
	u8 HDCP13_BKSV[5];
	u8 HDCP13_R0[2];
	u8 HDCP13_AKSV[5];
	u8 HDCP13_AN[8];
	u8 HDCP13_V_H0[4];
	u8 HDCP13_V_H1[4];
	u8 HDCP13_V_H2[4];
	u8 HDCP13_V_H3[4];
	u8 HDCP13_V_H4[4];
	u8 HDCP13_BCAP[1];
	u8 HDCP13_BSTATUS[1];
	u8 HDCP13_BINFO[2];
	u8 HDCP13_KSV_FIFO[15];
	u8 HDCP13_AINFO[1];
} HDCP13;

enum HDCP13_STATE {
	HDCP13_STATE_NOT_AUTHENTICATED,
	HDCP13_STATE_AUTH_PROCESS,
	HDCP13_STATE_SECOND_AUTH_DONE,
	HDCP13_STATE_AUTHENTICATED,
	HDCP13_STATE_FAIL
};

struct hdcp13_info {
	u8 cp_irq_flag;
	u8 is_repeater;
	u8 device_cnt;
	u8 revocation_check;
	u8 r0_read_flag;
	int link_check;
	enum HDCP13_STATE auth_state;
};

/* HDCP 1.3 */
extern HDCP13 HDCP13_DPCD;
extern struct hdcp13_info hdcp13_info;

#define ADDR_HDCP13_BKSV 0x68000
#define ADDR_HDCP13_R0 0x68005
#define ADDR_HDCP13_AKSV 0x68007
#define ADDR_HDCP13_AN 0x6800C
#define ADDR_HDCP13_V_H0 0x68014
#define ADDR_HDCP13_V_H1 0x68018
#define ADDR_HDCP13_V_H2 0x6801C
#define ADDR_HDCP13_V_H3 0x68020
#define ADDR_HDCP13_V_H4 0x68024
#define ADDR_HDCP13_BCAP 0x68028
#define ADDR_HDCP13_BSTATUS 0x68029
#define ADDR_HDCP13_BINFO 0x6802A
#define ADDR_HDCP13_KSV_FIFO 0x6802C
#define ADDR_HDCP13_AINFO 0x6803B
#define ADDR_HDCP13_RSVD 0x6803C
#define ADDR_HDCP13_DBG 0x680C0

#define BCAPS_RESERVED_BIT_MASK 0xFC
#define BCAPS_REPEATER (1 << 1)
#define BCAPS_HDCP_CAPABLE (1 << 0)

#define BSTATUS_READY (1<<0)
#define BSTATUS_R0_AVAILABLE (1<<1)
#define BSTATUS_LINK_INTEGRITY_FAIL (1<<2)
#define BSTATUS_REAUTH_REQ (1<<3)

#define BINFO_DEVICE_COUNT (0x7F<<0)
#define BINFO_MAX_DEVS_EXCEEDED (1<<7)
#define BINFO_DEPTH (7<<8)
#define BINFO_MAX_CASCADE_EXCEEDED (1<<11)

#define RI_READ_RETRY_CNT 3
#define RI_AVAILABLE_WAITING 2
#define RI_DELAY 100
#define RI_WAIT_COUNT (RI_DELAY / RI_AVAILABLE_WAITING)
#define REPEATER_READY_MAX_WAIT_DELAY 5000
#define REPEATER_READY_WAIT_COUNT (REPEATER_READY_MAX_WAIT_DELAY / RI_AVAILABLE_WAITING)
#define HDCP_RETRY_COUNT 1
#define KSV_SIZE 5
#define KSV_FIFO_SIZE 15
#define MAX_KSV_LIST_COUNT 127
#define M0_SIZE 8
#define SHA1_SIZE 20
#define BINFO_SIZE 2
#define V_READ_RETRY_CNT 3

enum{
	LINK_CHECK_PASS = 0,
	LINK_CHECK_NEED = 1,
	LINK_CHECK_FAIL = 2,
};

enum{
	FIRST_AUTH  = 0,
	SECOND_AUTH = 1,
};

static inline struct displayport_device *get_displayport_drvdata(void)
{
	return displayport_drvdata;
}

/* register access subroutines */
static inline u32 displayport_read(u32 reg_id)
{
	struct displayport_device *displayport = get_displayport_drvdata();

	return readl(displayport->res.regs + reg_id);
}

static inline u32 displayport_read_mask(u32 reg_id, u32 mask)
{
	u32 val = displayport_read(reg_id);

	val &= (mask);
	return val;
}

static inline void displayport_write(u32 reg_id, u32 val)
{
	struct displayport_device *displayport = get_displayport_drvdata();

	writel(val, displayport->res.regs + reg_id);
}

static inline void displayport_write_mask(u32 reg_id, u32 val, u32 mask)
{
	struct displayport_device *displayport = get_displayport_drvdata();
	u32 old = displayport_read(reg_id);
	u32 bit_shift;

	for (bit_shift = 0; bit_shift < 32; bit_shift++) {
		if ((mask >> bit_shift) & 0x00000001)
			break;
	}

	val = ((val<<bit_shift) & mask) | (old & ~mask);
	writel(val, displayport->res.regs + reg_id);
}

void displayport_reg_set_pixel_clock(videoformat video_format);
void displayport_reg_init(void);
void displayport_reg_set_interrupt_mask(enum displayport_interrupt_mask param, u8 set);
u32 displayport_reg_get_interrupt_and_clear(u32 interrupt_status_register);
void displayport_reg_start(void);
void displayport_reg_video_mute(u32 en);
void displayport_reg_stop(void);
void displayport_reg_set_video_configuration(u8 bpc);
int displayport_reg_dpcd_write(u32 address, u32 length, u8 *data);
int displayport_reg_dpcd_read(u32 address, u32 length, u8 *data);
int displayport_reg_dpcd_write_burst(u32 address, u32 length, u8 *data);
int displayport_reg_dpcd_read_burst(u32 address, u32 length, u8 *data);
int displayport_reg_edid_write(u8 edid_addr_offset, u32 length, u8 *data);
int displayport_reg_edid_read(u8 edid_addr_offset, u32 length, u8 *data);
void displayport_reg_phy_off(void);
void displayport_reg_phy_reset(u32 en);
void displayport_reg_lane_reset(u32 en);
void displayport_reg_set_link_bw(u8 link_rate);
u32 displayport_reg_get_link_bw(void);
void displayport_reg_set_lane_count(u8 lane_cnt);
u32 displayport_reg_get_lane_count(void);
void displayport_reg_wait_phy_pll_lock(void);
void displayport_reg_set_training_pattern(displayport_training_pattern pattern);
void displayport_reg_set_qual_pattern(displayport_qual_pattern pattern, displayport_scrambling scramble);
void displayport_reg_set_hbr2_scrambler_reset(u32 uResetCount);
void displayport_reg_set_PN_Inverse_PHY_Lane(u32 enable);
void displayport_reg_set_pattern_PLTPAT(void);
void displayport_reg_serdes_enable(u32 en);
void displayport_reg_set_voltage_and_pre_emphasis(u8 *voltage, u8 *pre_emphasis);
void displayport_reg_get_voltage_and_pre_emphasis_max_reach(u8 *max_reach_value);
void displayport_reg_set_bist_video_configuration(videoformat video_format, u8 bpc, u8 type, u8 range);
void displayport_reg_set_bist_video_configuration_for_blue_screen(videoformat video_format);
void displayport_reg_set_video_bist_mode(u32 en);
void displayport_reg_set_audio_bist_mode(u32 en);
void displayport_reg_set_phy_clk_bw(u8 link_rate);
u32 displayport_reg_get_cmn_ctl_sfr_ctl_mode(void);

void displayport_reg_set_audio_m_n(audio_sync_mode audio_sync_mode,
		enum audio_sampling_frequency audio_sampling_freq);
void displayport_reg_set_audio_function_enable(u32 en);
void displayport_reg_set_audio_master_mode(void);
void displayport_reg_set_dma_burst_size(enum audio_dma_word_length word_length);
void displayport_reg_set_dma_pack_mode(enum audio_16bit_dma_mode dma_mode);
void displayport_reg_set_pcm_size(enum audio_bit_per_channel audio_bit_size);
void displayport_reg_set_audio_ch_status_same(u32 en);
void displayport_reg_set_audio_ch(u32 audio_ch_cnt);
void displayport_reg_set_audio_fifo_function_enable(u32 en);
void displayport_reg_set_audio_sampling_frequency
		(enum audio_sampling_frequency audio_sampling_freq);
void displayport_reg_set_dp_audio_enable(u32 en);
void displayport_reg_set_audio_master_mode_enable(u32 en);
void displayport_reg_set_ch_status_sw_audio_coding(u32 en);
void displayport_reg_set_ch_status_ch_cnt(u32 audio_ch_cnt);
void displayport_reg_set_ch_status_word_length(enum audio_bit_per_channel audio_bit_size);
void displayport_reg_set_ch_status_sampling_frequency(enum audio_sampling_frequency audio_sampling_freq);
void displayport_reg_set_ch_status_clock_accuracy(enum audio_clock_accuracy clock_accuracy);

void HDCP13_Link_integrity_check(void);
void displayport_reg_set_hdcp22_lane_count(void);
void displayport_reg_set_hdcp22_system_enable(u32 en);
void displayport_reg_set_hdcp22_mode(u32 en);
void displayport_reg_set_hdcp22_encryption_enable(u32 en);
void displayport_reg_set_aux_pn_inv(u32 val);
void displayport_hpd_changed(int state);
int displayport_get_hpd_state(void);

int edid_read(struct displayport_device *hdev, u8 **data);
int edid_update(struct displayport_device *hdev);
struct v4l2_dv_timings edid_preferred_preset(void);
void edid_set_preferred_preset(int mode);
int edid_find_resolution(u16 xres, u16 yres, u16 refresh, u16 vmode);
u8 edid_read_checksum(void);
u32 edid_audio_informs(void);

int displayport_audio_bist_enable(struct displayport_audio_config_data audio_config_data);
void displayport_reg_set_avi_infoframe(struct infoframe avi_infofrmae);
void displayport_reg_set_audio_infoframe(struct infoframe audio_infofrmae, u32 en);

void HDCP13_run(void);
void HDCP13_DPCD_BUFFER(void);
u8 HDCP13_Read_Bcap(void);
extern int hdcp_calc_sha1(u8 *digest, const u8 *buf, unsigned int buflen);
extern int hdcp_dplink_authenticate(void); /* hdcp 2.2 */
extern int hdcp_dplink_get_rxstatus(uint8_t *status);
extern int hdcp_dplink_set_paring_available(void);
extern int hdcp_dplink_set_hprime_available(void);
extern int hdcp_dplink_set_rp_ready(void);
extern int hdcp_dplink_set_reauth(void);
extern int hdcp_dplink_set_integrity_fail(void);

#define DISPLAYPORT_IOC_DUMP			_IOW('V', 0, u32)
#define DISPLAYPORT_IOC_GET_ENUM_DV_TIMINGS	_IOW('V', 1, u8)
#define DISPLAYPORT_IOC_SET_RECONNECTION	_IOW('V', 2, u8)
#endif
