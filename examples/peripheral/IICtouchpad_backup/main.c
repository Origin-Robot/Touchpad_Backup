/** @file
 * @defgroup tw_scanner main.c
 * @{
 * @ingroup nrf_twi_example
 * @brief TWI Sensor Example main file.
 *
 * This file contains the source code for a sample application using TWI.
 *
 */

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"
#include "app_timer.h"
#include "drv_hal_HY461X.h"//包含触控板
/*
#define I2C_SCL_PIN 17
#define I2C_SDA_PIN	16
#define INT								28		//P0.08
#define RESET_TOUCH				13		//P0.07
*/

/**
 * @brief Function for main application entry.
 */
//app_timer_id_t time_id;
APP_TIMER_DEF(timer_id);//软件定时器ID预定义
#define MY_PERIOD    APP_TIMER_TICKS(5)



//第三个参数为timer的handler,每次触发时调用.时钟源来自RTC1
//ret_code_t app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context);
//ret_code_t app_timer_stop(app_timer_id_t timer_id);
//APP_TIMER_TICKS(300);//ms,最低设置为5ms
//ret_code_t app_timer_stop_all(void);

M_touch_date M_TP;//触摸数据联合体
bulk_data_t	 P_data;
uint8_t Touch_flag=0;//1为有触摸，0为没有触摸
uint8_t Scan_flag = 0;//扫描手指个数使用，用于判断何时启动软件定时器	
uint16_t CountDATA=0;//计算一次触摸事件的数据量
#define TIME_MAX_Thre		3000			//3秒，防止溢出

uint8_t Ges_Status=0;//手势状态



static void timeout_handler(void *arg)//定时器回调函数
{
	UNUSED_PARAMETER(arg);
	Scan_flag=0;
	app_timer_stop(timer_id);//定时器暂时停止
}
void one_single_tap(uint8_t F_EVENT)//9
{
	if((M_TP.Finger_date[0].F_EVENT==2)&&\
		(M_TP.Finger_date[1].F_EVENT==3)&&\
		(M_TP.Finger_date[2].F_EVENT==3)&&\
		(M_TP.Finger_date[3].F_EVENT==3)&&\
		(CountDATA==3))//单指事件开始
		{Ges_Status=ONE_FINGER_MODE;}
	if((M_TP.Finger_date[0].F_EVENT==2)&&\
		(M_TP.Finger_date[1].F_EVENT==2)&&\
		(M_TP.Finger_date[2].F_EVENT==3)&&\
		(M_TP.Finger_date[3].F_EVENT==3)&&\
		(CountDATA==3))//双指事件开始
		{Ges_Status=ONE_FINGER_MODE;}
		
}
void Scan_Finger_NUM(void)//计时
{
	if(M_TP.Finger_date[0].F_EVENT==0)
	{
		Ges_Status=ST_TOUCH;
		Scan_flag = 1;//打开软件定时器
		app_timer_start(timer_id,MY_PERIOD, timeout_handler);//打开软件定时器中断
	}//触摸事件开始
}

