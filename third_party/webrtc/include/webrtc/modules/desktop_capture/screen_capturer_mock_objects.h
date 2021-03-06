/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_DESKTOP_CAPTURE_SCREEN_CAPTURER_MOCK_OBJECTS_H_
#define WEBRTC_MODULES_DESKTOP_CAPTURE_SCREEN_CAPTURER_MOCK_OBJECTS_H_

#include "webrtc/base/constructormagic.h"
#include "webrtc/modules/desktop_capture/screen_capturer.h"
#include "webrtc/test/gmock.h"

namespace webrtc {

// Deprecated.
// Use webrtc/modules/desktop_capture/mock_desktop_capturer_callback.h.
class MockScreenCapturerCallback : public ScreenCapturer::Callback {
 public:
  MockScreenCapturerCallback() {}
  ~MockScreenCapturerCallback() override {}

  MOCK_METHOD2(OnCaptureResultPtr,
               void(DesktopCapturer::Result result,
                    std::unique_ptr<DesktopFrame>* frame));
  void OnCaptureResult(DesktopCapturer::Result result,
                       std::unique_ptr<DesktopFrame> frame) override {
    OnCaptureResultPtr(result, &frame);
  }

 private:
  RTC_DISALLOW_COPY_AND_ASSIGN(MockScreenCapturerCallback);
};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_DESKTOP_CAPTURE_SCREEN_CAPTURER_MOCK_OBJECTS_H_
