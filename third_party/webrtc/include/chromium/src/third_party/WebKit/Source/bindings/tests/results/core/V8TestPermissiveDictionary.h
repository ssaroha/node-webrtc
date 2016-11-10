// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

// clang-format off
#ifndef V8TestPermissiveDictionary_h
#define V8TestPermissiveDictionary_h

#include "bindings/core/v8/ToV8.h"
#include "bindings/core/v8/V8Binding.h"
#include "bindings/tests/idls/core/TestPermissiveDictionary.h"
#include "core/CoreExport.h"
#include "platform/heap/Handle.h"

namespace blink {

class ExceptionState;

class V8TestPermissiveDictionary {
public:
    CORE_EXPORT static void toImpl(v8::Isolate*, v8::Local<v8::Value>, TestPermissiveDictionary&, ExceptionState&);
};

CORE_EXPORT bool toV8TestPermissiveDictionary(const TestPermissiveDictionary&, v8::Local<v8::Object> dictionary, v8::Local<v8::Object> creationContext, v8::Isolate*);

template<class CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& callbackInfo, TestPermissiveDictionary& impl)
{
    v8SetReturnValue(callbackInfo, toV8(impl, callbackInfo.Holder(), callbackInfo.GetIsolate()));
}

template <>
struct NativeValueTraits<TestPermissiveDictionary> {
    static TestPermissiveDictionary nativeValue(v8::Isolate*, v8::Local<v8::Value>, ExceptionState&);
};

template <>
struct V8TypeOf<TestPermissiveDictionary> {
    typedef V8TestPermissiveDictionary Type;
};

} // namespace blink

#endif // V8TestPermissiveDictionary_h
