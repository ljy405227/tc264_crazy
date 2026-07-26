#ifndef __MENU_H
#define __MENU_H

#define MENU_ITEM_COUNT 6
#define secondary_menu_count 5




//�˵�״̬�ṹ��
typedef enum {
    main_menu = 0,
    secondary_menu,
    three_level_menu,
} MenuState;


//�˵���ṹ��
typedef struct {
    char* name;              // �˵�����
    void (*action)(void);    // �˵���Ӧ����������Ϊ�գ�
    float* value; // ָ��Flash��Ϣ��ָ�루����Ϊ�գ�
} MenuItem;

extern MenuState menu_state;

void menu_control(void);
void main_menu_handle(void);
void secondary_menu_handle(void);
void three_level_task_menu_handle(void);

#endif
