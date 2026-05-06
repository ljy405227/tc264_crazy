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
* �ļ�����          seekfree_assistant_interface
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

#ifndef _seekfree_assistant_interface_h_
#define _seekfree_assistant_interface_h_



// ���ݷ����豸ö��
typedef enum
{
    SEEKFREE_ASSISTANT_DEBUG_UART,      // ���Դ���    ʹ�õĴ�����DEBUG_UART_INDEX�궨��ָ��
    SEEKFREE_ASSISTANT_WIRELESS_UART,   // ����ת����
    SEEKFREE_ASSISTANT_BLE6A20,         // ����ת����
    SEEKFREE_ASSISTANT_CH9141,          // CH9141����
    SEEKFREE_ASSISTANT_WIFI_UART,       // WIFIת����
    SEEKFREE_ASSISTANT_WIFI_SPI,        // ����WIFI SPI
    SEEKFREE_ASSISTANT_CUSTOM,          // �Զ���ͨѶ��ʽ ��Ҫ����seekfree_assistant_transfer��seekfree_assistant_receive����
}seekfree_assistant_transfer_device_enum;


void    seekfree_assistant_interface_init   (seekfree_assistant_transfer_device_enum transfer_device);



#endif
