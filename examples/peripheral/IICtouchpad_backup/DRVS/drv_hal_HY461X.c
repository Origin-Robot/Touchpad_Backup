#include "drv_hal_HY461X.h"
#include <stdint.h>
#include "nrf_log.h"
#include "nrf_delay.h"
//uint16_t CountDATA=0;//计算一次触摸事件的数据量
void Power_ON_Ready(void)//redy,no delay上电复位准备动作，使用绿色led端口连接reset 端口
{
		nrf_gpio_cfg_output(RESET_TOUCH);//输出		
		nrf_gpio_pin_clear(RESET_TOUCH);
		nrf_delay_ms(5);
		nrf_gpio_pin_set(RESET_TOUCH);
		nrf_delay_ms(1000);
}

void getRawData(bulk_data_t *pBulk,M_touch_date *mBulk){//原始数据联合体，顺序读取传感器数据
	I2C_Read_8bits_Address(AXS962x_ADDR1,TOUCH_FINGER_NUM,&pBulk->dataBytes[0], 25);
	
	mBulk->fingerNumbers = pBulk->fingerNumber;//整理数据
	for(uint8_t i=0;i<4;i++){
		mBulk->Finger_date[i].F_EVENT = pBulk->fingers[i].absoluteX_H >> 6;//event
		mBulk->Finger_date[i].absoluteX = ((pBulk->fingers[i].absoluteX_H << 8)+pBulk->fingers[i].absoluteX_L)&0xfff;//拼接数据
		mBulk->Finger_date[i].absoluteY = ((pBulk->fingers[i].absoluteY_H << 8)+pBulk->fingers[i].absoluteY_L)&0xfff;
	}
}
//pGesture

