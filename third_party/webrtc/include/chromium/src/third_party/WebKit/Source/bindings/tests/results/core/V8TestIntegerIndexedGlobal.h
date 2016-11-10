// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

// clang-format off
#ifndef V8TestIntegerIndexedGlobal_h
#define V8TestIntegerIndexedGlobal_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "bindings/core/v8/ToV8.h"
#include "bindings/core/v8/V8Binding.h"
#include "bindings/core/v8/V8DOMWrapper.h"
#include "bindings/core/v8/WrapperTypeInfo.h"
#include "bindings/tests/idls/core/TestIntegerIndexedGlobal.h"
#include "core/CoreExport.h"
#include "platform/heap/Handle.h"

namespace blink {

class V8TestIntegerIndexedGlobal {
    STATIC_ONLY(V8TestIntegerIndexedGlobal);
public:
    CORE_EXPORT static bool hasInstance(v8::Local<v8::Value>, v8::Isolate*);
    static v8::Local<v8::Object> findInstanceInPrototypeChain(v8::Local<v8::Value>, v8::Isolate*);
    CORE_EXPORT static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate*, const DOMWrapperWorld&);
    CORE_EXPORT static v8::Local<v8::FunctionTemplate> domTemplateForNamedPropertiesObject(v8::Isolate*, const DOMWrapperWorld&);
    static TestIntegerIndexedGlobal* toImpl(v8::Local<v8::Object> object)
    {
        return toScriptWrappable(object)->toImpl<TestIntegerIndexedGlobal>();
    }
    CORE_EXPORT static TestIntegerIndexedGlobal* toImplWithTypeCheck(v8::Isolate*, v8::Local<v8::Value>);
    CORE_EXPORT static const WrapperTypeInfo wrapperTypeInfo;
    template<typename VisitorDispatcher>
    static void trace(VisitorDispatcher visitor, ScriptWrappable* scriptWrappable)
    {
        visitor->trace(scriptWrappable->toImpl<TestIntegerIndexedGlobal>());
    }
    static void traceWrappers(WrapperVisitor* visitor, ScriptWrappable* scriptWrappable)
    {
        visitor->traceWrappers(scriptWrappable->toImpl<TestIntegerIndexedGlobal>());
    }
    static void indexedPropertyGetterCustom(uint32_t, const v8::PropertyCallbackInfo<v8::Value>&);
    static void indexedPropertySetterCustom(uint32_t, v8::Local<v8::Value>, const v8::PropertyCallbackInfo<v8::Value>&);
    static void indexedPropertyDeleterCustom(uint32_t, const v8::PropertyCallbackInfo<v8::Boolean>&);
    static void namedPropertyGetterCustom(const AtomicString&, const v8::PropertyCallbackInfo<v8::Value>&);
    static void namedPropertySetterCustom(const AtomicString&, v8::Local<v8::Value>, const v8::PropertyCallbackInfo<v8::Value>&);
    static void namedPropertyQueryCustom(const AtomicString&, const v8::PropertyCallbackInfo<v8::Integer>&);
    static void namedPropertyDeleterCustom(const AtomicString&, const v8::PropertyCallbackInfo<v8::Boolean>&);
    static void namedPropertyEnumeratorCustom(const v8::PropertyCallbackInfo<v8::Array>&);
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + 0;
};

template <>
struct V8TypeOf<TestIntegerIndexedGlobal> {
    typedef V8TestIntegerIndexedGlobal Type;
};

} // namespace blink

#endif // V8TestIntegerIndexedGlobal_h
