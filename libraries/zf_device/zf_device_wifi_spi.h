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
* 寮�鍙戠幆澧�          ADS v1.10.2
* 閫傜敤骞冲彴          TC264D
* 搴楅摵閾炬帴          https://seekfree.taobao.com/
* 
* 淇敼璁板綍
* 鏃ユ湡              浣滆��                澶囨敞
* 2024-01-18        SeekFree            first version
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

#ifndef _zf_device_wifi_spi_h
#define _zf_device_wifi_spi_h

#include "zf_common_typedef.h"

          
#define WIFI_SPI_INDEX              (SPI_3             )        // 瀹氫箟浣跨敤鐨凷PI鍙�
#define WIFI_SPI_SPEED              (30 * 1000 * 1000  )        // 纭欢 SPI 閫熺巼
#define WIFI_SPI_SCK_PIN            (SPI3_SCLK_P22_3   )        // 瀹氫箟SPI_SCK寮曡剼
#define WIFI_SPI_MOSI_PIN           (SPI3_MOSI_P22_0   )        // 瀹氫箟SPI_MOSI寮曡剼
#define WIFI_SPI_MISO_PIN           (SPI3_MISO_P22_1   )        // 瀹氫箟SPI_MISO寮曡剼  IPS娌℃湁MISO寮曡剼锛屼絾鏄繖閲屼换鐒堕渶瑕佸畾涔夛紝鍦╯pi鐨勫垵濮嬪寲鏃堕渶瑕佷娇鐢�
#define WIFI_SPI_CS_PIN             (P22_2             )        // 瀹氫箟SPI_CS寮曡剼 閲囩敤杞欢CS寮曡剼
#define WIFI_SPI_INT_PIN            (P15_8             )        // 瀹氫箟鎻℃墜寮曡剼
#define WIFI_SPI_RST_PIN            (P23_1             )        // 瀹氫箟澶嶄綅寮曡剼


#define WIFI_SPI_RECVIVE_FIFO_SIZE  (1024)                      // 鎺ユ敹FIFO澶у皬
#define WIFI_SPI_READ_TRANSFER      (1)                         // 鍦ㄨ皟鐢╳ifi_spi_read_buffer 鏄惁灏濊瘯鍙戣捣SPI閫氳鏉ユ娴嬫ā鍧楀唴鏄惁鏈夋暟鎹渶瑕佽鍙� 1锛氬彂璧稴PI閫氳 0锛氫笉鍙戣捣SPI閫氳锛屼粎璇诲彇FIFO
                                                                // 濡傛灉搴旂敤绋嬪簭涓病鏈変换浣曠殑鍦版柟璋冪敤鍙戦�佸嚱鏁帮紝鍒橶IFI_SPI_READ_TRANSFER蹇呴』璁剧疆涓�1
                                                                
#define WIFI_SPI_AUTO_CONNECT       (0)                         // 瀹氫箟鏄惁鍒濆鍖栨椂寤虹珛TCP鎴栬�匲DP杩炴帴    0-涓嶈嚜鍔ㄨ繛鎺�  1-鑷姩杩炴帴TCP鏈嶅姟鍣�  2-鑷姩杩炴帴UDP

#if     (WIFI_SPI_AUTO_CONNECT > 2)    
#error "WIFI_SPI_AUTO_CONNECT 鐨勫�煎彧鑳戒负 [0,1,2]"
#else   
#define WIFI_SPI_TARGET_IP          "192.168.137.1"              // 杩炴帴鐩爣鐨� IP
#define WIFI_SPI_TARGET_PORT        "8080"                      // 杩炴帴鐩爣鐨勭鍙�
#define WIFI_SPI_LOCAL_PORT         "6666"                      // 鏈満鐨勭鍙� 0锛氶殢鏈�  鍙缃寖鍥�2048-65535  榛樿 6666
#endif


#define WIFI_SPI_RECVIVE_SIZE       (32)                        // 姣忔SPI浼犺緭鎺ユ敹鐨勫瓧鑺傛暟 涓嶅厑璁镐慨鏀�
#define WIFI_SPI_TRANSFER_SIZE      (4088)                      // 鏈�澶PI浼犺緭鎺ユ敹鐨勫瓧鑺傛暟 涓嶅厑璁镐慨鏀�



