#ifndef __DRV_HAL_HY461X_H
#define __DRV_HAL_HY461X_H
//#include "m_platform_cfg.h"
#include "nrf_drv_gpiote.h"
#include <stdint.h>
#include "I2C.h"
#pragma anon_unions//匿名结构体，联合体
#pragma pack(1)

//#define I2C_SCL_PIN				18
//#define I2C_SDA_PIN				14
#define INT								13		//P0.08
#define RESET_TOUCH				13		//P0.07
//LED test
#define PIN_LED_GREEN               31  //P0.11
#define PIN_LED_RED                 30  //P0.12 
#define PIN_LED_BLUE                29  //P0.13

//#define PIN_LED_GREEN               11  //P0.11
//#define PIN_LED_RED                 12  //P0.12 
//#define PIN_LED_BLUE                13  //P0.13


#define	AXS962x_ADDR1			0x7E
#define	AXS962x_ADDR2			0x38
//*****************************************************************************
//
//! ----------------------    HY461X MEMORY MAP REGISTERS    ------------------
//
//*****************************************************************************   
#define WORK_MODE					0X00	//work mode
#define	TEST_MODE					0xC0	//test mode

#define	TP_RUN_MODE 			0x00	//0x00?work mode 0xc0? test mode//读写
#define TOUCH_GESTURE			0x01	//
#define TOUCH_FINGER_NUM	0x02
/*-----------------坐标数据寄存器地址------------------*///最多十点触控，地址顺序
#define TOUCH1_XH					0x03
#define TOUCH1_XL					0x04
#define TOUCH1_YH					0x05
#define TOUCH1_YL					0x06
#define TOUCH1_ZH					0x07
#define TOUCH1_ZL					0x08

#define TOUCH2_XH					0x09
#define TOUCH2_XL					0x0A
#define TOUCH2_YH					0X0B
#define TOUCH2_YL					0x0C
#define TOUCH2_ZH					0x0D
#define TOUCH2_ZL					0x0E

#define TOUCH3_XH					0x0F
#define TOUCH3_XL					0x10
#define TOUCH3_YH					0x11
#define TOUCH3_YL					0x12
#define TOUCH3_ZH					0x13
#define TOUCH3_ZL					0x14

#define TOUCH4_XH					0x15
#define TOUCH4_XL					0x16
#define TOUCH4_YH					0x17
#define TOUCH4_YL					0x18
#define TOUCH4_ZH					0x19
#define TOUCH4_ZL					0x1A

#define TOUCH5_XH					0x1B
#define TOUCH5_XL					0x1C
#define TOUCH5_YH					0x1D
#define TOUCH5_YL					0x1E
#define TOUCH5_ZH					0x1F
#define TOUCH5_ZL					0x20
//*****************************************************************************
//
//! ----------------------    HY461X BIT DEFINITIONS     ----------------------
//
//*****************************************************************************
#define TOUCH_THRESHOLD		0x80	//R/W//hreshold=(DATE*10);
#define P_SENSEOR_ON			0x81	//R/W//控制距离传感功能打开/关闭，0：关闭，1：打开
#define TX_FREQUENCY_H		0x82	//R////work mode tx frequency hige date	//当前TX频率高位
#define TX_FREQUENCY_L		0x83	//R////work mode tx frequency Low date
#define GLOVE_EN					0x84	//R/W//手套模式
#define REPORT_SPEED			0X88	//R/W//报点率，REPORT SPEED[7:0]，if date=0x64,report rate=100hz
#define POWER_NOISE_EN		0X89	//R/W//设置0：turn off，1：turn on
#define FITLTER_DATA			0x8A	//R/W//从无触摸到有触摸，丢弃的触摸数据包次数，range:0--5
#define GAIN							0x92	//R/W//Diff data 增益，不推荐修改，range:0--5
#define EDGE_OFFSET				0x93	//R/W//边沿处理设定值，range:0--16
#define RX_NUMBER_USED		0x94	//R////获取TP使用的RX数量
#define TX_NUMBER_USED		0x95	//R////获取TP使用的TX数量
#define POWER_MODE				0xA5	//W////写0x03,控制TP进入SLEEP，需要reset pin拉LOW唤醒
#define FW_VERSION				0xA6	//R////获取firmware版本号
#define LIB_VERSION				0xA7	//R////获取TP使用的公版版本号
#define TP_ID							0xA8	//R////ID号
#define TP_CHIP_ID				0xA9	//R////获取当前TP使用的IC的型号
#define TX_NUMBER_USED2		0xAB	//R////获取TP使用的TX数量。同0x95
#define RX_NUMBER_USED2		0xAC	//R////获取TP使用的RX数量。同0x94
#define INT_TEST					0xAD	//R/W//工厂测试，对INT脚位自动测试，为1，输出INT坐标(255,255).
#define BOOTLOAD_VERSION	0xB0	//R////获取bootloader版本号低位数据
#define customize					0xBB	//R/W//自定义的配置区，首地址0xBB,共31bytes,可用于填入具体的字符串识别号
//************************************************************************************************************
#define SINGLE_TAP			 						0x01					//单击
#define TAP_AND_HOLD		 						0x02					//点住不放
#define	ZOOM						 						0x03					//缩放
#define	SINGLE_SCROLL		 						0x04					//单指滑动

#define TWO_FINGER_TAP	 						0x05					//双指单击
#define TWO_FINGER_SCROLL						0x06					//双指滑动
#define TWO_FINGER_TAP_AND_HOLD			0x07					//双指按住不动

#define THREE_FINGER_TAP 						0x08					//三指单击
#define THREE_FINGER_SCROLL					0x09					//三指滑动
#define	THREE_FINGER_TAP_AND_HOLD		0x0A					//三指按住不动	

#define FOUR_FINGER_TAP							0x0B					//四指单击
#define FOUR_FINGER_TAP_AND_HOLD		0x0C					//四指按住不动
#define NO_GES											0xFD					//当前无手势


#define DOWN												0							//Finger_event
#define	UP													1
#define CONTACT											3
#define RESERVED										4
/////////////////////////////////////////////////////////////////////

#define ST_TOUCH										0xA1
#define	END_TOUCH										0xA2
#define	ONE_FINGER_MODE							0xA3
#define TWO_FINGER_MODE							0xA4
#define	THREE_FINGER_MODE						0xA5
#define FOUR_FINGER_MODE						0xA6

/*******************************************************************************************************************/







typedef union			//原始坐标数据
{
    uint8_t dataBytes[6];
    struct{
        uint8_t absoluteX_H;//高位数据的高四位表示手指的状态
        uint8_t absoluteX_L;
				uint8_t absoluteY_H;
        uint8_t absoluteY_L;
				uint8_t absoluteZ_H;
        uint8_t absoluteZ_L;
    };
} finger_t;
typedef union{							//原始数据
    uint8_t dataBytes[25];
    struct{
        uint8_t fingerNumber;
        finger_t fingers[4];
    };
} bulk_data_t;

typedef union{							//处理过的手指触摸数据
		uint8_t dateBytes[5];
		struct{
				uint8_t 	F_EVENT;
				uint16_t	absoluteX;
				uint16_t  absoluteY;
		};
} m_fingers_t;

typedef union{							//处理过的触摸数据，联合体
		uint8_t dataBytes[21];
		struct{
				uint8_t 			fingerNumbers;
				m_fingers_t		Finger_date[4];
		};
}	M_touch_date;

void Power_ON_Ready(void);
void getRawData(bulk_data_t *pBulk,M_touch_date *mBulk);












#endif
