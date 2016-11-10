// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_SYNC_ENGINE_IMPL_SYNC_SCHEDULER_IMPL_H_
#define COMPONENTS_SYNC_ENGINE_IMPL_SYNC_SCHEDULER_IMPL_H_

#include <map>
#include <memory>
#include <string>

#include "base/callback.h"
#include "base/cancelable_callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/macros.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "components/sync/base/weak_handle.h"
#include "components/sync/engine/polling_constants.h"
#include "components/sync/engine_impl/cycle/nudge_tracker.h"
#include "components/sync/engine_impl/cycle/sync_cycle.h"
#include "components/sync/engine_impl/cycle/sync_cycle_context.h"
#include "components/sync/engine_impl/net/server_connection_manager.h"
#include "components/sync/engine_impl/nudge_source.h"
#include "components/sync/engine_impl/sync_scheduler.h"
#include "components/sync/engine_impl/syncer.h"

namespace syncer {

class BackoffDelayProvider;
struct ModelNeutralState;

class SyncSchedulerImpl : public SyncScheduler, public base::NonThreadSafe {
 public:
  // |name| is a display string to identify the syncer thread.  Takes
  // |ownership of |syncer| and |delay_provider|.
  SyncSchedulerImpl(const std::string& name,
                    BackoffDelayProvider* delay_provider,
                    SyncCycleContext* context,
                    Syncer* syncer);

  // Calls Stop().
  ~SyncSchedulerImpl() override;

  void Start(Mode mode, base::Time last_poll_time) override;
  void ScheduleConfiguration(const ConfigurationParams& params) override;
  void ScheduleClearServerData(const ClearParams& params) override;
  void Stop() override;
  void ScheduleLocalNudge(
      ModelTypeSet types,
      const tracked_objects::Location& nudge_location) override;
  void ScheduleLocalRefreshRequest(
      ModelTypeSet types,
      const tracked_objects::Location& nudge_location) override;
  void ScheduleInvalidationNudge(
      ModelType type,
      std::unique_ptr<InvalidationInterface> invalidation,
      const tracked_objects::Location& nudge_location) override;
  void ScheduleInitialSyncNudge(ModelType model_type) override;
  void SetNotificationsEnabled(bool notifications_enabled) override;

  void OnCredentialsUpdated() override;
  void OnConnectionStatusChange() override;

  // SyncCycle::Delegate implementation.
  void OnThrottled(const base::TimeDelta& throttle_duration) override;
  void OnTypesThrottled(ModelTypeSet types,
                        const base::TimeDelta& throttle_duration) override;
  bool IsCurrentlyThrottled() override;
  void OnReceivedShortPollIntervalUpdate(
      const base::TimeDelta& new_interval) override;
  void OnReceivedLongPollIntervalUpdate(
      const base::TimeDelta& new_interval) override;
  void OnReceivedCustomNudgeDelays(
      const std::map<ModelType, base::TimeDelta>& nudge_delays) override;
  void OnReceivedClientInvalidationHintBufferSize(int size) override;
  void OnSyncProtocolError(
      const SyncProtocolError& sync_protocol_error) override;
  void OnReceivedGuRetryDelay(const base::TimeDelta& delay) override;
  void OnReceivedMigrationRequest(ModelTypeSet types) override;

  // Returns true if the client is currently in exponential backoff.
  bool IsBackingOff() const;

 private:
  enum JobPriority {
    // Non-canary jobs respect exponential backoff.
    NORMAL_PRIORITY,
    // Canary jobs bypass exponential backoff, so use with extreme caution.
    CANARY_PRIORITY
  };

  enum PollAdjustType {
    // Restart the poll interval.
    FORCE_RESET,
    // Restart the poll interval only if its length has changed.
    UPDATE_INTERVAL,
  };

