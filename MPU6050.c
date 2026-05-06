/*
 * MPU6050.c
 *
 *  Created on: Mar 27, 2026
 *      Author: raafa
 */


#include "MPU6050/stm32g4_mpu6050.h"
#include <math.h>
#include <stdbool.h>


static MPU6050_t datas;

void MPU_init(void)
{
    MPU6050_Init(&datas,
                 GPIOB, GPIO_PIN_4,
                 MPU6050_Device_0,
                 MPU6050_Accelerometer_8G,
                 MPU6050_Gyroscope_2000s);
}

bool MPU_chute_detectee(void)
{
    MPU6050_ReadAll(&datas);

    // Détection simple : perte d'appui (accélération Z faible)
    if (datas.Accelerometer_Z < 3000)   // seuil à ajuster
        return true;

    return false;
}

bool MPU_immobilite_detectee(void)
{
    MPU6050_ReadAll(&datas);

    // Détection simple : gyroscope presque immobile
    if (datas.Gyroscope_X > -150 && datas.Gyroscope_X < 150 &&
        datas.Gyroscope_Y > -150 && datas.Gyroscope_Y < 150 &&
        datas.Gyroscope_Z > -150 && datas.Gyroscope_Z < 150)
    {
        return true;
    }

    return false;
}
