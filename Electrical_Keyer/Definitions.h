///////////////////////////////////////////////////////////
//  Definitions.h
//  Defintions for global wide use.
//  Created on:      2018/09/19
//  Original author: JJ1JQZ
///////////////////////////////////////////////////////////


#if !defined(__DEFINITIONS_H__)
#define __DEFINITIONS_H__


//=========================================================
//  列挙型
//=========================================================

/**
* クラスID。
*/
enum EClassID
{
  /**
  * CObjectクラスのクラスID
  */
  EVCObjectID = 0,
  /**
  * CTaskManagerクラスのクラスID
  */
  EVCTaskManagerID,
  /**
  * CBlueToothCommクラスのクラスID
  */
  EVCBlueToothCommID,
  /**
  * CCWElectricKeyerクラスのクラスID
  */
  EVCCWElectricKeyerID
};

/**
* エレキー動作モード
*/
enum EElectricKeyerMode
{
  /**
  * ボタンが押されている間トーン信号が流れるモード
  */
  EVDirectMode = 1,
  /**
  * paddleにて長点と短点のボタンがそれぞれ押される毎に長点と短点を出力するモード
  */
  EVElectricKeyerStraitForwardMode
};

/**
* paddleのボタン種別
*/
enum EACJPinSelect
{
  /**
  * 左のボタン
  */
  EVACJNone = 0,
  /**
  * 左のボタン
  */
  EVACJLeft,
  /**
  * 右のボタン
  */
  EVACJRight
};

/**
* Morse信号の長点、短点、無音の種別
*/
enum EMorseSignal
{
  /**
  * 無信号
  */
  EVMSNoneSel    = 0b0000000000001000,
  /**
  * 無音
  */
  EVMSNone       = 0b0000000000000100,
  /**
  * 短点
  */
  EVMSDot        = 0b0000000000000010,
  /**
  * 長点
  */
  EVMSDash       = 0b0000000000000001
};
//=========================================================
//  型定義
//=========================================================

/**
 * 基本のTask起動周期に対するTask起動周期を表す型。
 */
typedef unsigned char TInvokePeriod;

//=========================================================
//  定数定義
//=========================================================
//*******************************************
// Defines
//*******************************************
#define MAX_LONG 2147483647

//*******************************************
// Port Number Definition
//*******************************************
//--------------------
// INPUTS
//--------------------
const int TACT_SWITCH1_L = 8;
const int TACT_SWITCH1_R = 9;
const int TACT_SWITCH2_L = 10;
const int TACT_SWITCH2_R = 11;

//Audio Port
const int ACJ1_R        = 2;//2;
const int ACJ1_L        = 3;//3;
//--------------------
// OUTPUTS
//--------------------
//Audio Port
const int ACJ2_R        = 4;
const int ACJ2_L        = 5;
//LED
const int ON_BOARD_LED  = 13;


//*******************************************
// Constant Definitions
//*******************************************

/** Circular Buffer Size
 */
const unsigned long  c_CircularBufferSize= 16;

/** Serial outpu wait time
 * Unit-> ms
 */
const unsigned long  c_SerialWait        = 10;
/** Max Time counter
 */
const unsigned long  c_MaxTimeCounter    = 1000;
/** valid morse signal mask
 */
const unsigned int  c_morse_signal_mask    = 0b0000000000000011;

/** Timer Interval
 * Unit-> ms
 */
#define  c_Timer_Interval                 25000//set as 25000[micro sec] = 25ms

//*******************************************
// type Definitions
//*******************************************
typedef int TCBUF_D;

#endif // !defined(__DEFINITIONS_H__)

