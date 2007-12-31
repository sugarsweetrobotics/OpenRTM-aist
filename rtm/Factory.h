// -*- C++ -*-
/*!
 * @file Factory.h
 * @brief RTComponent factory class
 * @date $Date: 2007-12-31 03:08:03 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2008
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: Factory.h,v 1.5.4.1 2007-12-31 03:08:03 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.5  2006/11/06 01:28:36  n-ando
 * Now the "instance_name" is set to the component at creation time.
 *
 * Revision 1.4  2006/10/25 17:36:00  n-ando
 * Classes were renamed, and class reference manual was described.
 *
 * Revision 1.3  2006/10/24 14:23:45  n-ando
 * Renamed RtcFactory.h to Factory.h
 *
 * Revision 1.2  2005/05/16 06:06:30  n-ando
 * - RtcFactoryBase, RtcFactoryCXX classes were DLL exported for Windows port.
 *
 * Revision 1.1.1.1  2005/05/12 09:06:18  n-ando
 * Public release.
 *
 *
 */

#ifndef Factory_h
#define Factory_h

#include <rtm/Properties.h>
#include <rtm/RTObject.h>
#include <rtm/NumberingPolicy.h>


namespace RTC 
{
  typedef RTObject_impl RtcBase;
  class Manager;
  
  typedef RtcBase* (*RtcNewFunc)(Manager* manager);
  typedef void (*RtcDeleteFunc)(RtcBase* rtc);
  
  /*!
   * @if jp
   *
   * @brief RTコンポーネント生成用テンプレート関数
   * 
   * RTコンポーネントのインスタンスを生成するためのテンプレート関数。
   * RTコンポーネント管理用マネージャから呼び出される。
   * 実際には各コンポーネントのコンストラクタが呼び出される。
   * \<_New\>で生成対象RTコンポーネントの型を指定する。
   *
   * @param manager マネージャオブジェクト
   *
   * @return 生成した RTコンポーネント インスタンス
   * 
   * @else
   *
   * @endif
   */
  template <class _New>
  RtcBase* Create(Manager* manager)
  {
    return new _New(manager);
  }
  
  /*!
   * @if jp
   *
   * @brief RTコンポーネント破棄用テンプレート関数
   * 
   * RTコンポーネントのインスタンスを破棄するためのテンプレート関数。
   * \<_Delete\>にて破棄対象RTコンポーネントの型を指定する。
   *
   * @param rtc 破棄対象RTコンポーネントのインスタンス
   *
   * @else
   *
   * @endif
   */
  template <class _Delete>
  void Delete(RtcBase* rtc)
  {
    delete rtc;
  }
  
  /*!
   * @if jp
   *
   * @class FactoryBase
   * @brief FactoryBase 基底クラス
   * 
   * コンポーネントファクトリの基底クラス。
   *
   * @since 0.2.0
   *
   * @else
   *
   * @class FactoryBase
   * @brief FactoryBase base class
   *
   * RTComponent factory base class.
   *
   * @since 0.2.0
   *
   * @endif
   */
  class FactoryBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     * @param profile コンポーネントのプロファイル
     *
     * @else
     *
     * @brief Constructor.
     *
     * Constructor.
     *
     * @param profile component profile
     *
     * @endif
     */
    FactoryBase(const Properties& profile);
    
    /*!
     * @if jp
     * @brief デストラクタ
     *
     * デストラクタ
     *
     * @else
     * @brief Destructor
     * @endif
     */
    virtual ~FactoryBase();
    
    /*!
     * @if jp
     *
     * @brief コンポーネントの生成
     *
     * RTComponent のインスタンスを生成するための純粋仮想関数。
     *
     * @param mgr マネージャオブジェクト
     *
     * @return 生成したコンポーネント
     *
     * @else
     *
     * @brief Create component
     *
     * @param mgr pointer to RtcManager
     *
     * @endif
     */
    virtual RtcBase* create(Manager* mgr) = 0;
    
    /*!
     * @if jp
     *
     * @brief コンポーネントの破棄
     *
     * RTComponent のインスタンスを破棄するための純粋仮想関数。
     *
     * @param comp 破棄対象 RTコンポーネント
     *
     * @else
     *
     * @brief Destroy component
     *
     * @param comp pointer to RtcBase
     *
     * @endif
     */
    virtual void destroy(RtcBase* comp) = 0;
    
