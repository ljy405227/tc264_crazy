/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          seekfree_assistant
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-01-01       pudding            first version
********************************************************************************************************************/
#ifndef _seekfree_assistant_h_
#define _seekfree_assistant_h_


#include "zf_common_typedef.h"


// 1��ʹ�ܲ������ڵĹ���  0���رղ������ڵĹ���
#define SEEKFREE_ASSISTANT_SET_PARAMETR_ENABLE      ( 1 )



// �������FIFO��С
#define SEEKFREE_ASSISTANT_BUFFER_SIZE              ( 0x80 )
    
// ����ʾ���������ͨ������
#define SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT   ( 0x08 )
    
// ����������Ե����ͨ������
#define SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT       ( 0x08 )
    
// ����ͼ������������
#define SEEKFREE_ASSISTANT_CAMERA_MAX_BOUNDARY      ( 0x08 )
    
// ��Ƭ������λ�����͵�֡ͷ
#define SEEKFREE_ASSISTANT_SEND_HEAD                ( 0xAA )
    
// ����ͷ��
#define SEEKFREE_ASSISTANT_CAMERA_FUNCTION          ( 0x02 )
#define SEEKFREE_ASSISTANT_CAMERA_DOT_FUNCTION      ( 0x03 )
#define SEEKFREE_ASSISTANT_CAMERA_OSCILLOSCOPE      ( 0x10 )
    
// ��λ������Ƭ�����͵�֡ͷ
#define SEEKFREE_ASSISTANT_RECEIVE_HEAD             ( 0x55 )
    
// ����������
#define SEEKFREE_ASSISTANT_RECEIVE_SET_PARAMETER    ( 0x20 )


// ����ͷ����ö��
typedef enum
{
    // ��������ͷ�ͺŶ���
    SEEKFREE_ASSISTANT_OV7725_BIN = 1,
    SEEKFREE_ASSISTANT_MT9V03X,
    SEEKFREE_ASSISTANT_SCC8660,

    // ����ͼ�����Ͷ���
    SEEKFREE_ASSISTANT_BINARY = 1,
    SEEKFREE_ASSISTANT_GRAY,
    SEEKFREE_ASSISTANT_RGB565,
}seekfree_assistant_image_type_enum;

// ����ͷ����ö��
typedef enum
{
    // ��������ͷ�ͺŶ���
    X_BOUNDARY,     // ���͵�ͼ���б߽���Ϣֻ����X��Ҳ����ֻ�к�������Ϣ�����������ͼ��߶ȵõ�
    Y_BOUNDARY,     // ���͵�ͼ���б߽���Ϣֻ����Y��Ҳ����ֻ����������Ϣ�����������ͼ����ȵõ���ͨ������������������
    XY_BOUNDARY,    // ���͵�ͼ���б߽���Ϣ����X��Y����������ָ����������λ�ã��Ϳ��Է�����ʾ�������Ч��
    NO_BOUNDARY,    // ���͵�ͼ����û�б�����Ϣ
}seekfree_assistant_boundary_type_enum;

typedef struct
{
    uint8 head;                                                 // ֡ͷ
    uint8 channel_num;                                          // ����λΪ������  ����λΪͨ������
    uint8 check_sum;                                            // ��У��
    uint8 length;                                               // ������
    float data[SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT];      // ͨ������
}seekfree_assistant_oscilloscope_struct;


typedef struct
{
    uint8 head;                                                 // ֡ͷ
    uint8 function;                                             // ������
    uint8 camera_type;                                          // ����λ��ʾ�߽����� ����λ��ʾ�Ƿ���ͼ������  ����0x13������3��ʾһ��ͼ���������߽磨ͨ������߽硢���ߡ��ұ߽磩��1��ʾû��ͼ������
    uint8 length;                                               // �����ȣ�������Э�鲿�֣�
    uint16 image_width;                                         // ͼ�����
    uint16 image_height;                                        // ͼ��߶�
}seekfree_assistant_camera_struct;          
            
            
typedef struct          
{           
    uint8 head;                                                 // ֡ͷ
    uint8 function;                                             // ������
    uint8 dot_type;                                             // ������  BIT5��1��������16λ�� 0��������8λ��    BIT7-BIT6��0��ֻ��X���� 1��ֻ��Y���� 2��X��Y���궼��    BIT3-BIT0���߽�����
    uint8 length;                                               // �����ȣ�������Э�鲿�֣�
    uint16 dot_num;                                             // ��������
    uint8  valid_flag;                                          // ͨ����ʶ
    uint8  reserve;                                             // ����
}seekfree_assistant_camera_dot_struct;          
            
typedef struct          
{           
    void *image_addr;                                           // ����ͷ��ַ
    uint16 width;                                               // ͼ�����
    uint16 height;                                              // ͼ��߶�
    seekfree_assistant_image_type_enum camera_type;             // ����ͷ����
    void *boundary_x[SEEKFREE_ASSISTANT_CAMERA_MAX_BOUNDARY];   // �߽�����������ַ
    void *boundary_y[SEEKFREE_ASSISTANT_CAMERA_MAX_BOUNDARY];   // �߽������������ַ
}seekfree_assistant_camera_buffer_struct;

typedef struct
{
    uint8 head;                                                 // ֡ͷ
    uint8 function;                                             // ������
    uint8 channel;                                              // ͨ��
    uint8 check_sum;                                            // ��У��
    float data;                                                 // ����
}seekfree_assistant_parameter_struct;

typedef uint32 (*seekfree_assistant_transfer_callback_function) (const uint8 *buff, uint32 length);
typedef uint32 (*seekfree_assistant_receive_callback_function)  (uint8 *buff, uint32 length);

extern seekfree_assistant_oscilloscope_struct                   seekfree_assistant_oscilloscope_data;                                               // ����ʾ��������
extern float                                                    seekfree_assistant_parameter[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT];                // ������յ��Ĳ���
extern vuint8                                                   seekfree_assistant_parameter_update_flag[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT];    // �������±�־λ
    
    
void    seekfree_assistant_oscilloscope_send                    (seekfree_assistant_oscilloscope_struct *seekfree_assistant_oscilloscope);
void    seekfree_assistant_camera_information_config            (seekfree_assistant_image_type_enum camera_type, void *image_addr, uint16 width, uint16 height);
void    seekfree_assistant_camera_boundary_config               (seekfree_assistant_boundary_type_enum boundary_type, uint16 dot_num, void *dot_x1, void *dot_x2, void *dot_x3, void *dot_y1, void *dot_y2, void *dot_y3);
void    seekfree_assistant_camera_send                          (void);
void    seekfree_assistant_data_analysis                        (void);



#endif
