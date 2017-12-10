// -*- C++ -*-
/*!
 * @file ConnectorListener.h
 * @brief connector listener class
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009-2017
 *     Noriaki Ando
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#ifndef RTC_CONNECTORLISTENER_H
#define RTC_CONNECTORLISTENER_H

#include <vector>
#include <utility>
#include <coil/Mutex.h>
#include <coil/Guard.h>
#include <rtm/RTC.h>
#include <rtm/ConnectorBase.h>

class cdrMemoryStream;

namespace RTC
{
  class ConnectorInfo;
  /*!
   * @if jp
   * @class ConnectorListenerStatus mixin class
   * @brief ConnectorListenerStatus mixin クラス
   *
   * このクラスは、enum��E舛気�E織�E拭璽鵐魁璽匹髻�ConnectorListener関
   * 連のサブクラスで共通利用す��E燭瓩� mixin クラスであ��E�この��E拭�
   * ンコードを使用す��E�ラスでは、ConnectorListenerStatus クラス��E   * public 継承し、下にdefine してあ��ECONNLISTENER_STATUS_ENUM をク��E   * ス内に記述す��E海箸罵�用可能とな��E�こ��E砲茲蝓�enum ��E   * ReturnCode 型として typedef し、以��EReturnCode を利用でき��E茲Δ�
   * す��E箸箸發法¬樵斡�間に enum ��E舛気�E審銅永婿劼鯏�該クラス名前��E   * 間内に導入す��E�
   *
   * @else
   * @class DataPortStatus mixin class
   * @brief DataPortStatus mixin class
   *
   * This is a mixin class to provide enumed return codes that are
   * commonly utilised in connector listener related sub-classes. To
   * use this class, sub-class should inherit this class as a public
   * super class, and declare CONNLISTENERSTATUS_ENUM defined
   * below. Consequently, ReturnCode type that is typedefed by this
   * macro can be used in the sub-class, and enumed identifiers are
   * imported to the class's namespace.
   *
   * @endif
   */
  class ConnectorListenerStatus
  {
  public:
    enum Enum
      {
        NO_CHANGE    = 0,
        INFO_CHANGED = 1 << 0,
        DATA_CHANGED = 1 << 1,
        BOTH_CHANGED = INFO_CHANGED | DATA_CHANGED
      };
  };
  ConnectorListenerStatus::Enum operator|(ConnectorListenerStatus::Enum L,
                                          ConnectorListenerStatus::Enum R);
  ConnectorListenerStatus::Enum operator&(ConnectorListenerStatus::Enum L,
                                          ConnectorListenerStatus::Enum R);

  /*!
   * @if jp
   *

  /*!
   * @if jp
   * いて、クラス��E世寮萋�において DATAPORTSTATUS_ENUM を記載す��E世韻如�
   * DataPortStatus で��E世気�E討い�Eenum メンバが名前解決演算子なしにア
   * クセス可能にな��E�
   *
   * @else
   *
   * @brief Importing ::RTC::DataPortStatus macro
   *
   * This macro imports all the member of enum declared in
   * ::RTC::DataPortStatus into the current namespace.  Inserting at the
   * head of class declaration, classes which utilize DataPortStatus can
   * access Enum members of DataPortStatus without using namespace
   * resolve operator.
   *
   * @endif
   */
