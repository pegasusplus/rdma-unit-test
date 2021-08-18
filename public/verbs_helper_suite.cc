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
#include "public/verbs_helper_suite.h"

#include <cstdint>
#include <memory>

#include "glog/logging.h"
#include "absl/flags/flag.h"
#include "absl/status/status.h"
#include "infiniband/verbs.h"
#include "impl/roce_allocator.h"
#include "impl/roce_backend.h"
#include "impl/verbs_allocator.h"
#include "public/flags.h"
#include "public/util.h"

namespace rdma_unit_test {

VerbsHelperSuite::VerbsHelperSuite() {
  backend_ = std::make_unique<RoceBackend>();
  CHECK(backend_);  // Crash ok
  allocator_ = std::make_unique<RoceAllocator>();
  CHECK(allocator_);  // Crash ok
}

absl::Status VerbsHelperSuite::SetUpRcQp(ibv_qp* local_qp,
                                         const verbs_util::PortGid& local,
                                         ibv_gid remote_gid,
                                         uint32_t remote_qpn) {
  return backend_->SetUpRcQp(local_qp, local, remote_gid, remote_qpn);
}

void VerbsHelperSuite::SetUpSelfConnectedRcQp(
    ibv_qp* qp, const verbs_util::PortGid& local) {
  backend_->SetUpSelfConnectedRcQp(qp, local);
}

void VerbsHelperSuite::SetUpLoopbackRcQps(ibv_qp* qp1, ibv_qp* qp2,
                                          const verbs_util::PortGid& local) {
  backend_->SetUpLoopbackRcQps(qp1, qp2, local);
}

absl::Status VerbsHelperSuite::SetUpUdQp(ibv_qp* qp,
                                         const verbs_util::PortGid& local,
                                         uint32_t qkey) {
  return backend_->SetUpUdQp(qp, local, qkey);
}

absl::Status VerbsHelperSuite::SetQpInit(ibv_qp* qp, uint8_t port) {
  return backend_->SetQpInit(qp, port);
}

absl::Status VerbsHelperSuite::SetQpRtr(ibv_qp* qp,
                                        const verbs_util::PortGid& local,
                                        ibv_gid remote_gid,
                                        uint32_t remote_qpn) {
  return backend_->SetQpRtr(qp, local, remote_gid, remote_qpn);
}

absl::Status VerbsHelperSuite::SetQpRts(ibv_qp* qp) {
  return backend_->SetQpRts(qp);
}

absl::Status VerbsHelperSuite::SetQpRts(ibv_qp* qp, ibv_qp_attr custom_attr,
                                        int mask) {
  return backend_->SetQpRts(qp, custom_attr, mask);
}

absl::Status VerbsHelperSuite::SetQpError(ibv_qp* qp) {
  return backend_->SetQpError(qp);
}

RdmaMemBlock VerbsHelperSuite::AllocBuffer(int pages,
                                           bool requires_shared_memory) {
  return allocator_->AllocBuffer(pages, requires_shared_memory);
}

RdmaMemBlock VerbsHelperSuite::AllocAlignedBuffer(int pages, size_t alignment) {
  return allocator_->AllocAlignedBuffer(pages, alignment);
}

RdmaMemBlock VerbsHelperSuite::AllocBufferByBytes(size_t bytes,
                                                  size_t alignment) {
  return allocator_->AllocBufferByBytes(bytes, alignment);
}

absl::StatusOr<ibv_context*> VerbsHelperSuite::OpenDevice(
    bool no_ipv6_for_gid) {
  return allocator_->OpenDevice(no_ipv6_for_gid);
}

ibv_ah* VerbsHelperSuite::CreateAh(ibv_pd* pd, ibv_gid remote_gid) {
  return allocator_->CreateAh(pd, remote_gid);
}

int VerbsHelperSuite::DestroyAh(ibv_ah* ah) {
  return allocator_->DestroyAh(ah);
}

ibv_pd* VerbsHelperSuite::AllocPd(ibv_context* context) {
  return allocator_->AllocPd(context);
}

int VerbsHelperSuite::DeallocPd(ibv_pd* pd) {
  return allocator_->DeallocPd(pd);
}

ibv_mr* VerbsHelperSuite::RegMr(ibv_pd* pd, const RdmaMemBlock& memblock,
                                int access) {
  return allocator_->RegMr(pd, memblock, access);
}

int VerbsHelperSuite::DeregMr(ibv_mr* mr) { return allocator_->DeregMr(mr); }

ibv_mw* VerbsHelperSuite::AllocMw(ibv_pd* pd, ibv_mw_type type) {
  return allocator_->AllocMw(pd, type);
}

int VerbsHelperSuite::DeallocMw(ibv_mw* mw) {
  return allocator_->DeallocMw(mw);
}

ibv_comp_channel* VerbsHelperSuite::CreateChannel(ibv_context* context) {
  return allocator_->CreateChannel(context);
}

int VerbsHelperSuite::DestroyChannel(ibv_comp_channel* channel) {
  return allocator_->DestroyChannel(channel);
}

ibv_cq* VerbsHelperSuite::CreateCq(ibv_context* context, int max_wr,
                                   ibv_comp_channel* channel) {
  return allocator_->CreateCq(context, max_wr, channel);
}

int VerbsHelperSuite::DestroyCq(ibv_cq* cq) {
  return allocator_->DestroyCq(cq);
}

ibv_srq* VerbsHelperSuite::CreateSrq(ibv_pd* pd, uint32_t max_wr) {
  return allocator_->CreateSrq(pd, max_wr);
}

int VerbsHelperSuite::DestroySrq(ibv_srq* srq) {
  return allocator_->DestroySrq(srq);
}

ibv_srq* VerbsHelperSuite::CreateSrq(ibv_pd* pd, ibv_srq_init_attr& attr) {
  return allocator_->CreateSrq(pd, attr);
}

ibv_qp* VerbsHelperSuite::CreateQp(ibv_pd* pd, ibv_cq* cq) {
  return allocator_->CreateQp(pd, cq);
}

ibv_qp* VerbsHelperSuite::CreateQp(ibv_pd* pd, ibv_cq* cq, ibv_srq* srq) {
  return allocator_->CreateQp(pd, cq, srq);
}

ibv_qp* VerbsHelperSuite::CreateQp(ibv_pd* pd, ibv_cq* send_cq, ibv_cq* recv_cq,
                                   ibv_srq* srq, uint32_t max_send_wr,
                                   uint32_t max_recv_wr, ibv_qp_type qp_type,
                                   int sig_all) {
  return allocator_->CreateQp(pd, send_cq, recv_cq, srq, max_send_wr,
                              max_recv_wr, qp_type, sig_all);
}

ibv_qp* VerbsHelperSuite::CreateQp(ibv_pd* pd, ibv_qp_init_attr& basic_attr) {
  return allocator_->CreateQp(pd, basic_attr);
}

int VerbsHelperSuite::DestroyQp(ibv_qp* qp) {
  return allocator_->DestroyQp(qp);
}

verbs_util::PortGid VerbsHelperSuite::GetLocalPortGid(
    ibv_context* context) const {
  return allocator_->GetLocalPortGid(context);
}

}  // namespace rdma_unit_test
