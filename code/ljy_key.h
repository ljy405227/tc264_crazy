#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#define KEY_LIST  {P13_2, P13_0, P13_1, P15_5}
#define BM_LIST  {P15_6, P15_4, P15_2, P15_0, P15_1}
#define KEY_COUNT 4
#define BM_COUNT 5
typedef enum
{
    key_check = 0,
    key_confirm = 1,
    key_release = 2 
}KEY_STATE;


uint8_t key_read(int key_index);
#define key_up 0
#define key_short_down 1
#define key_long_down 2

extern uint8_t key_flag[KEY_COUNT];
#define key_up key_flag[0];
#define key_down key_flag[1];
#define key_ok key_flag[2];
#define key_back key_flag[3];

void Key_Init(void);
void key_scan(void);
bool is_key_dowm(uint8_t key_index);
void key_res(void);


#define   BM5      gpio_get_level(P15_6)
#define   BM4      gpio_get_level(P15_4)
#define   BM3      gpio_get_level(P15_2)
#define   BM2      gpio_get_level(P15_0)
#define   BM1      gpio_get_level(P15_1)
#define   up       1
#define   down     0


void BM_Control(void);


#endif /* CODE_KEY_H_ */
