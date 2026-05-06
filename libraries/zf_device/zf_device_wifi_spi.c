/*********************************************************************************************************************
* TC264 Opensourec Library 鍗筹紙TC264 寮�婧愬簱锛夋槸涓�涓熀浜庡畼鏂� SDK 鎺ュ彛鐨勭涓夋柟寮�婧愬簱
* Copyright (c) 2022 SEEKFREE 閫愰绉戞妧
*
* 鏈枃浠舵槸 TC264 寮�婧愬簱鐨勪竴閮ㄥ垎
*
* TC264 寮�婧愬簱 鏄厤璐硅蒋浠�
* 鎮ㄥ彲浠ユ牴鎹嚜鐢辫蒋浠跺熀閲戜細鍙戝竷鐨� GPL锛圙NU General Public License锛屽嵆 GNU閫氱敤鍏叡璁稿彲璇侊級鐨勬潯娆�
* 鍗� GPL 鐨勭3鐗堬紙鍗� GPL3.0锛夋垨锛堟偍閫夋嫨鐨勶級浠讳綍鍚庢潵鐨勭増鏈紝閲嶆柊鍙戝竷鍜�/鎴栦慨鏀瑰畠
*
* 鏈紑婧愬簱鐨勫彂甯冩槸甯屾湜瀹冭兘鍙戞尌浣滅敤锛屼絾骞舵湭瀵瑰叾浣滀换浣曠殑淇濊瘉
* 鐢氳嚦娌℃湁闅愬惈鐨勯�傞攢鎬ф垨閫傚悎鐗瑰畾鐢ㄩ�旂殑淇濊瘉
* 鏇村缁嗚妭璇峰弬瑙� GPL
*
* 鎮ㄥ簲璇ュ湪鏀跺埌鏈紑婧愬簱鐨勫悓鏃舵敹鍒颁竴浠� GPL 鐨勫壇鏈�
* 濡傛灉娌℃湁锛岃鍙傞槄<https://www.gnu.org/licenses/>
*
* 棰濆娉ㄦ槑锛�
* 鏈紑婧愬簱浣跨敤 GPL3.0 寮�婧愯鍙瘉鍗忚 浠ヤ笂璁稿彲鐢虫槑涓鸿瘧鏂囩増鏈�
* 璁稿彲鐢虫槑鑻辨枃鐗堝湪 libraries/doc 鏂囦欢澶逛笅鐨� GPL3_permission_statement.txt 鏂囦欢涓�
* 璁稿彲璇佸壇鏈湪 libraries 鏂囦欢澶逛笅 鍗宠鏂囦欢澶逛笅鐨� LICENSE 鏂囦欢
* 娆㈣繋鍚勪綅浣跨敤骞朵紶鎾湰绋嬪簭 浣嗕慨鏀瑰唴瀹规椂蹇呴』淇濈暀閫愰绉戞妧鐨勭増鏉冨０鏄庯紙鍗虫湰澹版槑锛�
*
* 鏂囦欢鍚嶇О          zf_device_wifi_spi
* 鍏徃鍚嶇О          鎴愰兘閫愰绉戞妧鏈夐檺鍏徃
* 鐗堟湰淇℃伅          鏌ョ湅 libraries/doc 鏂囦欢澶瑰唴 version 鏂囦欢 鐗堟湰璇存槑
* 寮�鍙戠幆澧�          ADS v1.9.20
* 閫傜敤骞冲彴          TC264D
* 搴楅摵閾炬帴          https://seekfree.taobao.com/
* 
* 淇敼璁板綍
* 鏃ユ湡              浣滆��                澶囨敞
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* 鎺ョ嚎瀹氫箟锛�
*                   ------------------------------------
*                   妯″潡绠¤剼            鍗曠墖鏈虹鑴�
*                   RST                 鏌ョ湅 zf_device_wifi_spi.h 涓� WIFI_SPI_RST_PIN 瀹忓畾涔�
*                   INT                 鏌ョ湅 zf_device_wifi_spi.h 涓� WIFI_SPI_INT_PIN 瀹忓畾涔�
*                   CS                  鏌ョ湅 zf_device_wifi_spi.h 涓� WIFI_SPI_CS_PIN 瀹忓畾涔�
*                   MISO                鏌ョ湅 zf_device_wifi_spi.h 涓� WIFI_SPI_MISO_PIN 瀹忓畾涔�
*                   SCK                 鏌ョ湅 zf_device_wifi_spi.h 涓� WIFI_SPI_SCK_PIN 瀹忓畾涔�
*                   MOSI                鏌ョ湅 zf_device_wifi_spi.h 涓� WIFI_SPI_MOSI_PIN 瀹忓畾涔�
*                   5V                  5V 鐢垫簮
*                   GND                 鐢垫簮鍦�
*                   鍏朵綑寮曡剼鎮┖
*                   ------------------------------------
*********************************************************************************************************************/

