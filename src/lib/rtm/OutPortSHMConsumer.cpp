﻿// -*- C++ -*-
/*!
 * @file  OutPortSHMConsumer.h
 * @brief OutPortSHMConsumer class
 * @date  $Date: 2017-10-30 07:49:59 $
 * @author Nobuhiko Miyamoto <n-miyamoto@aist.go.jp>
 *
 * Copyright (C) 2017 Nobuhiko Miyamoto
 *     Robot Innovation Research Center,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 */

#include <rtm/Manager.h>
#include <rtm/OutPortSHMConsumer.h>
#include <rtm/NVUtil.h>
#include <coil/UUID.h>
#include <coil/Guard.h>

namespace RTC
{
  /*!
   * @if jp
   * @brief コンストラクタ
   * @else
   * @brief Constructor
   * @endif
   */
  OutPortSHMConsumer::OutPortSHMConsumer()
    : rtclog("OutPortSHMConsumer"),
      m_buffer(NULL),
      m_listeners(NULL)
  {
  }

  /*!
   * @if jp
   * @brief デストラクタ
   * @else
   * @brief Destructor
   * @endif
   */
  OutPortSHMConsumer::~OutPortSHMConsumer()
  {
    try
      {
        _ptr()->close_memory(true);
      }
    catch(...)
      {
      }
  }

  /*!
   * @if jp
   * @brief 設定初期化
   * @else
   * @brief Initializing configuration
   * @endif
   */
  void OutPortSHMConsumer::init(coil::Properties& prop)
  {
    RTC_TRACE(("OutPortSHMConsumer::init()"));
  }

  /*!
   * @if jp
   * @brief バッファをセットする
   * @else
   * @brief Setting outside buffer's pointer
   * @endif
   */
  void OutPortSHMConsumer::setBuffer(CdrBufferBase* buffer)
  {
    RTC_TRACE(("OutPortSHMConsumer::setBuffer()"));
    m_buffer = buffer;
  }

  /*!
   * @if jp
   * @brief リスナを設定する。
   * @else
   * @brief Set the listener. 
   * @endif
   */
  void OutPortSHMConsumer::setListener(ConnectorInfo& info,
                                            ConnectorListeners* listeners)
  {
    RTC_TRACE(("OutPortSHMConsumer::setListener()"));
    m_listeners = listeners;
    m_profile = info;
  }

  bool OutPortSHMConsumer::setObject(CORBA::Object_ptr obj)
  {
    RTC_PARANOID(("setObject()"));
    if (CorbaConsumer< ::OpenRTM::PortSharedMemory >::setObject(obj))
      {
        //::OpenRTM::PortSharedMemory_var sm = m_shmem.getObjRef();
        _ptr()->setInterface(m_shmem._this());
        return true;
      }


    return false;
  }

  /*!
   * @if jp
   * @brief データを読み出す
   * @else
   * @brief Read data
   * @endif
   */
  OutPortConsumer::ReturnCode
  OutPortSHMConsumer::get(cdrMemoryStream& data)
  {
    RTC_TRACE(("OutPortSHMConsumer::get()"));

    try
      {
        Guard guard(m_mutex);

        ::OpenRTM::PortStatus ret(_ptr()->get());
        if (ret == ::OpenRTM::PORT_OK)
          {
            m_shmem.read(data);

            RTC_DEBUG(("get() successful"));
            RTC_PARANOID(("CDR data length: %d", data.bufSize()));

            onReceived(data);
            onBufferWrite(data);

            if (m_buffer->full())
              {
                RTC_INFO(("InPort buffer is full."));
                onBufferFull(data);
                onReceiverFull(data);
              }
            m_buffer->put(data);
            m_buffer->advanceWptr();
            m_buffer->advanceRptr();
            return PORT_OK;
          }
        return convertReturn(ret, data);
      }
    catch (...)
      {
        RTC_WARN(("Exception caought from OutPort::get()."));
        return CONNECTION_LOST;
      }
    RTC_ERROR(("OutPortSHMConsumer::get(): Never comes here."));
    return UNKNOWN_ERROR;
  }

