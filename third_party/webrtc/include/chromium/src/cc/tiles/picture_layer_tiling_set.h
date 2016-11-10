// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TILES_PICTURE_LAYER_TILING_SET_H_
#define CC_TILES_PICTURE_LAYER_TILING_SET_H_

#include <stddef.h>

#include <list>
#include <set>
#include <vector>

#include "base/macros.h"
#include "cc/base/region.h"
#include "cc/tiles/picture_layer_tiling.h"
#include "ui/gfx/geometry/size.h"

namespace base {
namespace trace_event {
class TracedValue;
}
}

namespace cc {

class CC_EXPORT PictureLayerTilingSet {
 public:
  enum TilingRangeType {
    HIGHER_THAN_HIGH_RES,
    HIGH_RES,
    BETWEEN_HIGH_AND_LOW_RES,
    LOW_RES,
    LOWER_THAN_LOW_RES
  };
  struct TilingRange {
    TilingRange(size_t start, size_t end) : start(start), end(end) {}

    size_t start;
    size_t end;
  };

  static std::unique_ptr<PictureLayerTilingSet> Create(
      WhichTree tree,
      PictureLayerTilingClient* client,
      int tiling_interest_area_padding,
      float skewport_target_time_in_seconds,
      int skewport_extrapolation_limit_in_screen_pixels,
      float max_preraster_distance);

  ~PictureLayerTilingSet();

  const PictureLayerTilingClient* client() const { return client_; }

  void CleanUpTilings(float min_acceptable_high_res_scale,
                      float max_acceptable_high_res_scale,
                      const std::vector<PictureLayerTiling*>& needed_tilings,
                      PictureLayerTilingSet* twin_set);
  void RemoveNonIdealTilings();

  // This function is called on the active tree during activation.
  void UpdateTilingsToCurrentRasterSourceForActivation(
      scoped_refptr<RasterSource> raster_source,
      const PictureLayerTilingSet* pending_twin_set,
      const Region& layer_invalidation,
      float minimum_contents_scale,
      float maximum_contents_scale);

  // This function is called on the sync tree during commit.
  void UpdateTilingsToCurrentRasterSourceForCommit(
      scoped_refptr<RasterSource> raster_source,
      const Region& layer_invalidation,
      float minimum_contents_scale,
      float maximum_contents_scale);

  // This function is called on the sync tree right after commit.
  void UpdateRasterSourceDueToLCDChange(
      scoped_refptr<RasterSource> raster_source,
      const Region& layer_invalidation);

  PictureLayerTiling* AddTiling(float contents_scale,
                                scoped_refptr<RasterSource> raster_source);
  size_t num_tilings() const { return tilings_.size(); }
  int NumHighResTilings() const;
  PictureLayerTiling* tiling_at(size_t idx) { return tilings_[idx].get(); }
  const PictureLayerTiling* tiling_at(size_t idx) const {
    return tilings_[idx].get();
  }
  WhichTree tree() const { return tree_; }

  PictureLayerTiling* FindTilingWithScale(float scale) const;
  PictureLayerTiling* FindTilingWithResolution(TileResolution resolution) const;

  void MarkAllTilingsNonIdeal();

  // If a tiling exists whose scale is within |snap_to_existing_tiling_ratio|
  // ratio of |start_scale|, then return that tiling's scale. Otherwise, return
  // |start_scale|. If multiple tilings match the criteria, return the one with
  // the least ratio to |start_scale|.
  float GetSnappedContentsScale(float start_scale,
                                float snap_to_existing_tiling_ratio) const;

  // Returns the maximum contents scale of all tilings, or 0 if no tilings
  // exist.
  float GetMaximumContentsScale() const;

  // Removes all tilings with a contents scale < |minimum_scale|.
  void RemoveTilingsBelowScale(float minimum_scale);

  // Removes all tilings with a contents scale > |maximum_scale|.
  void RemoveTilingsAboveScale(float maximum_scale);

  // Remove all tilings.
  void RemoveAllTilings();

  // Remove all tiles; keep all tilings.
  void RemoveAllTiles();

  // Update the rects and priorities for tiles based on the given information.
  // Returns true if PrepareTiles is required.
  bool UpdateTilePriorities(const gfx::Rect& required_rect_in_layer_space,
                            float ideal_contents_scale,
                            double current_frame_time_in_seconds,
                            const Occlusion& occlusion_in_layer_space,
                            bool can_require_tiles_for_activation);

  void GetAllPrioritizedTilesForTracing(
      std::vector<PrioritizedTile>* prioritized_tiles) const;

