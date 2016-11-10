/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LayoutGeometryMap_h
#define LayoutGeometryMap_h

#include "core/CoreExport.h"
#include "core/layout/LayoutGeometryMapStep.h"
#include "core/layout/LayoutObject.h"
#include "platform/geometry/FloatPoint.h"
#include "platform/geometry/FloatQuad.h"
#include "platform/geometry/IntSize.h"
#include "platform/geometry/LayoutSize.h"
#include "wtf/Allocator.h"

namespace blink {

class PaintLayer;
class LayoutBoxModelObject;
class TransformationMatrix;
class TransformState;

// Can be used while walking the layout tree to cache data about offsets and
// transforms.
class CORE_EXPORT LayoutGeometryMap {
  DISALLOW_NEW();
  WTF_MAKE_NONCOPYABLE(LayoutGeometryMap);

 public:
  LayoutGeometryMap(MapCoordinatesFlags = UseTransforms);
  ~LayoutGeometryMap();

  MapCoordinatesFlags getMapCoordinatesFlags() const {
    return m_mapCoordinatesFlags;
  }

  FloatRect absoluteRect(const FloatRect& rect) const {
    return mapToAncestor(rect, 0).boundingBox();
  }

  // Map to an ancestor. Will assert that the ancestor has been pushed onto this
  // map. A null ancestor maps through the LayoutView (including its scale
  // transform, if any). If the ancestor is the LayoutView, the scroll offset is
  // applied, but not the scale.
  FloatQuad mapToAncestor(const FloatRect&, const LayoutBoxModelObject*) const;

  // Called by code walking the layout or layer trees.
  void pushMappingsToAncestor(const PaintLayer*,
                              const PaintLayer* ancestorLayer);
  void popMappingsToAncestor(const PaintLayer*);
  void pushMappingsToAncestor(const LayoutObject*,
                              const LayoutBoxModelObject* ancestorLayoutObject);

  // The following methods should only be called by layoutObjects inside a call
  // to pushMappingsToAncestor().

  // Push geometry info between this layoutObject and some ancestor. The
  // ancestor must be its container() or some stacking context between the
  // layoutObject and its container.
  void push(const LayoutObject*,
            const LayoutSize&,
            GeometryInfoFlags = 0,
            LayoutSize offsetForFixedPosition = LayoutSize());
  void push(const LayoutObject*,
            const TransformationMatrix&,
            GeometryInfoFlags = 0,
            LayoutSize offsetForFixedPosition = LayoutSize());

 private:
  void popMappingsToAncestor(const LayoutBoxModelObject*);
  void mapToAncestor(TransformState&,
                     const LayoutBoxModelObject* ancestor = nullptr) const;

  void stepInserted(const LayoutGeometryMapStep&);
  void stepRemoved(const LayoutGeometryMapStep&);

  bool hasNonUniformStep() const { return m_nonUniformStepsCount; }
  bool hasTransformStep() const { return m_transformedStepsCount; }
  bool hasFixedPositionStep() const { return m_fixedStepsCount; }

#ifndef NDEBUG
  void dumpSteps() const;
#endif

#if ENABLE(ASSERT)
  bool isTopmostLayoutView(const LayoutObject*) const;
#endif

  typedef Vector<LayoutGeometryMapStep, 32> LayoutGeometryMapSteps;

  size_t m_insertionPosition;
  int m_nonUniformStepsCount;
  int m_transformedStepsCount;
  int m_fixedStepsCount;
  LayoutGeometryMapSteps m_mapping;
  LayoutSize m_accumulatedOffset;
  MapCoordinatesFlags m_mapCoordinatesFlags;
};

}  // namespace blink

#endif  // LayoutGeometryMap_h
