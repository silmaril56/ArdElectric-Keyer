///////////////////////////////////////////////////////////
//  CCircularBuffer.h
//  Implementation of the Class CCWElectricKeyer
//  Created on:      2018/09/19
//  Original author: JJ1JQZ
///////////////////////////////////////////////////////////

#if !defined(__CCCIRCULARBUFFER_H__)
#define __CCCIRCULARBUFFER_H__

#include "Definitions.h"
#include "CObject.h"
#include "HardWareserial.h"

/**
 * FIFOデータ構造
 */
class CCircularBuffer : public CObject
{
public:
  //=========================================================
  //  メソッド
  //=========================================================
  /**
  * コンストラクタ。
  */
  CCircularBuffer(){
    write_p = 0;
    read_p = 0;
  }
  /**
   * デストラクタ。
   */
  virtual ~CCircularBuffer(){}
  
  /**
   * 要素数取得
   */
  int count(){
    int count = read_p - write_p;
    if (count < 0) count += c_CircularBufferSize;
    return count;
  }
  /**
   * 要素追加
   */
  void push(TCBUF_D val){
    write_p = (write_p - 1) & c_Mask;
    data[write_p] = val;
  }
  /**
   * 要素取得(with delete)
   */
  TCBUF_D pop(){
    TCBUF_D return_value = data[read_p];
    bool is_empty = (read_p == write_p);
    
    if(is_empty != true){
      read_p = (read_p - 1) & c_Mask;
      return return_value;
    }
    else{
      return 0xFF;
    }
  }
  /**
   * 要素取得(without delete)
   */
  TCBUF_D top(){
    TCBUF_D return_value = data[read_p];
    bool is_empty = (read_p == write_p);
    
    if(is_empty != true){
      return return_value;
    }
    else{
      return 0xFF;
    }
  }
  /**
   * 要素数0か否か
   */
  bool is_null(){
    TCBUF_D return_value = data[read_p];
    
    return (read_p == write_p);
  }
protected:

protected:
  //=========================================================
  //  属性
  //=========================================================
  /**
   * 保持するデータの配列
   */
  TCBUF_D data[c_CircularBufferSize];
  
  /**
   * write pointer
   */
  int write_p;
  
  /**
   * read pointer
   */
  int read_p;
protected:
  /**
   * カウンター初期化範囲マスク
   */
  const int c_Mask = c_CircularBufferSize -1;
};
#endif // !defined(__CCCIRCULARBUFFER_H__)
