// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BROWSING_DATA_CONTENT_CONDITIONAL_CACHE_DELETION_HELPER_H_
#define COMPONENTS_BROWSING_DATA_CONTENT_CONDITIONAL_CACHE_DELETION_HELPER_H_

#include <memory>

#include "base/callback_forward.h"
#include "base/message_loop/message_loop.h"
#include "net/base/completion_callback.h"
#include "net/base/net_errors.h"
#include "net/disk_cache/disk_cache.h"
#include "url/gurl.h"

namespace disk_cache {
class Entry;
}

namespace browsing_data {

// Helper to remove http cache data from a StoragePartition.
class ConditionalCacheDeletionHelper {
 public:
  // Creates a helper to delete |cache| entries that match the |condition|.
  // Must be created on the IO thread!
  ConditionalCacheDeletionHelper(
      disk_cache::Backend* cache,
      const base::Callback<bool(const disk_cache::Entry*)>& condition);

  // A convenience method to create a condition matching cache entries whose
  // last modified time is between |begin_time| (inclusively), |end_time|
  // (exclusively) and whose URL is matched by the |url_predicate|. Note that
  // |begin_time| and |end_time| can be null to indicate unbounded time interval
  // in their respective direction.
  static base::Callback<bool(const disk_cache::Entry*)>
  CreateURLAndTimeCondition(
      const base::Callback<bool(const GURL&)>& url_predicate,
      const base::Time& begin_time,
      const base::Time& end_time);

  // Deletes the cache entries according to the specified condition. Destroys
  // this instance of ConditionalCacheDeletionHelper when finished.
  // Must be called on the IO thread!
  //
  // The return value is a net error code. If this method returns
  // ERR_IO_PENDING, the |completion_callback| will be invoked when the
  // operation completes.
  int DeleteAndDestroySelfWhenFinished(
      const net::CompletionCallback& completion_callback);

 private:
  friend class base::DeleteHelper<ConditionalCacheDeletionHelper>;
  ~ConditionalCacheDeletionHelper();

  void IterateOverEntries(int error);

  disk_cache::Backend* cache_;
  const base::Callback<bool(const disk_cache::Entry*)> condition_;

  net::CompletionCallback completion_callback_;

  std::unique_ptr<disk_cache::Backend::Iterator> iterator_;
  disk_cache::Entry* current_entry_;
  disk_cache::Entry* previous_entry_;

  DISALLOW_COPY_AND_ASSIGN(ConditionalCacheDeletionHelper);
};

}  // namespace browsing_data

#endif  // COMPONENTS_BROWSING_DATA_CONTENT_CONDITIONAL_CACHE_DELETION_HELPER_H_