    /*!
     * @if jp
     *
     * @brief コンポーネントプロファイルの取得
     *
     * コンポーネントのプロファイルを取得する
     *
     * @return コンポーネントのプロファイル
     *
     * @else
     *
     * @brief Get component profile
     *
     * Get component profile.
     *
     * @endif
     */
    virtual Properties& profile();
    
    /*!
     * @if jp
     *
     * @brief 現在のインスタンス数の取得
     *
     * コンポーネントの現在のインスタンス数を取得する。
     *
     * @return コンポーネントのインスタンス数
     *
     * @else
     *
     * @brief Get number of component instances
     *
     * Get number of current component instances.
     *
     * @endif
     */
    virtual int number();
    
  protected:
    /*!
     * @if jp
     * @brief コンポーネントのプロファイル
     * @else
     * @brief Component profile
     * @endif
     */
    Properties m_Profile;
    
    /*!
     * @if jp
     * @brief 現在のインスタンス数
     * @else
     * @brief Number of current component instances.
     * @endif
     */
    int m_Number;
  };
  
  /*!
   * @if jp
   * @class FactoryCXX
   * @brief FactoryCXX クラス
   * 
   * C++用コンポーネントファクトリクラス。
   *
   * @since 0.2.0
   *
   *
   * @else
   *
   * @class FactoryCXX
   * @brief FactoryCXX class
   *
   * RTComponent factory class for C++.
   *
   * @endif
   */
  class FactoryCXX
    : public FactoryBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief コンストラクタ
     *
     * コンストラクタ。
     * プロファイル、生成関数へのポインタ、破棄関数へのポインタ、
     * コンポーネント生成時の命名ポリシーを引数に取り、
     * C++ で実装されたコンポーネントのファクトリクラスを生成する。
     *
     * @param profile コンポーネントのプロファイル
     * @param new_func コンポーネントの生成関数へのポインタ
     * @param delete_func コンポーネントの破棄関数へのポインタ
     * @param policy コンポーネント生成時の命名ポリシー
     * (デフォルト値:DefaultNumberingPolicy)
     *
     * @else
     *
     * @brief Constructor.
     *
     * Constructor.
     * Create component factory class with three arguments:
     * component profile, function pointer to object create function and
     * object delete function.
     *
     * @param profile Component profile
     * @param new_func Pointer to component create function
     * @param delete_func Pointer to component delete function
     * @param policy Pointer to component delete function
     *
     * @endif
     */
    FactoryCXX(const Properties& profile,
	       RtcNewFunc new_func,
	       RtcDeleteFunc delete_func,
	       NumberingPolicy* policy = new DefaultNumberingPolicy());
    
    /*!
     * @if jp
     *
     * @brief コンポーネントの生成
     *
     * RTComponent のインスタンスを生成する。
     *
     * @param mgr マネージャオブジェクト
     *
     * @return 生成したコンポーネント
     *
     * @else
     *
     * @brief Create component
     *
     * Create component implemented in Python.
     *
     * @param mgr
     *
     * @endif
     */
    virtual RtcBase* create(Manager* mgr);
    
    /*!
     * @if jp
     *
     * @brief コンポーネントの破棄
     *
     * RTComponent のインスタンスを破棄する。
     *
     * @param comp 破棄対象 RTComponent
     *
     * @else
     *
     * @brief Destroy component
     *
     * Destroy component instance
     *
     * @param comp
     *
     * @endif
     */
    virtual void destroy(RtcBase* comp);
    
  protected:
    /*!
     * @if jp
     * @brief コンポーネントオブジェクト生成関数へのポインタ
     * @else
     * @brief The pointer to component object create function
     * @endif
     */
    RtcNewFunc m_New;
    
    /*!
     * @if jp
     * @brief コンポーネントオブジェクト破棄関数へのポインタ
     * @else
     * @brief The pointer to component object delete function
     * @endif
     */
    RtcDeleteFunc m_Delete;
    
    /*!
     * @if jp
     * @brief コンポーネント生成時の命名ポリシー
     * @else
     * @endif
     */
    NumberingPolicy* m_policy;
  };
};
#endif // Factory_h
