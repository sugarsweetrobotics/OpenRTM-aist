// -*- C++ -*-
/*!
 * @file NamingManager.h
 * @brief naming Service helper class
 * @date $Date: 2007-12-31 03:08:04 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: NamingManager.h,v 1.3.2.1 2007-12-31 03:08:04 n-ando Exp $
 *
 */

#ifndef NamingManager_h
#define NamingManager_h

#include <rtm/RTC.h>

#include <ace/Task.h>
#include <rtm/CorbaNaming.h>
#include <rtm/RTObject.h>
#include <rtm/SystemLogger.h>

namespace RTC
{
  class Manager;
  /*!
   * @if jp
   *
   * @class NamingBase
   * @brief NamingService 管理用抽象クラス
   *
   * NamingServer 管理用抽象インターフェースクラス。
   * 具象管理クラスは、以下の純粋仮想関数の実装を提供しなければならない。
   * - bindObject() : 指定したオブジェクトのNamingServiceへのバインド
   * - unbindObject() : 指定したオブジェクトのNamingServiceからのアンバインド
   *
   * @since 0.4.0
   *
   * @else
   *
   * @class NamingBase
   * @brief NamingService management abstract class
   *
   * This is the abstract interface class for NamingServer management.
   * Concrete management classes must implement the following pure virtual 
   * functions.
   * - bindObject() : Bind the specified object to NamingService
   * - unbindObject() : Unbind the specified object from NamingService
   *
   * @since 0.4.0
   *
   * @endif
   */
  class NamingBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief コンストラクタ
     *
     * @else
     *
     * @brief Constructor
     *
     * @endif
     */
    NamingBase() {};
    
    /*!
     * @if jp
     *
     * @brief デストラクタ
     *
     * @else
     *
     * @brief Destructor
     *
     * @endif
     */
    virtual ~NamingBase() {};
    
    /*!
     * @if jp
     *
     * @brief 指定したオブジェクトをNamingServiceへバインドする純粋仮想関数
     *
     * @param name バインド時の名称
     * @param rtobj バインド対象オブジェクト
     *
     * @else
     *
     * @brief Pure virtual function to bind the specified objects 
     *        to NamingService
     *
     * @param name Names at the binding
     * @param rtobj The target objects for the binding
     *
     * @endif
     */
    virtual void bindObject(const char* name, const RTObject_impl* rtobj) = 0;
    
