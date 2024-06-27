// SPDX-License-Identifier: Apache-2.0

#include "kompute/operations/OpImageCopy.hpp"
#include "kompute/Image.hpp"

namespace kp {

OpImageCopy::OpImageCopy(const std::vector<std::shared_ptr<Memory>>& images)
{
    KP_LOG_DEBUG("Kompute OpImageCopy constructor with params");

    if (images.size() < 2) {
        throw std::runtime_error(
          "Kompute OpImageCopy called with less than 2 images");
    }

    for (std::shared_ptr<Memory> image : images) {
        if (std::dynamic_pointer_cast<Image>(image) == nullptr) {
            throw std::runtime_error(
              "Kompute OpImageCopy: Memory object is not an Image");
        }
        this->mImages.push_back(std::dynamic_pointer_cast<Image>(image));
    }

    kp::Image::ImageDataTypes dataType = this->mImages[0]->dataType();
    uint32_t size = this->mImages[0]->size();
    for (const std::shared_ptr<Image>& image : this->mImages) {
        if (image->dataType() != dataType) {
            throw std::runtime_error(fmt::format(
              "Attempting to copy images of different types from {} to {}",
              Image::toString(dataType),
              Image::toString(image->dataType())));
        }
        if (image->size() != size) {
            throw std::runtime_error(fmt::format(
              "Attempting to copy images of different sizes from {} to {}",
              size,
              image->size()));
        }
    }
}

OpImageCopy::~OpImageCopy()
{
    KP_LOG_DEBUG("Kompute OpImageCopy destructor started");
}

void
OpImageCopy::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpImageCopy record called");

    // We iterate from the second image onwards and record a copy to all
    for (size_t i = 1; i < this->mImages.size(); i++) {
        this->mImages[i]->recordCopyFrom(commandBuffer, this->mImages[0]);
    }
}

void
OpImageCopy::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageCopy preEval called");
}

void
OpImageCopy::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageCopy postEval called");

    // Do not copy on CPU side if source is storage image
    if (this->mImages[0]->memoryType() == Memory::MemoryTypes::eStorage) {
        KP_LOG_DEBUG("Kompute OpImageCopy not copying image source given "
                     "it's of eStorage type");
        return;
    }
    void* data = this->mImages[0]->rawData();

    // Copy the data from the first image into all the images
    for (size_t i = 1; i < this->mImages.size(); i++) {
        if (this->mImages[i]->memoryType() == Memory::MemoryTypes::eStorage) {
            KP_LOG_DEBUG("Kompute OpImageCopy not copying to image dest "
                         "given it's of eStorage type");
            continue;
        }
        this->mImages[i]->setRawData(data);
    }
}

}
