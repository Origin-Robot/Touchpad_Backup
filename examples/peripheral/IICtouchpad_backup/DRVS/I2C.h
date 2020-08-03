#ifndef __I2C_h__
#define __I2C_h__

#include <stdint.h>
#include "nrf_drv_twi.h"
#include "sdk_errors.h"
#define I2C_SCL_PIN 18
#define I2C_SDA_PIN	14
ret_code_t twi_init(void);
ret_code_t I2C_Setup(void);
ret_code_t I2C_Write_16bits_Address(uint8_t ui8DeviceAddress, uint16_t ui16RegisterAddress, uint8_t *pData, uint8_t ui8NoOfBytes);
ret_code_t I2C_Read_16bits_Address(uint8_t ui8DeviceAddress, uint16_t registerAddress, uint8_t *data, uint8_t numberBytes);
uint8_t I2C_Read_8bits_Address(uint8_t ui8DeviceAddress, uint8_t ui8RegisterAddress, uint8_t *pData, uint8_t ui8NoOfBytes);
uint8_t I2C_Write_8bits_Address(uint8_t ui8DeviceAddress, uint8_t ui8RegisterAddress, uint8_t *pData, uint8_t ui8NoOfBytes);
#endif