    /*!
     * @if jp
     *
     * @brief 指定したオブジェクトをNamingServiceからアンバインドするための
     *        純粋仮想関数
     *
     * @param name アンバインド対象オブジェクト
     *
     * @else
     *
     * @brief Pure virtual function to unbind the specified objects from 
     *        NamingService
     *
     * @param name The target objects for the unbinding
     *
     * @endif
     */
    virtual void unbindObject(const char* name) = 0;
  };
  
  /*!
   * @if jp
   *
   * @class NamingOnCorba
   * @brief CORBA 用 NamingServer 管理クラス
   *
   * CORBA 用 NamingServer 管理用クラス。
   * CORBA コンポーネントのNamingServiceへの登録、解除などを管理する。
   *
   * @since 0.4.0
   *
   * @else
   *
   * @class NamingOnCorba
   * @brief NamingServer management class for CORBA
   *
   * NamingServer management class for CORBA.
   * Manage to register and unregister CORBA components to NamingService.
   *
   * @since 0.4.0
   *
   * @endif
   */
  class NamingOnCorba
    : public virtual NamingBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @param orb ORB
     * @param names NamingServer 名称
     *
     * @else
     *
     * @brief Constructor
     *
     * Constructor
     *
     * @param orb ORB
     * @param names Name of NamingServer
     *
     * @endif
     */
    NamingOnCorba(CORBA::ORB_ptr orb, const char* names)
      : m_cosnaming(orb, names)
    {};
    
    /*!
     * @if jp
     *
     * @brief デストラクタ
     *
     * @else
     *
     * @brief Destructor
     *
     * @endif
     */
    virtual ~NamingOnCorba(){};
    
    /*!
     * @if jp
     *
     * @brief 指定した CORBA オブジェクトのNamingServiceへバインド
     * 
     * 指定した CORBA オブジェクトを指定した名称で CORBA NamingService へ
     * バインドする。
     * 
     * @param name バインド時の名称
     * @param rtobj バインド対象オブジェクト
     *
     * @else
     *
     * @brief Bind the specified CORBA objects to NamingService
     * 
     * Bind the specified CORBA objects to CORBA NamingService
     * by specified names.
     * 
     * @param name Names at the binding
     * @param rtobj The target objects for the binding
     *
     * @endif
     */
    virtual void bindObject(const char* name, const RTObject_impl* rtobj);
    
    /*!
     * @if jp
     *
     * @brief 指定した CORBA オブジェクトをNamingServiceからアンバインド
     * 
     * 指定した CORBA オブジェクトを CORBA NamingService からアンバインドする。
     * 
     * @param name アンバインド対象オブジェクト
     *
     * @else
     *
     * @brief Unbind the specified CORBA objects from NamingService
     * 
     * Unbind the specified CORBA objects from CORBA NamingService.
     * 
     * @param name The target objects for the unbinding
     *
     * @endif
     */
    virtual void unbindObject(const char* name);
    
  private:
    CorbaNaming m_cosnaming;
    std::map<std::string, RTObject_impl*> m_names;
  };
  
  /*!
   * @if jp
   *
   * @class NamingManager
   * @brief NamingServer 管理クラス
   *
   * NamingServer 管理用クラス。
   * コンポーネントのNamingServiceへの登録、解除などを管理する。
   *
   * @since 0.4.0
   *
   * @else
   *
   * @class NamingManager
   * @brief NamingServer management class
   *
   * NamingServer management class.
   * Manage to register and unregister components to NamingService.
   *
   * @since 0.4.0
   *
   * @endif
   */
  class NamingManager
  {
  public:
    /*!
     * @if jp
     *
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @param manager マネージャオブジェクト
     *
     * @else
     *
     * @brief Constructor
     *
     * Constructor
     *
     * @param manager Manager object
     *
     * @endif
     */
    NamingManager(Manager* manager);
    
    /*!
     * @if jp
     *
     * @brief デストラクタ
     *
     * @else
     *
     * @brief Destructor
     *
     * @endif
     */
    virtual ~NamingManager();
    
    /*!
     * @if jp
     *
     * @brief NameServer の登録
     *
     * 指定した形式の NameServer を登録する。
     * 現在指定可能な形式は CORBA のみ。
     *
     * @param method NamingService の形式
     * @param name_server 登録する NameServer の名称
     *
     * @else
     *
     * @brief Regster the NameServer
     *
     * Register NameServer by specified format.
     * Currently. only CORBA can be specified.
     *
     * @param method Format of NamingService
     * @param name_server Name of NameServer for registration
     *
     * @endif
     */
    void registerNameServer(const char* method, const char* name_server);
    
    /*!
     * @if jp
     *
     * @brief 指定したオブジェクトのNamingServiceへバインド
     * 
     * 指定したオブジェクトを指定した名称で CORBA NamingService へバインドする。
     * 
     * @param name バインド時の名称
     * @param rtobj バインド対象オブジェクト
     *
     * @else
     *
     * @brief Bind the specified objects to NamingService
     * 
     * Bind the specified objects to CORBA NamingService by specified names.
     * 
     * @param name Names at the binding
     * @param rtobj The target objects for the binding
     *
     * @endif
     */
    void bindObject(const char* name, const RTObject_impl* rtobj);
    
    /*!
     * @if jp
     *
     * @brief NamingServer の情報の更新
     * 
     * 設定されている NameServer 内に登録されているオブジェクトの情報を
     * 更新する。
     * 
     * @else
     *
     * @brief Update information of NamingServer
     * 
     * Update the object information registered in the specified NameServer.
     * 
     * @endif
     */
    void update();
    
    /*!
     * @if jp
     *
     * @brief 指定したオブジェクトをNamingServiceからアンバインド
     * 
     * 指定したオブジェクトを NamingService からアンバインドする。
     * 
     * @param name アンバインド対象オブジェクト
     *
     * @else
     *
     * @brief Unbind the specified objects from NamingService
     * 
     * Unbind the specified objects from NamingService.
     * 
     * @param name The target objects for the unbinding
     *
     * @endif
     */
    void unbindObject(const char* name);
    
    /*!
     * @if jp
     *
     * @brief 全てのオブジェクトをNamingServiceからアンバインド
     * 
     * 全てのオブジェクトを CORBA NamingService からアンバインドする。
     * 
     * @else
     *
     * @brief Unbind all objects from NamingService
     * 
     * Unbind all objects from CORBA NamingService.
     * 
     * @endif
     */
    void unbindAll();
    
    /*!
     * @if jp
     *
     * @brief バインドされている全てのオブジェクトを取得
     * 
     * バインドされている全てのオブジェクトを 取得する。
     *
     * @return バインド済みオブジェクト リスト
     * 
     * @else
     *
     * @brief Get all bound objects
     * 
     * Get all bound objects.
     *
     * @return Bound object list
     * 
     * @endif
     */
    std::vector<RTObject_impl*> getObjects();
    
  protected:
    /*!
     * @if jp
     *
     * @brief NameServer 管理用オブジェクトの生成
     * 
     * 指定した型のNameServer 管理用オブジェクトを生成する。
     *
     * @param method NamingService 形式
     * @param name_server NameServer 名称
     * 
     * @return 生成した NameServer オブジェクト
     * 
     * @else
     *
     * @brief Create objects for NameServer management
     * 
     * Create objects of specified type for NameServer management.
     *
     * @param method NamingService format
     * @param name_server NameServer name
     * 
     * @return Created NameServer objects
     * 
     * @endif
     */
    NamingBase* createNamingObj(const char* method, const char* name_server);
    
    /*!
     * @if jp
     *
     * @brief 設定済みコンポーネントを NameServer に登録
     * 
     * 設定済みコンポーネントを指定した NameServer に登録する。
     *
     * @param ns 登録対象 NameServer
     * 
     * @else
     *
     * @brief Register the configured component to NameServer
     * 
     * Register the already configured components to NameServer.
     *
     * @param ns The target NameServer for the registration
     * 
     * @endif
     */
    void bindCompsTo(NamingBase* ns);
    
    /*!
     * @if jp
     *
     * @brief NameServer に登録するコンポーネントの設定
     * 
     * NameServer に登録するコンポーネントを設定する。
     *
     * @param name コンポーネントの登録時名称
     * @param rtobj 登録対象オブジェクト
     * 
     * @else
     *
     * @brief Configure the components that will be registered to NameServer
     * 
     * Configure the components that will be registered to NameServer.
     *
     * @param name Names of components at the registration
     * @param rtobj The target objects for registration
     * 
     * @endif
     */
    void registerCompName(const char* name, const RTObject_impl* rtobj);
    
    /*!
     * @if jp
     *
     * @brief NameServer に登録するコンポーネントの設定解除
     * 
     * NameServer に登録するコンポーネントの設定を解除する。
     *
     * @param name 設定解除対象コンポーネントの名称
     * 
     * @else
     *
     * @brief Unregister the components that will be registered to NameServer
     * 
     * Unregister the components that will be registered to NameServer.
     *
     * @param name Names of the target components for unregistration
     * 
     * @endif
     */
    void unregisterCompName(const char* name);
    
  protected:
    // Name Servers' method/name and object
    /*!
     * @if jp
     * @brief NameServer 管理用構造体
     * @else
     * @brief Structure for NameServer management
     * @endif
     */
    struct Names
    {
      Names(const char* meth, const char* name, NamingBase* naming)
	: method(meth), nsname(name), ns(naming)
      {
      }
      std::string method;
      std::string nsname;
      NamingBase* ns;
    };
    /*!
     * @if jp
     * @brief NameServer リスト
     * @else
     * @brief NameServer list
     * @endif
     */
    std::vector<Names*> m_names;
    ACE_Thread_Mutex m_namesMutex;
    
    // Components' name and object
    /*!
     * @if jp
     * @brief コンポーネント管理用構造体
     * @else
     * @brief Structure for component management
     * @endif
     */
    struct Comps
    {
      Comps(const char* n, const RTObject_impl* obj)
	: name(n), rtobj(obj)
      {}
      std::string name;
      const RTObject_impl* rtobj;
    };
    /*!
     * @if jp
     * @brief コンポーネントリスト
     * @else
     * @brief Component list
     * @endif
     */
    std::vector<Comps*> m_compNames;
    ACE_Thread_Mutex m_compNamesMutex;
    
    /*!
     * @if jp
     * @brief マネージャオブジェクト
     * @else
     * @brief Manager object
     * @endif
     */
    Manager* m_manager;
    
    /*!
     * @if jp
     * @brief ロガー仲介バッファ
     * @else
     * @brief Logger mediation buffer
     * @endif
     */
    MedLogbuf m_MedLogbuf;
    
    /*!
     * @if jp
     * @brief ロガーストリーム
     * @else
     * @brief Logger stream
     * @endif
     */
    LogStream rtcout;
  }; // class NamingManager
}; // namespace RTC

#endif // NamingManager_h
