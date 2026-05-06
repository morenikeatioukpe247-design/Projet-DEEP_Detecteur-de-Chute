/*
 * MPU6050.h
 *
 *  Created on: Mar 27, 2026
 *      Author: raafa
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdbool.h>

void MPU_init(void);
bool MPU_chute_detectee(void);
bool MPU_immobilite_detectee(void);

#endif /* MPU6050_H_ */
