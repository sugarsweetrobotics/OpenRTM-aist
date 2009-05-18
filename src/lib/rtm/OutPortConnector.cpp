/*! -*- C++ -*-
 *
 * @file ConnectorBase.h
 * @brief Connector base class
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#include <rtm/OutPortConnector.h>

namespace RTC
{
  /*!
   * @if jp
   * @brief コンストラクタ
   * @else
   * @brief Constructor
   * @endif
   */
  OutPortConnector::OutPortConnector(ConnectorBase::Profile& profile,
                                     CdrBufferBase* buffer)
    : rtclog("OutPortConnector"), m_profile(profile), m_buffer(buffer)
  {
  }

  /*!
   * @if jp
   * @brief デストラクタ
   * @else
   * @brief Destructor
   * @endif
   */
  OutPortConnector::~OutPortConnector()
  {
  }
  /*!
   * @if jp
   * @brief Profile 取得
   *
   * Connector Profile を取得する
   *
   * @else
   * @brief Getting Profile
   *
   * This operation returns Connector Profile
   *
   * @endif
   */
  const ConnectorBase::Profile& OutPortConnector::profile()
  {
    RTC_TRACE(("profile()"));
    return m_profile;
  }

  /*!
   * @if jp
   * @brief Connector ID 取得
   *
   * Connector ID を取得する
   *
   * @else
   * @brief Getting Connector ID
   *
   * This operation returns Connector ID
   *
   * @endif
   */
  const char* OutPortConnector::id()
  {
    RTC_TRACE(("id() = %s", profile().id.c_str()));
    return profile().id.c_str();
  }

  /*!
   * @if jp
   * @brief Connector 名取得
   *
   * Connector 名を取得する
   *
   * @else
   * @brief Getting Connector name
   *
   * This operation returns Connector name
   *
   * @endif
   */
  const char* OutPortConnector::name()
  {
    RTC_TRACE(("name() = %s", profile().name.c_str()));
    return profile().name.c_str();
  }

  /*!
   * @if jp
   * @brief Buffer を所得する
   *
   * Connector が保持している Buffer を返す
   *
   * @else
   * @brief Getting Buffer
   *
   * This operation returns this connector's buffer
   *
   * @endif
   */
  CdrBufferBase* OutPortConnector::getBuffer()
  {
    return m_buffer;
  }



}; // namespace RTC
