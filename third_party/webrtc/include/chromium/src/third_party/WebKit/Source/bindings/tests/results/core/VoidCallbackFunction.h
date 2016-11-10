// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

// clang-format off

#ifndef VoidCallbackFunction_h
#define VoidCallbackFunction_h

#include "bindings/core/v8/ScopedPersistent.h"
#include "bindings/core/v8/ScriptWrappable.h"
#include "core/CoreExport.h"
#include "platform/heap/Handle.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ScriptState;

class CORE_EXPORT VoidCallbackFunction final : public GarbageCollectedFinalized<VoidCallbackFunction>,
                                        public TraceWrapperBase {
public:
    static VoidCallbackFunction* create(v8::Isolate* isolate, v8::Local<v8::Function> callback)
    {
        return new VoidCallbackFunction(isolate, callback);
    }

    ~VoidCallbackFunction() = default;

    DECLARE_TRACE();
    DECLARE_TRACE_WRAPPERS();

    bool call(ScriptState* scriptState, ScriptWrappable* scriptWrappable);

    v8::Local<v8::Function> v8Value(v8::Isolate* isolate)
    {
        return m_callback.newLocal(isolate);
    }

    void setWrapperReference(v8::Isolate* isolate, const v8::Persistent<v8::Object>& wrapper)
    {
        DCHECK(!m_callback.isEmpty());
        m_callback.setReference(wrapper, isolate);
    }

private:
    VoidCallbackFunction(v8::Isolate* isolate, v8::Local<v8::Function>);
    ScopedPersistent<v8::Function> m_callback;
};

} // namespace blink

#endif // VoidCallbackFunction_h