  friend class SyncSchedulerImplTest;
  friend class SyncSchedulerWhiteboxTest;
  friend class SyncerTest;

  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest, TransientPollFailure);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest,
                           ServerConnectionChangeDuringBackoff);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest,
                           ConnectionChangeCanaryPreemptedByNudge);
  FRIEND_TEST_ALL_PREFIXES(BackoffTriggersSyncSchedulerTest,
                           FailGetEncryptionKey);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest, SuccessfulRetry);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest, FailedRetry);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest, ReceiveNewRetryDelay);

  struct WaitInterval {
    enum Mode {
      // Uninitialized state, should not be set in practice.
      UNKNOWN = -1,
      // We enter a series of increasingly longer WaitIntervals if we experience
      // repeated transient failures.  We retry at the end of each interval.
      EXPONENTIAL_BACKOFF,
      // A server-initiated throttled interval.  We do not allow any syncing
      // during such an interval.
      THROTTLED,
    };
    WaitInterval();
    ~WaitInterval();
    WaitInterval(Mode mode, base::TimeDelta length);

    static const char* GetModeString(Mode mode);

    Mode mode;
    base::TimeDelta length;
  };

  static const char* GetModeString(Mode mode);

  void SetDefaultNudgeDelay(base::TimeDelta delay_ms);

  // Invoke the syncer to perform a nudge job.
  void DoNudgeSyncCycleJob(JobPriority priority);

  // Invoke the syncer to perform a configuration job.
  void DoConfigurationSyncCycleJob(JobPriority priority);

  void DoClearServerDataSyncCycleJob(JobPriority priority);

  // Helper function for Do{Nudge,Configuration,Poll}SyncCycleJob.
  void HandleSuccess();

  // Helper function for Do{Nudge,Configuration,Poll}SyncCycleJob.
  void HandleFailure(const ModelNeutralState& model_neutral_state);

  // Invoke the Syncer to perform a poll job.
  void DoPollSyncCycleJob();

  // Helper function to calculate poll interval.
  base::TimeDelta GetPollInterval();

  // Adjusts the poll timer to account for new poll interval, and possibly
  // resets the poll interval, depedning on the flag's value.
  void AdjustPolling(PollAdjustType type);

  // Helper to restart waiting with |wait_interval_|'s timer.
  void RestartWaiting();

  // Determines if we're allowed to contact the server right now.
  bool CanRunJobNow(JobPriority priority);

  // Determines if we're allowed to contact the server right now.
  bool CanRunNudgeJobNow(JobPriority priority);

  // If the scheduler's current state supports it, this will create a job based
  // on the passed in parameters and coalesce it with any other pending jobs,
  // then post a delayed task to run it.  It may also choose to drop the job or
  // save it for later, depending on the scheduler's current state.
  void ScheduleNudgeImpl(const base::TimeDelta& delay,
                         const tracked_objects::Location& nudge_location);

  // Helper to signal listeners about changed retry time.
  void NotifyRetryTime(base::Time retry_time);

  // Helper to signal listeners about changed throttled types.
  void NotifyThrottledTypesChanged(ModelTypeSet types);

  // Looks for pending work and, if it finds any, run this work at "canary"
  // priority.
  void TryCanaryJob();

  // At the moment TrySyncCycleJob just posts call to TrySyncCycleJobImpl on
  // current thread. In the future it will request access token here.
  void TrySyncCycleJob();
  void TrySyncCycleJobImpl();

  // Transitions out of the THROTTLED WaitInterval then calls TryCanaryJob().
  void Unthrottle();

  // Called when a per-type throttling interval expires.
  void TypeUnthrottle(base::TimeTicks unthrottle_time);

  // Runs a normal nudge job when the scheduled timer expires.
  void PerformDelayedNudge();

  // Attempts to exit EXPONENTIAL_BACKOFF by calling TryCanaryJob().
  void ExponentialBackoffRetry();

  // Called when the root cause of the current connection error is fixed.
  void OnServerConnectionErrorFixed();

  // Creates a cycle for a poll and performs the sync.
  void PollTimerCallback();

  // Creates a cycle for a retry and performs the sync.
  void RetryTimerCallback();

  // Returns the set of types that are enabled and not currently throttled.
  ModelTypeSet GetEnabledAndUnthrottledTypes();

  // Called as we are started to broadcast an initial cycle snapshot
  // containing data like initial_sync_ended.  Important when the client starts
  // up and does not need to perform an initial sync.
  void SendInitialSnapshot();

  // This is used for histogramming and analysis of ScheduleNudge* APIs.
  // SyncScheduler is the ultimate choke-point for all such invocations (with
  // and without InvalidationState variants, all NudgeSources, etc) and as such
  // is the most flexible place to do this bookkeeping.
  void UpdateNudgeTimeRecords(ModelTypeSet types);

  // For certain methods that need to worry about X-thread posting.
  WeakHandle<SyncSchedulerImpl> weak_handle_this_;

  // Used for logging.
  const std::string name_;

  // Set in Start(), unset in Stop().
  bool started_;

  // Modifiable versions of kDefaultLongPollIntervalSeconds which can be
  // updated by the server.
  base::TimeDelta syncer_short_poll_interval_seconds_;
  base::TimeDelta syncer_long_poll_interval_seconds_;

  // Timer for polling. Restarted on each successful poll, and when entering
  // normal sync mode or exiting an error state. Not active in configuration
  // mode.
  base::OneShotTimer poll_timer_;

  // The mode of operation.
  Mode mode_;

  // Current wait state.  Null if we're not in backoff and not throttled.
  std::unique_ptr<WaitInterval> wait_interval_;

  std::unique_ptr<BackoffDelayProvider> delay_provider_;

  // The event that will wake us up.
  base::OneShotTimer pending_wakeup_timer_;

  // An event that fires when data type throttling expires.
  base::OneShotTimer type_unthrottle_timer_;

  // Storage for variables related to an in-progress configure request.  Note
  // that (mode_ != CONFIGURATION_MODE) \implies !pending_configure_params_.
  std::unique_ptr<ConfigurationParams> pending_configure_params_;

  std::unique_ptr<ClearParams> pending_clear_params_;

  // If we have a nudge pending to run soon, it will be listed here.
  base::TimeTicks scheduled_nudge_time_;

  // Keeps track of work that the syncer needs to handle.
  NudgeTracker nudge_tracker_;

  // Invoked to run through the sync cycle.
  std::unique_ptr<Syncer> syncer_;

  SyncCycleContext* cycle_context_;

  // A map tracking LOCAL NudgeSource invocations of ScheduleNudge* APIs,
  // organized by datatype. Each datatype that was part of the types requested
  // in the call will have its TimeTicks value updated.
  typedef std::map<ModelType, base::TimeTicks> ModelTypeTimeMap;
  ModelTypeTimeMap last_local_nudges_by_model_type_;

  // Used as an "anti-reentrancy defensive assertion".
  // While true, it is illegal for any new scheduling activity to take place.
  // Ensures that higher layers don't break this law in response to events that
  // take place during a sync cycle. We call this out because such violations
  // could result in tight sync loops hitting sync servers.
  bool no_scheduling_allowed_;

  // TryJob might get called for multiple reasons. It should only call
  // DoPollSyncCycleJob after some time since the last attempt.
  // last_poll_reset_ keeps track of when was last attempt.
  base::TimeTicks last_poll_reset_;

  // next_sync_cycle_job_priority_ defines which priority will be used next
  // time TrySyncCycleJobImpl is called. CANARY_PRIORITY allows syncer to run
  // even if scheduler is in exponential backoff. This is needed for events that
  // have chance of resolving previous error (e.g. network connection change
  // after NETWORK_UNAVAILABLE error).
  // It is reset back to NORMAL_PRIORITY on every call to TrySyncCycleJobImpl.
  JobPriority next_sync_cycle_job_priority_;

  // One-shot timer for scheduling GU retry according to delay set by server.
  base::OneShotTimer retry_timer_;

  base::WeakPtrFactory<SyncSchedulerImpl> weak_ptr_factory_;

  // A second factory specially for weak_handle_this_, to allow the handle
  // to be const and alleviate threading concerns.
  base::WeakPtrFactory<SyncSchedulerImpl> weak_ptr_factory_for_weak_handle_;

  DISALLOW_COPY_AND_ASSIGN(SyncSchedulerImpl);
};

}  // namespace syncer

#endif  // COMPONENTS_SYNC_ENGINE_IMPL_SYNC_SCHEDULER_IMPL_H_