  // For a given rect, iterates through tiles that can fill it.  If no
  // set of tiles with resources can fill the rect, then it will iterate
  // through null tiles with valid geometry_rect() until the rect is full.
  // If all tiles have resources, the union of all geometry_rects will
  // exactly fill rect with no overlap.
  class CC_EXPORT CoverageIterator {
   public:
    CoverageIterator(const PictureLayerTilingSet* set,
      float contents_scale,
      const gfx::Rect& content_rect,
      float ideal_contents_scale);
    ~CoverageIterator();

    // Visible rect (no borders), always in the space of rect,
    // regardless of the relative contents scale of the tiling.
    gfx::Rect geometry_rect() const;
    // Texture rect (in texels) for geometry_rect
    gfx::RectF texture_rect() const;

    Tile* operator->() const;
    Tile* operator*() const;

    CoverageIterator& operator++();
    operator bool() const;

    TileResolution resolution() const;
    PictureLayerTiling* CurrentTiling() const;

   private:
    size_t NextTiling() const;

    const PictureLayerTilingSet* set_;
    float contents_scale_;
    float ideal_contents_scale_;
    PictureLayerTiling::CoverageIterator tiling_iter_;
    size_t current_tiling_;
    size_t ideal_tiling_;

    Region current_region_;
    Region missing_region_;
    Region::Iterator region_iter_;
  };

  void AsValueInto(base::trace_event::TracedValue* array) const;
  size_t GPUMemoryUsageInBytes() const;

  TilingRange GetTilingRange(TilingRangeType type) const;

 protected:
  struct FrameVisibleRect {
    FrameVisibleRect(const gfx::Rect& rect, double time_in_seconds)
        : visible_rect_in_layer_space(rect),
          frame_time_in_seconds(time_in_seconds) {}

    gfx::Rect visible_rect_in_layer_space;
    double frame_time_in_seconds;
  };

  struct StateSinceLastTilePriorityUpdate {
    class AutoClear {
     public:
      explicit AutoClear(StateSinceLastTilePriorityUpdate* state_to_clear)
          : state_to_clear_(state_to_clear) {}
      ~AutoClear() { *state_to_clear_ = StateSinceLastTilePriorityUpdate(); }

     private:
      StateSinceLastTilePriorityUpdate* state_to_clear_;
    };

    StateSinceLastTilePriorityUpdate()
        : invalidated(false), added_tilings(false) {}

    bool invalidated;
    bool added_tilings;
  };

  explicit PictureLayerTilingSet(
      WhichTree tree,
      PictureLayerTilingClient* client,
      int tiling_interest_area_padding,
      float skewport_target_time_in_seconds,
      int skewport_extrapolation_limit_in_screen_pixels,
      float max_preraster_distance);

  void CopyTilingsAndPropertiesFromPendingTwin(
      const PictureLayerTilingSet* pending_twin_set,
      scoped_refptr<RasterSource> raster_source,
      const Region& layer_invalidation);

  // Remove one tiling.
  void Remove(PictureLayerTiling* tiling);
  void VerifyTilings(const PictureLayerTilingSet* pending_twin_set) const;

  bool TilingsNeedUpdate(const gfx::Rect& required_rect_in_layer_space,
                         double current_frame_time_in_Seconds);
  gfx::Rect ComputeSkewport(const gfx::Rect& visible_rect_in_layer_space,
                            double current_frame_time_in_seconds,
                            float ideal_contents_scale);
  gfx::Rect ComputeSoonBorderRect(const gfx::Rect& visible_rect_in_layer_space,
                                  float ideal_contents_scale);
  void UpdatePriorityRects(const gfx::Rect& visible_rect_in_layer_space,
                           double current_frame_time_in_seconds,
                           float ideal_contents_scale);

  std::vector<std::unique_ptr<PictureLayerTiling>> tilings_;

  const int tiling_interest_area_padding_;
  const float skewport_target_time_in_seconds_;
  const int skewport_extrapolation_limit_in_screen_pixels_;
  WhichTree tree_;
  PictureLayerTilingClient* client_;
  const float max_preraster_distance_;
  // State saved for computing velocities based on finite differences.
  // .front() of the list refers to the most recent FrameVisibleRect.
  std::list<FrameVisibleRect> visible_rect_history_;
  StateSinceLastTilePriorityUpdate state_since_last_tile_priority_update_;

  scoped_refptr<RasterSource> raster_source_;

  gfx::Rect visible_rect_in_layer_space_;
  gfx::Rect skewport_in_layer_space_;
  gfx::Rect soon_border_rect_in_layer_space_;
  gfx::Rect eventually_rect_in_layer_space_;

  friend class Iterator;

 private:
  DISALLOW_COPY_AND_ASSIGN(PictureLayerTilingSet);
};

}  // namespace cc

#endif  // CC_TILES_PICTURE_LAYER_TILING_SET_H_
