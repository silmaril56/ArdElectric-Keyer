#include <TimerOne.h>
#include <Time.h>
#include "Definitions.h"
#include "CObject.h"
#include "CTaskManager.h"
#include "CBlueToothComm.h"
#include "CCWElectricKeyer.h"

//////////////////////////////////////////////////////////////////
//  Gardening Support Equipment Control Software
// --------------------------------------------------------------
// Created Date: 29th, September, 2018
// Update Date:  29th, September, 2018
// Version: 0.1
// author: JJ1JQZ
// --------------------------------------------------------------
// Target HW: Arduino Duemilanove with some custom HW compornents.
// Library: DateTime Library
// Program Environment: Arduino IDE
// --------------------------------------------------------------
// Change History
// --------------------------------------------------------------
//               <Ver. 0.1 ( 29th, September, 2018 )>
//   1. New Creation with timer interruption model and current time & date setting
// --------------------------------------------
//////////////////////////////////////////////////////////////////

//*******************************************
// Grobal Value Definitions
//*******************************************
/** Current Moisture Value
 */
volatile boolean TaskInvokeEvent;

/** Task Counter
 */
int              TaskCounter;

/** Start time[ms] of this loop process
 */
long             StartTime;

/** End time[ms] of this loop process
 */
long             EndTime;

/** Processing period[ms] of this loop process
 */
long             ProcessTime;

/** Task manager class instance
 */
CTaskManager  *  TaskManager = NULL;

/** bluetooth communication class
 */
CBlueToothComm * BlueToothComm = NULL;

/** Electoric Keyer synthesizer class
 */
CCWElectricKeyer * CWElectricKeyer = NULL;

//*******************************************
// Function Definitions
//*******************************************

//******************************
// Utility functions
//******************************
/**
 * convert character to int
 */
//#define c2i ( c ) c - '0'
inline int c2i(char c){ return (c - '0'); }


//******************************
// Set up and loop
//******************************
/**
 * Initialization Functions
 */
void setup() {
  
  //---------------------
  //open serial port
  //---------------------
  Serial.begin  ( 115200 );//9600//115200
  
  //---------------------
  //set the swtches and ACJ2 pins as input then ACJ1, 3 pins as output that are turned off
  //---------------------
  pinMode       ( ACJ2_L,        OUTPUT );
  pinMode       ( ACJ2_R,        OUTPUT );
  pinMode       ( ON_BOARD_LED,  OUTPUT );
  pinMode       ( ACJ1_L,         INPUT_PULLUP );
  pinMode       ( ACJ1_R,         INPUT_PULLUP );
  //pinMode       ( TACT_SWITCH1_L, INPUT_PULLUP );
  //pinMode       ( TACT_SWITCH1_R, INPUT_PULLUP );
  //pinMode       ( TACT_SWITCH2_L, INPUT_PULLUP );
  //pinMode       ( TACT_SWITCH2_R, INPUT_PULLUP );
  //digitalWrite  ( ACJ1_L,       LOW);
  //digitalWrite  ( ACJ1_R,       LOW);
  //digitalWrite  ( ACJ2_L,       LOW);
  digitalWrite  ( ACJ2_R,       LOW);
  digitalWrite  ( ON_BOARD_LED, LOW);
  
  //---------------------

  //---------------------
  //class instance creation
  //---------------------
  CWElectricKeyer = new CCWElectricKeyer(&Serial);
  BlueToothComm   = new CBlueToothComm(&Serial, CWElectricKeyer);
  //---------------------
  
  
  //---------------------
  //task management class instance creation
  //---------------------
  TaskManager = new CTaskManager();
  //---------------------
  
  //---------------------
  //task registration
  //---------------------
  TaskManager->add_task(CWElectricKeyer, CWElectricKeyer->get_invoke_period(), CWElectricKeyer->get_invoke_period());
  TaskManager->add_task(BlueToothComm,    BlueToothComm->get_invoke_period(),    BlueToothComm->get_invoke_period());
  //---------------------
  
  //---------------------
  //time cont value initialization
  //---------------------
  StartTime                  = 0;
  EndTime                    = 0;
  //---------------------
  
  //---------------------
  //Initialize and entry the timer 1 interrupt and task invokation information
  //---------------------
  TaskInvokeEvent            = false;
  TaskCounter                = 1;//count for 1 to 10
  Timer1.initialize     ( c_Timer_Interval );
  Timer1.attachInterrupt( isr_timer1 );
  //---------------------
}

/**
 * Main loop functions
 * to select and invoke tasks when Task Invoke Event is set
 */
void loop() {
  // [Initialization Process]
  boolean TIE_preserve = TaskInvokeEvent;
  TaskInvokeEvent = false;
  
  StartTime       = millis();
  
  // -----
  //[Invoke Main Task Process]
  if( TIE_preserve == true ){
    
    //invoke tasks
    TaskManager->evaluate();
    
    //Task counter increment or reset
    if(TaskCounter >= c_MaxTimeCounter){
      TaskCounter = 1;
      Timer1.restart();
    }
    else{
      TaskCounter++;
    }
  }
  // -----
  
  // [Task process time calculation]
  EndTime         = millis();
  ProcessTime     = EndTime - StartTime;
  

}

//******************************
// Interrupt Service Routines
//******************************

/**
 * Timer1 Interrupt Service Routine
 * Sets the Task Invoke Event Flag
 */
void isr_timer1()
{
    TaskInvokeEvent = true;
}


//******************************
// Tasks
//******************************

