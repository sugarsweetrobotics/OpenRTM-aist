// -*- C++ -*-
/*!
 * @file RingBuffer.h
 * @brief Defautl Buffer class
 * @date $Date: 2007-12-31 03:08:06 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006-2008
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: RingBuffer.h,v 1.5.2.2 2007-12-31 03:08:06 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.5.2.1  2007/07/20 16:07:58  n-ando
 * Shimizu's patch to modify vector access violation.
 *
 * Revision 1.5  2007/04/26 15:32:56  n-ando
 * The header include order was modified to define _REENTRANT before
 * including ace/config-lite.h in Linux systems.
 * In ace 5.4.7 or later, _REENTRANT flag should be defined explicitly.
 *
 * Revision 1.4  2007/01/06 18:02:49  n-ando
 * The RingBuffer has been completely re-implemented.
 *
 *
 */

#ifndef RingBuffer_h
#define RingBuffer_h

#include <rtm/RTC.h>

#include <vector>
#include <algorithm>
//#include <ace/Synch.h>
#include <rtm/BufferBase.h>

/*!
 * @if jp
 * @namespace RTC
 *
 * @brief RTコンポーネント
 *
 * @else
 *
 * @namespace RTC
 *
 * @endif
 */
namespace RTC
{
  /*!
   * @if jp
   * @class RingBuffer
   * @brief リングバッファ実装クラス
   * 
   * 指定した長さのリング状バッファを持つバッファ実装クラス。
   * バッファ全体にデータが格納された場合、以降のデータは古いデータから
   * 順次上書きされる。
   * 従って、バッファ内には直近のバッファ長分のデータのみ保持される。
   * 注)現在の実装では、一番最後に格納したデータのみバッファから読み出し可能
   *
   * @param DataType バッファに格納するデータ型
   *
   * @since 0.4.0
   *
   * @else
   *
   * @endif
   */
  template <class DataType>
  class RingBuffer
    : public BufferBase<DataType>
  {
  public:
    /*!
     * @if jp
     *
     * @brief コンストラクタ
     * 
     * コンストラクタ
     * 指定されたバッファ長でバッファを初期化する。
     * ただし、指定された長さが２未満の場合、長さ２でバッファを初期化する。
     *
     * @param length バッファ長
     * 
     * @else
     *
     * @endif
     */
    RingBuffer(long int length)
      : m_length(length < 2 ? 2 : length),
	m_oldPtr(0),
	m_newPtr(length < 2 ? 1 : length - 1)
    {
      m_buffer.resize(m_length);
    }
    
    /*!
     * @if jp
     *
     * @brief 仮想デストラクタ
     * 
     * 仮想デストラクタ。
     * 
     * @else
     *
     * @brief virtual destractor
     *
     * @endif
     */
    virtual ~RingBuffer(){};
    
    /*!
     * @if jp
     *
     * @brief 初期化
     * 
     * バッファの初期化
     * 指定された値をバッファ全体に格納する。
     *
     * @param data 初期化用データ
     * 
     * @else
     *
     * @endif
     */
    void init(DataType& data)
    {
      for (long int i = 0; i < m_length; ++i)
	{
	  put(data);
	}
    }
    
    /*!
     * @if jp
     *
     * @brief バッファ長を取得する
     * 
     * バッファ長を取得する。
     * 
     * @return バッファ長
     * 
     * @else
     *
     * @brief Get the buffer length
     *
     * @endif
     */
    virtual long int length() const
    {
      return m_length;
    }
    
    /*!
     * @if jp
     *
     * @brief バッファに書き込む
     * 
     * 引数で与えられたデータをバッファに書き込む。
     * 
     * @param value 書き込み対象データ
     *
     * @return データ書き込み結果(常にtrue:書き込み成功を返す)
     * 
     * @else
     *
     * @brief Write data into the buffer
     *
     * @endif
     */
    virtual bool write(const DataType& value)
    {
      put(value);
      return true;
    }
    
    /*!
     * @if jp
     *
     * @brief バッファから読み出す
     * 
     * バッファに格納されたデータを読み出す。
     * 
     * @param value 読み出したデータ
     *
     * @return データ読み出し結果(常に true:読み出し成功 を返す)
     * 
     * @else
     *
     * @brief Write data into the buffer
     *
     * @endif
     */
    virtual bool read(DataType& value)
    {
      value = get();
      return true;
    }
    
    /*!
     * @if jp
     *
     * @brief バッファが満杯であるか確認する
     * 
     * バッファ満杯を確認する。(常にfalseを返す。)
     *
     * @return 満杯確認結果(常にfalse)
     * 
     * @else
     *
     * @brief True if the buffer is full, else false.
     *
     * @endif
     */
    virtual bool isFull() const
    {
      return false;
    }
    
    /*!
     * @if jp
     *
     * @brief バッファが空であるか確認する
     * 
     * バッファ空を確認する。
     * 注)現在の実装では，現在のバッファ位置に格納されたデータが読み出されたか
     * どうかを返す。( true:データ読み出し済，false:データ未読み出し)
     *
     * @return 空確認結果
     * 
     * @else
     *
     * @brief True if the buffer is empty, else false.
     *
     * @endif
     */
    virtual bool isEmpty() const
    {
      return !(this->isNew());
    }
    
    /*!
     * @if jp
     *
     * @brief 最新データか確認する
     * 
     * 現在のバッファ位置に格納されているデータが最新データか確認する。
     *
     * @return 最新データ確認結果
     *           ( true:最新データ．データはまだ読み出されていない
     *            false:過去のデータ．データは既に読み出されている)
     * 
     * @else
     *
     * @endif
     */
    bool isNew() const
    {
      return m_buffer[m_newPtr].isNew();
    }
    
  protected:
    /*!
     * @if jp
     *
     * @brief バッファにデータを格納する
     * 
     * 引数で与えられたデータをバッファに格納する。
     * 注)現在の実装ではデータを格納すると同時に、データの読み出し位置を
     * 格納したデータ位置に設定している。このため、常に直近に格納したデータを
     * 取得する形となっている。
     * 
     * @param data 格納対象データ
     * 
     * @else
     *
     * @brief Write data into the buffer
     *
     * @endif
     */
    virtual void put(const DataType& data)
    {
      m_buffer[m_oldPtr].write(data);
      //      ACE_Guard<ACE_Thread_Mutex> guard(m_Mutex);
      m_newPtr = m_oldPtr;
      m_oldPtr = (++m_oldPtr) % m_length;
    }
    
    /*!
     * @if jp
     *
     * @brief バッファからデータを取得する
     * 
     * バッファに格納されたデータを取得する。
     *
     * @return 取得データ
     * 
     * @else
     *
     * @brief Get data from the buffer
     *
     * @endif
     */
    virtual const DataType& get()
    {
      return m_buffer[m_newPtr].read();
    }
    
    /*!
     * @if jp
     *
     * @brief 次に書き込むバッファへの参照を取得する
     * 
     * 書き込みバッファへの参照を取得する。
     * 
     * @return 次の書き込み対象バッファへの参照
     *
     * @else
     *
     * @brief Get the buffer's reference to be written the next
     *
     * @endif
     */
    virtual DataType& getRef()
    {
      return m_buffer[m_newPtr].data;
    }
    
  private:
    long int m_length;
    long int m_oldPtr;
    long int m_newPtr;
    
    /*!
     * @if jp
     * @class Data
     * @brief バッファ配列
     * 
     * バッファデータ格納用配列クラス。
     *
     * @param D バッファに格納するデータの型
     *
     * @since 0.4.0
     *
     * @else
     * @brief Buffer sequence
     * @endif
     */
    template <class D>
    class Data
    {
    public:
      Data() : data(), is_new(false){;}
      inline Data& operator=(const D& other)
      {
	this->data = other;
	this->is_new = true;
	return *this;
      }
      inline void write(const D& other)
      {
	this->is_new = true;
	this->data = other;
      }
      inline D& read()
      {
	this->is_new = false;
	return this->data;
      }
      inline bool isNew() const
      {
	return is_new;
      }
      D    data;
      bool is_new;
    };
    
    std::vector<Data<DataType> > m_buffer;
    
  };
}; // namespace RTC

#endif // RingBuffer_h
