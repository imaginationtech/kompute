// SPDX-License-Identifier: Apache-2.0

#include "kompute/operations/OpImageSyncDevice.hpp"

namespace kp {

OpImageSyncDevice::OpImageSyncDevice(
  const std::vector<std::shared_ptr<Memory>>& images)
{
    KP_LOG_DEBUG("Kompute OpImageSyncDevice constructor with params");

    if (images.size() < 1) {
        throw std::runtime_error(
          "Kompute OpImageSyncDevice called with less than 1 image");
    }

    for (std::shared_ptr<Memory> image : images) {
        if (std::dynamic_pointer_cast<Image>(image) == nullptr)
        {
            throw std::runtime_error("Kompute OpImageSyncDevice: Memory object is not an Image");
        }
        this->mImages.push_back(std::dynamic_pointer_cast<Image>(image));
    }
}

OpImageSyncDevice::~OpImageSyncDevice()
{
    KP_LOG_DEBUG("Kompute OpImageSyncDevice destructor started");

    this->mImages.clear();
}

void
OpImageSyncDevice::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpImageSyncDevice record called");

    for (size_t i = 0; i < this->mImages.size(); i++) {
        if (this->mImages[i]->memoryType() == Memory::MemoryTypes::eDevice) {
            this->mImages[i]->recordCopyFromStagingToDevice(commandBuffer);
        }
    }
}

void
OpImageSyncDevice::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageSyncDevice preEval called");
}

void
OpImageSyncDevice::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageSyncDevice postEval called");
}

}
