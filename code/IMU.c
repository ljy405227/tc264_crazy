#include "zf_common_headfile.h"
#include "IMU.h"

float avl_gyro_z = 0;
float dt = 0.001f;

float hx, hy, hz, bx,by, bz;
float wx,wy,wz;
float vx,vy,vz;                         //ʵ���������ٶ�

float q0 = 1;
float q1, q2, q3 = 0;

float q0q0 = 1;
float q0q1 = 0;
float q0q2 = 0;
float q0q3 = 0;
float q1q1 = 0;
float q1q2 = 0;
float q1q3 = 0;
float q2q2 = 0;
float q2q3 = 0;
float q3q3 = 0;

float ex,ey,ez = 0;                     //�����������
float ex_i, ey_i, ez_i= 0;

float Kp = 0.4f;
float Ki = 0.01f;

float pitch,roll,yaw = 0.0f;


uint8_t Gyro_Flag= 0;
uint16_t Gyro_Count = 0;
float gyro_offset[3];
float accel_offsets[3] = {0.1124, -0.0443, 0.0398};
float accel_scale[3] = {9.7797, 9.7841, 9.7367};
IMU gyro[3],accel[3];

void Process_Gyro(void)
{
    imu660rb_get_gyro();
    LPF_1(40, 1.0e-3, imu660rb_gyro_x, &gyro[0].raw);
    LPF_1(40, 1.0e-3, imu660rb_gyro_y, &gyro[1].raw);
    LPF_1(40, 1.0e-3, imu660rb_gyro_z, &gyro[2].raw);

    imu660rb_get_acc();
    LPF_1(40, 1.0e-3, imu660rb_acc_x, &accel[0].raw);
    LPF_1(40, 1.0e-3, imu660rb_acc_y, &accel[1].raw);
    LPF_1(40, 1.0e-3, imu660rb_acc_z, &accel[2].raw);

    avl_gyro_z = imu660rb_gyro_transition(imu660rb_gyro_z);

    for(uint8_t i = 0; i <= 2; i++)
    {
        accel[i].tmp = accel[i].raw;

        gyro[i].tmp = gyro[i].raw;

        /*����Matlab���ע��*/
//        accel[i].data = accel[i].tmp * 0.00239;

        accel[i].data = ((accel[i].tmp - accel_offsets[i]) / accel_scale[i])*9.8;
    }

    if(Gyro_Count < 50)
    {
        Gyro_Count++;
        for(uint8_t i = 0; i <= 2; i++)
        {
            gyro_offset[i] += gyro[i].tmp;
        }
        Gyro_Flag = 0;
    }
    else
    {
        for(uint8_t i = 0; i <= 2; i++)
        {
            gyro[i].data = gyro[i].tmp - (gyro_offset[i] / 50);
            gyro[i].data *= BMI088_GYRO_2000_SEN;
        }
        Gyro_Flag = 1;
    }
}




void Updata_IMU(float gx,float gy,float gz,float ax,float ay,float az)
{
    float norm = 0;
    float accel_norm = 0;
    //���ݴ���
    q0q0 = q0*q0;
    q0q1 = q0*q1;
    q0q2 = q0*q2;
    q0q3 = q0*q3;
    q1q1 = q1*q1;
    q1q2 = q1*q2;
    q1q3 = q1*q3;
    q2q2 = q2*q2;
    q2q3 = q2*q3;
    q3q3 = q3*q3;

    ax /= 41.8;
    ay /= 41.8;
    az /= 41.8;
    accel_norm = sqrt(ax*ax+ay*ay+az*az);
    if (accel_norm != 0.0f && Gyro_Flag == 1)
    {
        ax /= accel_norm;
        ay /= accel_norm;
        az /= accel_norm;

        vx = 2*(q1q3 - q0q2);
        vy = 2*(q0q1 + q2q3);
        vz = q0q0 - q1q1 - q2q2 + q3q3;

        if(accel_norm >= 110 || accel_norm <= 90)
        {
            ex = 0;
            ey = 0;
        }
        else
        {
            ex = (ay*vz - az*vy);
            ey = (az*vx - ax*vz);
        }

        ex_i += LIMIT(ex,-0.1,0.1)* dt;
        ey_i += LIMIT(ey,-0.1,0.1)* dt;

        ex_i = LIMIT(ex_i,-0.5,0.5);
        ey_i = LIMIT(ey_i,-0.5,0.5);
        ez_i = ez_i + ez * dt;

        //���ٶȲ���
        gx += Kp*ex + Ki*ex_i;
        gy += Kp*ey + Ki*ey_i;

        // ������Ԫ���ʺ�������,���������ǵĲ���ֵ�ͱ���-��������ֵ������Ԫ�����и��¡�
        //����΢�ַ��̵���ɢ����ʽ������Ԫ����ÿ������������Ӧ��΢������Բ������ڵ�һ�루halfT����
        q0 += 0.5*(-q1*gx - q2*gy - q3*gz)*dt;
        q1 += 0.5*(q0*gx + q2*gz - q3*gy)*dt;
        q2 += 0.5*(q0*gy - q1*gz + q3*gx)*dt;
        q3 += 0.5*(q0*gz + q1*gy - q2*gx)*dt;

        // ��������Ԫ��
        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        q0 /= norm;
        q1 /= norm;
        q2 /= norm;
        q3 /= norm;

        pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,ת��Ϊ����//3-4  //3-4
        roll = atan2(2*(q0*q1 + q2*q3),q0*q0-q1*q1-q2*q2+q3*q3) * 57.3;// rollv//11  //12
        yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, q0*q0+q1*q1-q2*q2-q3*q3)* 57.3;
    }


    
}
