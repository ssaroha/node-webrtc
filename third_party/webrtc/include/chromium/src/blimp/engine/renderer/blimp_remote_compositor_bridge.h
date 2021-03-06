// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLIMP_ENGINE_RENDERER_BLIMP_REMOTE_COMPOSITOR_BRIDGE_H_
#define BLIMP_ENGINE_RENDERER_BLIMP_REMOTE_COMPOSITOR_BRIDGE_H_

#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "blimp/engine/renderer/frame_scheduler.h"
#include "cc/blimp/remote_compositor_bridge.h"
#include "cc/trees/remote_proto_channel.h"

namespace blimp {
namespace engine {

class BlimpRemoteCompositorBridge
    : public cc::RemoteCompositorBridge,
      public cc::RemoteProtoChannel::ProtoReceiver,
      public FrameSchedulerClient {
 public:
  // TODO(khushalsagar): Stop using the RemoteProtoChannel. See
  // crbug.com/653697.
  // |remote_proto_channel| should outlive the BlimpRemoteCompositorBridge.
  BlimpRemoteCompositorBridge(
      cc::RemoteProtoChannel* remote_proto_channel,
      scoped_refptr<base::SingleThreadTaskRunner> compositor_main_task_runner);
  ~BlimpRemoteCompositorBridge() override;

  // cc::RemoteCompositorBridge implementation.
  void BindToClient(cc::RemoteCompositorBridgeClient* client) override;
  void ScheduleMainFrame() override;
  void ProcessCompositorStateUpdate(std::unique_ptr<cc::CompositorProtoState>
                                        compositor_proto_state) override;

 private:
  // cc::RemoteProtoChannel::ProtoReceiver implementation.
  void OnProtoReceived(
      std::unique_ptr<cc::proto::CompositorMessage> proto) override;

  // FrameSchedulerClient implementation.
  void StartFrameUpdate() override;

  cc::RemoteProtoChannel* remote_proto_channel_;
  cc::RemoteCompositorBridgeClient* client_ = nullptr;

  FrameScheduler scheduler_;

  DISALLOW_COPY_AND_ASSIGN(BlimpRemoteCompositorBridge);
};

}  // namespace engine
}  // namespace blimp

#endif  // BLIMP_ENGINE_RENDERER_BLIMP_REMOTE_COMPOSITOR_BRIDGE_H_
