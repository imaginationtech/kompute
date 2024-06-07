// SPDX-License-Identifier: Apache-2.0

#include "kompute/Tensor.hpp"

#include "kompute/operations/OpTensorSyncLocal.hpp"

namespace kp {

OpTensorSyncLocal::OpTensorSyncLocal(
  const std::vector<std::shared_ptr<Memory>>& tensors)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncLocal constructor with params");

    if (tensors.size() < 1) {
        throw std::runtime_error(
          "Kompute OpTensorSyncLocal called with less than 1 tensor");
    }

    for (std::shared_ptr<Memory> tensor : tensors) {
        if (std::dynamic_pointer_cast<Tensor>(tensor) == nullptr)
        {
            throw std::runtime_error("Kompute OpTensorCopy: Memory object is not a Tensor");
        }
        this->mTensors.push_back(std::dynamic_pointer_cast<Tensor>(tensor));
    }
}

OpTensorSyncLocal::~OpTensorSyncLocal()
{
    KP_LOG_DEBUG("Kompute OpTensorSyncLocal destructor started");
}

void
OpTensorSyncLocal::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncLocal record called");

    for (size_t i = 0; i < this->mTensors.size(); i++) {
        if (this->mTensors[i]->memoryType() == Memory::MemoryTypes::eDevice) {

            this->mTensors[i]->recordPrimaryMemoryBarrier(
              commandBuffer,
              vk::AccessFlagBits::eShaderWrite,
              // FIXME: eTransferRead is not supported for the compute pipeline
              vk::AccessFlagBits::eTransferRead,
              vk::PipelineStageFlagBits::eComputeShader,
              vk::PipelineStageFlagBits::eTransfer);

            this->mTensors[i]->recordCopyFromDeviceToStaging(commandBuffer);

            this->mTensors[i]->recordPrimaryMemoryBarrier(
              commandBuffer,
              // FIXME: eTransferRead is not supported for the compute pipeline
              vk::AccessFlagBits::eTransferWrite,
              vk::AccessFlagBits::eHostRead,
              vk::PipelineStageFlagBits::eTransfer,
              vk::PipelineStageFlagBits::eHost);
        }
    }
}

void
OpTensorSyncLocal::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncLocal preEval called");
}

void
OpTensorSyncLocal::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncLocal postEval called");

    KP_LOG_DEBUG("Kompute OpTensorSyncLocal mapping data into tensor local");
}

}
