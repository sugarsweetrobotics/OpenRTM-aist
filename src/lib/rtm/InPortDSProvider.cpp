﻿// -*- C++ -*-
/*!
 * @file  InPortDSProvider.cpp
 * @brief InPortDSProvider class
 * @date  $Date: 2018-09-20 07:49:59 $
 * @author Nobuhiko Miyamoto <n-miyamoto@aist.go.jp>
 *
 * Copyright (C) 2018
 *     Nobuhiko Miyamoto
 *     Robot Innovation Research Center,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 *
 */

#include <rtm/InPortDSProvider.h>

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

namespace RTC
{
  /*!
   * @if jp
   * @brief コンストラクタ
   * @else
   * @brief Constructor
   * @endif
   */
  InPortDSProvider::InPortDSProvider(void)
      : m_buffer(0), m_connector(0)
  {
    // PortProfile setting
    setInterfaceType("data_service");

    // ConnectorProfile setting

#ifdef ORB_IS_OMNIORB
    ::RTC::Manager::instance().theShortCutPOA()->activate_object(this);
#endif
    m_objref = this->_this();

    // set InPort's reference
    CORBA::ORB_var orb = ::RTC::Manager::instance().getORB();
    CORBA::String_var ior = orb->object_to_string(m_objref.in());
    CORBA_SeqUtil::
      push_back(m_properties,
                NVUtil::newNV("dataport.data_service.inport_ior", ior.in()));
    CORBA_SeqUtil::
      push_back(m_properties,
                NVUtil::newNV("dataport.data_service.inport_ref", m_objref));
  }

  /*!
   * @if jp
   * @brief デストラクタ
   * @else
   * @brief Destructor
   * @endif
   */
  InPortDSProvider::~InPortDSProvider(void)
  {
    try
      {
        PortableServer::ObjectId_var oid;
#ifdef ORB_IS_OMNIORB
        oid = ::RTC::Manager::instance().theShortCutPOA()->servant_to_id(this);
        ::RTC::Manager::instance().theShortCutPOA()->deactivate_object(oid);
#else
        oid = _default_POA()->servant_to_id(this);
        _default_POA()->deactivate_object(oid);
#endif
      }
    catch (PortableServer::POA::ServantNotActive &e)
      {
#ifdef ORB_IS_ORBEXPRESS
        oe_out << e << oe_endl << oe_flush;
#else
        RTC_ERROR(("%s", e._name()));
#endif
      }
    catch (PortableServer::POA::WrongPolicy &e)
      {
#ifdef ORB_IS_ORBEXPRESS
        oe_out << e << oe_endl << oe_flush;
#else
        RTC_ERROR(("%s", e._name()));
#endif
      }
    catch (...)
      {
        // never throws exception
        RTC_ERROR(("Unknown exception caught."));
      }
  }

  void InPortDSProvider::init(coil::Properties& prop)
  {
  }

  /*!
   * @if jp
   * @brief バッファをセットする
   * @else
   * @brief Setting outside buffer's pointer
   * @endif
   */
  void InPortDSProvider::
  setBuffer(BufferBase<ByteData>* buffer)
  {
    m_buffer = buffer;
  }

  /*!
   * @if jp
   * @brief リスナを設定する
   * @else
   * @brief Set the listener
   * @endif
   */
  void InPortDSProvider::setListener(ConnectorInfo& info,
                                           ConnectorListeners* listeners)
  {
    m_profile = info;
    m_listeners = listeners;
  }

  /*!
   * @if jp
   * @brief Connectorを設定する。
   * @else
   * @brief set Connector
   * @endif
   */
  void InPortDSProvider::setConnector(InPortConnector* connector)
  {
    m_connector = connector;
  }

  /*!
   * @if jp
   * @brief バッファにデータを書き込む
   * @else
   * @brief Write data into the buffer
   * @endif
   */
  ::RTC::PortStatus
  InPortDSProvider::push(const ::RTC::OctetSeq& data)
    throw (CORBA::SystemException)
  {
    RTC_PARANOID(("InPortDSProvider::push()"));

    if (m_connector == 0)
      {
        ByteData cdr;

#ifdef ORB_IS_ORBEXPRESS
        cdr.writeData((unsigned char*)data.get_buffer(), (CORBA::ULong)data.length());
#elif defined(ORB_IS_TAO)
        cdr.writeData((unsigned char*)data.get_buffer(), (CORBA::ULong)data.length());
#else
        cdr.writeData((unsigned char*)&(data[0]), (CORBA::ULong)data.length());
#endif

        onReceiverError(cdr);
        return ::RTC::PORT_ERROR;
      }

    RTC_PARANOID(("received data size: %d", data.length()))
    ByteData cdr;
    // set endian type
    bool endian_type = m_connector->isLittleEndian();
    RTC_TRACE(("connector endian: %s", endian_type ? "little":"big"));

    cdr.isLittleEndian(endian_type);
#ifdef ORB_IS_ORBEXPRESS
    cdr.writeData((unsigned char*)data.get_buffer(), (CORBA::ULong)data.length());
#elif defined(ORB_IS_TAO)
    cdr.writeData((unsigned char*)data.get_buffer(), (CORBA::ULong)data.length());
#else
    cdr.writeData((unsigned char*)&(data[0]), (CORBA::ULong)data.length());
#endif
    RTC_PARANOID(("converted CDR data size: %d", cdr.getDataLength()));


    onReceived(cdr);
    BufferStatus::Enum ret = m_connector->write(cdr);

    return convertReturn(ret, cdr);
  }

  /*!
   * @if jp
   * @brief リターンコード変換
   * @else
   * @brief Return codes conversion
   * @endif
   */
  ::RTC::PortStatus
  InPortDSProvider::convertReturn(BufferStatus::Enum status,
                                        ByteData& data)
  {
    switch (status)
      {
      case BufferStatus::BUFFER_OK:
        onBufferWrite(data);
        return ::RTC::PORT_OK;
        break;

      case BufferStatus::BUFFER_ERROR:
        onReceiverError(data);
        return ::RTC::PORT_ERROR;
        break;

      case BufferStatus::BUFFER_FULL:
        onBufferFull(data);
        onReceiverFull(data);
        return ::RTC::BUFFER_FULL;
        break;

      case BufferStatus::BUFFER_EMPTY:
        // never come here
        return ::RTC::BUFFER_EMPTY;
        break;

      case BufferStatus::PRECONDITION_NOT_MET:
        onReceiverError(data);
        return ::RTC::PORT_ERROR;
        break;

      case BufferStatus::TIMEOUT:
        onBufferWriteTimeout(data);
        onReceiverTimeout(data);
        return ::RTC::BUFFER_TIMEOUT;
        break;

      default:
        return ::RTC::UNKNOWN_ERROR;
      }

    onReceiverError(data);
    return ::RTC::UNKNOWN_ERROR;
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
  void InPortDSProviderInit(void)
  {
    RTC::InPortProviderFactory& factory(RTC::InPortProviderFactory::instance());
    factory.addFactory("data_service",
                       ::coil::Creator< ::RTC::InPortProvider,
                                        ::RTC::InPortDSProvider>,
                       ::coil::Destructor< ::RTC::InPortProvider,
                                           ::RTC::InPortDSProvider>);
  }
};