typedef enum
{
    // 涓绘満鍙戦�佺殑鍛戒护
    WIFI_SPI_INVALID1               = 0x00,                     // 鏃犳晥鏁版嵁鍖�
    WIFI_SPI_RESET                  = 0x01,                     // 澶嶄綅鍛戒护
    WIFI_SPI_DATA                   = 0x02,                     // 閫忎紶鏁版嵁鍖�
    WIFI_SPI_UDP_SEND               = 0x03,                     // UDP涓嬬珛鍗冲彂閫佸懡浠�,榛樿SPI鎺ユ敹鏁版嵁鍚�2MS鏈敹鍒版暟鎹嚜鍔ㄥ彂閫佹暟鎹�
    WIFI_SPI_CLOSE_SOCKET           = 0x04,                     // 鏂紑杩炴帴
                
    WIFI_SPI_SET_WIFI_INFORMATION   = 0x10,                     // 璁剧疆WIFI淇℃伅鍛戒护
    WIFI_SPI_SET_SOCKET_INFORMATION = 0x11,                     // 璁剧疆SOCKET淇℃伅鍛戒护
                    
    WIFI_SPI_GET_VERSION            = 0x20,                     // 鑾峰彇妯″潡鐗堟湰
    WIFI_SPI_GET_MAC_ADDR           = 0x21,                     // 鑾峰彇妯″潡MAC鍦板潃
    WIFI_SPI_GET_IP_ADDR            = 0x22,                     // 鑾峰彇妯″潡IP鍦板潃
                    
    // 浠庢満鍥炰紶鐨勫懡浠�
    WIFI_SPI_REPLY_OK               = 0x80,                     // 浠庢満搴旂瓟鐨勬纭懡浠�
    WIFI_SPI_REPLY_ERROR            = 0x81,                     // 浠庢満搴旂瓟鐨勯敊璇懡浠�
                    
    WIFI_SPI_REPLY_DATA_START       = 0x90,                     // 浠庢満鍥炰紶鐨勬暟鎹寘锛屽苟涓旇繕鏈夋暟鎹渶瑕佷富鏈鸿鍙�
    WIFI_SPI_REPLY_DATA_END         = 0x91,                     // 浠庢満鍥炰紶鐨勬暟鎹寘锛屾暟鎹凡璇诲彇瀹屾瘯
                    
    WIFI_SPI_REPLY_VERSION          = 0xA0,                     // 浠庢満鍥炲鍥轰欢鐗堟湰
    WIFI_SPI_REPLY_MAC_ADDR         = 0xA1,                     // 浠庢満鍥炲鏈満MAC鍦板潃绛変俊鎭�
    WIFI_SPI_REPLY_IP_ADDR          = 0xA2,                     // 浠庢満鍥炲鏈満IP鍦板潃銆佺鍙ｅ彿
    WIFI_SPI_INVALID2               = 0xFF                      // 鏃犳晥鏁版嵁鍖�
}wifi_spi_packets_command_enum;             
                
typedef enum                
{               
    WIFI_SPI_IDLE,                                              // 妯″潡绌洪棽锛屽彲浠ヨ繘琛孲PI閫氳
    WIFI_SPI_BUSY,                                              // 妯″潡姝ｅ繖锛屼笉鍙繘琛孲PI閫氳
}wifi_spi_state_enum;               
                
                
typedef struct              
{               
    uint8   command;                                            // 鍛戒护瀛�
    uint8   reserve;                                            // 淇濈暀
    uint16  length;                                             // 鍖呮湁鏁堥暱搴�
}wifi_spi_head_struct;              
                
                
typedef struct              
{               
    wifi_spi_head_struct  head;                                 // 甯уご
    uint8 buffer[WIFI_SPI_RECVIVE_SIZE];                        // 缂撳啿鍖�
}wifi_spi_packets_struct;               
                
                
extern char wifi_spi_version[12];                               // 鍥轰欢鐗堟湰         瀛楃涓�
extern char wifi_spi_mac_addr[20];                              // 妯″潡MAC鍦板潃      瀛楃涓�
extern char wifi_spi_ip_addr_port[25];                          // IP鍦板潃涓庣鍙ｅ彿   瀛楃涓�

uint8  wifi_spi_wifi_connect        (char *wifi_ssid, char *pass_word);
uint8  wifi_spi_socket_connect      (char *transport_type, char *ip_addr, char *port, char *local_port);
uint8  wifi_spi_socket_disconnect   (void);
uint8  wifi_spi_udp_send_now        (void);
uint32 wifi_spi_send_buffer         (const uint8 *buff, uint32 length);
uint32 wifi_spi_read_buffer         (uint8 *buffer, uint32 length);
void wifi_spi_send_string           (const char *format, ...);
uint8  wifi_spi_init                (char *wifi_ssid, char *pass_word);

#endif

