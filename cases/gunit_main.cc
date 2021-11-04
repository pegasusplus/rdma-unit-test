// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Initialize absl::Flags before initializing/running unit tests.

#include <cstdint>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "absl/debugging/failure_signal_handler.h"
#include "absl/flags/parse.h"
#include "internal/introspection_mlx4.h"
#include "internal/introspection_mlx5.h"
#include "internal/introspection_rxe.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  google::InitGoogleLogging(argv[0]);
  absl::FailureSignalHandlerOptions options;
  absl::InstallFailureSignalHandler(options);
  // gtest/glog use gflags which conflict with absl::flags. Therefore we
  // cannot call gflags::ParseCommandLineFlags(&argc, &argv, true).
  // There is work underway for opensource logging in absl.
  absl::ParseCommandLine(argc, argv);

  // Register supported NIC's
  rdma_unit_test::IntrospectionMlx4::Register();
  rdma_unit_test::IntrospectionMlx5::Register();
  rdma_unit_test::IntrospectionRxe::Register();

  return RUN_ALL_TESTS();
}
