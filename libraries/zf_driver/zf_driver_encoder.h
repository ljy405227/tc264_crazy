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
* 鏂囦欢鍚嶇О          zf_driver_encoder
* 鍏徃鍚嶇О          鎴愰兘閫愰绉戞妧鏈夐檺鍏徃
* 鐗堟湰淇℃伅          鏌ョ湅 libraries/doc 鏂囦欢澶瑰唴 version 鏂囦欢 鐗堟湰璇存槑
* 寮�鍙戠幆澧�          ADS v1.10.2
* 閫傜敤骞冲彴          TC264D
* 搴楅摵閾炬帴          https://seekfree.taobao.com/
*
* 淇敼璁板綍
* 鏃ユ湡              浣滆��                澶囨敞
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#ifndef _zf_driver_encoder_h_
#define _zf_driver_encoder_h_

#include "zf_common_typedef.h"

// 姝ゆ灇涓惧畾涔変笉鍏佽鐢ㄦ埛淇敼
typedef enum // 鏋氫妇缂栫爜鍣ㄥ紩鑴�
{
    TIM2_ENCODER_CH1_P00_7,      // T2瀹氭椂鍣� 璁℃暟寮曡剼鍙�夎寖鍥�
    TIM2_ENCODER_CH1_P33_7,

    TIM3_ENCODER_CH1_P02_6,      // T3瀹氭椂鍣� 璁℃暟寮曡剼鍙�夎寖鍥�

    TIM4_ENCODER_CH1_P02_8,      // T4瀹氭椂鍣� 璁℃暟寮曡剼鍙�夎寖鍥�

    TIM5_ENCODER_CH1_P21_7,      // T5瀹氭椂鍣� 璁℃暟寮曡剼鍙�夎寖鍥�
    TIM5_ENCODER_CH1_P10_3,

    TIM6_ENCODER_CH1_P20_3,      // T6瀹氭椂鍣� 璁℃暟寮曡剼鍙�夎寖鍥�
    TIM6_ENCODER_CH1_P10_2,
}encoder_channel1_enum;

// 姝ゆ灇涓惧畾涔変笉鍏佽鐢ㄦ埛淇敼
typedef enum // 鏋氫妇缂栫爜鍣ㄥ紩鑴�
{
    TIM2_ENCODER_CH2_P00_8,     // T2瀹氭椂鍣� 璁℃暟鏂瑰悜寮曡剼鍙�夎寖鍥�
    TIM2_ENCODER_CH2_P33_6,

    TIM3_ENCODER_CH2_P02_7,     // T3瀹氭椂鍣� 璁℃暟鏂瑰悜寮曡剼鍙�夎寖鍥�

    TIM4_ENCODER_CH2_P00_9,     // T4瀹氭椂鍣� 璁℃暟鏂瑰悜寮曡剼鍙�夎寖鍥�
    TIM4_ENCODER_CH2_P33_5,

    TIM5_ENCODER_CH2_P21_6,     // T5瀹氭椂鍣� 璁℃暟鏂瑰悜寮曡剼鍙�夎寖鍥�
    TIM5_ENCODER_CH2_P10_1,

    TIM6_ENCODER_CH2_P20_0,     // T6瀹氭椂鍣� 璁℃暟鏂瑰悜寮曡剼鍙�夎寖鍥�
}encoder_channel2_enum;

typedef enum  // 鏋氫妇 瀹氭椂鍣ㄧ紪鍙�
{
    TIM2_ENCODER,
    TIM3_ENCODER,
    TIM4_ENCODER,
    TIM5_ENCODER,
    TIM6_ENCODER,
}encoder_index_enum;

//====================================================缂栫爜鍣� 鍩虹鍑芥暟====================================================
int16 encoder_get_count     (encoder_index_enum encoder_n);
void  encoder_clear_count   (encoder_index_enum encoder_n);

void  encoder_quad_init     (encoder_index_enum encoder_n, encoder_channel1_enum count_pin, encoder_channel2_enum dir_pin);
void  encoder_dir_init      (encoder_index_enum encoder_n, encoder_channel1_enum ch1_pin, encoder_channel2_enum ch2_pin);
//====================================================缂栫爜鍣� 鍩虹鍑芥暟====================================================

#endif
