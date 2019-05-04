/*
 * project.h
 *
 *  Created on: 2019-3-2
 *      Author: lifim
 */

#ifndef PROJECT_H_
#define PROJECT_H_

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "spi.h"
#include "W5500.h"
#include "rs232.h"

#define DEVICE_ID "W5500"
#define FW_VER_HIGH     1
#define FW_VER_LOW      0

typedef struct __TCP_INFO {
    uint8_t ip[4];
    uint16_t server_port;
    uint16_t local_port;
    uint16_t delay_time;

} _TCP_INFO;



#endif /* PROJECT_H_ */
