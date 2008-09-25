// -*- C++ -*-
/*!
 * @file SdoConfiguration.cpp
 * @brief SDO's Configuration implementation class
 * @date $Date: 2008-01-14 07:49:37 $
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
 * $Id$
 *
 */

#include <coil/UUID.h>
#include "rtm/SdoConfiguration.h"
#include <rtm/CORBA_SeqUtil.h>
#include <rtm/NVUtil.h>
#include <memory>
#include <iostream>
// ACE

namespace SDOPackage
{
  /*!
   * @if jp
   *
   * @brief SDO ConfigurationSetを プロパティに格納
   * 
   * SDO ConfigurationSetを RTC用プロパティに格納する。
   *
   * @param prop 格納先プロパティ
   * @param conf SDO ConfigurationSet
   * 
   * @else
   *
   * @brief Store SDO ConfigurationSet to properties
   * 
   * Store SDO ConfigurationSet to properties for RTC.
   *
   * @param prop Properties to store
   * @param conf SDO ConfigurationSet
   * 
   * @endif
   */
  void 
  toProperties(RTC::Properties& prop, const SDOPackage::ConfigurationSet& conf)
  {
    NVUtil::copyToProperties(prop, conf.configuration_data);
    //    prop["description"] = conf.description;
  }
  
  /*!
   * @if jp
   *
   * @brief プロパティを SDO ConfigurationSetに変換
   * 
   * RTC用プロパティをSDO ConfigurationSetに変換する。
   *
   * @param conf 格納先 SDO ConfigurationSet
   * @param prop プロパティ
   * 
   * @else
   *
   * @brief Convert properties into SDO ConfigurationSet
   * 
   * Convert properties for RTC into SDO ConfigurationSet.
   *
   * @param conf SDO ConfigurationSet to store
   * @param prop Properties
   * 
   * @endif
   */
  void
  toConfigurationSet(SDOPackage::ConfigurationSet& conf,
		     const RTC::Properties& prop)
  {
    conf.description = CORBA::string_dup(prop["description"].c_str());
    conf.id = CORBA::string_dup(prop.getName());
    NVUtil::copyFromProperties(conf.configuration_data, prop);
  }
  
  //============================================================
  // Ctor and Dtor
  //============================================================
  
  /* @if jp
   * @brief コンストラクタ
   * @else
   * @brief Constructor
   * @endif
   */
  Configuration_impl::Configuration_impl(RTC::ConfigAdmin& configsets)
    : m_configsets(configsets)
  {
    m_objref = this->_this();
  }
  
  /* @if jp
   * @brief 仮想デストラクタ
   * @else
   * @brief Virtual destructor
   * @endif
   */
  Configuration_impl::~Configuration_impl()
  {
  }
  
