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
* 鏂囦欢鍚嶇О          zf_device_ips200
* 鍏徃鍚嶇О          鎴愰兘閫愰绉戞妧鏈夐檺鍏徃
* 鐗堟湰淇℃伅          鏌ョ湅 libraries/doc 鏂囦欢澶瑰唴 version 鏂囦欢 鐗堟湰璇存槑
* 寮�鍙戠幆澧�          ADS v1.10.2
* 閫傜敤骞冲彴          TC264D
* 搴楅摵閾炬帴          https://seekfree.taobao.com/
*
* 淇敼璁板綍
* 鏃ユ湡              浣滆��                澶囨敞
* 2022-09-15       pudding            first version
* 2023-04-28       pudding            澧炲姞涓枃娉ㄩ噴璇存槑
* 2025-02-08       pudding            淇敼閮ㄥ垎娉ㄩ噴 绉婚櫎鈥滃崟鎺掓帓閽堚�濃�滃弻鎺掓帓閽堚�濆懡鍚嶈鑼�
********************************************************************************************************************/
/********************************************************************************************************************
* 鎺ョ嚎瀹氫箟锛�
*                  ------------------------------------
*                  妯″潡绠¤剼             鍗曠墖鏈虹鑴�
*                  // 鍏綅骞跺彛 骞跺彛涓ゅ灞� 纭欢寮曡剼
*                  RD                 鏌ョ湅 zf_device_ips200.h 涓� IPS200_RD_PIN_PARALLEL8     瀹忓畾涔�
*                  WR                 鏌ョ湅 zf_device_ips200.h 涓� IPS200_WR_PIN_PARALLEL8     瀹忓畾涔�
*                  RS                 鏌ョ湅 zf_device_ips200.h 涓� IPS200_RS_PIN_PARALLEL8     瀹忓畾涔�
*                  RST                鏌ョ湅 zf_device_ips200.h 涓� IPS200_RST_PIN_PARALLEL8    瀹忓畾涔�
*                  CS                 鏌ョ湅 zf_device_ips200.h 涓� IPS200_CS_PIN_PARALLEL8     瀹忓畾涔�
*                  BL                 鏌ョ湅 zf_device_ips200.h 涓� IPS200_BL_PIN_PARALLEL8     瀹忓畾涔�
*                  D0-D7              鏌ョ湅 zf_device_ips200.h 涓� IPS200_Dx_PIN_PARALLEL8     瀹忓畾涔�
*                  VCC                3.3V鐢垫簮
*                  GND                鐢垫簮鍦�
*
*                  // SPI 涓插彛 SPI 涓ゅ灞� 纭欢寮曡剼
*                  SCL                鏌ョ湅 zf_device_ips200.h 涓� IPS200_SCL_PIN_SPI  瀹忓畾涔�
*                  SDA                鏌ョ湅 zf_device_ips200.h 涓� IPS200_SDA_PIN_SPI  瀹忓畾涔�
*                  RST                鏌ョ湅 zf_device_ips200.h 涓� IPS200_RST_PIN_SPI  瀹忓畾涔�
*                  DC                 鏌ョ湅 zf_device_ips200.h 涓� IPS200_DC_PIN_SPI   瀹忓畾涔�
*                  CS                 鏌ョ湅 zf_device_ips200.h 涓� IPS200_CS_PIN_SPI   瀹忓畾涔�
*                  BLk                鏌ョ湅 zf_device_ips200.h 涓� IPS200_BLk_PIN_SPI  瀹忓畾涔�
*                  VCC                3.3V鐢垫簮
*                  GND                鐢垫簮鍦�
*                  鏈�澶у垎杈ㄧ巼 320 * 240
*                  ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_ips200_h_
#define _zf_device_ips200_h_

#include "zf_common_typedef.h"

//==================================================瀹氫箟 IPS200 鍩烘湰閰嶇疆================================================
#define IPS200_USE_SOFT_SPI             (0 )                                    // 榛樿浣跨敤纭欢 SPI 鏂瑰紡椹卞姩 寤鸿浣跨敤纭欢 SPI 鏂瑰紡椹卞姩
#if IPS200_USE_SOFT_SPI                                                         // 杩欎袱娈� 棰滆壊姝ｅ父鐨勬墠鏄纭殑 棰滆壊鐏扮殑灏辨槸娌℃湁鐢ㄧ殑
//====================================================杞欢 SPI 椹卞姩====================================================
// 濡傛灉浣跨敤鐨勬槸SPI 涓插彛鐨勪袱瀵稿睆骞� SPI 椹卞姩鎺у埗寮曡剼 鍙互淇敼
#define IPS200_SOFT_SPI_DELAY           (0 )                                    // 杞欢 SPI 鐨勬椂閽熷欢鏃跺懆鏈� 鏁板�艰秺灏� SPI 閫氫俊閫熺巼瓒婂揩
#define IPS200_SCL_PIN                  (P15_3)                                 // 杞欢 SPI SCK 寮曡剼
#define IPS200_SDA_PIN                  (P15_5)                                 // 杞欢 SPI MOSI 寮曡剼
//====================================================杞欢 SPI 椹卞姩====================================================
#else
//====================================================纭欢 SPI 椹卞姩====================================================
// 濡傛灉浣跨敤鐨勬槸SPI 涓插彛鐨勪袱瀵稿睆骞� SPI 椹卞姩鎺у埗寮曡剼 鍙互淇敼
#define IPS200_SPI_SPEED                (60*1000*1000)                          // 纭欢 SPI 閫熺巼
#define IPS200_SPI                      (SPI_0           )                      // 纭欢 SPI 鍙�
#define IPS200_SCL_PIN_SPI              (SPI0_SCLK_P20_13 )                      // 纭欢 SPI SCK 寮曡剼
#define IPS200_SDA_PIN_SPI              (SPI0_MOSI_P20_14 )                      // 纭欢 SPI MOSI 寮曡剼
#define IPS200_SDA_IN_PIN_SPI           (SPI0_MISO_P20_12 )                      // 纭欢 SPI MISO 寮曡剼  IPS娌℃湁MISO寮曡剼锛屼絾鏄繖閲屼换鐒堕渶瑕佸畾涔夛紝鍦╯pi鐨勫垵濮嬪寲鏃堕渶瑕佷娇鐢�
//====================================================纭欢 SPI 椹卞姩====================================================
#endif
// 濡傛灉浣跨敤鐨勬槸SPI 涓插彛鐨勪袱瀵稿睆骞� SPI 椹卞姩鎺у埗寮曡剼 鍙互淇敼
#define IPS200_RST_PIN_SPI              (P20_11)                                 // (SPI涓插彛)娑叉櫠澶嶄綅寮曡剼瀹氫箟
#define IPS200_DC_PIN_SPI               (P20_10)                                 // (SPI涓插彛)娑叉櫠鍛戒护浣嶅紩鑴氬畾涔�
#define IPS200_CS_PIN_SPI               (P20_9)                                 // (SPI涓插彛)娑叉櫠鐗囬�夊紩鑴氬畾涔�
#define IPS200_BLk_PIN_SPI              (P20_12)                                 // (SPI涓插彛)娑叉櫠鑳屽厜寮曡剼瀹氫箟

// #define IPS200_SPI_SPEED                (60*1000*1000)                          // 纭欢 SPI 閫熺巼
// #define IPS200_SPI                      (SPI_2           )                      // 纭欢 SPI 鍙�
// #define IPS200_SCL_PIN_SPI              (SPI2_SCLK_P15_3 )                      // 纭欢 SPI SCK 寮曡剼
// #define IPS200_SDA_PIN_SPI              (SPI2_MOSI_P15_5 )                      // 纭欢 SPI MOSI 寮曡剼
// #define IPS200_SDA_IN_PIN_SPI           (SPI2_MISO_P15_4 )                      // 纭欢 SPI MISO 寮曡剼  IPS娌℃湁MISO寮曡剼锛屼絾鏄繖閲屼换鐒堕渶瑕佸畾涔夛紝鍦╯pi鐨勫垵濮嬪寲鏃堕渶瑕佷娇鐢�
// //====================================================纭欢 SPI 椹卞姩====================================================
// #endif
// // 濡傛灉浣跨敤鐨勬槸SPI 涓插彛鐨勪袱瀵稿睆骞� SPI 椹卞姩鎺у埗寮曡剼 鍙互淇敼
// #define IPS200_RST_PIN_SPI              (P15_1)                                 // (SPI涓插彛)娑叉櫠澶嶄綅寮曡剼瀹氫箟
// #define IPS200_DC_PIN_SPI               (P15_0)                                 // (SPI涓插彛)娑叉櫠鍛戒护浣嶅紩鑴氬畾涔�
// #define IPS200_CS_PIN_SPI               (P15_2)                                 // (SPI涓插彛)娑叉櫠鐗囬�夊紩鑴氬畾涔�
// #define IPS200_BLk_PIN_SPI              (P15_4)                                 // (SPI涓插彛)娑叉櫠鑳屽厜寮曡剼瀹氫箟


// 濡傛灉浣跨敤鐨勬槸鍏綅骞跺彛鐨勪袱瀵稿睆骞� 骞跺彛椹卞姩鎺у埗寮曡剼 鍙互淇敼
#define IPS200_RD_PIN_PARALLEL8         (P15_3)                                 // (鍏綅骞跺彛)娑叉櫠璇诲彇浣嶅紩鑴氬畾涔�
#define IPS200_WR_PIN_PARALLEL8         (P15_5)                                 // (鍏綅骞跺彛)娑叉櫠鍐欏叆浣嶅紩鑴氬畾涔�
#define IPS200_RST_PIN_PARALLEL8        (P15_0)                                 // (鍏綅骞跺彛)娑叉櫠澶嶄綅寮曡剼瀹氫箟
#define IPS200_RS_PIN_PARALLEL8         (P15_1)                                 // (鍏綅骞跺彛)娑叉櫠鍛戒护浣嶅紩鑴氬畾涔�
#define IPS200_CS_PIN_PARALLEL8         (P15_2)                                 // (鍏綅骞跺彛)娑叉櫠鐗囬�夊紩鑴氬畾涔�
#define IPS200_BL_PIN_PARALLEL8         (P15_4)                                 // (鍏綅骞跺彛)娑叉櫠鑳屽厜寮曡剼瀹氫箟
//骞跺彛椹卞姩鏁版嵁寮曡剼 鍙互淇敼 濡傛灉浣犵殑灞忓箷鏄叓浣嶅苟鍙� 杩欓噷鐨勫紩鑴氱敤寰楀埌
//D0-D3鍥涗釜鏁版嵁寮曡剼蹇呴』杩炵画 渚嬪C0-C3,C1-C4绛夌瓑锛�
//D4-D7鍥涗釜鏁版嵁寮曡剼蹇呴』杩炵画 渚嬪B0-B3,B1-B4绛夌瓑銆�
//鍙互杩炴帴鍒颁笉鍚岀鍙ｇ殑鎰忔�濆氨鏄睆骞曠殑D0-D3涓嶤1-C4杩炴帴锛孌4-D7涓嶣2-B5杩炴帴銆�
//鍒囨崲寮曡剼鍚庢敞鎰忎慨鏀笽PS200_DATA_PORT1鍜孖PS200_DATA_PORT2瀹忓畾涔�
#define IPS200_D0_PIN_PARALLEL8         (P11_9 )                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D0
#define IPS200_D1_PIN_PARALLEL8         (P11_10)                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D1
#define IPS200_D2_PIN_PARALLEL8         (P11_11)                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D2
#define IPS200_D3_PIN_PARALLEL8         (P11_12)                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D3
#define IPS200_D4_PIN_PARALLEL8         (P13_0 )                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D4
#define IPS200_D5_PIN_PARALLEL8         (P13_1 )                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D5
#define IPS200_D6_PIN_PARALLEL8         (P13_2 )                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D6
#define IPS200_D7_PIN_PARALLEL8         (P13_3 )                                // (鍏綅骞跺彛)娑叉櫠鏁版嵁寮曡剼D7

#define IPS200_DEFAULT_DISPLAY_DIR      (IPS200_PORTAIT)                        // 榛樿鐨勬樉绀烘柟鍚�
#define IPS200_DEFAULT_PENCOLOR         (RGB565_RED    )                        // 榛樿鐨勭敾绗旈鑹�
#define IPS200_DEFAULT_BGCOLOR          (RGB565_WHITE  )                        // 榛樿鐨勮儗鏅鑹�
#define IPS200_DEFAULT_DISPLAY_FONT     (IPS200_8X16_FONT)                      // 榛樿鐨勫瓧浣撴ā寮�

//瀹氫箟鏁版嵁绔彛鎵�鍦≒ORT锛屽垏鎹㈠紩鑴氬悗鍔″繀鏍规嵁寮曡剼鎵�鍦≒ORT杩涜鏇存敼   杩欓噷浣跨敤浜嗕袱缁勭鍙ｈ繘琛岀粍鍚�  鍥犳瀹氫箟浜嗕袱涓紩鑴氳捣濮嬬紪鍙�
#define IPS200_DATA_PORT1               (3)       //0锛歅00绔彛  1锛歅02绔彛  2锛歅10绔彛  3锛歅11绔彛  4锛歅13绔彛  5锛歅14绔彛  6锛歅15绔彛  7锛歅20绔彛  8锛歅21绔彛  9锛歅22绔彛  10锛歅23绔彛  11锛歅32绔彛  12锛歅33绔彛
#define IPS200_DATAPORT1                (get_port_out_addr(IPS200_DATA_PORT1))
#define DATA_START_NUM1                 (IPS200_D0_PIN_PARALLEL8&0x1f)          // 瀹氫箟鏁版嵁寮曡剼鐨勮捣濮嬬紪鍙�
#define IPS200_DATA_PORT2               (4)       //0锛歅00绔彛  1锛歅02绔彛  2锛歅10绔彛  3锛歅11绔彛  4锛歅13绔彛  5锛歅14绔彛  6锛歅15绔彛  7锛歅20绔彛  8锛歅21绔彛  9锛歅22绔彛  10锛歅23绔彛  11锛歅32绔彛  12锛歅33绔彛
#define IPS200_DATAPORT2                (get_port_out_addr(IPS200_DATA_PORT2))
#define DATA_START_NUM2                 (IPS200_D4_PIN_PARALLEL8&0x1f)          // 瀹氫箟鏁版嵁寮曡剼鐨勮捣濮嬬紪鍙�

// 鎺у埗璇彞
#define IPS200_RD(x)                    ((x) ? (gpio_high(IPS200_RD_PIN_PARALLEL8)) : (gpio_low(IPS200_RD_PIN_PARALLEL8)))
#define IPS200_WR(x)                    ((x) ? (gpio_high(IPS200_WR_PIN_PARALLEL8)) : (gpio_low(IPS200_WR_PIN_PARALLEL8)))
#define IPS200_RST(x)                   ((x) ? (gpio_high(ips_rst_pin))             : (gpio_low(ips_rst_pin)))
#define IPS200_BL(x)                    ((x) ? (gpio_high(ips_bl_pin))              : (gpio_low(ips_bl_pin)))
#define IPS200_RS(x)                    ((x) ? (gpio_high(IPS200_RS_PIN_PARALLEL8)) : (gpio_low(IPS200_RS_PIN_PARALLEL8)))
#define IPS200_DC(x)                    ((x) ? (gpio_high(IPS200_DC_PIN_SPI))       : (gpio_low(IPS200_DC_PIN_SPI)))
#define IPS200_CS(x)                    ((x) ? (gpio_high(IPS200_CS_PIN_SPI))       : (gpio_low(IPS200_CS_PIN_SPI)))
//==================================================瀹氫箟 IPS200 鍩烘湰閰嶇疆================================================


//==================================================瀹氫箟 IPS200 鍙傛暟缁撴瀯浣�===============================================
typedef enum
{
    IPS200_TYPE_SPI,                                                            // SPI 椹卞姩
    IPS200_TYPE_PARALLEL8,                                                      // 骞跺彛椹卞姩
}ips200_type_enum;

typedef enum
{
    IPS200_PORTAIT                      = 0,                                    // 绔栧睆妯″紡
    IPS200_PORTAIT_180                  = 1,                                    // 绔栧睆妯″紡  鏃嬭浆180
    IPS200_CROSSWISE                    = 2,                                    // 妯睆妯″紡
    IPS200_CROSSWISE_180                = 3,                                    // 妯睆妯″紡  鏃嬭浆180
}ips200_dir_enum;

typedef enum
{
    IPS200_6X8_FONT                     = 0,                                    // 6x8      瀛椾綋
    IPS200_8X16_FONT                    = 1,                                    // 8x16     瀛椾綋
    IPS200_16X16_FONT                   = 2,                                    // 16x16    瀛椾綋 鐩墠涓嶆敮鎸�
}ips200_font_size_enum;

extern  uint16  ips200_width_max;
extern  uint16  ips200_height_max;

//==================================================瀹氫箟 IPS200 鍙傛暟缁撴瀯浣�===============================================

//==================================================澹版槑 IPS200 鍩虹鍑芥暟================================================
void    ips200_clear                    (void);                                                                                // IPS200 娓呭睆鍑芥暟
void    ips200_full                     (const uint16 color);                                                                  // IPS200 灞忓箷濉厖鍑芥暟
void    ips200_set_dir                  (ips200_dir_enum dir);                                                                 // IPS200 璁剧疆鏄剧ず鏂瑰悜
void    ips200_set_font                 (ips200_font_size_enum font);                                                          // IPS200 璁剧疆鏄剧ず瀛椾綋
void    ips200_set_color                (const uint16 pen, const uint16 bgcolor);                                              // IPS200 璁剧疆鏄剧ず棰滆壊
void    ips200_draw_point               (uint16 x, uint16 y, const uint16 color);                                              // IPS200 鐢荤偣鍑芥暟
void    ips200_draw_line                (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);      // IPS200 鐢荤嚎鍑芥暟

void    ips200_show_char                (uint16 x, uint16 y, const char dat);
void    ips200_show_string              (uint16 x, uint16 y, const char dat[]);
void    ips200_show_int                 (uint16 x, uint16 y, const int32 dat, uint8 num);
void    ips200_show_uint                (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    ips200_show_float               (uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);

void    ips200_show_binary_image        (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);                      // IPS200 鏄剧ず浜屽�煎浘鍍� 鏁版嵁姣忓叓涓偣缁勬垚涓�涓瓧鑺傛暟鎹�
void    ips200_show_gray_image          (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold);     // IPS200 鏄剧ず 8bit 鐏板害鍥惧儚 甯︿簩鍊煎寲闃堝��
void    ips200_show_rgb565_image        (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode);   // IPS200 鏄剧ず RGB565 褰╄壊鍥惧儚

void    ips200_show_wave                (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);                // IPS200 鏄剧ず娉㈠舰
void    ips200_show_chinese             (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);                                 // IPS200 姹夊瓧鏄剧ず

void    ips200_init                     (ips200_type_enum type_select);                                                         // 2瀵� IPS娑叉櫠鍒濆鍖�
void ljy_ips200_printf(uint16 x, uint16 y, const char *fmt, ...);
void ljy_ips200_data_printf(uint8 count, const char *fmt, ...);
void ljy_ips200_clear(void);
//==================================================澹版槑 IPS200 鍩虹鍑芥暟================================================

//==================================================澹版槑 IPS200 鎵╁睍鍑芥暟================================================
//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     IPS200 鏄剧ず灏忛捇椋庡浘鍍�
// 鍙傛暟璇存槑     p               鍥惧儚鏁扮粍鎸囬拡
// 鍙傛暟璇存槑     width           鍥惧儚瀹為檯瀹藉害
// 鍙傛暟璇存槑     height          鍥惧儚瀹為檯楂樺害
// 杩斿洖鍙傛暟     void
// 浣跨敤绀轰緥     ips200_displayimage7725(ov7725_image_binary[0], OV7725_W, OV7725_H);
// 澶囨敞淇℃伅     鎷撳睍鐨勪竴閿樉绀哄嚱鏁帮紝榛樿鏃犵缉鏀撅紝浠庡睆骞曞潗鏍囪捣濮嬬偣寮�濮嬫樉绀�
//-------------------------------------------------------------------------------------------------------------------
#define ips200_displayimage7725(p, width, height)       (ips200_show_binary_image(0, 0, (p), OV7725_W, OV7725_H, (width), (height)))
//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     IPS200 鏄剧ず鎬婚捇椋庡浘鍍�
// 鍙傛暟璇存槑     p               鍥惧儚鏁扮粍鎸囬拡
// 鍙傛暟璇存槑     width           鍥惧儚瀹為檯瀹藉害
// 鍙傛暟璇存槑     height          鍥惧儚瀹為檯楂樺害
// 杩斿洖鍙傛暟     void
// 浣跨敤绀轰緥     ips200_displayimage03x(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
// 澶囨敞淇℃伅     鎷撳睍鐨勪竴閿樉绀哄嚱鏁帮紝榛樿鏃犵缉鏀撅紝浠庡睆骞曞潗鏍囪捣濮嬬偣寮�濮嬫樉绀�
//-------------------------------------------------------------------------------------------------------------------
#define ips200_displayimage03x(p, width, height)        (ips200_show_gray_image(0, 0, (p), MT9V03X_W, MT9V03X_H, (width), (height), 0))
//-------------------------------------------------------------------------------------------------------------------
// 鍑芥暟绠�浠�     IPS200 鏄剧ず鍑岀灣鍥惧儚
// 鍙傛暟璇存槑     p               鍥惧儚鏁扮粍鎸囬拡
// 鍙傛暟璇存槑     width           鍥惧儚瀹為檯瀹藉害
// 鍙傛暟璇存槑     height          鍥惧儚瀹為檯楂樺害
// 杩斿洖鍙傛暟     void
// 浣跨敤绀轰緥     ips200_displayimage8660(scc8660_image[0], SCC8660_W, SCC8660_H);
// 澶囨敞淇℃伅     鎷撳睍鐨勪竴閿樉绀哄嚱鏁帮紝榛樿鏃犵缉鏀撅紝浠庡睆骞曞潗鏍囪捣濮嬬偣寮�濮嬫樉绀�
//-------------------------------------------------------------------------------------------------------------------
#define ips200_displayimage8660(p, width, height)       (ips200_show_rgb565_image(0, 0, (p), SCC8660_W, SCC8660_H, (width), (height), 1))
//==================================================澹版槑 IPS200 鎵╁睍鍑芥暟================================================


#endif