#include "stdio.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"
#include "zf_driver_spi.h"
#include "zf_device_type.h"

#include "zf_device_wifi_spi.h"

#define WIFI_CONNECT_TIME_OUT       10000       // 鍗曚綅姣
#define SOCKET_CONNECT_TIME_OUT     50000       // 鍗曚綅姣
#define OTHER_TIME_OUT              1000        // 鍗曚綅姣

char wifi_spi_version[12];                      // 淇濆瓨妯″潡鍥轰欢鐗堟湰淇℃伅
char wifi_spi_mac_addr[20];                     // 淇濆瓨妯″潡MAC鍦板潃淇℃伅
char wifi_spi_ip_addr_port[25];                 // 淇濆瓨妯″潡IP鍦板潃涓庣鍙ｄ俊鎭�

static fifo_struct  wifi_spi_fifo;
static uint8        wifi_spi_buffer[WIFI_SPI_RECVIVE_FIFO_SIZE];
static volatile     wifi_spi_state_enum wifi_spi_mutex;
//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     绛夊緟WIFI SPI灏辩华
// 鍙傛暟璇存槑     wait_time       鏈�澶х瓑寰呮椂闂� 鍗曚綅姣
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥     鍐呴儴浣跨敤锛岀敤鎴锋棤闇�鍏冲績
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
static uint8 wifi_spi_wait_idle (uint32 wait_time)
{
    uint32 time = 0;
    
    wait_time = wait_time*100;
    while(0 == gpio_get_level(WIFI_SPI_INT_PIN))
    {
        system_delay_us(10);
        time++;
        if(wait_time <= time)
        {
            break;
        }
    }
    return (wait_time <= time);
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     鍐欏叆鏁版嵁鍒癢IFI SPI
// 鍙傛暟璇存槑     *buffer1        绗竴缁勯渶瑕佸彂閫佺殑鏁版嵁缂撳啿鍖哄湴鍧�
// 鍙傛暟璇存槑     length1         绗竴缁勬暟鎹暱搴�
// 鍙傛暟璇存槑     *buffer2        绗簩缁勯渶瑕佸彂閫佺殑鏁版嵁缂撳啿鍖哄湴鍧�
// 鍙傛暟璇存槑     length2         绗簩缁勬暟鎹暱搴�
// 杩斿洖鍙傛暟     void
// 浣跨敤绀轰緥     鍐呴儴浣跨敤锛岀敤鎴锋棤闇�鍏冲績
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
static void wifi_spi_write (const uint8 *buffer1, uint16 length1, const uint8 *buffer2, uint16 length2)
{
    gpio_low(WIFI_SPI_CS_PIN);
    if(NULL != buffer1)
    {
        spi_write_8bit_array(WIFI_SPI_INDEX, buffer1, length1);
    }
    if(NULL != buffer2)
    {
        spi_write_8bit_array(WIFI_SPI_INDEX, buffer2, length2);
    }
    gpio_high(WIFI_SPI_CS_PIN);
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 鍙戦�佷笌鎺ユ敹鍚屾椂杩涜锛堝懡浠ゆ敹鍙戯級
// 鍙傛暟璇存槑     *packets        鍙戦�佷笌鎺ユ敹鐨勫湴鍧�
// 鍙傛暟璇存槑     length          闇�瑕佹帴鏀剁殑闀垮害
// 杩斿洖鍙傛暟     void
// 浣跨敤绀轰緥     鍐呴儴浣跨敤锛岀敤鎴锋棤闇�鍏冲績
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
static void wifi_spi_transfer_command (wifi_spi_packets_struct *packets, uint16 length)
{
    gpio_low(WIFI_SPI_CS_PIN);
    
    spi_transfer_8bit(WIFI_SPI_INDEX, (uint8 *)&(packets->head), (uint8 *)&(packets->head), sizeof(wifi_spi_head_struct));
    
    if(length)
    {
        spi_transfer_8bit(WIFI_SPI_INDEX, (const uint8 *)(packets->buffer), packets->buffer, length);
    }
    
    gpio_high(WIFI_SPI_CS_PIN);
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 鍙戦�佷笌鎺ユ敹鍚屾椂杩涜(鏁版嵁鏀跺彂)
// 鍙傛暟璇存槑     *write_data     鍙戦�佺殑鏁版嵁缂撳啿鍖哄湴鍧�
// 鍙傛暟璇存槑     *read_data      鎺ユ敹鍒扮殑鏁版嵁鐨勫瓨鍌ㄥ湴鍧�
// 鍙傛暟璇存槑     length          闇�瑕佹帴鏀剁殑闀垮害
// 杩斿洖鍙傛暟     void
// 浣跨敤绀轰緥     鍐呴儴浣跨敤锛岀敤鎴锋棤闇�鍏冲績
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
static void wifi_spi_transfer_data (const uint8 *write_data, wifi_spi_packets_struct *read_data, uint16 length)
{
    gpio_low(WIFI_SPI_CS_PIN);
    
    read_data->head.command = WIFI_SPI_DATA;
    read_data->head.length  = length;
    
    spi_transfer_8bit(WIFI_SPI_INDEX, (uint8 *)&(read_data->head), (uint8 *)&(read_data->head), sizeof(wifi_spi_head_struct));
    
    if(WIFI_SPI_RECVIVE_SIZE < length)
    {
        spi_transfer_8bit(WIFI_SPI_INDEX, write_data, read_data->buffer, WIFI_SPI_RECVIVE_SIZE);
        spi_write_8bit_array(WIFI_SPI_INDEX, &write_data[WIFI_SPI_RECVIVE_SIZE], length - WIFI_SPI_RECVIVE_SIZE);
    }
    else
    {
        // 灏嗛渶瑕佸彂閫佺殑鏁版嵁鎷疯礉鍒拌鍙栫紦鍐插尯锛岄伩鍏嶅嚭鐜皐rite_data瓒婄晫璁块棶
        memcpy(read_data->buffer, write_data, length);
        spi_transfer_8bit(WIFI_SPI_INDEX, read_data->buffer, read_data->buffer, WIFI_SPI_RECVIVE_SIZE);
    }
    gpio_high(WIFI_SPI_CS_PIN);
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 鍙傛暟璁剧疆
// 鍙傛暟璇存槑     command         鍛戒护绫诲瀷
// 鍙傛暟璇存槑     *buffer         鍙傛暟鍦板潃
// 鍙傛暟璇存槑     length          鍙傛暟闀垮害
// 鍙傛暟璇存槑     wait_time       鏈�澶х瓑寰呮椂闂� 鍗曚綅100寰
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥     鍐呴儴浣跨敤锛岀敤鎴锋棤闇�鍏冲績
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
static uint8 wifi_spi_set_parameter (wifi_spi_packets_command_enum command, uint8 *buffer, uint16 length, uint32 wait_time)
{
    uint8 return_state;
    wifi_spi_head_struct head;
    return_state = 1;
    do
    {
        head.command = command;
        head.length  = length;
        
        // 绛夊緟浠庢満鍑嗗灏辩华
        if(wifi_spi_wait_idle(wait_time))
        {
            break;
        }

        wifi_spi_write(&head.command, sizeof(wifi_spi_head_struct), buffer, length);
        if(wifi_spi_wait_idle(wait_time))
        {
            break;
        }
        // 鎺ユ敹搴旂瓟淇″彿

        head.command = WIFI_SPI_DATA;
        head.length = 0;
        wifi_spi_transfer_command((wifi_spi_packets_struct *)&head, head.length);
        system_delay_us(20);
        if(WIFI_SPI_REPLY_OK == head.command)
        {
            return_state = 0;
        }
    }while(0);
    
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 妯″潡淇℃伅鑾峰彇
// 鍙傛暟璇存槑     command         鍛戒护绫诲瀷
// 鍙傛暟璇存槑     *buffer         淇濆瓨鎺ユ敹鍒扮殑鍙傛暟鍦板潃
// 鍙傛暟璇存槑     wait_time       鏈�澶х瓑寰呮椂闂� 鍗曚綅100寰
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥     鍐呴儴浣跨敤锛岀敤鎴锋棤闇�鍏冲績
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
static uint8 wifi_spi_get_parameter (wifi_spi_packets_command_enum command, wifi_spi_packets_struct *read_data, uint32 wait_time)
{
    uint8 return_state;

    return_state = 1;
    do
    {
        // 绛夊緟浠庢満鍑嗗灏辩华
        if(wifi_spi_wait_idle(wait_time))
        {
            break;
        }
        read_data->head.command = command;
        wifi_spi_write(&(read_data->head.command), WIFI_SPI_RECVIVE_SIZE, NULL, 0);

        if(wifi_spi_wait_idle(wait_time))
        {
            break;
        }
        read_data->head.command = WIFI_SPI_DATA;
        read_data->head.length = 0;
        wifi_spi_transfer_command(read_data, WIFI_SPI_RECVIVE_SIZE);
        return_state = 0;
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 鍥轰欢鐗堟湰鑾峰彇
// 鍙傛暟璇存槑     void            绔彛鍙�
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥
// 澶囨敞淇℃伅     璋冪敤鍑芥暟涔嬪悗锛屽浐浠剁増鏈俊鎭互瀛楃涓插舰寮忎繚瀛樺湪wifi_spi_version鏁扮粍涓�
//-------------------------------------------------------------------------------------------------------------------
static uint8 wifi_spi_get_version (void)
{
    uint8 return_state;
    wifi_spi_packets_struct temp_packets;

    return_state = wifi_spi_get_parameter(WIFI_SPI_GET_VERSION, &temp_packets, OTHER_TIME_OUT);
    if((0 == return_state) && (WIFI_SPI_REPLY_VERSION == temp_packets.head.command))
    {
        memcpy(wifi_spi_version, temp_packets.buffer, temp_packets.head.length);
    }
    return_state = (return_state == 0) ? (WIFI_SPI_REPLY_VERSION != temp_packets.head.command) : 1;

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI MAC鍦板潃鑾峰彇
// 鍙傛暟璇存槑     void            绔彛鍙�
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥
// 澶囨敞淇℃伅     璋冪敤鍑芥暟涔嬪悗锛孧AC鍦板潃淇℃伅浠ュ瓧绗︿覆褰㈠紡淇濆瓨鍦╳ifi_spi_mac_addr鏁扮粍涓�
//-------------------------------------------------------------------------------------------------------------------
static uint8 wifi_spi_get_mac_addr (void)
{
    uint8 return_state;
    wifi_spi_packets_struct temp_packets;

    return_state = wifi_spi_get_parameter(WIFI_SPI_GET_MAC_ADDR, &temp_packets, OTHER_TIME_OUT);
    if((0 == return_state) && (WIFI_SPI_REPLY_MAC_ADDR == temp_packets.head.command))
    {
        memcpy(wifi_spi_mac_addr, temp_packets.buffer, temp_packets.head.length);
    }
    return_state = (return_state == 0) ? (WIFI_SPI_REPLY_MAC_ADDR != temp_packets.head.command) : 1;

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI IP鍦板潃涓庣鍙ｅ彿鑾峰彇
// 鍙傛暟璇存槑     void            绔彛鍙�
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥
// 澶囨敞淇℃伅     璋冪敤鍑芥暟涔嬪悗锛孖P鍦板潃涓庣鍙ｅ彿淇℃伅浠ュ瓧绗︿覆褰㈠紡淇濆瓨鍦╳ifi_spi_ip_addr_port鏁扮粍涓�
//              闇�瑕佸湪杩炴帴Socket涔嬪悗璋冪敤姝ゅ嚱鏁版墠鑳芥甯歌幏鍙栦俊鎭�
//-------------------------------------------------------------------------------------------------------------------
static uint8 wifi_spi_get_ip_addr_port (void)
{
    uint8 return_state;
    wifi_spi_packets_struct temp_packets;

    return_state = wifi_spi_get_parameter(WIFI_SPI_GET_IP_ADDR, &temp_packets, OTHER_TIME_OUT);
    if((0 == return_state) && (WIFI_SPI_REPLY_IP_ADDR == temp_packets.head.command))
    {
        memcpy(wifi_spi_ip_addr_port, temp_packets.buffer, temp_packets.head.length);
    }
    return_state = (return_state == 0) ? (WIFI_SPI_REPLY_IP_ADDR != temp_packets.head.command) : 1;

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 璁剧疆杩炴帴鐨刉iFi淇℃伅骞跺皾璇曡繛鎺iFi
// 鍙傛暟璇存槑     *wifi_ssid      WIFI鍚嶇О
// 鍙傛暟璇存槑     *pass_word      WIFI瀵嗙爜
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥     wifi_spi_wifi_connect("SEEKFREE", "SEEKFREE123");
// 澶囨敞淇℃伅     wifi_spi_wifi_connect("SEEKFREE", NULL); // 杩炴帴娌℃湁瀵嗙爜鐨刉IFI鐑偣
//-------------------------------------------------------------------------------------------------------------------
uint8 wifi_spi_wifi_connect (char *wifi_ssid, char *pass_word)
{
    uint8 return_state;
    uint8 temp_buffer[64];
    uint16 length;
    
    if(NULL != pass_word)
    {
        // WIFI鐑偣鏈夊瘑鐮佸彂閫佺儹鐐瑰悕绉颁笌瀵嗙爜
        length = (uint16)sprintf((char *)temp_buffer, "%s\r\n%s\r\n", wifi_ssid, pass_word);
    }
    else
    {
        // WIFI鐑偣娌℃湁瀵嗙爜鍙渶瑕佸彂閫佺儹鐐瑰悕绉�
        length = (uint16)sprintf((char *)temp_buffer, "%s\r\n", wifi_ssid);
    }

    return_state = wifi_spi_set_parameter(WIFI_SPI_SET_WIFI_INFORMATION, temp_buffer, length, WIFI_CONNECT_TIME_OUT);

    // 鏈満IP鍦板潃涓庣鍙ｅ彿淇℃伅浠ュ瓧绗︿覆褰㈠紡淇濆瓨鍦╳ifi_spi_ip_addr_port鏁扮粍涓�
    wifi_spi_get_ip_addr_port();

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 璁剧疆杩炴帴鐨凷ocket淇℃伅骞跺皾璇曡繛鎺ocket
// 鍙傛暟璇存槑     *transport_type 浼犺緭绫诲瀷
// 鍙傛暟璇存槑     *ip_addr        IP鍦板潃
// 鍙傛暟璇存槑     *port           鐩爣绔彛鍙�
// 鍙傛暟璇存槑     *local_port     鏈満绔彛鍙�
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥     wifi_spi_socket_connect("TCP", "192.168.2.5", "8080", "6060");
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
uint8 wifi_spi_socket_connect (char *transport_type, char *ip_addr, char *port, char *local_port)
{
    uint8 return_state;
    uint8 temp_buffer[41];
    uint16 length;
    
    length = (uint16)sprintf((char *)temp_buffer, "%s\r\n%s\r\n%s\r\n%s\r\n", transport_type, ip_addr, port, local_port);

    return_state = wifi_spi_set_parameter(WIFI_SPI_SET_SOCKET_INFORMATION, temp_buffer, length, SOCKET_CONNECT_TIME_OUT);

    // 鏈満IP鍦板潃涓庣鍙ｅ彿淇℃伅浠ュ瓧绗︿覆褰㈠紡淇濆瓨鍦╳ifi_spi_ip_addr_port鏁扮粍涓�
    wifi_spi_get_ip_addr_port();

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 鏂紑Socket杩炴帴
// 鍙傛暟璇存槑     void
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥     wifi_spi_socket_disconnect();
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
uint8 wifi_spi_socket_disconnect (void)
{
    wifi_spi_packets_struct temp_packets;

    return wifi_spi_get_parameter(WIFI_SPI_CLOSE_SOCKET, &temp_packets, OTHER_TIME_OUT);
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 杞浣�
// 鍙傛暟璇存槑     void
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
uint8 wifi_spi_reset (void)
{
    uint8 return_state;
    wifi_spi_head_struct head;
    return_state = 1;
    do
    {
        head.command = WIFI_SPI_RESET;
        head.length  = 0xA5A5;
        return_state = wifi_spi_wait_idle(OTHER_TIME_OUT);
        if(return_state)
        {
            break;
        }
        wifi_spi_write(&head.command, sizeof(wifi_spi_head_struct), NULL, 0);
    }while(0);
    
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI UDP妯″紡鏃剁珛鍗冲彂閫佸嚱鏁�
// 鍙傛暟璇存槑     void
// 杩斿洖鍙傛暟     uint8           鐘舵�� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥
// 澶囨敞淇℃伅     鍦║DP妯″紡涓嬫ā鍧楁敹鍒版暟鎹悗浼氱瓑寰�2姣锛�2姣鍚庢湭鏀跺埌鏁版嵁鍒欏皢鏁版嵁閫氳繃socket鍙戦�佸埌缃戠粶锛屽鏋滃笇鏈涚珛鍗冲彂閫佸垯鍦ㄦ暟鎹紶杈撳畬姣曞悗璋冪敤姝ゅ嚱鏁�
//-------------------------------------------------------------------------------------------------------------------
uint8 wifi_spi_udp_send_now (void)
{
    uint8 return_state = 1;
    wifi_spi_packets_struct temp_packets;
    
    if(WIFI_SPI_IDLE == wifi_spi_mutex)
    {
        // 灏嗛�氳鐘舵�佽缃负蹇�
        wifi_spi_mutex = WIFI_SPI_BUSY;
        do
        {
            if(wifi_spi_wait_idle(OTHER_TIME_OUT))
            {
                break;
            }

            // 绔嬪嵆寮�濮媠ocket鍙戦��
            temp_packets.head.command = WIFI_SPI_UDP_SEND;
            temp_packets.head.length = 0;
            wifi_spi_transfer_command(&temp_packets, WIFI_SPI_RECVIVE_SIZE);
            
            // 妫�鏌ユ敹鍒扮殑鍖呬腑鏄惁鏈夋暟鎹�
            if((WIFI_SPI_REPLY_DATA_START == temp_packets.head.command) || (WIFI_SPI_REPLY_DATA_END == temp_packets.head.command))
            {
                // 淇濆瓨鎺ユ敹鍒扮殑鏁版嵁
                if(temp_packets.head.length)
                {
                    fifo_write_buffer(&wifi_spi_fifo, temp_packets.buffer, temp_packets.head.length);
                }
            }
            
            // 绛夊緟搴旂瓟淇″彿
            if(wifi_spi_wait_idle(OTHER_TIME_OUT))
            {
                break;
            }
            
            // 鎺ユ敹搴旂瓟淇″彿
            temp_packets.head.command = WIFI_SPI_DATA;
            temp_packets.head.length = 0;
            wifi_spi_transfer_command(&temp_packets, temp_packets.head.length);
            
            if(WIFI_SPI_REPLY_OK == temp_packets.head.command)
            {
                return_state = 0;
            }
            
        }while(0);
        
        // 灏嗛�氳鐘舵�佽缃负绌洪棽
        wifi_spi_mutex = WIFI_SPI_IDLE;
    } 
    
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 鏁版嵁鍧楀彂閫佸嚱鏁板苟鍚屾鎺ユ敹鏁版嵁
// 鍙傛暟璇存槑     *buff           闇�瑕佸彂閫佺殑鏁版嵁鍦板潃
// 鍙傛暟璇存槑     length          鍙戦�侀暱搴�
// 杩斿洖鍙傛暟     uint32          鍓╀綑鏈彂閫佺殑闀垮害
// 浣跨敤绀轰緥     wifi_spi_send_buffer(buffer, 100);
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
uint32 wifi_spi_send_buffer (const uint8 *buffer, uint32 length)
{
    uint16 send_length;
    wifi_spi_packets_struct temp_packets;
    
    // 妫�鏌IFI SPI鐘舵�侊紝濡傛灉鍦ㄥ叾浠栦腑鏂垨鑰呯嚎绋嬩腑宸茬粡鍙戣捣浜嗛�氳锛屽垯鏈涓嶈兘鍙戦�佹暟鎹�
    if(WIFI_SPI_IDLE == wifi_spi_mutex)
    {
        // 灏嗛�氳鐘舵�佽缃负蹇�
        wifi_spi_mutex = WIFI_SPI_BUSY;
        
        while(length)
        {
            send_length = length > WIFI_SPI_TRANSFER_SIZE ? (uint16)WIFI_SPI_TRANSFER_SIZE : (uint16)length;
            
            if(wifi_spi_wait_idle(OTHER_TIME_OUT))
            {
                break;
            }
            
            wifi_spi_transfer_data(buffer, &temp_packets, send_length);
            
            // 妫�鏌ユ敹鍒扮殑鍖呬腑鏄惁鏈夋暟鎹�
            if((WIFI_SPI_REPLY_DATA_START == temp_packets.head.command) || (WIFI_SPI_REPLY_DATA_END == temp_packets.head.command))
            {
                // 淇濆瓨鎺ユ敹鍒扮殑鏁版嵁
                if(temp_packets.head.length)
                {
                    fifo_write_buffer(&wifi_spi_fifo, temp_packets.buffer, temp_packets.head.length);
                }
            }
            
            length -= send_length;
            buffer += send_length;
        }
        
        // 妫�鏌ユ渶鍚庝竴娆＄殑鎺ユ敹鏄惁灏嗘墍鏈夌殑鏁版嵁閮芥帴鏀跺畬姣�
        while(WIFI_SPI_REPLY_DATA_START == temp_packets.head.command)
        {
            if(wifi_spi_wait_idle(OTHER_TIME_OUT))
            {
                break;
            }
            
            // 缁х画璇诲彇妯″潡鍓╀綑鏁版嵁
            temp_packets.head.command = WIFI_SPI_DATA;
            temp_packets.head.length  = 0;
            wifi_spi_transfer_command(&temp_packets, WIFI_SPI_RECVIVE_SIZE);
            // 妫�鏌ユ敹鍒扮殑鍖呬腑鏄惁鏈夋暟鎹�
            if((WIFI_SPI_REPLY_DATA_START == temp_packets.head.command) || (WIFI_SPI_REPLY_DATA_END == temp_packets.head.command))
            {
                // 淇濆瓨鎺ユ敹鍒扮殑鏁版嵁
                if(temp_packets.head.length)
                {
                    fifo_write_buffer(&wifi_spi_fifo, temp_packets.buffer, temp_packets.head.length);
                }
            }
        }
        wifi_spi_mutex = WIFI_SPI_IDLE;
    }
    return length;
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WIFI SPI 璇诲彇缂撳啿鍖�
// 鍙傛暟璇存槑     *buff           鎺ユ敹缂撳啿鍖�
// 鍙傛暟璇存槑     length          璇诲彇鏁版嵁闀垮害
// 杩斿洖鍙傛暟     uint32          瀹為檯璇诲彇鏁版嵁闀垮害
// 浣跨敤绀轰緥     wifi_spi_read_buffer(buffer, 100);
// 澶囨敞淇℃伅
//-------------------------------------------------------------------------------------------------------------------
uint32 wifi_spi_read_buffer (uint8 *buffer, uint32 length)
{
    zf_assert(NULL != buffer);
    uint32 data_len = length;
    
#if(1 == WIFI_SPI_READ_TRANSFER)
    
    wifi_spi_packets_struct temp_packets;
    // 妫�鏌IFI SPI鐘舵�侊紝濡傛灉鍦ㄥ叾浠栦腑鏂垨鑰呯嚎绋嬩腑宸茬粡鍙戣捣浜嗛�氳锛屽垯鏈涓嶈兘鍙戦�佹暟鎹�
    if(WIFI_SPI_IDLE == wifi_spi_mutex)
    {
        // 灏嗛�氳鐘舵�佽缃负蹇�
        wifi_spi_mutex = WIFI_SPI_BUSY;
        
        // 鍙戣捣閫氳鏌ョ湅妯″潡鍐呮槸鍚︽湁鏁版嵁鏈鍙�
        do
        {
            if(wifi_spi_wait_idle(OTHER_TIME_OUT))
            {
                break;
            }
            temp_packets.head.command = WIFI_SPI_DATA;
            temp_packets.head.length  = 0;
            wifi_spi_transfer_command(&temp_packets, WIFI_SPI_RECVIVE_SIZE);
            // 妫�鏌ユ敹鍒扮殑鍖呬腑鏄惁鏈夋暟鎹�
            if((WIFI_SPI_REPLY_DATA_START == temp_packets.head.command) || (WIFI_SPI_REPLY_DATA_END == temp_packets.head.command))
            {
                // 淇濆瓨鎺ユ敹鍒扮殑鏁版嵁
                if(temp_packets.head.length)
                {
                    fifo_write_buffer(&wifi_spi_fifo, temp_packets.buffer, temp_packets.head.length);
                }
            }
        }while(WIFI_SPI_REPLY_DATA_START == temp_packets.head.command);
        wifi_spi_mutex = WIFI_SPI_IDLE;
    }
#endif 
    
    fifo_read_buffer(&wifi_spi_fifo, buffer, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WIFI SPI 字符串发送函数并同步接收数据
// 参数说明     *string           需要发送的字符串
// 返回参数     void
// 使用示例     wifi_spi_send_string("123");
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void wifi_spi_send_string(const char *format, ...)
{
    char buffer[256];            // 根据最大消息长度调整
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    wifi_spi_send_buffer((uint8*)buffer, strlen(buffer));
}

//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     WiFi 妯″潡鍒濆鍖�
// 鍙傛暟璇存槑     *wifi_ssid      鐩爣杩炴帴鐨� WiFi 鐨勫悕绉� 瀛楃涓插舰寮�
// 鍙傛暟璇存槑     *pass_word      鐩爣杩炴帴鐨� WiFi 鐨勫瘑鐮� 瀛楃涓插舰寮�
// 杩斿洖鍙傛暟     uint8           妯″潡鍒濆鍖栫姸鎬� 0-鎴愬姛 1-閿欒
// 浣跨敤绀轰緥     wifi_spi_init("SEEKFREE", "SEEKFREE123");
// 澶囨敞淇℃伅     wifi_spi_init("SEEKFREE", NULL); // 杩炴帴娌℃湁瀵嗙爜鐨刉IFI鐑偣
//-------------------------------------------------------------------------------------------------------------------
uint8 wifi_spi_init (char *wifi_ssid, char *pass_word)
{
    uint8 return_state = 0;
    
    fifo_init(&wifi_spi_fifo, FIFO_DATA_8BIT, wifi_spi_buffer, WIFI_SPI_RECVIVE_FIFO_SIZE);
    spi_init(WIFI_SPI_INDEX, SPI_MODE0, WIFI_SPI_SPEED, WIFI_SPI_SCK_PIN, WIFI_SPI_MOSI_PIN, WIFI_SPI_MISO_PIN, SPI_CS_NULL);//纭欢SPI鍒濆鍖�
    gpio_init(WIFI_SPI_CS_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(WIFI_SPI_RST_PIN, GPO, 1, GPO_PUSH_PULL);
    gpio_init(WIFI_SPI_INT_PIN, GPI, 0, GPI_PULL_DOWN);
    
    // 澶嶄綅
    gpio_set_level(WIFI_SPI_RST_PIN, 0);
    system_delay_ms(10);
    gpio_set_level(WIFI_SPI_RST_PIN, 1);
    
    // 绛夊緟妯″潡鍒濆鍖�
    system_delay_ms(100);
    wifi_spi_mutex = WIFI_SPI_IDLE;

    do
    {
        // 鍥轰欢鐗堟湰淇℃伅浠ュ瓧绗︿覆褰㈠紡淇濆瓨鍦╳ifi_spi_version鏁扮粍涓�
        return_state = wifi_spi_get_version();
        if(return_state)
        {
            break;
        }

        // MAC鍦板潃淇℃伅浠ュ瓧绗︿覆褰㈠紡淇濆瓨鍦╳ifi_spi_mac_addr鏁扮粍涓�
        wifi_spi_get_mac_addr();


        return_state = wifi_spi_wifi_connect(wifi_ssid, pass_word);
        if(return_state)
        {
            break;
        }
        
    #if(1 == WIFI_SPI_AUTO_CONNECT)
        return_state = wifi_spi_socket_connect("TCP", WIFI_SPI_TARGET_IP, WIFI_SPI_TARGET_PORT, WIFI_SPI_LOCAL_PORT);
        if(return_state)
        {
            break;
        }
    #endif
        
    #if(2 == WIFI_SPI_AUTO_CONNECT)
        return_state = wifi_spi_socket_connect("UDP", WIFI_SPI_TARGET_IP, WIFI_SPI_TARGET_PORT, WIFI_SPI_LOCAL_PORT);
        if(return_state)
        {
            break;
        }
    #endif
    }while(0);

    return return_state;
}
