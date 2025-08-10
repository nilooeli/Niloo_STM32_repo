#ifndef BMP280_H
#define BMP280_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// 7-bit I2C addresses
#define BMP280_ADDR7_76   0x76
#define BMP280_ADDR7_77   0x77

// Register map (subset we need)
#define BMP280_REG_ID         0xD0
#define BMP280_REG_CTRL_MEAS  0xF4
#define BMP280_REG_PRESS_MSB  0xF7  // P[19:12]
#define BMP280_REG_TEMP_MSB   0xFA  // T[19:12]

// Expected chip IDs (Bosch docs: 0x58 for BMP280; some clones may show 0x60)
#define BMP280_ID_BMP280      0x58
#define BMP280_ID_BME280      0x60

typedef struct {
	I2C_HandleTypeDef *hi2c;  // which I2C to use (I2C1)
	uint16_t dev_addr8;       // HAL 8-bit address (7-bit << 1)
	uint8_t  chip_id;         // Read from 0xD0
}BMP280;

/* Initialize and probe the sensor */
HAL_StatusTypeDef BMP280_Init(BMP280 *s, I2C_HandleTypeDef *hi2c, uint8_t addr7);

/* Configure basic oversampling + normal mode */
HAL_StatusTypeDef BMP280_ConfigureDefault(BMP280 *s);

/* Read ID into *out_id */
HAL_StatusTypeDef BMP280_ReadID(BMP280 *s, uint8_t *out_id);

/* Read raw 20-bit pressure and temperature */
HAL_StatusTypeDef BMP280_ReadRaw(BMP280 *s, uint32_t *raw_press, uint32_t *raw_temp);

#ifdef __cplusplus
}

#endif
#endif //BMP280_H
