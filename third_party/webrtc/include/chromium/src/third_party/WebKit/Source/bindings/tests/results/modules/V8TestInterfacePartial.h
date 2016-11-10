// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

// clang-format off
#ifndef V8TestInterfacePartial_h
#define V8TestInterfacePartial_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "bindings/core/v8/ToV8.h"
#include "bindings/core/v8/V8Binding.h"
#include "bindings/core/v8/V8DOMWrapper.h"
#include "bindings/core/v8/WrapperTypeInfo.h"
#include "bindings/tests/idls/core/TestInterfaceImplementation.h"
#include "platform/heap/Handle.h"

namespace blink {
class ScriptState;

class V8TestInterfacePartial {
    STATIC_ONLY(V8TestInterfacePartial);
public:
    static void initialize();
    static void implementsCustomVoidMethodMethodCustom(const v8::FunctionCallbackInfo<v8::Value>&);
    static void preparePrototypeAndInterfaceObject(v8::Local<v8::Context>, const DOMWrapperWorld&, v8::Local<v8::Object>, v8::Local<v8::Function>, v8::Local<v8::FunctionTemplate>);

    static void installOriginTrialPartialFeature(ScriptState*, v8::Local<v8::Object> instance);
    static void installOriginTrialPartialFeature(v8::Isolate*, const DOMWrapperWorld&, v8::Local<v8::Object> instance, v8::Local<v8::Object> prototype, v8::Local<v8::Function> interface);
    static void installOriginTrialPartialFeature(ScriptState*);
private:
    static void installV8TestInterfaceTemplate(v8::Isolate*, const DOMWrapperWorld&, v8::Local<v8::FunctionTemplate> interfaceTemplate);
};
}
#endif // V8TestInterfacePartial_h
