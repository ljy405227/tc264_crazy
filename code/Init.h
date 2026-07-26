#ifndef CODE_INIT_H_
#define CODE_INIT_H_


struct FlashInformation
{
    float tar_speed;

    float speed_kp;
    float speed_ki;
    float speed_kd;
    float speed_kp_right;
    float speed_ki_right;

    float gyro_kp;
    float gyro_kd;
    float dir_kp;
    float dir_kd;

    float gyro_kp_left;
    float gyro_kd_left;
    float dir_kp_left;
    float dir_kd_left;

    float gyro_kp_right;
    float gyro_kd_right;
    float dir_kp_right;
    float dir_kd_right;

    float other_ramp_time;
    float other_turn_speed;


    // float speed_row;




    // float tar_speed;
    // float speed_kf;
    // float speed_kp;
    // float speed_ki;
    // float speed_kd;

    // float direction_kp1;
    // float direction_kp2;
    // float direction_kd;
    // float direction_max_out;

    // float speed_max_intel;

    // float speedl_kp;
    // float speedl_ki;

    // float speedr_kp;
    // float speedr_ki;

    // float speed_max_out;

    // float dis_kf;
    // float dis_kp;
    // float dis_ki;
    // float dis_kd;
    // float dis_max_intel;
    // float dis_max_out;

    // float img_kp;
    // float img_kp2;
    // float img_max_out;

    // float Brushless_duty;
    // float Sped_dec;
};

extern struct FlashInformation Flash;

void Flash_Init(void);

void Device_Init(void);

#endif /* CODE_INIT_H_ */
