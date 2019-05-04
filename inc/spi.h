/*
 * spi.h
 *
 *  Created on: 2019-3-2
 *      Author: lifim
 */

#ifndef SPI_H_
#define SPI_H_

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"

extern void spi_write_byte(unsigned char ch);
extern void spi_init(void);
extern void spi_gpio_init(void);
extern uint8_t SPI2_SendByte(uint8_t byte);
extern void WIZ_CS(uint8_t val);
#endif /* SPI_H_ */
