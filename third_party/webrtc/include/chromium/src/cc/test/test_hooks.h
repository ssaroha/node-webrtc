// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_TEST_HOOKS_H_
#define CC_TEST_TEST_HOOKS_H_

#include "base/macros.h"
#include "cc/animation/animation_delegate.h"
#include "cc/trees/layer_tree_host.h"
#include "cc/trees/layer_tree_host_impl.h"

namespace cc {

class CompositorFrame;

namespace proto {
class CompositorMessageToImpl;
}

// Used by test stubs to notify the test when something interesting happens.
class TestHooks : public AnimationDelegate {
 public:
  TestHooks();
  ~TestHooks() override;

  // Compositor thread hooks.
  virtual void CreateResourceAndRasterBufferProvider(
      LayerTreeHostImpl* host_impl,
      std::unique_ptr<RasterBufferProvider>* raster_buffer_provider,
      std::unique_ptr<ResourcePool>* resource_pool);
  virtual void WillBeginImplFrameOnThread(LayerTreeHostImpl* host_impl,
                                          const BeginFrameArgs& args) {}
  virtual void DidFinishImplFrameOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void BeginMainFrameAbortedOnThread(LayerTreeHostImpl* host_impl,
                                             CommitEarlyOutReason reason) {}
  virtual void ReadyToCommitOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void WillPrepareTilesOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void BeginCommitOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void WillCommitCompleteOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void CommitCompleteOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void WillActivateTreeOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void DidActivateTreeOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void InitializedRendererOnThread(LayerTreeHostImpl* host_impl,
                                           bool success) {}
  virtual void WillPrepareToDrawOnThread(LayerTreeHostImpl* host_impl) {}
  virtual DrawResult PrepareToDrawOnThread(
      LayerTreeHostImpl* host_impl,
      LayerTreeHostImpl::FrameData* frame_data,
      DrawResult draw_result);
  virtual void DrawLayersOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void NotifyReadyToActivateOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void NotifyReadyToDrawOnThread(LayerTreeHostImpl* host_impl) {}
  virtual void NotifyAllTileTasksCompleted(LayerTreeHostImpl* host_impl) {}
  virtual void NotifyTileStateChangedOnThread(LayerTreeHostImpl* host_impl,
                                              const Tile* tile) {}
  virtual void DidSetVisibleOnImplTree(LayerTreeHostImpl* host_impl,
                                       bool visible) {}
  virtual void AnimateLayers(LayerTreeHostImpl* host_impl,
                             base::TimeTicks monotonic_time) {}
  virtual void UpdateAnimationState(LayerTreeHostImpl* host_impl,
                                    bool has_unfinished_animation) {}
  virtual void WillAnimateLayers(LayerTreeHostImpl* host_impl,
                                 base::TimeTicks monotonic_time) {}

  // Asynchronous compositor thread hooks.
  // These are called asynchronously from the LayerTreeHostImpl performing its
  // draw, so you should record state you want to use here in
  // DrawLayersOnThread() instead. For that reason these methods do not receive
  // a LayerTreeHostImpl pointer.
  virtual void DisplayReceivedCompositorFrameOnThread(
      const CompositorFrame& frame) {}
  virtual void DisplayWillDrawAndSwapOnThread(
      bool will_draw_and_swap,
      const RenderPassList& render_passes) {}
  virtual void DisplayDidDrawAndSwapOnThread() {}

  // Main thread hooks.
  virtual void ApplyViewportDeltas(
      const gfx::Vector2dF& inner_delta,
      const gfx::Vector2dF& outer_delta,
      const gfx::Vector2dF& elastic_overscroll_delta,
      float scale,
      float top_controls_delta) {}
  virtual void BeginMainFrameNotExpectedSoon() {}
  virtual void BeginMainFrame(const BeginFrameArgs& args) {}
  virtual void WillBeginMainFrame() {}
  virtual void DidBeginMainFrame() {}
  virtual void UpdateLayerTreeHost() {}
  virtual void DidInitializeCompositorFrameSink() {}
  virtual void DidFailToInitializeCompositorFrameSink() {}
  virtual void DidAddAnimation() {}
  virtual void WillCommit() {}
  virtual void DidCommit() {}
  virtual void DidCommitAndDrawFrame() {}
  virtual void DidReceiveCompositorFrameAck() {}
  virtual void ScheduleComposite() {}
  virtual void DidActivateSyncTree() {}

  // AnimationDelegate implementation.
  void NotifyAnimationStarted(base::TimeTicks monotonic_time,
                              TargetProperty::Type target_property,
                              int group) override {}
  void NotifyAnimationFinished(base::TimeTicks monotonic_time,
                               TargetProperty::Type target_property,
                               int group) override {}
  void NotifyAnimationAborted(base::TimeTicks monotonic_time,
                              TargetProperty::Type target_property,
                              int group) override {}
  void NotifyAnimationTakeover(base::TimeTicks monotonic_time,
                               TargetProperty::Type target_property,
                               double animation_start_time,
                               std::unique_ptr<AnimationCurve> curve) override {
  }

  // OutputSurface indirections to the LayerTreeTest, that can be further
  // overridden.
  virtual void RequestNewCompositorFrameSink() = 0;
  virtual std::unique_ptr<OutputSurface> CreateDisplayOutputSurfaceOnThread(
      scoped_refptr<ContextProvider> compositor_context_provider) = 0;
};

}  // namespace cc

#endif  // CC_TEST_TEST_HOOKS_H_
