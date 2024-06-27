// SPDX-License-Identifier: Apache-2.0

#include "kompute/operations/OpImageCopyToTensor.hpp"
#include "kompute/Tensor.hpp"

namespace kp {

OpImageCopyToTensor::OpImageCopyToTensor(const std::vector<std::shared_ptr<Memory>>& mem_objects)
{
    KP_LOG_DEBUG("Kompute OpImageCopyToTensor constructor with params");

    if (mem_objects.size() < 2) {
        throw std::runtime_error(
          "Kompute OpImageCopyToTensor called with less than 2 mem objects");
    }

    if (std::dynamic_pointer_cast<Image>(mem_objects.at(0)) == nullptr)
    {
        throw std::runtime_error("Kompute OpImageCopyToTensor: Memory object is not an Image");
    }
    this->mImage =  std::dynamic_pointer_cast<Image>(mem_objects.at(0));

    for (size_t i = 1; i < mem_objects.size(); i++)
    {
        if (std::dynamic_pointer_cast<Tensor>(mem_objects.at(i)) == nullptr)
        {
            throw std::runtime_error("Kompute OpImageCopyToTensor: Memory object is not a Tensor");
        }
        this->mTensors.push_back(std::dynamic_pointer_cast<Tensor>(mem_objects.at(i)));
    }

    kp::Image::ImageDataTypes dataType = this->mImage->dataType();
    uint32_t size = this->mImage->size();
    for (const std::shared_ptr<Tensor>& tensor : this->mTensors) {
        if (Image::getTensorDataType(dataType) != tensor->dataType()) {
            throw std::runtime_error(fmt::format(
              "Attempting to copy tensors of different types from {} to {}",
              Image::toString(dataType),
              Tensor::toString(tensor->dataType())));
        }
        if (tensor->size() != size) {
            throw std::runtime_error(fmt::format(
              "Attempting to copy tensors of different sizes from {} to {}",
              size,
              tensor->size()));
        }
    }
}

OpImageCopyToTensor::~OpImageCopyToTensor()
{
    KP_LOG_DEBUG("Kompute OpImageCopyToTensor destructor started");
}

void
OpImageCopyToTensor::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpImageCopyToTensor record called");

    for (size_t i = 0; i < this->mTensors.size(); i++) {
        this->mTensors[i]->recordCopyFrom(commandBuffer, this->mImage);
    }
}

void
OpImageCopyToTensor::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageCopyToTensor preEval called");
}

void
OpImageCopyToTensor::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpImageCopyToTensor postEval called");

    // Do not copy on CPU side if source is storage tensor
    if (this->mImage->memoryType() == kp::Memory::MemoryTypes::eStorage) {
        KP_LOG_DEBUG("Kompute OpImageCopyToTensor not copying tensor source given "
                     "it's of eStorage type");
        return;
    }
    void* data = this->mImage->rawData();

    // Copy the data from the tensor into all the images
    for (size_t i = 0; i < this->mTensors.size(); i++) {
        if (this->mTensors[i]->memoryType() ==
            kp::Memory::MemoryTypes::eStorage) {
            KP_LOG_DEBUG("Kompute OpImageCopyToTensor not copying to tensor dest "
                         "given it's of eStorage type");
            continue;
        }
        this->mTensors[i]->setRawData(data);
    }
}

}