#define USE_CONNLISTENER_STATUS                             \
  typedef ::RTC::ConnectorListenerStatus::Enum ReturnCode;  \
  using ::RTC::ConnectorListenerStatus::NO_CHANGE;          \
  using ::RTC::ConnectorListenerStatus::INFO_CHANGED;       \
  using ::RTC::ConnectorListenerStatus::DATA_CHANGED;       \
  using ::RTC::ConnectorListenerStatus::BOTH_CHANGED;

  /*!
   * @if jp
   * @brief ConnectorDataListener のタイプ
   *
   * - ON_BUFFER_WRITE:          バッファ書き込み時
   * - ON_BUFFER_FULL:           バッファフ��E�
   * - ON_BUFFER_WRITE_TIMEOUT:  バッファ書き込みタイムアウト時
   * @brief ConnectorDataListener のタイプ
   *
   * - ON_BUFFER_WRITE:          バッファ書き込み時
   * - ON_BUFFER_FULL:           バッファフ��E�
   * - ON_BUFFER_WRITE_TIMEOUT:  バッファ書き込みタイムアウト時
   * - ON_BUFFER_OVERWRITE:      バッファ上書き時
   * - ON_BUFFER_READ:           バッファ読み出し時
   * - ON_SEND:                  InProtへの送信時
   * - ON_RECEIVED:              InProtへの送信完了時
   * - ON_RECEIVER_FULL:         InProt側バッファフ��E�
   * - ON_RECEIVER_TIMEOUT:      InProt側バッファタイムアウト時
   * - ON_RECEIVER_ERROR:        InProt側エラー時
   *
   * @else
   * @brief The types of ConnectorDataListener
   * - ON_BUFFER_READ:           At the time of buffer read
   * - ON_BUFFER_WRITE:          At the time of buffer write
   * - ON_BUFFER_FULL:           At the time of buffer full
   * - ON_BUFFER_WRITE_TIMEOUT:  At the time of buffer write timeout
   * - ON_BUFFER_OVERWRITE:      At the time of buffer overwrite
   * - ON_BUFFER_READ:           At the time of buffer read
   * - ON_SEND:                  At the time of sending to InPort
   * - ON_RECEIVED:              At the time of finishing sending to InPort
   * - ON_RECEIVER_FULL:         At the time of bufferfull of InPort
   * - ON_RECEIVER_TIMEOUT:      At the time of timeout of InPort
   * - ON_RECEIVER_ERROR:        At the time of error of InPort
   *
   * @endif
   */
  enum ConnectorDataListenerType
    {
      ON_SEND,
      ON_RECEIVED,
      ON_RECEIVER_FULL,
      ON_RECEIVER_TIMEOUT,
      ON_RECEIVER_ERROR,
      CONNECTOR_DATA_LISTENER_NUM
      ON_RECEIVED,

  /*!
      ON_RECEIVER_ERROR,
      CONNECTOR_DATA_LISTENER_NUM
    };

  /*!
   * @if jp
   * @class ConnectorDataListener クラス
   * @brief ConnectorDataListener クラス
   *
   * データポートの Connector において発生す��E銅�Eぅ戰鵐箸紡个垢�E魁�
   * ��E丱奪�を実現す��E�E好淵�ラスの基��E�ラス。
   *
   * コア��糎ックがOutPortに対してデータ書き込み、InPort側でデータが��E   * 得さ��E�E泙任隆屬波�生す��E銅�Eぅ戰鵐箸鬟侫奪�す��E魁璽�E丱奪�を設
   * 定す��E海箸�でき��E�なお、��E好福璽�ラスは2��E狢減澆掘▲丱奪侫．�
   * ��E篩�信時のコー��E丱奪�で、その時点で有効なデータをファンクタの��E   * 数として受け取��EConnectorDataListener であり、もう��E�はデータエ
   * ンプティやバッファ読み込み時のタイムアウトなどデータが取得できない
   * ��E腓覆匹縫魁璽�E気�E�E侫．鵐�タの引数に何もとらならい
   * ConnecotorListener があ��E�
   *
   * データポートには、接続時にデータの送受信方法についてデータフ��充型、
   * サブスク��E廛轡腑鷏薪�を設定す��E海箸�でき��E�
   *
   * <pre>
   * class MyDataListener
   *   : public ConnectorDataListenerT<RTC::TimedLong>
   * {
   * public:
   *   MyDataListener(const char* name) : m_name(name) {}
   *   virtual ~MyDataListener() {}
   *   virtual ReturnCode operator()(ConnectorInfo& info, TimedLong& data)
   * -  Push型: Subscription Typeによりさらにイベントの��E爐�分か��E�E�
   *     std::cout << "Data Listener: " << m_name       << std::endl;
   *     std::cout << "Data:          " << data.data    << std::endl;
   *     std::cout << "Profile::name: " << info.name    << std::endl;
   *     std::cout << "Profile::id:   " << info.id      << std::endl;
   *     std::cout << "Profile::properties: "           << std::endl;
   *     std::cout << info.properties;
   *   };
   *   std::string m_name;
   * };
   * </pre>
   *
   * このようにして��E舛気�E織�E好淵�ラスは、以下のようにデータポートに
   * 対して、以下のようにセットさ��E�E�
   *
   * <pre>
   * RTC::ReturnCode_t ConsoleIn::onInitialize()
   * {
   *     m_outOut.
   *         addConnectorDataListener(ON_BUFFER_WRITE,
   *                                  new MyDataListener("ON_BUFFER_WRITE"));
   *    :
   * </pre>
   *
   * ��E引数の "ON_BUFFER_WRITE" は、コー��E丱奪�をフックす��E櫂ぅ鵐箸�
   * あり、以下に列挙す��E佑鮗茲�E海箸�可能であ��E�データポートには、接
   * 続時にデータの送受信方法について、インターフェース型、データフ��充
   * 型、サブスク��E廛轡腑鷏薪�を設定す��E海箸�でき��E�、こ��E蕕寮瀋蠅�
   * よ��E侫奪�さ��E�E櫂ぅ鵐箸楼曚覆�E０焚爾法▲ぅ鵐拭璽侫А璽垢�CORBA
   * CDR型の��E腓離魁璽�E丱奪���E�を示す。
   *
   * OutPort:
   * -  Push型: Subscription Typeによりさらにイベントの��E爐�分か��E�E�
   *   - Flush: Flush型にはバッファがないた��EON_BUFFER 系のイベントは発生しない
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     .
   *   - Flush: Flush型にはバッファがないた��EON_BUFFER 系のイベントは発生しない
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_OVERWRITE
   *     - ON_BUFFER_READ
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     - ON_SENDER_ERROR
   *     - ON_CONNECT
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_READ
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     - ON_BUFFER_EMPTY
   *     - ON_SENDER_EMPTY
   * - Pull型
   *   - ON_BUFFER_READ
   *   - ON_SEND
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_WRITE_OVERWRITE
   *   - ON_BUFFER_EMPTY
   *   - ON_BUFFER_READ_TIMEOUT
   *     - ON_RECEIVED
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_WRITE_OVERWRITE
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *
   * @else
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *
   * @class ConnectorDataListener class
   * @brief ConnectorDataListener class
   *
   * This class is abstract base class for listener classes that
   * provides callbacks for various events in the data port's
   * connectors.
   * such as buffer-empty event, buffer-read-timeout event and so on.
   * @endif
   */
   * ConnectorDataListener, a class which inherits this class should
   * be defined, and the functor should receive a connector
   * information and a data value as arguments. And then, the defined
   * class must be set to data-port object through its member
   * function, as follows.  Two types of ConnectorDataListeners are
   * available. One is "ConnectorDataListener" which receives
   * marshalled data as data value, the other is
   * "ConnectorDataListenerT" which receives unmarshalled data as data
   * value. The following example is the use of ConnectorDataListenerT.
   *
   * <pre>
   * class MyDataListener
   *   : public ConnectorDataListenerT<RTC::TimedLong>
   * {
   * public:
   *   MyDataListener(const char* name) : m_name(name) {}
   *   virtual ~MyDataListener() {}
   *   virtual ReturnCode operator()(ConnectorInfo& info, TimedLong& data)
   *   {
   *     std::cout << "Data Listener: " << m_name       << std::endl;
   *     std::cout << "Data:          " << data.data    << std::endl;
   *     std::cout << "Profile::name: " << info.name    << std::endl;
   *     std::cout << "Profile::id:   " << info.id      << std::endl;
   *     std::cout << "Profile::properties: "           << std::endl;
   *     std::cout << info.properties;
   *   };
   *   std::string m_name;
   * };
   * </pre>
   *
   * The listener class defained as above can be attached to a
   * data-port as follows.
   *
   * <pre>
   * RTC::ReturnCode_t ConsoleIn::onInitialize()
   * {
   *     m_outOut.
   *         addConnectorDataListener(ON_BUFFER_WRITE,
   *                                  new MyDataListener("ON_BUFFER_WRITE"));
   *    :
   * </pre>
   *
   * The first argument "ON_BUFFER_WRITE" specifies hook point of
   * callback, and the following values are available. Data-port can
   * be specified some properties such as interface-type,
   * dataflow-type, subscription type and so on. Available hook points
   * vary by the those settings. The following hook points are
   * available when interface type is CORBA CDR type.
   *
   * OutPort:
   * -  Push type: Available hook event varies by subscription type.
   *   - Flush: No ON_BUFFER* events since flush-type has no buffer.
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     .
   *   - New type:
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_OVERWRITE
   *     - ON_BUFFER_READ
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     .
   *   - Periodic type:
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_READ
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     .
   *   .
   * - Pull type:
   *   - ON_BUFFER_WRITE
   *   - ON_BUFFER_FULL
   *   - ON_BUFFER_WRITE_TIMEOUT
   *   - ON_BUFFER_OVERWRITE
   *   - ON_BUFFER_READ
   *   - ON_SEND
   *   .
   * InPort:
   * - Push type:
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_WRITE_OVERWRITE
   *     - ON_BUFFER_READ
   *     - ON_BUFFER_READ_TIMEOUT
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   *     .
   * - Pull type
   *     - ON_BUFFER_READ
   *
   * @endif
   */
  class ConnectorDataListener
    : public ConnectorListenerStatus
  {
  public:
    USE_CONNLISTENER_STATUS;
    /*!
     * @if jp
  class ConnectorDataListener
  {
  public:
     * ConnectorDataListenerType を文字列に変換す��E     *
    /*!
     * @if jp
     * @return 文字列変換��E�
     *
     * @brief ConnectorDataListenerType を文字列に変換
     *
     * ConnectorDataListenerType を文字列に変換す��E     *
     * @param type 変換対象 ConnectorDataListenerType
     *
     * @return 文字列変換��E�
     *
     * @else
     *
     * @brief Convert ConnectorDataListenerType into the string.
     *
     * Convert ConnectorDataListenerType into the string.
     *
     * @param type The target ConnectorDataListenerType for transformation
     *
     * @return Trnasformation result of string representation
     *
     * @endif
     */
    static const char* toString(ConnectorDataListenerType type)
    {
      static const char* typeString[] =
        {
          "ON_BUFFER_WRITE",
          "ON_BUFFER_FULL",
          "ON_BUFFER_WRITE_TIMEOUT",
          "ON_BUFFER_OVERWRITE",
          "ON_BUFFER_READ", 
          "ON_SEND", 
          "ON_RECEIVED",
          "ON_RECEIVER_FULL", 
          "ON_RECEIVER_TIMEOUT", 
          "ON_RECEIVER_ERROR",
          "CONNECTOR_DATA_LISTENER_NUM"
        };
      if (type < CONNECTOR_DATA_LISTENER_NUM) { return typeString[type]; }
      return "";
    }

    /*!
     * @if jp
     * @brief デストラクタ
     * @else
     * @brief Destructor
     * @endif
     */
    virtual ~ConnectorDataListener();

    /*!
     * @if jp
     *
     * @brief 仮想コー��E丱奪�メソッド
     *
     * データポートの Connector において発生す��E銅�Eぅ戰鵐箸紡个垢�E魁�
     * ��E丱奪�メソッド
     *
     * @else
     *
     * @brief Virtual Callback method
     *
     * This is a the Callback method to various events generated in Connector. 
     *
     * @endif
     */
    virtual void operator()(const ConnectorInfo& info,
                            const cdrMemoryStream& data) = 0;
   * ��E丱奪�を実現す��E�E好淵�ラスの基��E�ラス。
   * 

  /*!
   * @if jp
   * @class ConnectorDataListenerT クラス
   * @brief ConnectorDataListenerT クラス
   *
   * データポートの Connector において発生す��E銅�Eぅ戰鵐箸紡个垢�E魁�
   * ��E丱奪�を実現す��E�E好淵�ラスの基��E�ラス。
   * 
   * このクラスは、operator()() の��E引数に cdrMemoryStream 型ではなく、
   * 実際にデータポートで使用さ��E�E竸�型をテンプ��E璽醗�数として
   * 渡すことができ��E�
   *
   * @else
   * @class ConnectorDataListenerT class
   * @brief ConnectorDataListenerT class
   *
   * This class is abstract base class for listener classes that
   * provides callbacks for various events in the data port's
   * connectors.
   *
   * This class template can have practical data types that are used
   * as typed variable for DataPort as an argument of template instead
   * of cdrMemoryStream.
   *
   * @endif
   */
  template <class DataType>
  class ConnectorDataListenerT
    : public ConnectorDataListener
  {
  public:
    /*!
     * @if jp
     * @brief デストラクタ
     * @else
     * @brief Destructor
     * @endif
     */
    virtual ~ConnectorDataListenerT(){}

    /*!
     * @if jp
     *
     * @brief コー��E丱奪�メソッド
     *
     * データをデータポートで使用さ��E�E竸�型に変換して ConnectorDataListenerT
     * のコー��E丱奪�メソッドを呼び出す。
     *
     * @param info ConnectorInfo 
     * @param cdrdata cdrMemoryStream型のデータ
     *
     * @else
     *
     * @brief Callback method
     *
     * This method invokes the callback method of ConnectorDataListenerT. 
     * Data is converted into the variable type used in DataPort.
     *
     * @param info ConnectorInfo 
     * @param cdrdata Data of cdrMemoryStream type
     *
     * @endif
     */
    virtual void operator()(const ConnectorInfo& info,
                            const cdrMemoryStream& cdrdata)
    {
      DataType data;
#ifdef ORB_IS_ORBEXPRESS
      cdrMemoryStream cdr(cdrdata);
#elif defined(ORB_IS_TAO)
      cdrMemoryStream cdr(cdrdata);
#else
      cdrMemoryStream cdr(cdrdata.bufPtr(), cdrdata.bufSize());
#endif
      // endian type check
      std::string endian_type;
      endian_type = info.properties.getProperty("serializer.cdr.endian",
                                                "little");
      coil::normalize(endian_type);
      std::vector<std::string> endian(coil::split(endian_type, ","));

#ifdef ORB_IS_ORBEXPRESS
      if (endian[0] == "little")
        {
          cdr.cdr.is_little_endian(true);
        }
      else if (endian[0] == "big")
        {
          cdr.cdr.is_little_endian(false);
        }
       cdr.cdr >> data;
#elif defined(ORB_IS_TAO)
       TAO_InputCDR tao_cdr = TAO_InputCDR(cdr.cdr);
       tao_cdr >> data;
#else
      if (endian[0] == "little")
        {
          cdr.setByteSwapFlag(true);
        }
      else if (endian[0] == "big")
        {
          cdr.setByteSwapFlag(false);
        }
      data <<= cdr;
#endif





      this->operator()(info, data);
    }

    /*!
     * @if jp
     *
     * @brief 仮想コー��E丱奪�メソッド
     *
     * データポートの Connector において発生す��E銅�Eぅ戰鵐箸紡个垢�E魁�
     * ��E丱奪�メソッド
     *
     * @else
     *
     * @brief Virtual Callback method
     *
     * This method invokes the callback method of ConnectorDataListenerT. 
     * Data is converted into the variable type used in DataPort.
     *
     * @endif
     */
    virtual void operator()(const ConnectorInfo& info,
                            const DataType& data) = 0;
                            
   * - ON_SENDER_ERROR:       OutPort側エラー時
   * - ON_CONNECT:            接続確立時
  /*!
   * @if jp
   * @brief ConnectorListener のタイプ
   *  
   * - ON_BUFFER_EMPTY:       バッファが空の��E�E   * - ON_BUFFER_READTIMEOUT: バッファが空でタイムアウトした��E�E   * - ON_SENDER_EMPTY:       OutPort側バッファが��E   * - ON_SENDER_TIMEOUT:     OutPort側タイムアウト時
   * - ON_SENDER_ERROR:       OutPort側エラー時
   * - ON_CONNECT:            接続確立時
   * - ON_DISCONNECT:         接続切断時
   *
   * @else
   * @brief The types of ConnectorListener
   * 
   * - ON_BUFFER_EMPTY:       At the time of buffer empty
   * - ON_BUFFER_READTIMEOUT: At the time of buffer read timeout
   * - ON_BUFFER_EMPTY:       At the time of empty of OutPort
   * - ON_SENDER_TIMEOUT:     At the time of timeout of OutPort
   * - ON_SENDER_ERROR:       At the time of error of OutPort
   * - ON_CONNECT:            At the time of connection
   * - ON_DISCONNECT:         At the time of disconnection
   *
   * @endif
   */
  enum ConnectorListenerType
    {
      ON_BUFFER_EMPTY = 0,
      ON_BUFFER_READ_TIMEOUT,
      ON_SENDER_EMPTY, 
      ON_SENDER_TIMEOUT, 
      ON_SENDER_ERROR, 
      ON_CONNECT,
      ON_DISCONNECT,
      CONNECTOR_LISTENER_NUM
   * データポートの Connector において発生す��E銅�Eぅ戰鵐箸紡个垢�E魁�
   * ��E丱奪�を実現す��E�E好淵�ラスの基��E�ラス。
   *
   * @if jp
   * @class ConnectorListener クラス
   * @brief ConnectorListener クラス
   *
   * データポートの Connector において発生す��E銅�Eぅ戰鵐箸紡个垢�E魁�
   * ��E丱奪�を実現す��E�E好淵�ラスの基��E�ラス。
   *
   * コア��糎ックがOutPortに対してデータ書き込み、InPort側でデータが��E   * 得さ��E�E泙任隆屬波�生す��E銅�Eぅ戰鵐箸鬟侫奪�す��E魁璽�E丱奪�を設
   * 定す��E海箸�でき��E�なお、��E好福璽�ラスは2��E狢減澆掘▲丱奪侫．�
   * ��E篩�信時のコー��E丱奪�で、その時点で有効なデータをファンクタの��E   * 数として受け取��EConnectorDataListener であり、もう��E�はデータエ
   * ンプティやバッファ読み込み時のタイムアウトなどデータが取得できない
   * ��E腓覆匹縫魁璽�E気�E�E侫．鵐�タの引数に何もとらならい
   * ConnecotorListener があ��E�
   *
   * データポートには、接続時にデータの送受信方法についてデータフ��充型、
   * サブスク��E廛轡腑鷏薪�を設定す��E海箸�でき��E�
   * ConnectorDaataListener/ConnectorListener は共にに、様々なイベント
   * に対す��E魁璽�E丱奪�を設定す��E海箸�でき��E�、こ��E薀如璽織侫�充型
   * およびサブスク��E廛轡腑鷏燭寮瀋蠅鳳�じて、利用でき��E發痢△任�ない
   * もの、また呼び出さ��E�E織ぅ潺鵐阿�異な��E０焚爾法▲ぅ鵐拭璽侫А璽�
   * がCORBA CDR型の��E腓離魁璽�E丱奪���E�を示す。
   *
   *     std::cout << "Profile::name: " << info.name    << std::endl;
   *     std::cout << "Profile::id:   " << info.id      << std::endl;
   *     std::cout << "Profile::properties: "           << std::endl;
   *     std::cout << info.properties;
   *     - ON_RECEIVER_ERROR
   *   std::string m_name;
   * };
   * </pre>
   *
   * このようにして��E舛気�E織�E好淵�ラスは、以下のようにデータポートに
   * 対して、以下のようにセットさ��E�E�
   *
   * <pre>
   * RTC::ReturnCode_t ConsoleIn::onInitialize()
   * {
   *     m_outOut.
   *         addConnectorListener(ON_BUFFER_EMPTY,
   *                              new MyListener("ON_BUFFER_EMPTY"));
   *    :
   * </pre>
   *
   * ��E引数の "ON_BUFFER_EMPTY" は、コー��E丱奪�をフックす��E櫂ぅ鵐箸�
   * あり、以下に列挙す��E佑鮗茲�E海箸�可能であ��E�データポートには、接
   * 続時にデータの送受信方法について、インターフェース型、データフ��充
   * 型、サブスク��E廛轡腑鷏薪�を設定す��E海箸�でき��E�、こ��E蕕寮瀋蠅�
   * よ��E侫奪�さ��E�E櫂ぅ鵐箸楼曚覆�E０焚爾法▲ぅ鵐拭璽侫А璽垢�CORBA
   * CDR型の��E腓離魁璽�E丱奪���E�を示す。
   *
   * OutPort:
   * -  Push型: Subscription Typeによりさらにイベントの��E爐�分か��E�E�
   *   - Flush: Flush型にはバッファがないた��EON_BUFFER 系のイベントは発生しない
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     .
   *   - New型
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     .
   *   - Periodic型
   *     - ON_BUFFER_EMPTY
   *     - ON_BUFFER_READ_TIMEOUT
   *     - ON_SENDER_EMPTY
   *     - ON_SENDER_ERROR
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     .
   *     - ON_BUFFER_READ
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_SEND
   *     - ON_RECEIVED
   *     - ON_RECEIVER_FULL
   *     - ON_RECEIVER_TIMEOUT
   *   - ON_BUFFER_READ
   *   .
   *   - ON_SEND
   *   - ON_BUFFER_EMPTY
   *   - ON_BUFFER_READ_TIMEOUT
   *   - ON_SENDER_EMPTY
   *   - ON_SENDER_TIMEOUT
   *   - ON_SENDER_ERROR
   *   - ON_CONNECT
   * InPort:
   * - Push型:
   *     - ON_BUFFER_WRITE
   *     - ON_BUFFER_FULL
   *     - ON_BUFFER_WRITE_TIMEOUT
   *     - ON_BUFFER_WRITE_OVERWRITE
   *     - ON_RECEIVED
   *
   *     - ON_RECEIVER_TIMEOUT
   *     - ON_RECEIVER_ERROR
   * connectors.
   *
   * @class ConnectorListener class
   * @brief ConnectorListener class
   *
   * This class is abstract base class for listener classes that
   * provides callbacks for various events in the data port's
   * connectors.
   *
   * @endif
   */
  class ConnectorListener
  {
  public:
   * class must be set to data-port object through its member
     * @if jp
     *
   * <pre>
   * class MyListener
   *   : public ConnectorListener
   * {
   * public:
   *   MyListener(const char* name) : m_name(name) {}
   *   virtual ~MyListener() {}
   *   virtual ReturnCode operator()(ConnectorInfo& info)
   *   {
   *     std::cout << "Data Listener: " << m_name       << std::endl;
   *     std::cout << "Profile::name: " << info.name    << std::endl;
   *     std::cout << "Profile::id:   " << info.id      << std::endl;
   *     std::cout << "Profile::properties: "           << std::endl;
   *     std::cout << info.properties;
   *   };
   *   std::string m_name;
   * };
   * </pre>
   *
   * The listener class defained as above can be attached to a
   * data-port as follows.
   *
   * <pre>
   * RTC::ReturnCode_t ConsoleIn::onInitialize()
   * {
   *     m_outOut.
   *         addConnectorListener(ON_BUFFER_EMPTY,
   *                              new MyDataListener("ON_BUFFER_EMPTY"));
   *    :
   * </pre>
   *
   * The first argument "ON_BUFFER_EMPTY" specifies hook point of
   * callback, and the following values are available. Data-port can
   * be specified some properties such as interface-type,
   * dataflow-type, subscription type and so on. Available hook points
   * vary by the those settings. The following hook points are
   * available when interface type is CORBA CDR type.
   *
   * OutPort:
   * -  Push type: Available hook event varies by subscription type.
   *   - Flush: No ON_BUFFER* events since flush-type has no buffer.
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     .
   *   - New type:
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     .
   *   - Periodic type:
   *     - ON_BUFFER_EMPTY
   *     - ON_BUFFER_READ_TIMEOUT
   *     - ON_SENDER_EMPTY
   *     - ON_SENDER_ERROR
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     .
   *   .
   * - Pull type:
   *   - ON_BUFFER_EMPTY
   *   - ON_BUFFER_READ_TIMEOUT
   *   - ON_SENDER_EMPTY
   *   - ON_SENDER_TIMEOUT
   *   - ON_SENDER_ERROR
   *   - ON_CONNECT
   *   - ON_DISCONNECT
   *   .
   * InPort:
   * - Push type:
   *     - ON_BUFFER_EMPTY
   *     - ON_BUFFER_READ_TIMEOUT
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *     .
   * - Pull type:
   *     - ON_CONNECT
   *     - ON_DISCONNECT
   *
   * @endif
   */
  class ConnectorListener
    : public ConnectorListenerStatus
  {
  public:
    USE_CONNLISTENER_STATUS;
    /*!
     * @if jp
     *
     * @brief ConnectorListenerType を文字列に変換
     *
     * ConnectorListenerType を文字列に変換す��E     *
     * @param type 変換対象 ConnectorListenerType
     *
     * @brief ConnectorListenerType を文字列に変換
     *
     * ConnectorListenerType を文字列に変換す��E     *
     *
     * @param type 変換対象 ConnectorListenerType
     *
     * Convert ConnectorListenerType into the string.
     * @return 文字列変換��E�
     *
     * @else
     *
     * @brief Convert ConnectorListenerType into the string.
     *
     * Convert ConnectorListenerType into the string.
     *
     * @param type The target ConnectorListenerType for transformation
     *
     * @return Trnasformation result of string representation
     *
     * @endif
     */
    static const char* toString(ConnectorListenerType type)
    { 
      static const char* typeStr[] =
        {
          "ON_BUFFER_EMPTY",
          "ON_BUFFER_READ_TIMEOUT",
          "ON_SENDER_EMPTY", 
          "ON_SENDER_TIMEOUT", 
          "ON_SENDER_ERROR", 
          "ON_CONNECT",
          "ON_DISCONNECT",
          "CONNECTOR_LISTENER_NUM"
        };
      if (type < CONNECTOR_LISTENER_NUM) { return typeStr[type]; }
      return "";
    }

    /*!
     * @if jp
     * @brief デストラクタ
     * @else
     * @brief Destructor
     * @endif
     */
    virtual ~ConnectorListener();

    /*!
     * @if jp
     *
     * @brief 仮想コー��E丱奪�メソッド
     *
     * データポートの Connector において発生す��E銅�Eぅ戰鵐箸紡个垢�E魁�
     * ��E丱奪�メソッド
     *
     * @else
     *
     * @brief Virtual Callback method
     *
     * This method invokes the callback method of ConnectorDataListenerT. 
     * Data is converted into the variable type used in DataPort.
     *
     * @endif
     */
    virtual void operator()(const ConnectorInfo& info) = 0;
  };


  /*!
   * @if jp
   * @class ConnectorDataListenerHolder
   * @brief ConnectorDataListener ホ��E瀬�ラス
   *
   *
   *
   * @else
   * @class ConnectorDataListenerHolder
   * @brief ConnectorDataListener holder class
   *
   * This class manages one ore more instances of ConnectorDataListener class.
   *
   * @endif
   */
  class ConnectorDataListenerHolder
  {
    typedef std::pair<ConnectorDataListener*, bool> Entry;
    typedef coil::Guard<coil::Mutex> Guard;
  public:
    /*!
     * @if jp
     * @brief コンストラクタ
     * @else
     * @brief Constructor
     * @brief デストラクタ
     * @endif
     */
    ConnectorDataListenerHolder();
    /*!
    virtual ~ConnectorDataListenerHolder();
     * @if jp
     * @brief デストラクタ
     * @else
     * @brief Destructor
     * @endif
     */
    virtual ~ConnectorDataListenerHolder();
    
    /*!
     * @if jp
     *
     * @brief ��E好福爾猟媛�
     *
     * ��E好福爾鯆媛辰垢�E�
     *
     * @param listener 追加す��E�E好�
     * @param autoclean true:デストラクタで��E�E垢�E
     *                  false:デストラクタで��E�E靴覆�
     * @else
     *
     * @brief Add the listener.
     *
     * This method adds the listener. 
     *
     * @param listener Added listener
     * @param autoclean true:The listener is deleted at the destructor.,
     *                  false:The listener is not deleted at the destructor. 
     * @endif
     */
    void addListener(ConnectorDataListener* listener, bool autoclean);
    
    /*!
     * @if jp
     *
     * @brief ��E好福爾虜�E�E     *
     * ��E好覆鮑�E�E垢�E�
     *
     * @param listener ��E�E垢�E�E好�
     * @else
     *
     * @brief Remove the listener. 
     *
     * This method removes the listener. 
     *
     * @param listener Removed listener
     * @endif
     */
    void removeListener(ConnectorDataListener* listener);
    
    /*!
     * @if jp
     *
     * @brief ��E好福爾凌瑤鯑世�E     *
     * 現在登録さ��E討い�E�E好福漆瑤鯑世�E�
     *
     * @return listener��E     * @else
     *
     * @brief Getting number of listeners.
     *
     * This method returns current number of listenrs.
     *
     * @return number of listeners
     * @endif
     */
    size_t size();

    /*!
     * @if jp
     *
     * @brief ��E好福爾慊銘里垢�E     *
     * 登録さ��E討い�E�E好覆離魁璽�E丱奪�メソッドを呼び出す。
     *
     * @param info ConnectorInfo
     * @param cdrdata データ
     * @else
     *
     * @brief Notify listeners. 
     *
     * This calls the Callback method of the registered listener. 
     *
     * @param info ConnectorInfo
     * @param cdrdata Data
     * @endif
     */
    void notify(const ConnectorInfo& info,
                const cdrMemoryStream& cdrdata);

    /*!
     * @if jp
     *
     * @brief ��E好福爾慊銘里垢�Eデータ型指��E�)
     *
     * 登録さ��E討い�E�E好覆離魁璽�E丱奪�メソッドを呼び出す。
     * COnnectorDataListenerT 型のコー��E丱奪�のみコー��E気�E�E�
     *
     * @param info ConnectorInfo
     * @param typeddata データ（データ型指定あり）
     * @param info ConnectorInfo
     * @param typeddata Data
     * @brief Notify listeners. (Typed data version)
     *
     * This calls the Callback method of the registered listener.
     * This operation calls only ConnectorDataListenerT type callback.
     *
     * @param info ConnectorInfo
     * @param typeddata Data
      for (int i(0), len(m_listeners.size()); i < len; ++i)
     */
    template <class DataType>
    void notify(const ConnectorInfo& info, const DataType& typeddata)
    {
      Guard guard(m_mutex);
      for (int i(0), len(m_listeners.size()); i < len; ++i)
        {
          ConnectorDataListenerT<DataType>* listener(0);
          listener =
          dynamic_cast<ConnectorDataListenerT<DataType>*>(m_listeners[i].first);
          if (listener != 0)
            {
              listener->operator()(info, typeddata);
            }
        }
      return ret;

  private:
  private:
    std::vector<Entry> m_listeners;
    coil::Mutex m_mutex;
  };


  /*!
   * @if jp
   * @class ConnectorListenerHolder 
   * @brief ConnectorListener ホ��E瀬�ラス
   *
   * 複数の ConnectorListener を保持し管理す��E�ラス。
   *
   * @else
   * @class ConnectorListenerHolder
   * @brief ConnectorListener holder class
   *
   * This class manages one ore more instances of ConnectorListener class.
   * 複数の ConnectorListener を保持し管理す��E�ラス。
   *
   * @else
   * @class ConnectorListenerHolder
   * @brief ConnectorListener holder class
   *
   * This class manages one ore more instances of ConnectorListener class.
   *
   * @endif
   */
  class ConnectorListenerHolder
  {
    typedef std::pair<ConnectorListener*, bool> Entry;
    typedef coil::Guard<coil::Mutex> Guard;
  public:
    /*!
     * @if jp
     * @brief コンストラクタ
     * @else
     * @brief Constructor
     * @endif
     */
    ConnectorListenerHolder();
    
     * @endif
    /*!
     * @if jp
     * @brief デストラクタ
     * @else
     * @if jp
     * @brief Destructor
     * @endif
     */
    virtual ~ConnectorListenerHolder();
    
    /*!
     * @if jp
     *
     * @brief ��E好福爾猟媛�
     *
     * ��E好福爾鯆媛辰垢�E�
     *
     * @param listener 追加す��E�E好�
     * @param autoclean true:デストラクタで��E�E垢�E
     *                  false:デストラクタで��E�E靴覆�
     * @else
     *
     * @brief Add the listener.
     *
     * This method adds the listener. 
     *
     * @param listener Added listener
     * @param autoclean true:The listener is deleted at the destructor.,
     *                  false:The listener is not deleted at the destructor. 
     * @endif
     */
    void addListener(ConnectorListener* listener, bool autoclean);
    
    /*!
     * @if jp
     *
     * @brief ��E好福爾虜�E�E     *
     * ��E好覆鮑�E�E垢�E�
     *
     * @param listener ��E�E垢�E�E好�
     * @else
     *
     * @brief Remove the listener. 
     *
     * This method removes the listener. 
     *
     * @param listener Removed listener
     * @endif
     */
    void removeListener(ConnectorListener* listener);

    /*!
     * @if jp
     *
     * @brief ��E好福爾凌瑤鯑世�E     *
     * 現在登録さ��E討い�E�E好福漆瑤鯑世�E�
     *
     * @return listener��E     * @else
     *
     * @brief Getting number of listeners.
     *
     * This method returns current number of listenrs.
     *
     * @return number of listeners
     * @endif
     */
    size_t size();

    /*!
     * @if jp
     *
     * @brief ��E好福爾慊銘里垢�E     *
     * 登録さ��E討い�E�E好覆離魁璽�E丱奪�メソッドを呼び出す。
     *
     * @param info ConnectorInfo
     * @else
     *
     * @brief Notify listeners. 
     *
     * This calls the Callback method of the registered listener. 
     *
     * @param info ConnectorInfo
     * @endif
     */
    void notify(const ConnectorInfo& info);
      
  private:
    std::vector<Entry> m_listeners;
    coil::Mutex m_mutex;
  };
  
  /*!
   * @if jp
   * @class ConnectorListeners
   * @brief ConnectorListeners クラス
   *
   *
   * @else
   * @class ConnectorListeners
   * @brief ConnectorListeners class
   *
     * @if jp
   * @endif
   */
  class ConnectorListeners
  {
  public:
    /*!
     * @if jp
     * @brief ConnectorDataListenerType��E好頁枸�E     * ConnectorDataListenerType��E好覆魍頁�
     * @else
     * @brief ConnectorDataListenerType listener array
     * The ConnectorDataListenerType listener is stored.
     * @endif
     */
    ConnectorDataListenerHolder connectorData_[CONNECTOR_DATA_LISTENER_NUM];
    /*!
     * @if jp
     * @brief ConnectorListenerType��E好頁枸�E     * ConnectorListenerType��E好覆魍頁�
     * @else
     * @brief ConnectorListenerType listener array
     * The ConnectorListenerType listener is stored. 
     * @endif
