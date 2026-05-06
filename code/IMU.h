#ifndef CODE_IMU_H_
#define CODE_IMU_H_

#define BMI088_GYRO_2000_SEN  0.00106526443603169529841533860381f

typedef struct{
        float raw;
        float tmp;
        float data;
}IMU;

#endif /* CODE_IMU_H_ */


