/*
 * Copyright (C) 2023 The Android Open Source Project
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

#ifndef INCLUDE_PERFETTO_PUBLIC_PROTOS_CONFIG_TRACK_EVENT_TRACK_EVENT_CONFIG_PZC_H_
#define INCLUDE_PERFETTO_PUBLIC_PROTOS_CONFIG_TRACK_EVENT_TRACK_EVENT_CONFIG_PZC_H_

#include <stdbool.h>
#include <stdint.h>

#include "perfetto/public/pb_macros.h"

PERFETTO_PB_MSG(perfetto_protos_TrackEventConfig);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  STRING,
                  const char*,
                  disabled_categories,
                  1);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  STRING,
                  const char*,
                  enabled_categories,
                  2);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  STRING,
                  const char*,
                  disabled_tags,
                  3);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  STRING,
                  const char*,
                  enabled_tags,
                  4);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  VARINT,
                  bool,
                  disable_incremental_timestamps,
                  5);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  VARINT,
                  uint64_t,
                  timestamp_unit_multiplier,
                  6);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  VARINT,
                  bool,
                  filter_debug_annotations,
                  7);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  VARINT,
                  bool,
                  enable_thread_time_sampling,
                  8);
PERFETTO_PB_FIELD(perfetto_protos_TrackEventConfig,
                  VARINT,
                  bool,
                  filter_dynamic_event_names,
                  9);

#endif  // INCLUDE_PERFETTO_PUBLIC_PROTOS_CONFIG_TRACK_EVENT_TRACK_EVENT_CONFIG_PZC_H_