  //============================================================
  // Basic Configuration 
  //============================================================
  /* @if jp
   * @brief [CORBA interface] SDO の DeviceProfile をセットする
   * @else
   * @brief [CORBA interface] Set DeviceProfile of SDO
   * @endif
   */
  CORBA::Boolean
  Configuration_impl::set_device_profile(const DeviceProfile& dProfile)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    try
      {
	Guard gurad(m_dprofile_mutex);
	m_deviceProfile = dProfile;
      }
    catch (...)
      {
	throw InternalError("Unknown Error");
	// never reach here
	return false;
      }
    return true;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] SDO の ServiceProfile のセット
   * @else
   * @brief [CORBA interface] Set SDO's ServiceProfile
   * @endif
   */
  CORBA::Boolean
  Configuration_impl::set_service_profile(const ServiceProfile& sProfile)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    // SDO specification defines that InvalidParameter() exception
    // is thrown when sProfile is null.
    // But sProfile is reference and it becomes never null.
    // So this operation does not throw InvalidParameter exception.
    //    if (CORBA::is_nil(sProfile.service)) throw InvalidParameter();
    try
      {
	if (strcmp(sProfile.id, "") == 0)
	  {
	    ServiceProfile prof(sProfile);
	    prof.id = CORBA::string_dup(getUUID().c_str());
	    CORBA_SeqUtil::push_back(m_serviceProfiles, prof);
	    return true;
	  }
	
	CORBA::Long index;
	index = CORBA_SeqUtil::find(m_serviceProfiles,
				    service_id(sProfile.id));
	if (index >= 0)
	  {
	    CORBA_SeqUtil::erase(m_serviceProfiles, index);
	  }
	CORBA_SeqUtil::push_back(m_serviceProfiles, sProfile);
	return true;
      }
    catch (...)
      {
	throw InternalError("Configuration::set_service_profile");
	return false;
      }
    // never reach here
    return true;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] Organization の追加
   * @else
   * @brief [CORBA interface] Add Organization
   * @endif
   */ 
  CORBA::Boolean
  Configuration_impl::add_organization(Organization_ptr org)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    try
      {
	CORBA_SeqUtil::push_back(m_organizations, org);
      }
    catch (...)
      {
	throw InternalError("Configuration::set_service_profile");
	// never reach here
	return false;
      }
    return true;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] ServiceProfile の削除
   * @else
   * @brief [CORBA interface] Remove ServiceProfile
   * @endif
   */  
  CORBA::Boolean
  Configuration_impl::remove_service_profile(const char* id)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    try
      {
	CORBA_SeqUtil::erase_if(m_serviceProfiles, service_id(id));
      }
    catch (...)
      {
	throw InternalError("Configuration::remove_service_profile");
	// never reach here
	return false;
      }
    return true;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] Organization の参照の削除
   * @else
   * @brief [CORBA interface] Remove the reference of Organization 
   * @endif
   */
  CORBA::Boolean
  Configuration_impl::remove_organization(const char* organization_id)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    try
      {
	Guard gurad(m_org_mutex);
	CORBA_SeqUtil::erase_if(m_organizations, org_id(organization_id));
      }
    catch (...)
      {
	throw InternalError("Configuration::remove_organization");
	// never reach here
	return false;
      }
    return true;
  }
  
  //============================================================
  // Configuration Parameter manipulation
  //============================================================
  /*!
   * @if jp
   * @brief [CORBA interface] 設定パラメータのリストの取得
   * @else
   * @brief [CORBA interface] Get a list of configuration parameters
   * @endif
   */ 
  ParameterList*
  Configuration_impl::get_configuration_parameters()
    throw (CORBA::SystemException,
	   NotAvailable, InternalError)
  {
    try
      {
	Guard gaurd(m_params_mutex);
	ParameterList_var param;
	param = new ParameterList(m_parameters);
	return param._retn();
      }
    catch (...)
      {
	throw InternalError("Configuration::get_configuration_parameters()");
	//never reach here
      }
    // never reach here
    return new ParameterList(0);
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] Configuration parameter の値のリストの取得
   * @else
   * @brief [CORBA interface] Get a list of the value of configuration parameters
   * @endif
   */
  NVList*
  Configuration_impl::get_configuration_parameter_values()
    throw (CORBA::SystemException,
	   NotAvailable, InternalError)
  {
    Guard guard(m_config_mutex);
    NVList_var nvlist;
    nvlist = new NVList((CORBA::ULong)0);
    
    /*
      CORBA::Long index;
      index = getActiveConfigIndex();
      if (index >= 0)
      {
      nvlist = new NVList(m_configurations[index].configuration_data);
      }
    */
    return nvlist._retn();
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] Configuration parameter の値の取得
   * @else
   * @brief [CORBA interface] Get the value of configuration parameter
   * @endif
   */
  CORBA::Any*
  Configuration_impl::get_configuration_parameter_value(const char* name)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    if (std::string(name).empty()) throw InvalidParameter("Name is empty.");
    
    //    CORBA::Long index;
    CORBA::Any_var value;
    value = new CORBA::Any();
    /*
      index = getActiveConfigIndex();
      if (index < 0) throw InternalError("No active configuration.");
      
      CORBA::Long item;
      item = CORBA_SeqUtil::find(m_configurations[index].configuration_data,
      nv_name(name));
      if (item < 0) throw InvalidParameter("No such name."); 
      
      value = new CORBA::Any(m_configurations[index].configuration_data[item].value);
    */
    return value._retn();
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] Configuration パラメータの変更
   * @else
   * @brief [CORBA interface] Modify the configuration parameter value
   * @endif
   */
  CORBA::Boolean
  Configuration_impl::set_configuration_parameter(const char* name,
						  const CORBA::Any& value)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    /*
      if (name == "") throw InvalidParameter("Name is empty.");
      
      CORBA::Long index(getActiveConfigIndex());
      if (index < 0) throw InternalError("No active config.");
      
      CORBA::Long item;
      item = CORBA_SeqUtil::find(m_configurations[index].configuration_data,
      nv_name(name));
      if (item < 0) throw InvalidParameter("No such name.");
      
      m_configurations[index].configuration_data[item].value = value;
    */
    return true;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] ConfigurationSet リストの取得 
   * @else
   * @brief [CORBA interface] Get a list of ConfigurationSet
   * @endif
   */
  ConfigurationSetList*
  Configuration_impl::get_configuration_sets()
    throw (CORBA::SystemException,
	   NotAvailable, InternalError)
  {
    try
      {
	Guard guard(m_config_mutex);
	
	ConfigurationSetList_var config_sets;
	config_sets = new ConfigurationSetList((CORBA::ULong)0);
	
	std::vector<RTC::Properties*> cf(m_configsets.getConfigurationSets());
	config_sets->length(cf.size());
	for (CORBA::ULong i(0), len(cf.size()); i < len; ++i)
	  {
	    toConfigurationSet(config_sets[i], *(cf[i]));
	  }
	
	return config_sets._retn();
      }
    catch (...)
      {
	throw InternalError("Configuration::get_configuration_sets()");
      }
    // never reach here
    return new ConfigurationSetList(0);
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] ConfigurationSet の取得
   * @else
   * @brief [CORBA interface] Get a ConfigurationSet
   * @endif
   */
  ConfigurationSet*
  Configuration_impl::get_configuration_set(const char* id)
    throw (CORBA::SystemException,
	   NotAvailable, InternalError)
  {
    if (std::string(id).empty()) throw InternalError("ID is empty");
    // Originally getConfigurationSet raises InvalidParameter according to the 
    // SDO specification. However, SDO's IDL lacks InvalidParameter.
    
    Guard guard(m_config_mutex);
    
    try {
      if (!m_configsets.haveConfig(id))
	throw InvalidParameter("No such ConfigurationSet");
    }
    catch(...) {
      // throw InvalidParameter("No such ConfigurationSet");
    }
    
    const RTC::Properties& configset(m_configsets.getConfigurationSet(id));
    
    try
      {
	ConfigurationSet_var config;
	config = new ConfigurationSet();
	toConfigurationSet(config, configset);
	return config._retn();
      }
    catch (...)
      {
	throw InternalError("Configuration::get_configuration_set()");
      }
    // never reach here
    return new ConfigurationSet();
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] ConfigurationSet をセットする
   * @else
   * @brief [CORBA interface] Set ConfigurationSet
   * @endif
   */
  CORBA::Boolean
  Configuration_impl::
  set_configuration_set_values(const char* id,
			       const ConfigurationSet& configuration_set)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    if (std::string(id).empty()) throw InvalidParameter("ID is empty.");
    
    try
      {
	RTC::Properties conf(id);
	toProperties(conf, configuration_set);
	return m_configsets.setConfigurationSetValues(id, conf);
      }
    catch (...)
      {
	throw InternalError("Configuration::set_configuration_set_values()");
      }
    return true;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] アクティブな ConfigurationSet を取得する
   * @else
   * @brief [CORBA interface] Get active ConfigurationSet
   * @endif
   */ 
  ConfigurationSet*
  Configuration_impl::get_active_configuration_set()
    throw (CORBA::SystemException,
	   NotAvailable, InternalError)
  {
    // activeなConfigurationSetは無い
    if (!m_configsets.isActive()) throw NotAvailable();    
    
    try
      {
	Guard gurad(m_config_mutex);
	// activeなConfigurationSetを返す
	ConfigurationSet_var config;
	config = new ConfigurationSet();
	toConfigurationSet(config, m_configsets.getActiveConfigurationSet());
	return config._retn();
      }
    catch (...)
      {
	throw InternalError("Configuration::get_active_configuration_set()");
      }
    // never reach here
    return new ConfigurationSet();
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] ConfigurationSet を追加する
   * @else
   * @brief [CORBA interface] Add ConfigurationSet
   * @endif
   */
  CORBA::Boolean
  Configuration_impl::
  add_configuration_set(const ConfigurationSet& configuration_set)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    try
      {
	Guard gurad(m_config_mutex);
	const char* config_id(configuration_set.id);
	RTC::Properties config(config_id);
	toProperties(config, configuration_set);
	return m_configsets.addConfigurationSet(config);
      }
    catch (...)
      {
	throw InternalError("Configuration::add_configuration_set()");
	return false;
      }
    return true;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] ConfigurationSet を削除する
   * @else
   * @brief [CORBA interface] Remove ConfigurationSet
   * @endif
   */
  CORBA::Boolean
  Configuration_impl::remove_configuration_set(const char* id)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    if (std::string(id).empty())
      throw InvalidParameter("ID is empty.");
    
    try
      {
	Guard guard(m_config_mutex);
	return m_configsets.removeConfigurationSet(id);
      }
    catch (...)
      {
	throw InternalError("Configuration::remove_configuration_set()");
	return false;
      }
    return false;
  }
  
  /*!
   * @if jp
   * @brief [CORBA interface] ConfigurationSet のアクティブ化
   * @else
   * @brief [CORBA interface] Activate ConfigurationSet
   * @endif
   */ 
  CORBA::Boolean
  Configuration_impl::activate_configuration_set(const char* id)
    throw (CORBA::SystemException,
	   InvalidParameter, NotAvailable, InternalError)
  {
    if (std::string(id).empty())
      throw InvalidParameter("ID is empty.");
    
    try
      {
	return m_configsets.activateConfigurationSet(id);
      }
    catch (...)
      {
	throw InternalError("Configuration::activate_configuration_set()");
	return false;
      }
    return false;
  }
  
  //============================================================
  // Local interfaces
  //============================================================
  
  /*!
   * @if jp
   * @brief オブジェクト　リファレンスを取得する
   * @else
   * @brief Get object reference
   * @endif
   */
  Configuration_ptr Configuration_impl::getObjRef()
  {
    return m_objref;
  }
  
  /*!
   * @if jp
   * @brief SDO の DeviceProfile を取得する
   * @else
   * @brief Get the DeviceProfile of SDO
   * @endif
   */
  const DeviceProfile Configuration_impl::getDeviceProfile()
  {
    return m_deviceProfile;
  }
  
  /*!
   * @if jp
   * @brief SDO の ServiceProfile のリストを取得する
   * @else
   * @brief Get a list of ServiceProfile of SDO
   * @endif
   */
  const ServiceProfileList Configuration_impl::getServiceProfiles()
  {
    return m_serviceProfiles;
  }
  /*!
   * @if jp
   * @brief SDO の ServiceProfile を取得する
   * @else
   * @brief Get Service Profile of SDO
   * @endif
   */
  const ServiceProfile Configuration_impl::getServiceProfile(const char* id)
  {
    CORBA::Long index;
    index =  CORBA_SeqUtil::find(m_serviceProfiles, service_id(id));
    if (index < 0) return ServiceProfile();
    return m_serviceProfiles[index];
  }
  
  /*!
   * @if jp
   * @brief SDO の Organization リストを取得する
   * @else
   * @brief Get a list of Organization of SDO
   * @endif
   */
  const OrganizationList Configuration_impl::getOrganizations()
  {
    return m_organizations;
  }
  
  /*!
   * @if jp
   * @brief UUIDを生成する
   * @else
   * @brief Generate UUID
   * @endif
   */
  const std::string Configuration_impl::getUUID() const
  {
    coil::UUID_Generator uugen;
    uugen.init();
    std::auto_ptr<coil::UUID> uuid(uugen.generateUUID(2,0x01));
    
    return (const char*) uuid->to_string();
  }
};