  /*!
   * @if jp
   * @brief データ受信通知への登録
   * @else
   * @brief Subscribe the data receive notification
   * @endif
   */
  bool OutPortSHMConsumer::
  subscribeInterface(const SDOPackage::NVList& properties)
  {
    RTC_TRACE(("OutPortSHMConsumer::subscribeInterface()"));
    CORBA::Long index;
    index = NVUtil::find_index(properties,
                               "dataport.corba_cdr.outport_ior");
    if (index < 0)
      {
        RTC_DEBUG(("dataport.corba_cdr.outport_ior not found."));
        return false;
      }

    if (NVUtil::isString(properties,
                         "dataport.corba_cdr.outport_ior"))
      {
        RTC_DEBUG(("dataport.corba_cdr.outport_ior found."));
        const char* ior;
        properties[index].value >>= ior;

        CORBA::ORB_var orb = ::RTC::Manager::instance().getORB();
        CORBA::Object_var var = orb->string_to_object(ior);
        bool ret(setObject(var.in()));
        if (ret)
          {
            RTC_DEBUG(("CorbaConsumer was set successfully."));
          }
        else
          {
            RTC_ERROR(("Invalid object reference."))
          }
        return ret;
      }

    return false;
  }

  /*!
   * @if jp
   * @brief データ受信通知からの登録解除
   * @else
   * @brief Unsubscribe the data receive notification
   * @endif
   */
  void OutPortSHMConsumer::
  unsubscribeInterface(const SDOPackage::NVList& properties)
  {
    RTC_TRACE(("OutPortSHMConsumer::unsubscribeInterface()"));
    CORBA::Long index;
    index = NVUtil::find_index(properties,
                               "dataport.corba_cdr.outport_ior");
    if (index < 0)
      {
        RTC_DEBUG(("dataport.corba_cdr.outport_ior not found."));
        return;
      }

    const char* ior;
    if (properties[index].value >>= ior)
      {
        RTC_DEBUG(("dataport.corba_cdr.outport_ior found."));
        CORBA::ORB_var orb = ::RTC::Manager::instance().getORB();
        CORBA::Object_var var = orb->string_to_object(ior);
        if (_ptr()->_is_equivalent(var))
          {
            releaseObject();
            RTC_DEBUG(("CorbaConsumer's reference was released."));
            return;
          }
        RTC_ERROR(("hmm. Inconsistent object reference."));
      }
  }

  /*!
   * @if jp
   * @brief リターンコード変換 (DataPortStatus -> BufferStatus)
   * @else
   * @brief Return codes conversion
   * @endif
   */
  OutPortConsumer::ReturnCode
  OutPortSHMConsumer::convertReturn(::OpenRTM::PortStatus status,
                                         const cdrMemoryStream& data)
  {
    switch(status)
      {
      case ::OpenRTM::PORT_OK:
        // never comes here
        return PORT_OK;
        break;
      case ::OpenRTM::PORT_ERROR:
        onSenderError();
        return PORT_ERROR;
        break;
      case ::OpenRTM::BUFFER_FULL:
        // never comes here
        return BUFFER_FULL;
        break;
      case ::OpenRTM::BUFFER_EMPTY:
        onSenderEmpty();
        return BUFFER_EMPTY;
        break;
      case ::OpenRTM::BUFFER_TIMEOUT:
        onSenderTimeout();
        return BUFFER_TIMEOUT;
        break;
      case ::OpenRTM::UNKNOWN_ERROR:
        onSenderError();
        return UNKNOWN_ERROR;
        break;
      default:
        onSenderError();
        return UNKNOWN_ERROR;
      }
  }
};     // namespace RTC

extern "C"
{
  /*!
   * @if jp
   * @brief モジュール初期化関数
   * @else
   * @brief Module initialization
   * @endif
   */
  void OutPortSHMConsumerInit(void)
  {
    RTC::OutPortConsumerFactory&
      factory(RTC::OutPortConsumerFactory::instance());
    factory.addFactory("shared_memory",
                       ::coil::Creator< ::RTC::OutPortConsumer,
                                        ::RTC::OutPortSHMConsumer>,
                       ::coil::Destructor< ::RTC::OutPortConsumer,
                                           ::RTC::OutPortSHMConsumer>);
  }
};
