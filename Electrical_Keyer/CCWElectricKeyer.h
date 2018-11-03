///////////////////////////////////////////////////////////
//  CCWElectricKeyer.h
//  Implementation of the Class CCWElectricKeyer
//  Created on:      2018/09/19
//  Original author: JJ1JQZ
///////////////////////////////////////////////////////////

#if !defined(__CCCWELECTRICKEYER_H__)
#define __CCCWELECTRICKEYER_H__

#include "Definitions.h"
#include "CObject.h"
#include "ITask.h"
#include "CCircularBuffer.h"
#include "HardWareserial.h"

/**
 * Paddleのボタンを監視してMorse信号を生成する。
 */
class CCWElectricKeyer : public CObject, public ITask
{
public:
  //=========================================================
  //  メソッド
  //=========================================================
  /**
  * コンストラクタ。
  */
  CCWElectricKeyer(HardwareSerial *serial){
    _serial = serial;
    invoke_counter = 0;
    output_dot_width_for_counter = 4;//4 x 25[ms] = 100[ms]
    keyer_mode = EElectricKeyerMode::EVElectricKeyerStraitForwardMode;
    acj1_l_signal_stable_counter = 0;
    acj1_r_signal_stable_counter = 0;
    acj1_l_signal_level          = 0;
    acj1_r_signal_level          = 0;
    acj1_l_code                  = false;
    acj1_r_code                  = false;
    acj1_key                     = EACJPinSelect::EVACJNone;
    prev_key                     = EACJPinSelect::EVACJNone;
    acj1_key_counter             = 0;
    prev_output_code             = EMorseSignal::EVMSNoneSel;
    current_output_code          = EMorseSignal::EVMSNoneSel;
    tone_duration                = 1;
    tone_status                  = false;
    prev_tone_status             = false;
    tone_frequency               = c_tone_frequency;
    signal_buffer                = new CCircularBuffer();
  }
  /**
   * デストラクタ。
   */
  virtual ~CCWElectricKeyer(){}
  /**
   * タスクの起動周期を取得する(最小起動周期で割った値)。
   */
  virtual TInvokePeriod get_invoke_period(){ return c_invoked_period; }
  /**
   * タスクを起動する。
   * ここでは計測角、指示角をdata_strobeで集めて、actuate()する。
   */
  virtual void invoke();
  /**
   * ACJ1の入力情報を判定する
   */
  void acj1_sense();
  /**
   * ACJ1の入力情報をコントロール入力値に変換
   */
  void acj1_control_input_value();
  /**
   * ACJ1のコマンドによって出力値を決める
   */
  void analyze_input_value();
  /**
   * Morse符号からsignalを生成
   */
  void generate_output_signal();
  /**
   * トーンを出力する
   */
  void output_tone();
  /**
   * 主要処理後に行う後処理
   */
  void post_proc();
  /**
   * 出力するMorse信号の短点の長さを設定する
   * 短点の長さは起動周期単位で設定する。
   */
  void set_output_dot_width(int dot_width){ output_dot_width_for_counter = dot_width; }
  /**
   * エレキーとしての動作モードを設定する
   */
  void set_keyer_mode(EElectricKeyerMode mode){ keyer_mode = mode; }
  /**
   * エレキーとしての動作モードを取得する
   */
  EElectricKeyerMode get_keyer_mode(){ return keyer_mode; }
  /**
   * ACJ1の値を取得する
   */
  bool get_acj1_code(EACJPinSelect pin_select){ 
    if(pin_select == EACJPinSelect::EVACJLeft){
      return acj1_l_signal_level;
    }
    else if(pin_select == EACJPinSelect::EVACJRight){
      return acj1_r_signal_level;
    }
    else{
      return 0;
    }
  }
  /**
   * ACJ1のどちらのボタンが押されているか取得する。
   */
  EACJPinSelect get_acj1_select(){ 
    if(acj1_l_signal_level == true){
      return EACJPinSelect::EVACJLeft;
    }
    else if(acj1_r_signal_level == true){
      return EACJPinSelect::EVACJRight;
    }
    else{
      return EACJPinSelect::EVACJNone;
    }
  }
  /**
   * 入力された現在のコマンドを取得する
   */
  EACJPinSelect get_current_input_key(){ return acj1_key; }
  /**
   * 出力中の現在の信号を取得する
   */
  EMorseSignal get_current_output_code(){ return current_output_code; }
  /**
   * ACJ1のどちらのボタンが押されているか取得する。
   */
  int translate_tone_duration(EMorseSignal morse_signal){ 
    if(morse_signal == EMorseSignal::EVMSNone){
      return c_non_assert_width * c_signal_pulse_width;
    }
    else if(morse_signal == EMorseSignal::EVMSDot){
      return c_dot_width * c_signal_pulse_width;
    }
    else if(morse_signal == EMorseSignal::EVMSDash){
      return c_dash_width * c_signal_pulse_width;
    }

    return 0;
  }
  /**
   * トーンの信号周波数を取得する
   */
  int get_tone_frequency(){ return tone_frequency; }
  /**
   * トーンの信号周波数を設定する
   */
  void set_tone_frequency(int frequency){ tone_frequency = frequency; }
  
protected:

protected:
  //=========================================================
  //  属性
  //=========================================================
  /**
   * タスクの起動回数カウンター。
   */
  long invoke_counter;
  /**
   * ACJ1の左側の信号のレベル安定時間計測カウンタ
   */
  int acj1_l_signal_stable_counter;
  /**
   * ACJ1の右側の信号のレベル安定時間計測カウンタ
   */
  int acj1_r_signal_stable_counter;
  /**
   * ACJ1の左側の信号の現在のレベル
   */
  int acj1_l_signal_level;
  /**
   * ACJ1の右側の信号の現在のレベル
   */
  int acj1_r_signal_level;
  /**
   * ACJ1の左側の符号(contact bouncing等を除去し、解釈した現在のポートの情報)
   */
  bool acj1_l_code;
  /**
   * ACJ1の右側の符号(contact bouncing等を除去し、解釈した現在のポートの情報)
   */
  bool acj1_r_code;
  /**
   * ACJ1のコマンドカウンタ
   */
  int acj1_key_counter;
  /**
   * ACJ1のどちらのボタンが押下されたかの符号
   */
  EACJPinSelect acj1_key;
  /**
   * ACJ1のどちらのボタンが押下されたかの符号(前回)
   */
  EACJPinSelect prev_key;
  /**
   * 出力信号の前回の値(長点/短点)
   */
  EMorseSignal prev_output_code;
  /**
   * 現在出力中の信号(長点/短点)
   */
  EMorseSignal current_output_code;
  /**
   * 出力中のトーンの長さ
   */
  int tone_duration;
  /**
   * トーンのON/OFF
   */
  bool tone_status;
  /**
   * トーンのON/OFFの前回の値
   */
  bool prev_tone_status;
  /**
   * トーンの信号周波数
   */
  int tone_frequency;
  /**
   * 短点の対起動周期に対する長さ。
   */
  int output_dot_width_for_counter;
  /**
   * エレキーの動作モード設定
   */
  EElectricKeyerMode keyer_mode;
  /**
   * キー入力と送出信号の速度差を埋めるためのキー入力信号バッファ
   */
  CCircularBuffer * signal_buffer;
  /**
   * デバッグ用出力
   */
  HardwareSerial * _serial;
protected:
  /**
   * タスクの起動周期。
   */
  static const TInvokePeriod c_invoked_period;
  /**
   * contact bouncing除去期間をタスク起動周期にて表したもの
   */
  const int c_contact_bouncing_width = 3;//1 x 3[usec] = 3[usec]
  /**
   * Contact Bounce cancel period
   */
  const int c_contact_bounce_period = 1;//[usec]
  /**
   * 1回のボタン押下の長さをタスク起動周期にて表したもの
   */
  const int c_command_width = 20;//20 x 25[ms] = 0.5[sec]
  /**
   * 信号出力基準周期。
   */
  const int c_signal_pulse_width = 2;
  /**
   * 長点/短点間の長さ。
   */
  const int c_non_assert_width = 1;
  /**
   * 短点の長さ。
   */
  const int c_dot_width = 1;
  /**
   * 長点の短点における長さ。
   */
  const int c_dash_width = 3;
  /**
   * 長点/短点間の短点における長さ。
   */
  const int c_code_space_width = 1;
  /**
   * 文字間の短点における長さ。
   */
  const int c_char_space_width = 3;
  /**
   * 単語間の短点における長さ。
   */
  const int c_word_space_width = 7;
  /**
   * トーンの信号周波数
   */
  const int c_tone_frequency = 700;
};
#endif // !defined(__CCCWELECTRICKEYER_H__)
