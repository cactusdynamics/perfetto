/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_TRACE_PROCESSOR_HEAP_PROFILE_TRACKER_H_
#define SRC_TRACE_PROCESSOR_HEAP_PROFILE_TRACKER_H_

#include <deque>
#include <unordered_map>

#include "perfetto/ext/base/optional.h"

#include "protos/perfetto/trace/profiling/profile_common.pbzero.h"
#include "protos/perfetto/trace/profiling/profile_packet.pbzero.h"
#include "src/trace_processor/stack_profile_tracker.h"
#include "src/trace_processor/trace_storage.h"

namespace perfetto {
namespace trace_processor {

class TraceProcessorContext;

class HeapProfileTracker {
 public:
  struct SourceAllocation {
    uint64_t pid = 0;
    // This is int64_t, because we get this from the TraceSorter which also
    // converts this for us.
    int64_t timestamp = 0;
    StackProfileTracker::SourceCallstackId callstack_id = 0;
    uint64_t self_allocated = 0;
    uint64_t self_freed = 0;
    uint64_t alloc_count = 0;
    uint64_t free_count = 0;
  };

  void SetProfilePacketIndex(uint32_t seq_id, uint64_t id);

  explicit HeapProfileTracker(TraceProcessorContext* context);

  void StoreAllocation(uint32_t seq_id, SourceAllocation);

  // Call after the last profile packet of a dump to commit the allocations
  // that had been stored using StoreAllocation and clear internal indices
  // for that dump.
  void FinalizeProfile(uint32_t seq_id,
                       StackProfileTracker* stack_profile_tracker,
                       const StackProfileTracker::InternLookup* lookup);

  // Only commit the allocations that had been stored using StoreAllocations.
  // This is only needed in tests, use FinalizeProfile instead.
  void CommitAllocations(uint32_t seq_id,
                         StackProfileTracker* stack_profile_tracker,
                         const StackProfileTracker::InternLookup* lookup);

  ~HeapProfileTracker();

 private:
  void AddAllocation(
      uint32_t seq_id,
      StackProfileTracker* stack_profile_tracker,
      const SourceAllocation&,
      const StackProfileTracker::InternLookup* intern_lookup = nullptr);

  struct SequenceState {
    std::vector<SourceAllocation> pending_allocs;

    std::unordered_map<std::pair<UniquePid, CallsiteId>,
                       tables::HeapProfileAllocationTable::Row>
        prev_alloc;
    std::unordered_map<std::pair<UniquePid, CallsiteId>,
                       tables::HeapProfileAllocationTable::Row>
        prev_free;

    uint64_t last_profile_packet_index = 0;
  };
  std::map<uint32_t, SequenceState> sequence_state_;
  TraceProcessorContext* const context_;
  const StringId empty_;
};

}  // namespace trace_processor
}  // namespace perfetto

#endif  // SRC_TRACE_PROCESSOR_HEAP_PROFILE_TRACKER_H_
