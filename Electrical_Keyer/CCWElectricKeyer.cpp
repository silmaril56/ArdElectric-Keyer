
///////////////////////////////////////////////////////////
//  CCWElectricKeyer.cpp
//  Implementation of the Class CCWElectricKeyer
//  Created on:      2018/09/29
//  Original author: JJ1JQZ
///////////////////////////////////////////////////////////

#include "CCWElectricKeyer.h"

#include "Arduino.h"


const TInvokePeriod CCWElectricKeyer::c_invoked_period = 1;



/**
 * タスクを起動する。
 * ここではPaddleのボタンを監視し、CWコードをデコードする
 */
void CCWElectricKeyer::invoke(){
  
  //_serial->print( "INVOKE:" );
  //_serial->print( millis() );
  //[debouncing process]
  for(int i = 0; i < c_contact_bouncing_width; ++i){
    acj1_sense();
    delayMicroseconds(c_contact_bounce_period);
  }
  
  //[input analyze process]
  acj1_control_input_value();

  //[input info analyze process]
  analyze_input_value();

  //[generate signal with time period process]
  generate_output_signal();
  
  //[tone output process]
  output_tone();

  //[post process]
  post_proc();
}
/**
 * ACJ1の入力情報を判定する
 */
void CCWElectricKeyer::acj1_sense(){
  int l_val = 0;
  int r_val = 0;

  //get ACJ1 value
  l_val = !digitalRead(ACJ1_L);
  r_val = !digitalRead(ACJ1_R);
  
  //check stability
  if(l_val == acj1_l_signal_level){
    if(acj1_l_signal_stable_counter >= c_contact_bouncing_width){
      acj1_l_code = (acj1_l_signal_level == HIGH);
      //acj1_l_signal_stable_counter = acj1_l_signal_stable_counter;
      //acj1_l_signal_level = l_val;
    }
    else{
      //acj1_l_code = acj1_l_code;
      acj1_l_signal_stable_counter++;
      //acj1_l_signal_level = l_val;
    }
  }
  else{
    //acj1_l_code = acj1_l_code;
    acj1_l_signal_stable_counter = 0;
    acj1_l_signal_level = l_val;
  }
  
  //check stability
  if(r_val == acj1_r_signal_level){
    if(acj1_r_signal_stable_counter >= c_contact_bouncing_width){
      acj1_r_code = (acj1_r_signal_level == HIGH);
      //acj1_r_signal_stable_counter = acj1_r_signal_stable_counter;
      //acj1_r_signal_level = r_val;
    }
    else{
      //acj1_r_code = acj1_l_code;
      acj1_r_signal_stable_counter++;
      //acj1_r_signal_level = r_val;
    }
  }
  else{
    //acj1_r_code = acj1_r_code;
    acj1_r_signal_stable_counter = 0;
    acj1_r_signal_level = r_val;
  }
}

/**
 * ACJ1の入力情報をコントロール入力値に変換
 */
void CCWElectricKeyer::acj1_control_input_value(){
  EACJPinSelect selected_key = EACJPinSelect::EVACJNone;

  //get ACJ1 input value
  selected_key = get_acj1_select();
  
  //check duration
  if(selected_key == acj1_key){
    if(acj1_key_counter >= c_command_width){
      acj1_key = EACJPinSelect::EVACJNone;
      //acj1_key_counter = acj1_key_counter;
    }
    else{
      //acj1_key = selected_key;
      acj1_key_counter++;
    }
  }
  else{
    acj1_key = selected_key;
    acj1_key_counter = 0;
  }
  //_serial->print( ", KC:" );
  //_serial->print(acj1_key_counter );
  //_serial->print( ", Key:" );
  //_serial->print(acj1_key );
  //_serial->print( ", RWCodeL:" );
  //_serial->print(acj1_l_code );
  //_serial->print( ", RWCodeR:" );
  //_serial->print(acj1_r_code );
  //_serial->print( "\n" );
}

/**
 * analyze key value to morse signal(dot/dash)
 */
