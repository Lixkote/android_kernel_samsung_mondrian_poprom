#ifndef __PICASSO__POWER_SAVE_H__
#define __PICASSO__POWER_SAVE_H__

#define TCON_AUTO_BR_MAX	2
#define TCON_LEVEL_MAX	3
#define TCON_MODE_MAX	14

#define TCON_REG_MAX 30

struct tcon_reg_info {
	int reg_cnt;
	unsigned short addr[TCON_REG_MAX];
	unsigned char data[TCON_REG_MAX];
};

struct tcon_reg_info TCON_UI = {
	.reg_cnt = 16,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DB5,
		0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA,
		0x0DBB, 0x0DBC, 0x0DBD, 0x0DBE, 0x0DBF,
		0x0DC0,
		},
	.data = {
		0xB1, 0xFF, 0xF0, 0x1F, 0xF5,
		0xFE, 0x82, 0x46, 0x60, 0x3F,
		0xC1, 0x04, 0x24, 0x26, 0x01,
		0x1F,
		},
};

struct tcon_reg_info TCON_POWER_SAVE = {
	.reg_cnt = 16,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DB5,
		0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA,
		0x0DBB, 0x0DBC, 0x0DBD, 0x0DBE, 0x0DBF,
		0x0DC0,
		
		},
	.data = {
		0xB1, 0xFF, 0xF0, 0x0E, 0xC9,
		0x4E, 0x92, 0x4A, 0x59, 0xBF,
		0xC1, 0x04, 0x24, 0x26, 0x01,
		0x1F,
		},
};

struct tcon_reg_info TCON_VIDEO = {
	.reg_cnt = 16,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DB5,
		0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA,
		0x0DBB, 0x0DBC, 0x0DBD, 0x0DBE, 0x0DBF,
		0x0DC0,
		},
	.data = {
		0xB1, 0xFF, 0xF0, 0x08, 0x85,
		0x5E, 0x92, 0x4A, 0x59, 0xBF,
		0xC1, 0x04, 0x24, 0x26, 0x01,
		0x1F,
		},
};

struct tcon_reg_info TCON_BROWSER = {
	.reg_cnt = 16,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DB5,
		0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA,
		0x0DBB, 0x0DBC, 0x0DBD, 0x0DBE, 0x0DBF,
		0x0DC0,
		},
	.data = {
		0xB1, 0xFF, 0xF0, 0x08, 0x85,
		0x5E, 0x92, 0x42, 0x59, 0xBF,
		0xC1, 0x04, 0x24, 0x26, 0x01,
		0x1F,
		},
};

struct tcon_reg_info TCON_OUTDOOR = {
	.reg_cnt = 10,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DBA,
		0x0DBB, 0x0DBC, 0x0DBD, 0x0DBE, 0x0DBF,
		},
	.data = {
		0xB0, 0x79, 0x50, 0x1F, 0xBF,
		0xC1, 0x04, 0x24, 0x26, 0x01,
		},
};

struct tcon_reg_info TCON_SNOTE1 = {
	.reg_cnt = 11,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DB5,
		0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA,
		0x0DC0,
		},
	.data = {
		0xB1, 0xFF, 0xF0, 0x1F, 0xF5,
		0xFE, 0x82, 0x42, 0x5A, 0xBF,
		0x1F,
		},
};

struct tcon_reg_info TCON_SNOTE2 = {
	.reg_cnt = 11,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DB5,
		0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA,
		0x0DC0,
		},
	.data = {
		0xB1, 0xFF, 0xF0, 0x1F, 0xF5,
		0xFE, 0x82, 0x42, 0x7F, 0xFF,
		0x1F,
		},
};

struct tcon_reg_info TCON_CALL = {
	.reg_cnt = 11,

	.addr = {
		0x0DB1, 0x0DB2, 0x0DB3, 0x0DB4, 0x0DB5,
		0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA,
		0x0DC0,
		},
	.data = {
		0xB1, 0xFF, 0xF0, 0x1F, 0xF5,
		0xFE, 0x82, 0x46, 0x7F, 0xFF,
		0x1F,
		},
};
struct tcon_reg_info TCON_BLACK_IMAGE_BLU_ENABLE = {
	.reg_cnt = 1,
	.addr = {
		0x0DB1,
		},
	.data = {
		0xB0,
		},
};

struct tcon_reg_info *power_save_tune_value[TCON_AUTO_BR_MAX][TCON_LEVEL_MAX][TCON_MODE_MAX] = {
		/*
			UI_APP = 0,
			VIDEO_APP,
			VIDEO_WARM_APP,
			VIDEO_COLD_APP,
			CAMERA_APP,
			NAVI_APP,
			GALLERY_APP,
			VT_APP,
			BROWSER_APP,
			eBOOK_APP,
			EMAIL_APP,
			SNOTE1_APP,
			SNOTE2_APP,
			CALL_APP, = 13
		*/
   	 /* auto brightness off */
	{

		/* Illumiatation Level 1 */
		{
			&TCON_UI,
			&TCON_VIDEO,
			&TCON_VIDEO,
			&TCON_VIDEO,
			NULL,
			NULL,
			NULL,
			NULL,
			&TCON_BROWSER,
			NULL,
			NULL,
			&TCON_SNOTE1,
			&TCON_SNOTE2,
			&TCON_CALL,
		},
		/* Illumiatation Level 2 */
		{
			&TCON_UI,
			&TCON_VIDEO,
			&TCON_VIDEO,
			&TCON_VIDEO,
			NULL,
			NULL,
			NULL,
			NULL,
			&TCON_BROWSER,
			NULL,
			NULL,
			&TCON_SNOTE1,
			&TCON_SNOTE2,
			&TCON_CALL,
		},
		/* Illumiatation Level 3 */
		{
			&TCON_UI,
			&TCON_VIDEO,
			&TCON_VIDEO,
			&TCON_VIDEO,
			NULL,
			NULL,
			NULL,
			NULL,
			&TCON_BROWSER,
			NULL,
			NULL,
			&TCON_SNOTE1,
			&TCON_SNOTE2,
			&TCON_CALL,
		},
	},
	/* auto brightness on */
	{
		/* Illumiatation Level 1 */
		{
			&TCON_UI,
			&TCON_VIDEO,
			&TCON_VIDEO,
			&TCON_VIDEO,
			NULL,
			NULL,
			NULL,
			NULL,
			&TCON_BROWSER,
			NULL,
			NULL,
			&TCON_SNOTE1,
			&TCON_SNOTE2,
			&TCON_CALL,
		},
		/* Illumiatation Level 2 */
		{
			&TCON_POWER_SAVE,
			&TCON_VIDEO,
			&TCON_VIDEO,
			&TCON_VIDEO,
			NULL,
			NULL,
			NULL,
			NULL,
			&TCON_BROWSER,
			NULL,
			NULL,
			&TCON_SNOTE1,
			&TCON_SNOTE2,
			&TCON_CALL,
		},
		/* Illumiatation Level 3 */
		{
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
			&TCON_OUTDOOR,
		}
	}
};

#endif
