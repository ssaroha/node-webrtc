// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RemotePlayback_h
#define RemotePlayback_h

#include "bindings/core/v8/ActiveScriptWrappable.h"
#include "bindings/core/v8/ScriptPromise.h"
#include "bindings/core/v8/ScriptState.h"
#include "core/events/EventTarget.h"
#include "modules/ModulesExport.h"
#include "platform/heap/Handle.h"
#include "public/platform/modules/remoteplayback/WebRemotePlaybackClient.h"
#include "public/platform/modules/remoteplayback/WebRemotePlaybackState.h"
#include "wtf/Compiler.h"
#include "wtf/text/AtomicString.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ExecutionContext;
class HTMLMediaElement;
class LocalFrame;
class RemotePlaybackAvailabilityCallback;
class ScriptPromiseResolver;

class MODULES_EXPORT RemotePlayback final
    : public EventTargetWithInlineData,
      public ActiveScriptWrappable,
      WTF_NON_EXPORTED_BASE(private WebRemotePlaybackClient) {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(RemotePlayback);

 public:
  static RemotePlayback* create(ScriptState*, HTMLMediaElement&);

  // Notifies this object that disableRemotePlayback attribute was set on the
  // corresponding media element.
  void remotePlaybackDisabled();

  // EventTarget implementation.
  const WTF::AtomicString& interfaceName() const override;
  ExecutionContext* getExecutionContext() const override;

  // Starts notifying the page about the changes to the remote playback devices
  // availability via the provided callback. May start the monitoring of remote
  // playback devices if it isn't running yet.
  ScriptPromise watchAvailability(RemotePlaybackAvailabilityCallback*);

  // Cancels updating the page via the callback specified by its id.
  ScriptPromise cancelWatchAvailability(int id);

  // Cancels all the callbacks watching remote playback availability changes
  // registered with this element.
  ScriptPromise cancelWatchAvailability();

  // Shows the UI allowing user to change the remote playback state of the media
  // element (by picking a remote playback device from the list, for example).
  ScriptPromise prompt();

  String state() const;

  // ScriptWrappable implementation.
  bool hasPendingActivity() const final;

  DEFINE_ATTRIBUTE_EVENT_LISTENER(connecting);
  DEFINE_ATTRIBUTE_EVENT_LISTENER(connect);
  DEFINE_ATTRIBUTE_EVENT_LISTENER(disconnect);

  DECLARE_VIRTUAL_TRACE();
  DECLARE_VIRTUAL_TRACE_WRAPPERS();

 private:
  friend class V8RemotePlayback;
  friend class RemotePlaybackTest;

  explicit RemotePlayback(ScriptState*, HTMLMediaElement&);

  // Calls the specified availability callback with the current availability.
  // Need a void() method to post it as a task.
  void notifyInitialAvailability(int callbackId);

  // WebRemotePlaybackClient implementation.
  void stateChanged(WebRemotePlaybackState) override;
  void availabilityChanged(bool available) override;
  void promptCancelled() override;

  // Prevent v8 from garbage collecting the availability callbacks.
  // TODO(avayvod): remove when crbug.com/468240 is fixed and the references
  // are maintained automatically.
  void setV8ReferencesForCallbacks(v8::Isolate*,
                                   const v8::Persistent<v8::Object>& wrapper);

  RefPtr<ScriptState> m_scriptState;
  WebRemotePlaybackState m_state;
  bool m_availability;
  HeapHashMap<int, Member<RemotePlaybackAvailabilityCallback>>
      m_availabilityCallbacks;
  Member<HTMLMediaElement> m_mediaElement;
  Member<ScriptPromiseResolver> m_promptPromiseResolver;
};

}  // namespace blink

#endif  // RemotePlayback_h