void CCWElectricKeyer::analyze_input_value(){
  EACJPinSelect current_key = EACJPinSelect::EVACJNone;

  //get current command
  current_key = get_current_input_key();

  //push analyze key to morse code and push to buffer
  if(current_key != prev_key){
    if(current_key == EACJPinSelect::EVACJRight){
      signal_buffer->push(EMorseSignal::EVMSDot);
    }
    else if(current_key == EACJPinSelect::EVACJLeft){
      signal_buffer->push(EMorseSignal::EVMSDash);
    }
    else{
      //_serial->print( "**\n" );
      signal_buffer->push(EMorseSignal::EVMSNone);//dot時はpush直後にpopされる可能性あり
    }
  }
  else{
    //signal_buffer->read();
  }
}

/**
 * Morse符号からsignalを生成
 */
void CCWElectricKeyer::generate_output_signal(){
  EMorseSignal temp_output_code = current_output_code;
  
  //check duration
  if(tone_duration >= translate_tone_duration(current_output_code)){
    tone_status = false;
    tone_duration = 1;
    current_output_code = EMorseSignal::EVMSNoneSel;
  }
  else{
    //tone_status = true;
    tone_duration++;
    //current_output_code = current_output_code;
  }

  //送出可能
  if(current_output_code == EMorseSignal::EVMSNoneSel){//none signal time duration delay
    if(signal_buffer->is_null() != true){
      current_output_code = signal_buffer->pop();//_serial->print( "pop1**\n" );
      
      //余計な無音をスキップする(今までassertしていた信号が無音なら無音信号をスキップする)
      while(((temp_output_code & c_morse_signal_mask) == 0) && (signal_buffer->is_null() != true)){
        current_output_code = signal_buffer->pop();//_serial->print( "pop2**\n" );
        temp_output_code = current_output_code;
      }
      
      tone_duration = 1;
      tone_status = ((current_output_code & c_morse_signal_mask) != 0);//if dot or dash then signal will be none 01 or 10
    }
    else{
      current_output_code = EMorseSignal::EVMSNone;
      tone_duration = 1;
      tone_status = false;
    }
  }
  else{
    //current_output_code = current_output_code;
    //tone_duration = tone_duration;
    //tone_status = tone_status;
  }
  //_serial->print( "CK:" );
  //_serial->print(get_current_input_key() );
  //_serial->print( "PK:" );
  //_serial->print(prev_key );
  //_serial->print( ", BS:" );
  //_serial->print(signal_buffer->count() );
  //_serial->print( ", TD:" );
  //_serial->print(tone_duration );
  //_serial->print( ", POC:" );
  //_serial->print(prev_output_code );
  //_serial->print( ", COC:" );
  //_serial->print(current_output_code );
  //_serial->print( ", TS:" );
  //_serial->print((current_output_code & c_morse_signal_mask) );
  //_serial->print( ", TTD:" );
  //_serial->print(translate_tone_duration(current_output_code) );
  //_serial->print( "\n" );
}

/**
 * トーンを出力する
 */
void CCWElectricKeyer::output_tone(){
  int output_tone_frequency = get_tone_frequency();
  //output tone
  _serial->print( "TS:" );
  _serial->print( tone_status );
  _serial->print( ", PTS:" );
  _serial->print( prev_tone_status );
  _serial->print( "\n" );
  if(prev_tone_status != tone_status){
    if(tone_status == true){
      _serial->print( "ON\n" );
      //tone(ACJ2_L, output_tone_frequency);// start tone
      tone(ACJ2_R, output_tone_frequency);// start tone(only one tone per PWM timer(2))
    }
    else{
      _serial->print( "OFF\n" );
      //noTone(ACJ2_L);    // stop tone
      noTone(ACJ2_R);    // stop tone
    }
  }
}

/**
 * 主要処理後に行う後処理
 */
void CCWElectricKeyer::post_proc(){
  prev_key = get_current_input_key();
  prev_tone_status = tone_status;
}

