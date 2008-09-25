// -*- C++ -*-
/*!
 * @file PeriodicExecutionContext.h
 * @brief PeriodicExecutionContext class
 * @date $Date: 2008-01-14 07:53:05 $
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

#ifndef PeriodicExecutionContext_h
#define PeriodicExecutionContext_h

#include <coil/Task.h>
#include <vector>
#include <iostream>

#include <rtm/RTC.h>
#include <rtm_corba/idl/RTCSkel.h>
#include <rtm_corba/idl/OpenRTMSkel.h>
#include <rtm/Manager.h>
#include <rtm/StateMachine.h>
#include <rtm/ExecutionContextBase.h>

// ACE
#define NUM_OF_LIFECYCLESTATE 4
namespace RTC
{
  /*!
   * @if jp
   * @class PeriodicExecutionContext
   * @brief PeriodicExecutionContext クラス
   *
   * Periodic Sampled Data Processing(周期実行用)ExecutionContextクラス。
   *
   * @since 0.4.0
   *
   * @else
   * @class PeriodicExecutionContext
   * @brief PeriodicExecutionContext class
   *
   * Periodic Sampled Data Processing (for the execution cycles)
   * ExecutionContext class
   *
   * @since 0.4.0
   *
   * @endif
   */
  class PeriodicExecutionContext
    : public virtual ExecutionContextBase,
      public coil::Task
  {
  public:
    /*!
     * @if jp
     * @brief デフォルトコンストラクタ
     *
     * デフォルトコンストラクタ
     * プロファイルに以下の項目を設定する。
     *  - kind : PERIODIC
     *  - rate : 0.0
     *
     * @else
     * @brief Default Constructor
     *
     * Default Constructor
     * Set the following items to profile.
     *  - kind : PERIODIC
     *  - rate : 0.0
     *
     * @endif
     */
    PeriodicExecutionContext();
    
    /*!
     * @if jp
     * @brief コンストラクタ
     *
     * コンストラクタ
     * 設定された値をプロファイルに設定する。
     *
     * @param owner 当該 Executioncontext の owner
     * @param rate 動作周期(Hz)(デフォルト値:1000)
     *
     * @else
     * @brief Constructor
     *
     * Constructor
     * Set the configuration value to profile.
     *
     * @param owner The owner of this Executioncontext
     * @param rate Execution cycle(Hz)(The default value:1000)
     *
     * @endif
     */
    PeriodicExecutionContext(OpenRTM::DataFlowComponent_ptr owner,
			     double rate = 1000.0);
    
    /*!
     * @if jp
     * @brief デストラクタ
     *
     * デストラクタ
     *
     * @else
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~PeriodicExecutionContext();
    
    /*!
     * @if jp
     * @brief CORBA オブジェクト参照の取得
     *
     * 本オブジェクトの ExecutioncontextService としての CORBA オブジェクト参照
     * を取得する。
     *
     * @return CORBA オブジェクト参照
     *
     * @else
     * @brief Get the reference to the CORBA object
     *
     * Get the reference to the CORBA object as ExecutioncontextService
     * of this object.
     *
     * @return The reference to CORBA object
     *
     * @endif
     */
    ExecutionContextService_ptr getRef() {return m_ref;}
    
    /*!
     * @if jp
     * @brief ExecutionContext用アクティビティスレッドを生成する
     *
     * Executioncontext 用の内部アクティビティスレッドを生成し起動する。
     * これは ACE_Task サービスクラスメソッドのオーバーライド。
     *
     * @param args 通常は0
     *
     * @return 生成処理実行結果
     *
     * @else
     *
     * @brief Generate internal activity thread for ExecutionContext
     *
     * Generate internal activity thread and run. 
     * This is ACE_Task class method's override.
     *
     * @param args Usually give 0
     *
     * @return The generation result
     *
     * @endif
     */     
    virtual int open(void *args);
    
    /*!
     * @if jp
     * @brief ExecutionContext 用のスレッド実行関数
     *
     * ExecutionContext 用のスレッド実行関数。
     * 登録されたコンポーネントの処理を呼び出す。
     *
     * @return 実行結果
     *
     * @else
     * @brief Thread execution function for ExecutionContext
     *
     * Thread execution function for ExecutionContext.
     * Invoke the registered components operation.
     *
     * @return The execution result
     *
     * @endif
     */     
    virtual int svc(void);
    
    /*!
     * @if jp
     * @brief ExecutionContext 用のスレッド実行関数
     *
     * ExecutionContext 用のスレッド終了時に呼ばれる。
     * コンポーネントオブジェクトの非アクティブ化、マネージャへの通知を行う。
     * これは ACE_Task サービスクラスメソッドのオーバーライド。
     *
     * @param flags 終了処理フラグ
     *
     * @return 終了処理結果
     *
     * @else
     *
     * @brief Thread execution function for ExecutionContext
     *
     * This function is invoked when activity thread for ExecutionContext exits.
     * Deactivate the component object and notify it to manager.
     * This is ACE_Task class method's override.
     *
     * @param flags Flag of the close
     *
     * @return The close result
     *
     * @endif
     */     
    virtual int close(unsigned long flags);
    
    //============================================================
    // ExecutionContext
    //============================================================
    /*!
     * @if jp
     * @brief ExecutionContext 実行状態確認関数
     *
     * この操作は ExecutionContext が Runnning 状態の場合に true を返す。
     * Executioncontext が Running の間、当該 Executioncontext に参加している
     * 全てのアクティブRTコンポーネントが、ExecutionContext の実行種類に応じて
     * 実行される。
     *
     * @return 状態確認関数(動作中:true、停止中:false)
     *
     * @else
     *
     * @brief Check for ExecutionContext running state
     *
     * This operation shall return true if the context is in the Running state.
     * While the context is Running, all Active RTCs participating
     * in the context shall be executed according to the context’s execution
     * kind.
     *
     * @return Check state function (Running:true、Stopping:false)
     *
     * @endif
     */
    virtual CORBA::Boolean is_running()
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief ExecutionContext の実行を開始
     *
     * ExecutionContext の実行状態を Runnning とするためのリクエストを発行する。
     * ExecutionContext の状態が遷移すると ComponentAction::on_startup が
     * 呼び出される。
     * 参加しているRTコンポーネントが、初期化されるまで ExecutionContext を開始
     * することはできない。
     * ExecutionContext は複数回開始/停止を繰り返すことができる。
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Start the ExecutionContext
     *
     * Request that the context enter the Running state. 
     * Once the state transition occurs, the ComponentAction::on_startup 
     * operation will be invoked.
     * An execution context may not be started until the RT-Components that
     * participate in it have been initialized.
     * An execution context may be started and stopped multiple times.
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t start()
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief ExecutionContext の実行を停止
     *
     * ExecutionContext の状態を Stopped とするためのリクエストを発行する。
     * 遷移が発生した場合は、ComponentAction::on_shutdown が呼び出される。
     * 参加しているRTコンポーネントが終了する前に ExecutionContext を停止する
     * 必要がある。
     * ExecutionContext は複数回開始/停止を繰り返すことができる。
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Stop the ExecutionContext
     *
     * Request that the context enter the Stopped state. 
     * Once the transition occurs, the ComponentAction::on_shutdown operation
     * will be invoked.
     * An execution context must be stopped before the RT components that
     * participate in it are finalized.
     * An execution context may be started and stopped multiple times.
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t stop()
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief ExecutionContext の実行周期(Hz)を取得する
     *
     * Active 状態にてRTコンポーネントが実行される周期(単位:Hz)を取得する。
     *
     * @return 処理周期(単位:Hz)
     *
     * @else
     *
     * @brief Get execution rate(Hz) of ExecutionContext
     *
     * This operation shall return the rate (in hertz) at which its Active
     * participating RTCs are being invoked.
     *
     * @return Execution cycle(Unit:Hz)
     *
     * @endif
     */
    virtual CORBA::Double get_rate()
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief ExecutionContext の実行周期(Hz)を設定する
     *
     * Active 状態にてRTコンポーネントが実行される周期(単位:Hz)を設定する。
     * 実行周期の変更は、DataFlowComponentAction の on_rate_changed によって
     * 各RTコンポーネントに伝達される。
     *
     * @param rate 処理周期(単位:Hz)
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Set execution rate(Hz) of ExecutionContext
     *
     * This operation shall set the rate (in hertz) at which this context’s 
     * Active participating RTCs are being called.
     * If the execution kind of the context is PERIODIC, a rate change shall
     * result in the invocation of on_rate_changed on any RTCs realizing
     * DataFlowComponentAction that are registered with any RTCs participating
     * in the context.
     *
     * @param rate Execution cycle(Unit:Hz)
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t  set_rate(CORBA::Double rate)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief RTコンポーネントをアクティブ化する
     *
     * Inactive 状態にあるRTコンポーネントをActive に遷移させ、
     * アクティブ化する。
     * この操作が呼ばれた結果、on_activate が呼び出される。
     * 指定したRTコンポーネントが参加者リストに含まれない場合は、BAD_PARAMETER 
     * が返される。
     * 指定したRTコンポーネントの状態が Inactive 以外の場合は、
     *  PRECONDITION_NOT_MET が返される。
     *
     * @param comp アクティブ化対象RTコンポーネント
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Activate an RT-component
     *
     * The given participant RTC is Inactive and is therefore not being invoked
     * according to the execution context’s execution kind. This operation
     * shall cause the RTC to transition to the Active state such that it may
     * subsequently be invoked in this execution context.
     * The callback on_activate shall be called as a result of calling this
     * operation. This operation shall not return until the callback has
     * returned, and shall result in an error if the callback does.
     *
     * @param comp The target RT-Component for activation
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t activate_component(LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief RTコンポーネントを非アクティブ化する
     *
     * Inactive 状態にあるRTコンポーネントを非アクティブ化し、
     * Inactive に遷移させる。
     * この操作が呼ばれた結果、on_deactivate が呼び出される。
     * 指定したRTコンポーネントが参加者リストに含まれない場合は、BAD_PARAMETER 
     * が返される。
     * 指定したRTコンポーネントの状態が Active 以外の場合は、
     * PRECONDITION_NOT_MET が返される。
     *
     * @param comp 非アクティブ化対象RTコンポーネント
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Deactivate an RT-component
     *
     * The given RTC is Active in the execution context. Cause it to transition 
     * to the Inactive state such that it will not be subsequently invoked from
     * the context unless and until it is activated again.
     * The callback on_deactivate shall be called as a result of calling this
     * operation. This operation shall not return until the callback has 
     * returned, and shall result in an error if the callback does.
     *
     * @param comp The target RT-Component for deactivate
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t deactivate_component(LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief RTコンポーネントをリセットする
     *
     * Error 状態のRTコンポーネントの復帰を試みる。
     * この操作が呼ばれた結果、on_reset が呼び出される。
     * 指定したRTコンポーネントが参加者リストに含まれない場合は、BAD_PARAMETER
     * が返される。
     * 指定したRTコンポーネントの状態が Error 以外の場合は、PRECONDITION_NOT_MET
     * が返される。
     *
     * @param comp リセット対象RTコンポーネント
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Reset the RT-component
     *
     * Attempt to recover the RTC when it is in Error.
     * The ComponentAction::on_reset callback shall be invoked. This operation
     * shall not return until the callback has returned, and shall result in an
     * error if the callback does. If possible, the RTC developer should
     * implement that callback such that the RTC may be returned to a valid
     * state.
     *
     * @param comp The target RT-Component for reset
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t reset_component(LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief RTコンポーネントの状態を取得する
     *
     * 指定したRTコンポーネントの状態(LifeCycleState)を取得する。
     * 指定したRTコンポーネントが参加者リストに含まれない場合は、UNKNOWN_STATE 
     * が返される。
     *
     * @param comp 状態取得対象RTコンポーネント
     *
     * @return 現在の状態(LifeCycleState)
     *
     * @else
     *
     * @brief Get RT-component's state
     *
     * This operation shall report the LifeCycleState of the given participant
     * RTC.
     * UNKNOWN_STATE will be returned, if the given RT-Component is not inclued
     * in the participant list.
     *
     * @param comp The target RT-Component to get the state
     *
     * @return The current state of the target RT-Component(LifeCycleState)
     *
     * @endif
     */
    virtual LifeCycleState get_component_state(LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief ExecutionKind を取得する
     *
     * 本 ExecutionContext の ExecutionKind を取得する
     *
     * @return ExecutionKind
     *
     * @else
     *
     * @brief Get the ExecutionKind
     *
     * This operation shall report the execution kind of the execution context.
     *
     * @return ExecutionKind
     *
     * @endif
     */
    virtual ExecutionKind get_kind()
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief RTコンポーネントを追加する
     *
     * 指定したRTコンポーネントを参加者リストに追加する。
     * 追加されたRTコンポーネントは attach_context が呼ばれ、Inactive 状態に遷移
     * する。
     * 指定されたRTコンポーネントがnullの場合は、BAD_PARAMETER が返される。
     * 指定されたRTコンポーネントが DataFlowComponent 以外の場合は、
     * BAD_PARAMETER が返される。
     *
     * @param comp 追加対象RTコンポーネント
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Add an RT-component
     *
     * The operation causes the given RTC to begin participating in the
     * execution context.
     * The newly added RTC will receive a call to 
     * LightweightRTComponent::attach_context and then enter the Inactive state.
     * BAD_PARAMETER will be invoked, if the given RT-Component is null or
     * if the given RT-Component is other than DataFlowComponent.
     *
     * @param comp The target RT-Component for add
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t add_component(LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief RTコンポーネントを参加者リストから削除する
     *
     * 指定したRTコンポーネントを参加者リストから削除する。
     * 削除されたRTコンポーネントは detach_context が呼ばれる。
     * 指定されたRTコンポーネントが参加者リストに登録されていない場合は、
     * BAD_PARAMETER が返される。
     *
     * @param comp 削除対象RTコンポーネント
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Remove the RT-Component from participant list
     *
     * This operation causes a participant RTC to stop participating in the
     * execution context.
     * The removed RTC will receive a call to
     * LightweightRTComponent::detach_context.
     * BAD_PARAMETER will be returned, if the given RT-Component is not 
     * participating in the participant list.
     *
     * @param comp The target RT-Component for delete
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual ReturnCode_t remove_component(LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief ExecutionContextProfile を取得する
     *
     * 本 ExecutionContext のプロファイルを取得する。
     *
     * @return ExecutionContextProfile
     *
     * @else
     *
     * @brief Get the ExecutionContextProfile
     *
     * This operation provides a profile “descriptor” for the execution 
     * context.
     *
     * @return ExecutionContextProfile
     *
     * @endif
     */
    virtual ExecutionContextProfile* get_profile()
      throw (CORBA::SystemException);
    
  protected:
    //============================================================
    // DFPBase
    //============================================================
    typedef LifeCycleState ExecContextState;
    /***
	enum ExecContextState
	{
	INACTIVE_STATE,
	ACTIVE_STATE,
	ERROR_STATE,
	};
    */
    typedef RTC_Utils::StateHolder<ExecContextState> ECStates;
    
    /*!
     * @if jp
     * @class DFPBase
     * @brief DFPBase クラス
     *
     * 参加者リストに登録された DataFlowParticipant を管理するための抽象クラス。
     *
     * @since 0.4.0
     *
     * @else
     * @class DFPBase
     * @brief DFPBase class
     *
     * The abstract class to manage DataFlowParticipant registered in 
     * tha participant list.
     *
     * @since 0.4.0
     *
     * @endif
     */
    class DFPBase
    {
    public:
      
      /*!
       * @if jp
       * @brief コンストラクタ
       *
       * コンストラクタ
       *
       * @param id 所属する ExecutionContext のID
       *
       * @else
       * @brief Constructor
       *
       * Constructor
       *
       * @param id ID of participating ExecutionContext
       *
       * @endif
       */
      DFPBase(RTC::ExecutionContextHandle_t id)
	: ec_id(id), m_sm(NUM_OF_LIFECYCLESTATE)
      {
	m_sm.setListener(this);
	m_sm.setEntryAction (ACTIVE_STATE, &DFPBase::on_activated);
	m_sm.setDoAction    (ACTIVE_STATE, &DFPBase::on_execute);
	m_sm.setPostDoAction(ACTIVE_STATE, &DFPBase::on_state_update);
	m_sm.setExitAction  (ACTIVE_STATE, &DFPBase::on_deactivated);
	m_sm.setEntryAction (ERROR_STATE,  &DFPBase::on_aborting);
	m_sm.setDoAction    (ERROR_STATE,  &DFPBase::on_error);
	m_sm.setExitAction  (ERROR_STATE,  &DFPBase::on_reset);
	
	ECStates st;
	st.prev = INACTIVE_STATE;
	st.curr = INACTIVE_STATE;
	st.next = INACTIVE_STATE;
	m_sm.setStartState(st);
	m_sm.goTo(INACTIVE_STATE);
      }	
      
      /*!
       * @if jp
       * @brief デストラクタ
       *
       * デストラクタ
       *
       * @else
       * @brief Destructor
       *
       * Destructor
       *
       * @endif
       */
      virtual ~DFPBase(){}
      
      /*!
       * @if jp
       * @brief ExecutionContext 実行開始時に呼ばれる純粋仮想関数
       *
       * 参加している ExecutionContext が実行を開始する時(Running状態へ遷移時)
       * に呼ばれる純粋仮想関数。
       *
       * @else
       *
       * @brief Pure virtual function to be invoked when ExecutionContext starts
       *
       * Pure virtual function to be invoked when given execution context, in
       * which the RTC is participating, has transited from Stopped to Running.
       *
       * @endif
       */
      virtual void on_startup() = 0;
      
      /*!
       * @if jp
       * @brief ExecutionContext 停止時に呼ばれる純粋仮想関数
       *
       * 参加している ExecutionContext が実行を停止する時(Stopped状態へ遷移時)
       * に呼ばれる純粋仮想関数。
       *
       * @else
       *
       * @brief Pure virtual function to be invoked when ExecutionContext stops
       *
       * Pure virtual function to be invoked when given execution context, in
       * which the RTC is participating, has transited from Running to Stopped.
       *
       * @endif
       */
      virtual void on_shutdown() = 0;
      
      /*!
       * @if jp
       * @brief RTコンポーネントがアクティブ化された時に呼ばれる純粋仮想関数
       *
       * 管理対象のRTコンポーネントがアクティブ化された時
       * (Active状態へ遷移時)に呼ばれる純粋仮想関数。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       *
       * @brief Pure virtual function to be invoked when RT-Component is 
       *        activated
       *
       * Pure virtual function to be invoked when the RTC has been activated
       * in the given execution context.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      virtual void on_activated(const ECStates& st) = 0;
      
      /*!
       * @if jp
       * @brief RTコンポーネントが非アクティブ化された時に呼ばれる純粋仮想関数
       *
       * 管理対象のRTコンポーネントが非アクティブ化された時
       * (Deactive状態へ遷移時)に呼ばれる純粋仮想関数。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       *
       * @brief Pure virtual function to be invoked when RT-Component is 
       *        deactivated
       *
       * Pure virtual function to be invoked when the RTC has been deactivated
       * in the given execution context.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      virtual void on_deactivated(const ECStates& st) = 0;
      
      /*!
       * @if jp
       * @brief RTコンポーネントでエラーが発生した時に呼ばれる純粋仮想関数
       *
       * 管理対象のRTコンポーネントにエラーが発生した時(Error状態へ遷移時)
       * に呼ばれる純粋仮想関数。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       *
       * @brief Pure virtual function to be invoked when RT-Component occurs 
       *        error
       *
       * Pure virtual function to be invoked when the RTC is transiting from
       * the Active state to the Error state in some execution context.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      virtual void on_aborting(const ECStates& st) = 0;
      
      /*!
       * @if jp
       * @brief RTコンポーネントがエラー状態の時に呼ばれる純粋仮想関数
       *
       * 管理対象のRTコンポーネントがエラー状態にいる間、on_execute と
       * on_state_update に替わって定期的に呼び出される純粋仮想関数。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       *
       * @brief Pure virtual function to be invoked while RT-Component
       * is in the error state
       *
       * If the RTC is in the Error state relative to some execution context
       * when it would otherwise be invoked from that context.
       * This operation shall be invoked in sorted order at the rate of the
       * context instead of DataFlowComponentAction::on_execute and 
       * on_state_update.The RTC is transitioning from the Active state to 
       * the Error state in some execution context.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      virtual void on_error(const ECStates& st) = 0;
      
      /*!
       * @if jp
       * @brief RTコンポーネントをリセットする時に呼ばれる純粋仮想関数
       *
       * 管理対象のRTコンポーネントをリセットする際に呼ばれる純粋仮想関数。
       * この関数が正常に終了すると，RTCは Inactive 状態に復帰する。
       * この関数が正常に終了しなかった場合は， Error 状態に留まる。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       *
       * @brief Pure virtual function to be invoked when RT-Component resets.
       *
       * The RTC is in the Error state. An attempt is being made to recover it 
       * such that it can return to the Inactive state.
       * If the RTC was successfully recovered and can safely return to 
       * the Inactive state, this method shall complete with ReturnCode_t::OK.
       * Any other result shall indicate that the RTC should remain in the 
       * Error state.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      virtual void on_reset(const ECStates& st) = 0;
      
      /*!
       * @if jp
       * @brief RTコンポーネント実行時に定期的に呼ばれる純粋仮想関数
       *
       * 管理対象のRTコンポーネントが Active 状態であるとともに、
       * ExecutionContext が Running 状態の場合に、設定された動作周期で定期的に
       * 呼び出される純粋仮想関数。
       * Two-Pass Execution の最初の実行で呼ばれる。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       *
       * @brief Pure virtual function to be periodically invoked while 
       *        RT-Component is running
       *
       * This operation will be invoked periodically at the rate of the given
       * execution context as long as the following conditions hold:
       *  - The RTC is Active.
       *  - The given execution context is Running.
       * This callback occurs during the first execution pass.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      virtual void on_execute(const ECStates& st) = 0;
      
      /*!
       * @if jp
       * @brief RTコンポーネント実行時に定期的に呼ばれる純粋仮想関数
       *
       * 管理対象のRTコンポーネントが Active 状態であるとともに、
       * ExecutionContext が Running 状態の場合に、設定された動作周期で定期的に
       * 呼び出される純粋仮想関数。
       * Two-Pass Execution の２番目の実行で呼ばれる。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       *
       * @brief Pure virtual function to be periodically invoked while 
       *        RT-Component is running
       *
       * This operation will be invoked periodically at the rate of the given
       * execution context as long as the following conditions hold:
       *  - The RTC is Active.
       *  - The given execution context is Running.
       * This callback occurs during the second execution pass.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      virtual void on_state_update(const ECStates& st) = 0;
      
      /*!
       * @if jp
       * @brief ExecutionContext の実行周期変更時に呼ばれる純粋仮想関数
       *
       * 参加している ExecutionContext の実行周期が変更となった場合に、
       * この変更を伝達するために呼び出される純粋仮想関数。
       *
       * @else
       *
       * @brief Pure virtual function to be invoked when when the execution 
       *        cycles of ExecutionContext is changed.
       *
       * This operation is a notification that the rate of the indicated
       * execution context has changed.
       *
       * @endif
       */
      virtual void on_rate_changed() = 0;
      
      /*!
       * @if jp
       * @brief 状態遷移を実行するワーカーを取得する
       *
       * 管理対象RTコンポーネントの状態遷移を実行するワーカーを取得する。
       *
       * @return ワーカー
       *
       * @else
       * @brief Get the worker to execute the state transition
       *
       * Get the worker that executes the state transition of the target
       * component to manage.
       *
       * @return The worker
       *
       * @endif
       */
      virtual void worker() {return m_sm.worker();}
      
      /*!
       * @if jp
       * @brief 現在の状態を取得する
       *
       * 管理対象RTコンポーネントの現在の状態を取得する。
       *
       * @return 現在状態
       *
       * @else
       * @brief Get the current state of the target component
       *
       * Get the current state of the target component to manage
       *
       * @return The current state of the target RT-Component
       *
       * @endif
       */
      virtual ExecContextState get_state(){ return m_sm.getState();}
      
      /*!
       * @if jp
       * @brief 参加している ExecutionContext の ID
       * @else
       * @brief ID of participating ExecutionContext
       * @endif
       */
      UniqueId ec_id;
      
      /*!
       * @if jp
       * @brief 管理対象RTコンポーネントのステートマシン
       * @else
       * @brief The state machine of the target RT-Component to manage
       * @endif
       */
      RTC_Utils::StateMachine<ExecContextState, DFPBase> m_sm;
    };
    
    //============================================================
    // DFP
    //============================================================
    /*!
     * @if jp
     * @class DFP
     * @brief DFP クラス
     *
     * 参加者リストに登録された DataFlowParticipant の関数を起動するための
     * テンプレートクラス。
     *
     * @param Object 管理対象コンポーネントの型
     *
     * @since 0.4.0
     *
     * @else
     * @class DFP
     * @brief DFP class
     *
     * Template class to invoke DataFlowParticipant registered
     * in the participant list.
     *
     * @param Object Type of the target component to manage
     *
     * @since 0.4.0
     *
     * @endif
     */
    template <class Object>
    class DFP
      : public DFPBase
    {
    public:
      /*!
       * @if jp
       * @brief デフォルトコンストラクタ
       *
       * デフォルトコンストラクタ
       *
       * @param obj 管理対象コンポーネント
       * @param id 所属する ExecutionContext のID
       *
       * @else
       * @brief Default constructor
       *
       * Default constructor
       *
       * @param obj The target component to manage
       * @param id ID of participating ExecutionContext
       *
       * @endif
       */
      DFP(Object obj, ExecutionContextHandle_t id)
	: DFPBase(id), m_obj(obj), m_active(true)
      {
      }
      
      /*!
       * @if jp
       * @brief ExecutionContext 実行開始時に呼ばれる関数
       *
       * 参加している ExecutionContext が実行を開始する時(Running状態へ遷移時)
       * に、管理対象コンポーネントの on_startup を呼びだす。
       *
       * @else
       * @brief Function to be invoked when ExecutionContext starts
       *
       * When the given ExecutionContext transits from Stopped to Running,
       * on_startup of the participation component will be invoked.
       *
       * @endif
       */
      void on_startup()
      {
	m_obj->on_startup(ec_id);
      }
      
      /*!
       * @if jp
       * @brief ExecutionContext 停止時に呼ばれる関数
       *
       * 参加している ExecutionContext が実行を停止する時(Stopped状態へ遷移時)
       * に、管理対象コンポーネントの on_shutdown を呼びだす。
       *
       * @else
       * @brief Function to be invoked when ExecutionContext stops
       *
       * When the given ExecutionContext transits from Running to Stopped,
       * on_shutdown of the participation component will be invoked.
       *
       * @endif
       */
      void on_shutdown()
      {
	m_obj->on_shutdown(ec_id);
      }
      
      /*!
       * @if jp
       * @brief RTコンポーネントがアクティブ化された時に呼ばれる関数
       *
       * 管理対象のRTコンポーネントがアクティブ化された時(Active状態へ遷移時)
       * に、管理対象コンポーネントの on_activated を呼びだす。
       * 管理対象コンポーネントのアクティブ化が失敗した場合には、ステートマシン
       * を Error 状態に遷移させる。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       * @brief Function to be invoked when RT-Component was activated
       *
       * When the given ExecutionContext transits to the Active state,
       * on_activated of the participation component will be invoked.
       * If it fails, the state machine transits to the Errot state.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      void on_activated(const ECStates& st)
      {
	if (m_obj->on_activated(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }
	return;
      }
      
      /*!
       * @if jp
       * @brief RTコンポーネントが非アクティブ化された時に呼ばれる関数
       *
       * 管理対象のRTコンポーネントが非アクティブ化された時
       * (Deactive状態へ遷移時)に、管理対象コンポーネントの on_deactivated を
       * 呼びだす。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       * @brief Function to be invoked when RT-Component was deactivated
       *
       * When the given ExecutionContext transits the Deactivate state,
       * on_deactivated of the participation component will be invoked.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      void on_deactivated(const ECStates& st)
      {
	m_obj->on_deactivated(ec_id);
      }
      
      /*!
       * @if jp
       * @brief RTコンポーネントでエラーが発生した時に呼ばれる関数
       *
       * 管理対象のRTコンポーネントにエラーが発生した時(Error状態へ遷移時)
       * に管理対象コンポーネントの on_aborting を呼びだす。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       * @brief Function to be invoked when RT-Component occured error
       *
       * When the given ExecutionContext transits the Error state,
       * on_aborting of the participation component will be invoked.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      void on_aborting(const ECStates& st)
      {
	m_obj->on_aborting(ec_id);
      }
      
      /*!
       * @if jp
       * @brief RTコンポーネントがエラー状態の時に呼ばれる関数
       *
       * 管理対象のRTコンポーネントがエラー状態にいる間、
       * 管理対象コンポーネントの on_aborting を定期的に呼びだす。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       * @brief Function to be invoked while RT-Component is in the error state
       *
       * While the given RT-Component is in the Error state,
       * its on_aborting will be periodically invoked.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      void on_error(const ECStates& st)
      {
	m_obj->on_error(ec_id);
      }
      
      /*!
       * @if jp
       * @brief RTコンポーネントをリセットする時に呼ばれる関数
       *
       * 管理対象のRTコンポーネントをリセットする際に、管理対象コンポーネント
       * の on_reset を呼びだす。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       * @brief Function to be invoked when RT-Component is reset.
       *
       * When the target RT-Component is reset,
       * invoke on_reset of the target component to manage.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      void on_reset(const ECStates& st)
      {
	if (m_obj->on_reset(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }
	return;
      }
      
      /*!
       * @if jp
       * @brief RTコンポーネント実行時に定期的に呼ばれる関数
       *
       * 管理対象のRTコンポーネントが Active 状態であるとともに、
       * ExecutionContext が Running 状態の場合に、設定された動作周期で定期的に
       * 管理対象コンポーネントの on_execute を呼びだす。
       * 関数の実行に失敗した場合(返値が RTC_OK 以外)、管理対象コンポーネントの
       * 状態を Error 状態に遷移させる。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       * @brief Function to be periodically invoked while RT-Component is running
       *
       * If the given RT-Component is in the Active state and ExecutionContext
       * is in the Running state, on_execute of the given component will be
       * invoked periodically at the specified execution cycle.
       * If it fails (the return value is other than RTC_OK), its state transits 
       * to the Errot state.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      void on_execute(const ECStates& st)
      {
	if (m_obj->on_execute(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }  
	return;
      }
      
      /*!
       * @if jp
       * @brief RTコンポーネント実行時に定期的に呼ばれる関数
       *
       * 管理対象のRTコンポーネントが Active 状態であるとともに、
       * ExecutionContext が Running 状態の場合に、設定された動作周期で定期的に
       * 管理対象コンポーネントの on_state_update を呼びだす。
       * 関数の実行に失敗した場合(返値が RTC_OK 以外)、管理対象コンポーネントの
       * 状態を Error 状態に遷移させる。
       *
       * @param st 対象RTコンポーネントの現在の状態
       *
       * @else
       * @brief Function to be invoked periodically while RT-Component executes
       *
       * When the target RT-Component to manage is in the Active state and
       * ExecutionContext is the Running, invoke on_state_update of the target
       * component to manage periodically in specified execution cycle.
       * If it fails (the return value is other than RTC_OK), its state transits 
       * to the Errot state.
       *
       * @param st The current state of the target RT-Component
       *
       * @endif
       */
      void on_state_update(const ECStates& st)
      {
	if (m_obj->on_state_update(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }
	return;
      }
      
      /*!
       * @if jp
       * @brief ExecutionContext の実行周期変更時に呼ばれる関数
       *
       * 参加している ExecutionContext の実行周期が変更となった場合に、
       * 管理対象コンポーネントの on_rate_changed を呼びだす。
       *
       * @else
       * @brief Function to be invoked when the execution cycles of
       *        ExecutionContext is changed
       *
       * When the execution cycle of the participating ExecutionContext is
       * changed, invoke on_rate_changed of the target component will be invoked.
       *
       * @endif
       */
      void on_rate_changed()
      {
	m_obj->on_rate_changed(ec_id);
      }
      
      /*!
       * @if jp
       * @brief 管理対象コンポーネント
       * @else
       * @brief The target component to manage
       * @endif
       */
      Object m_obj;
      
      /*!
       * @if jp
       * @brief 管理対象コンポーネントの動作状態フラグ
       * @else
       * @brief State flag of the target component to manage
       * @endif
       */
      bool m_active;
    };
    
    /*!
     * @if jp
     * @brief コンポーネント管理用構造体
     * @else
     * @brief The structure for the component management
     * @endif
     */
    struct Comp
    {
      Comp(LightweightRTObject_ptr ref, OpenRTM::DataFlowComponent_ptr dfp,
	   UniqueId id)
	: _ref(ref), _sm(dfp, id)
      {
      }
      ~Comp()
      {
      }
      Comp(const Comp& comp)
	: _ref(comp._ref), _sm(comp._sm.m_obj, comp._sm.ec_id)
      {
      }
      Comp& operator=(const Comp& comp)
      {
	_ref = comp._ref;
	_sm.m_obj = comp._sm.m_obj;
	_sm.ec_id = comp._sm.ec_id;
	return *this;
      }
      LightweightRTObject_var _ref;
      DFP<OpenRTM::DataFlowComponent_var> _sm;
    };

    /*!
     * @if jp
     * @brief コンポーネント検索用ファンクタ
     * @else
     * @brief Functor to find the component
     * @endif
     */
    struct find_comp
    {
      LightweightRTObject_var m_comp;
      find_comp(LightweightRTObject_ptr comp) : m_comp(comp) {}
      bool operator()(Comp& comp)
      {
	return comp._ref->_is_equivalent(m_comp);
      }
    };
    
    /*!
     * @if jp
     * @brief on_startup 起動用ファンクタ
     * @else
     * @brief Functor to invoke on_startup
     * @endif
     */
    struct invoke_on_startup
    {
      void operator()(Comp& comp)
      {
	comp._sm.on_startup();
      }
    };
    
    /*!
     * @if jp
     * @brief on_shutdown 起動用ファンクタ
     * @else
     * @brief Functor to invoke on_shutdown
     * @endif
     */
    struct invoke_on_shutdown
    {
      void operator()(Comp& comp)
      {
	comp._sm.on_shutdown();
      }
    };
    
    /*!
     * @if jp
     * @brief on_rate_changed 起動用ファンクタ
     * @else
     * @brief Functor to invoke on_rate_changed
     * @endif
     */
    struct invoke_on_rate_changed
    {
      void operator()(Comp& comp)
      {
	comp._sm.on_rate_changed();
      }
    };
    
    /*!
     * @if jp
     * @brief ワーカー実行用ファンクタ
     * @else
     * @brief Functor to invoke worker
     * @endif
     */
    struct invoke_worker
    {
      void operator()(Comp& comp)
      {
	comp._sm.worker();
      }
    };
    
    /*!
     * @if jp
     * @brief コンポーネントの参加者リスト
     * @else
     * @brief List of the participating component
     * @endif
     */
    std::vector<Comp> m_comps;
    typedef std::vector<Comp>::iterator CompItr;
    
    /*!
     * @if jp
     * @brief ExecutionContext の実行状態
     * true: running, false: stopped
     * @else
     * @brief The running state of ExecutionContext
     * true: running, false: stopped
     * @endif
     */
    bool m_state;

    /*!
     * @if jp
     * @brief ExecutionContext のスレッド実行フラグ
     * @else
     * @brief The thread running flag of ExecutionContext
     * @endif
     */
    bool m_running;
    
    /*!
     * @if jp
     * @brief ExecutionContextProfile
     * @else
     * @brief ExecutionContextProfile
     * @endif
     */
    ExecutionContextProfile m_profile;
    
    /*!
     * @if jp
     * @brief ExecutionContext の実行周期
     * @else
     * @brief Execution cycle of ExecutionContext
     * @endif
     */
    long int m_usec;
    
    /*!
     * @if jp
     * @brief ExecutionContextService オブジェクトへの参照
     * @else
     * @brief Reference to ExecutionContextService object
     * @endif
     */
    ExecutionContextService_var m_ref;
    
    /*!
     * @if jp
     * @brief ExecutionContext 即時実行(wait無し実行)フラグ
     * @else
     * @brief Flag of ExecutionContext to run immediately
     *        (to run without waiting)
     * @endif
     */
    bool m_nowait;
  }; // class PeriodicExecutionContext
}; // namespace RTC

extern "C"
{
  void PeriodicExecutionContextInit(RTC::Manager* manager);
};

#endif
