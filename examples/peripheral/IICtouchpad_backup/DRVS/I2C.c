#include "I2C.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include <stdbool.h>
#include "app_util_platform.h"
//#include 
//#include "m_platform_cfg.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0
/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
//#if 1
#include <stdbool.h>
#define MAX_RETRIES 0xFF

static bool isInnited = false;
static bool isBusy = false;

//#define TWI_INSTANCE_ID 1

ret_code_t twi_init(void){
    ret_code_t err_code;
    const nrf_drv_twi_config_t i2c_config = {
        .scl = I2C_SCL_PIN,
        .sda = I2C_SDA_PIN,
        .frequency = NRF_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        .clear_bus_init = false
    };
    err_code = nrf_drv_twi_init(&m_twi, &i2c_config, NULL, NULL);

    APP_ERROR_CHECK(err_code);
    nrf_drv_twi_enable(&m_twi);
    return err_code;
}

ret_code_t I2C_Setup(void){
    ret_code_t err_code = NRF_SUCCESS;
    if(!isInnited){
        err_code = twi_init();
        isInnited = true;
    }
    return err_code;
}

ret_code_t I2C_Write_16bits_Address(uint8_t ui8DeviceAddress, uint16_t ui16RegisterAddress, uint8_t *pData, uint8_t ui8NoOfBytes){
    if(isBusy)
        return NRF_ERROR_BUSY;
    isBusy = true;
    uint8_t tries = 0;
    ret_code_t err_code = NRF_ERROR_BUSY;
    uint8_t nums = 0;
    uint8_t reg[10];
    reg[nums++] = (uint8_t)(ui16RegisterAddress >> 8);
    reg[nums++] = (uint8_t)(ui16RegisterAddress);
    while (ui8NoOfBytes--) {
        reg[nums++] = *pData;
        pData++;
    }
    while ((err_code != NRF_SUCCESS) && (tries < MAX_RETRIES)) {
        err_code = nrf_drv_twi_tx(&m_twi, ui8DeviceAddress, reg, nums, false);
        tries++;
    }
    isBusy = false;
    return err_code;
}

ret_code_t I2C_Read_16bits_Address(uint8_t ui8DeviceAddress, uint16_t ui16RegisterAddress, uint8_t *pData, uint8_t ui8NoOfBytes) {
    if (isBusy)
        return NRF_ERROR_BUSY;
    isBusy = true;
    ret_code_t err_code = NRF_ERROR_BUSY;
    uint8_t tries = 0;
    uint8_t nums = 0;
    uint8_t reg[2];
    reg[nums++] = (uint8_t)(ui16RegisterAddress >> 8);
    reg[nums++] = (uint8_t)(ui16RegisterAddress);
    while ((err_code != NRF_SUCCESS) && (tries < MAX_RETRIES)) {
        err_code = nrf_drv_twi_tx(&m_twi, ui8DeviceAddress, reg, nums, true);
        tries++;
    }
    APP_ERROR_CHECK(err_code);
    tries = 0;
    err_code = NRF_ERROR_BUSY;
    while((err_code != NRF_SUCCESS) && (tries < MAX_RETRIES)) {
        err_code = nrf_drv_twi_rx(&m_twi, ui8DeviceAddress, pData, ui8NoOfBytes);
        tries++;
    }
    isBusy = false;
    return err_code;
}

uint8_t I2C_Read_8bits_Address(uint8_t ui8DeviceAddress, uint8_t ui8RegisterAddress, uint8_t *pData, uint8_t ui8NoOfBytes) {
    if (isBusy)
        return NRF_ERROR_BUSY;
    isBusy = true;
    ret_code_t err_code = NRF_ERROR_BUSY;
    uint8_t tries = 0;
    while ((err_code != NRF_SUCCESS) && (tries < MAX_RETRIES)) {
        err_code = nrf_drv_twi_tx(&m_twi, ui8DeviceAddress, &ui8RegisterAddress, 1, true);
        tries++;
    }
    APP_ERROR_CHECK(err_code);
    err_code = NRF_ERROR_BUSY;
    tries = 0;
    while ((err_code != NRF_SUCCESS) && (tries < MAX_RETRIES)) {
        err_code = nrf_drv_twi_rx(&m_twi, ui8DeviceAddress, pData, ui8NoOfBytes);
        tries++;
    }
    APP_ERROR_CHECK(err_code);
    isBusy = false;
    return err_code == NRF_SUCCESS;
}

uint8_t I2C_Write_8bits_Address(uint8_t ui8DeviceAddress, uint8_t ui8RegisterAddress, uint8_t *pData, uint8_t ui8NoOfBytes) {
    if (isBusy)
        return NRF_ERROR_BUSY;
    isBusy = true;
    ret_code_t err_code = NRF_ERROR_BUSY;
    uint8_t tries = 0;
    uint8_t data[10];
    uint8_t length = 0;
    data[length++] = ui8RegisterAddress;
    while (ui8NoOfBytes--) {
        data[length++] = *pData;
        pData++;
    }
    while ((err_code != NRF_SUCCESS) && (tries < MAX_RETRIES)) {
        err_code = nrf_drv_twi_tx(&m_twi, ui8DeviceAddress, data, length, false);
        tries++;
    }
    APP_ERROR_CHECK(err_code);
    isBusy = false;
    return err_code == NRF_SUCCESS;
}
