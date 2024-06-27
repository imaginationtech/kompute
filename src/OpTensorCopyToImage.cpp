// SPDX-License-Identifier: Apache-2.0

#include "kompute/operations/OpTensorCopyToImage.hpp"
#include "kompute/Tensor.hpp"

namespace kp {

OpTensorCopyToImage::OpTensorCopyToImage(
  const std::vector<std::shared_ptr<Memory>>& mem_objects)
{
    KP_LOG_DEBUG("Kompute OpTensorCopyToImage constructor with params");

    if (mem_objects.size() < 2) {
        throw std::runtime_error(
          "Kompute OpTensorCopyToImage called with less than 2 mem objects");
    }

    if (std::dynamic_pointer_cast<Tensor>(mem_objects.at(0)) == nullptr) {
        throw std::runtime_error(
          "Kompute OpTensorCopyToImage: Memory object is not a Tensor");
    }
    this->mTensor = std::dynamic_pointer_cast<Tensor>(mem_objects.at(0));

    for (size_t i = 1; i < mem_objects.size(); i++) {
        if (std::dynamic_pointer_cast<Image>(mem_objects.at(i)) == nullptr) {
            throw std::runtime_error(
              "Kompute OpTensorCopyToImage: Memory object is not an Image");
        }
        this->mImages.push_back(
          std::dynamic_pointer_cast<Image>(mem_objects.at(i)));
    }

    kp::Tensor::TensorDataTypes dataType = this->mTensor->dataType();
    uint32_t size = this->mTensor->size();
    for (const std::shared_ptr<Image>& image : this->mImages) {
        if (Image::getTensorDataType(image->dataType()) != dataType) {
            throw std::runtime_error(fmt::format(
              "Attempting to copy tensors of different types from {} to {}",
              Tensor::toString(dataType),
              Image::toString(image->dataType())));
        }
        if (image->size() != size) {
            throw std::runtime_error(fmt::format(
              "Attempting to copy tensors of different sizes from {} to {}",
              size,
              image->size()));
        }
    }
}

OpTensorCopyToImage::~OpTensorCopyToImage()
{
    KP_LOG_DEBUG("Kompute OpTensorCopyToImage destructor started");
}

void
OpTensorCopyToImage::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpTensorCopyToImage record called");

    for (size_t i = 0; i < this->mImages.size(); i++) {
        this->mImages[i]->recordCopyFrom(commandBuffer, this->mTensor);
    }
}

void
OpTensorCopyToImage::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpTensorCopyToImage preEval called");
}

void
OpTensorCopyToImage::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpTensorCopyToImage postEval called");

    // Do not copy on CPU side if source is storage tensor
    if (this->mTensor->memoryType() == kp::Memory::MemoryTypes::eStorage) {
        KP_LOG_DEBUG(
          "Kompute OpTensorCopyToImage not copying tensor source given "
          "it's of eStorage type");
        return;
    }
    void* data = this->mTensor->rawData();

    // Copy the data from the tensor into all the images
    for (size_t i = 0; i < this->mImages.size(); i++) {
        if (this->mImages[i]->memoryType() ==
            kp::Memory::MemoryTypes::eStorage) {
            KP_LOG_DEBUG(
              "Kompute OpTensorCopyToImage not copying to tensor dest "
              "given it's of eStorage type");
            continue;
        }
        this->mImages[i]->setRawData(data);
    }
}

}
