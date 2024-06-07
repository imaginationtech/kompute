// SPDX-License-Identifier: Apache-2.0

#include "kompute/Image.hpp"

#include "kompute/operations/OpImageSyncLocal.hpp"

namespace kp {

OpImageSyncLocal::OpImageSyncLocal(
  const std::vector<std::shared_ptr<Memory>>& images)
{
    KP_LOG_DEBUG("Kompute OpImageSyncLocal constructor with params");

    if (images.size() < 1) {
        throw std::runtime_error(
          "Kompute OpImageSyncLocal called with less than 1 image");
    }

    for (std::shared_ptr<Memory> image : images) {
        if (std::dynamic_pointer_cast<Image>(image) == nullptr)
        {
            throw std::runtime_error("Kompute OpImageSyncLocal: Memory object is not an Image");
        }
        this->mImages.push_back(std::dynamic_pointer_cast<Image>(image));
    }
}

OpImageSyncLocal::~OpImageSyncLocal()
{
    KP_LOG_DEBUG("Kompute OpImageSyncLocal destructor started");
}

void
OpImageSyncLocal::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpImageSyncLocal record called");

    for (size_t i = 0; i < this->mImages.size(); i++) {
        if (this->mImages[i]->memoryType() == Memory::MemoryTypes::eDevice) {

            this->mImages[i]->recordPrimaryMemoryBarrier(
              commandBuffer,
              vk::AccessFlagBits::eShaderWrite,
              vk::AccessFlagBits::eTransferRead,
              vk::PipelineStageFlagBits::eComputeShader,
              vk::PipelineStageFlagBits::eTransfer);

            this->mImages[i]->recordCopyFromDeviceToStaging(commandBuffer);

            this->mImages[i]->recordPrimaryMemoryBarrier(
              commandBuffer,
              vk::AccessFlagBits::eTransferWrite,
              vk::AccessFlagBits::eHostRead,
              vk::PipelineStageFlagBits::eTransfer,
              vk::PipelineStageFlagBits::eHost);
        }
    }
}

void
OpImageSyncLocal::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageSyncLocal preEval called");
}

void
OpImageSyncLocal::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageSyncLocal postEval called");

    KP_LOG_DEBUG("Kompute OpImageSyncLocal mapping data into image local");
}

}
