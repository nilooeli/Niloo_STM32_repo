#include "bmp280.h"

#define HAL_ADDR(a7)   ((uint16_t)((a7) << 1))

static HAL_StatusTypeDef rd(I2C_HandleTypeDef *hi2c, uint16_t a8,
                            uint8_t reg, uint8_t *buf, uint16_t n)
{
    return HAL_I2C_Mem_Read(hi2c, a8, reg, I2C_MEMADD_SIZE_8BIT, buf, n, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef wr(I2C_HandleTypeDef *hi2c, uint16_t a8,
                            uint8_t reg, uint8_t val)
{
    return HAL_I2C_Mem_Write(hi2c, a8, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, HAL_MAX_DELAY);
}

static uint32_t u20_from_3bytes(uint8_t b[3]) {
    // 20-bit value packed as MSB, LSB, XLSB[7:4]
    return ((uint32_t)b[0] << 12) | ((uint32_t)b[1] << 4) | ((uint32_t)b[2] >> 4);
}

static HAL_StatusTypeDef try_addr(BMP280 *s, uint8_t addr7)
{
    s->dev_addr8 = HAL_ADDR(addr7);
    uint8_t id = 0;
    HAL_StatusTypeDef st = rd(s->hi2c, s->dev_addr8, BMP280_REG_ID, &id, 1);
    if (st == HAL_OK && (id == BMP280_ID_BMP280 || id == BMP280_ID_BME280)) {
        s->chip_id = id;
        return HAL_OK;
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef BMP280_Init(BMP280 *s, I2C_HandleTypeDef *hi2c, uint8_t addr7)
{
    s->hi2c = hi2c;
    s->dev_addr8 = 0;
    s->chip_id = 0;

    if (addr7 == 0) {
        if (try_addr(s, BMP280_ADDR7_76) == HAL_OK) return HAL_OK;
        if (try_addr(s, BMP280_ADDR7_77) == HAL_OK) return HAL_OK;
        return HAL_ERROR;
    } else {
        return try_addr(s, addr7);
    }
}

HAL_StatusTypeDef BMP280_ConfigureDefault(BMP280 *s)
{
    // ctrl_meas: pressure x1 (001), temperature x1 (001), normal mode (11)
    // 0b0010 0111 = 0x27
    return wr(s->hi2c, s->dev_addr8, BMP280_REG_CTRL_MEAS, 0x27);
}

HAL_StatusTypeDef BMP280_ReadID(BMP280 *s, uint8_t *out_id)
{
    return rd(s->hi2c, s->dev_addr8, BMP280_REG_ID, out_id, 1);
}

HAL_StatusTypeDef BMP280_ReadRaw(BMP280 *s, uint32_t *raw_press, uint32_t *raw_temp)
{
    uint8_t p[3], t[3];
    if (rd(s->hi2c, s->dev_addr8, BMP280_REG_PRESS_MSB, p, 3) != HAL_OK) return HAL_ERROR;
    if (rd(s->hi2c, s->dev_addr8, BMP280_REG_TEMP_MSB,  t, 3) != HAL_OK) return HAL_ERROR;
    if (raw_press)
    	*raw_press = u20_from_3bytes(p);
    if (raw_temp)
    	*raw_temp  = u20_from_3bytes(t);
    return HAL_OK;
}
