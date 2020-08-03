#include "drv_hal_HY461X.h"
#include <stdint.h>
#include "nrf_log.h"
#include "nrf_delay.h"
//uint16_t CountDATA=0;//����һ�δ����¼���������
void Power_ON_Ready(void)//redy,no delay�ϵ縴λ׼��������ʹ����ɫled�˿�����reset �˿�
{
		nrf_gpio_cfg_output(RESET_TOUCH);//���		
		nrf_gpio_pin_clear(RESET_TOUCH);
		nrf_delay_ms(5);
		nrf_gpio_pin_set(RESET_TOUCH);
		nrf_delay_ms(1000);
}

void getRawData(bulk_data_t *pBulk,M_touch_date *mBulk){//ԭʼ���������壬˳���ȡ����������
	I2C_Read_8bits_Address(AXS962x_ADDR1,TOUCH_FINGER_NUM,&pBulk->dataBytes[0], 25);
	
	mBulk->fingerNumbers = pBulk->fingerNumber;//��������
	for(uint8_t i=0;i<4;i++){
		mBulk->Finger_date[i].F_EVENT = pBulk->fingers[i].absoluteX_H >> 6;//event
		mBulk->Finger_date[i].absoluteX = ((pBulk->fingers[i].absoluteX_H << 8)+pBulk->fingers[i].absoluteX_L)&0xfff;//ƴ������
		mBulk->Finger_date[i].absoluteY = ((pBulk->fingers[i].absoluteY_H << 8)+pBulk->fingers[i].absoluteY_L)&0xfff;
	}
}
//pGesture