void LED_init(void)
{
		nrf_gpio_cfg_output(PIN_LED_GREEN);//输出
		nrf_gpio_cfg_output(PIN_LED_RED);//输出
		nrf_gpio_cfg_output(PIN_LED_BLUE);//输出
		nrf_gpio_pin_set(PIN_LED_GREEN);
		nrf_gpio_pin_set(PIN_LED_RED);
		nrf_gpio_pin_set(PIN_LED_BLUE);
}
void in_pin_hander(nrf_drv_gpiote_pin_t pin,nrf_gpiote_polarity_t action)// 外部中断回调函数
{
	nrf_drv_gpiote_in_event_disable(INT);//关闭使能，保护进程
	getRawData(&P_data,&M_TP);//梳理数据
	NRF_LOG_INFO("<<<<<<<<<<---%d--->>>>>>>>>>",M_TP.fingerNumbers);
//	NRF_LOG_INFO("Finger1:EVENT:%d,X:%d,Y:%d",\
//								M_TP.Finger_date[0].F_EVENT,M_TP.Finger_date[0].absoluteX,M_TP.Finger_date[0].absoluteY);
//	NRF_LOG_INFO("Finger1:EVENT:%d,X:%d,Y:%d \r\n Finger2:EVENT:%d,X:%d,Y:%d",\
//								M_TP.Finger_date[0].F_EVENT,M_TP.Finger_date[0].absoluteX,M_TP.Finger_date[0].absoluteY,\
//								M_TP.Finger_date[1].F_EVENT,M_TP.Finger_date[1].absoluteX,M_TP.Finger_date[1].absoluteY);
//	NRF_LOG_INFO("Finger3:EVENT:%d,X:%d,Y:%d \r\n Finger4:EVENT:%d,X:%d,Y:%d",\
//								M_TP.Finger_date[2].F_EVENT,M_TP.Finger_date[2].absoluteX,M_TP.Finger_date[2].absoluteY,\
//								M_TP.Finger_date[3].F_EVENT,M_TP.Finger_date[3].absoluteX,M_TP.Finger_date[3].absoluteY);
	if(M_TP.Finger_date[0].F_EVENT==0||M_TP.Finger_date[0].F_EVENT==2)
	{CountDATA++;}
	else if(M_TP.Finger_date[0].F_EVENT==1)
	{NRF_LOG_INFO("<-----------------%d------------------>",CountDATA);
		CountDATA=0;}
	Scan_Finger_NUM();
	NRF_LOG_FLUSH();
	//NRF_LOG_FLUSH();
		
		switch(M_TP.fingerNumbers)//脱机检测手指个数
		{
			case 255:
					nrf_gpio_pin_set(PIN_LED_GREEN);
					nrf_gpio_pin_set(PIN_LED_RED);
					nrf_gpio_pin_set(PIN_LED_BLUE);
				break;
			case 1:
					nrf_gpio_pin_clear(PIN_LED_GREEN);//绿色
					nrf_gpio_pin_set(PIN_LED_RED);
					nrf_gpio_pin_set(PIN_LED_BLUE);
				break;
			case 2:
					nrf_gpio_pin_set(PIN_LED_GREEN);//红色
					nrf_gpio_pin_clear(PIN_LED_RED);
					nrf_gpio_pin_set(PIN_LED_BLUE);
				break;
			case 3:
					nrf_gpio_pin_set(PIN_LED_GREEN);//蓝色
					nrf_gpio_pin_set(PIN_LED_RED);
					nrf_gpio_pin_clear(PIN_LED_BLUE);
				break;
			case 4:
					nrf_gpio_pin_clear(PIN_LED_GREEN);//全亮
					nrf_gpio_pin_clear(PIN_LED_RED);
					nrf_gpio_pin_clear(PIN_LED_BLUE);
				break;
			default:
					nrf_gpio_pin_set(PIN_LED_GREEN);
					nrf_gpio_pin_set(PIN_LED_RED);
					nrf_gpio_pin_set(PIN_LED_BLUE);
				break;
		}
	
	nrf_drv_gpiote_in_event_enable(INT,true);
}

void gpiote_init(void)
{
		ret_code_t err_code;
		err_code = nrf_drv_gpiote_init();	
		APP_ERROR_CHECK(err_code);
//	if(!nrf_drv_gpiote_is_init()){
//        err_code = nrf_drv_gpiote_init();   // Must be done before using gpiote module.
//        if (err_code != NRF_SUCCESS){};
//    }
	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
	in_config.pull = NRF_GPIO_PIN_PULLUP;
	err_code = nrf_drv_gpiote_in_init(INT,&in_config,in_pin_hander);
	APP_ERROR_CHECK(err_code);//
	nrf_drv_gpiote_in_event_enable(INT, true);
}
/*********************************************软件定时器相关函数*****************************************************/


static void timer_init(void)
{
    ret_code_t err_code;
    
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}
int main(void)
{
//    uint8_t mode_data = WORK_MODE;
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("TWI scanner.");
    NRF_LOG_FLUSH();
    twi_init();
		timer_init();//定时器初始化
		app_timer_create(&timer_id,APP_TIMER_MODE_REPEATED,timeout_handler);//创建一个定时器
//    nrf_gpio_cfg_output(RESET_TOUCH);//输出
//		nrf_gpio_cfg_input(INT, GPIO_PIN_CNF_PULL_Disabled);//上拉检测RESET_TOUCH
//		nrf_gpio_cfg_input(RESET_TOUCH, NRF_GPIO_PIN_PULLUP);
		Power_ON_Ready();
		LED_init();
		gpiote_init();//GPIOTE初始化

    while (true)
    {
    }
}

/** @} */
