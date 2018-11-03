
///////////////////////////////////////////////////////////
//  CBlueToothComm.cpp
//  Implementation of the Class CBlueToothComm
//  Created on:      2018/09/29
//  Original author: JJ1JQZ
///////////////////////////////////////////////////////////

#include "CBlueToothComm.h"

#include "Arduino.h"


const TInvokePeriod CBlueToothComm::c_invoked_period = 20;//20 x 25[ms] = 0.5[sec]



/**
 * タスクを起動する。
 * ここでは送信、そして受信を行う。
 */
void CBlueToothComm::invoke(){
  //[Transfering process]
  transmit();
  
  //[Receiving process]
  recieve();
  
}

/**
 * データを送信する
 */
void CBlueToothComm::transmit(){
  get_serial()->print( "KM:" );
  get_serial()->print(_keyer->get_keyer_mode() );
  get_serial()->print( ", 1CL:" );
  get_serial()->print(_keyer->get_acj1_code(EACJPinSelect::EVACJLeft) );
  get_serial()->print( ", 1CR:" );
  get_serial()->print(_keyer->get_acj1_code(EACJPinSelect::EVACJRight) );
  get_serial()->print( ", 1SL:" );
  get_serial()->print(_keyer->get_acj1_select() );
  get_serial()->print( ", Key:" );
  get_serial()->print(_keyer->get_current_input_key() );
  get_serial()->print( ", OS:" );
  get_serial()->print(_keyer->get_current_output_code() );
  get_serial()->print( ", TF:" );
  get_serial()->print(_keyer->get_tone_frequency() );
  get_serial()->print( "\n" );
}

/**
 * データを受信する
 */
void CBlueToothComm::recieve(){
  //  char recv_data = 0;
  //  recv_data      = get_serial()->read();
}
