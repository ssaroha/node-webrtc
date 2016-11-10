// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_COMMON_WM_IMMERSIVE_CONTEXT_ASH_H_
#define ASH_COMMON_WM_IMMERSIVE_CONTEXT_ASH_H_

#include "ash/shared/immersive_context.h"
#include "base/macros.h"

namespace ash {

class ASH_EXPORT ImmersiveContextAsh : public ImmersiveContext {
 public:
  ImmersiveContextAsh();
  ~ImmersiveContextAsh() override;

  // ImmersiveContext:
  void InstallResizeHandleWindowTargeter(
      ImmersiveFullscreenController* controller) override;
  void OnEnteringOrExitingImmersive(ImmersiveFullscreenController* controller,
                                    bool entering) override;
  gfx::Rect GetDisplayBoundsInScreen(views::Widget* widget) override;
  void AddPointerWatcher(views::PointerWatcher* watcher,
                         views::PointerWatcherEventTypes events) override;
  void RemovePointerWatcher(views::PointerWatcher* watcher) override;
  bool DoesAnyWindowHaveCapture() override;
  bool IsMouseEventsEnabled() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(ImmersiveContextAsh);
};

}  // namespace ash

#endif  // ASH_COMMON_WM_IMMERSIVE_CONTEXT_ASH_H_
