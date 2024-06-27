// SPDX-License-Identifier: Apache-2.0

#include "kompute/Tensor.hpp"
#include "kompute/Image.hpp"

namespace kp {

std::string
Tensor::toString(Tensor::TensorDataTypes dt)
{
    switch (dt) {
        case TensorDataTypes::eBool:
            return "eBool";
        case TensorDataTypes::eInt:
            return "eInt";
        case TensorDataTypes::eUnsignedInt:
            return "eUnsignedInt";
        case TensorDataTypes::eFloat:
            return "eFloat";
        case TensorDataTypes::eDouble:
            return "eDouble";
        default:
            return "unknown";
    }
}

Tensor::Tensor(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
               std::shared_ptr<vk::Device> device,
               void* data,
               uint32_t elementTotalCount,
               uint32_t elementMemorySize,
               const TensorDataTypes& dataType,
               const MemoryTypes& memoryType)
{
    KP_LOG_DEBUG("Kompute Tensor constructor data length: {}, and type: {}",
                 elementTotalCount,
                 Memory::toString(memoryType));

    if (memoryType == MemoryTypes::eStorage && data != nullptr)
    {
        KP_LOG_WARN("Kompute Tensor of type eStorage do not need to be initialised with data");
    }

    this->mPhysicalDevice = physicalDevice;
    this->mDevice = device;
    this->mDataType = dataType;
    this->mMemoryType = memoryType;
    this->mDescriptorType = vk::DescriptorType::eStorageBuffer;

    this->rebuild(data, elementTotalCount, elementMemorySize);
}

Tensor::~Tensor()
{
    KP_LOG_DEBUG("Kompute Tensor destructor started. Type: {}",
                 Memory::toString(this->memoryType()));

    if (this->mDevice) {
        this->destroy();
    }

    KP_LOG_DEBUG("Kompute Tensor destructor success");
}

void
Tensor::rebuild(void* data,
                uint32_t elementTotalCount,
                uint32_t elementMemorySize)
{
    KP_LOG_DEBUG("Kompute Tensor rebuilding with size {}", elementTotalCount);

    this->mSize = elementTotalCount;
    this->mDataTypeMemorySize = elementMemorySize;

    if (this->mPrimaryBuffer || this->mPrimaryMemory) {
        KP_LOG_DEBUG(
          "Kompute Tensor destroying existing resources before rebuild");
        this->destroy();
    }

    this->allocateMemoryCreateGPUResources();

    if (this->memoryType() != Memory::MemoryTypes::eStorage &&
        data != nullptr) {
        this->mapRawData();
        memcpy(this->mRawData, data, this->memorySize());
    }
}

bool
Tensor::isInit()
{
    return this->mDevice && this->mPrimaryBuffer && this->mPrimaryMemory;
}

kp::Tensor::TensorDataTypes
Tensor::dataType()
{
    return this->mDataType;
}

void
Tensor::recordCopyFrom(const vk::CommandBuffer& commandBuffer,
                       std::shared_ptr<Tensor> copyFromTensor)
{

    vk::DeviceSize bufferSize(this->memorySize());
    vk::BufferCopy copyRegion(0, 0, bufferSize);

    KP_LOG_DEBUG("Kompute Tensor recordCopyFrom data size {}.", bufferSize);

    this->recordCopyBuffer(commandBuffer,
                           copyFromTensor->mPrimaryBuffer,
                           this->mPrimaryBuffer,
                           bufferSize,
                           copyRegion);
}

void
Tensor::recordCopyFrom(const vk::CommandBuffer& commandBuffer,
                       std::shared_ptr<Image> copyFromImage)
{

    vk::DeviceSize bufferSize(this->memorySize());

    vk::ImageSubresourceLayers layer = {};
    layer.aspectMask = vk::ImageAspectFlagBits::eColor;
    layer.layerCount = 1;
    vk::Offset3D offset = { 0, 0, 0 };

    // FIXME: Check the size of the dest and source images match
    vk::Extent3D size = { copyFromImage->getWidth(), copyFromImage->getHeight(), 1 };

    vk::BufferImageCopy copyRegion(0, 0, 0, layer, offset, size);

    KP_LOG_DEBUG("Kompute Tensor recordCopyFrom data size {}.", bufferSize);

    this->recordCopyBufferFromImage(commandBuffer,
                           copyFromImage->getPrimaryImage(),
                           this->mPrimaryBuffer,
                           bufferSize,
                           copyRegion);
}

void
Tensor::recordCopyFromStagingToDevice(const vk::CommandBuffer& commandBuffer)
{
    vk::DeviceSize bufferSize(this->memorySize());
    vk::BufferCopy copyRegion(0, 0, bufferSize);

    KP_LOG_DEBUG("Kompute Tensor copying data size {}.", bufferSize);

    this->recordCopyBuffer(commandBuffer,
                           this->mStagingBuffer,
                           this->mPrimaryBuffer,
                           bufferSize,
                           copyRegion);
}

void
Tensor::recordCopyFromDeviceToStaging(const vk::CommandBuffer& commandBuffer)
{
    vk::DeviceSize bufferSize(this->memorySize());
    vk::BufferCopy copyRegion(0, 0, bufferSize);

    KP_LOG_DEBUG("Kompute Tensor copying data size {}.", bufferSize);

    this->recordCopyBuffer(commandBuffer,
                           this->mPrimaryBuffer,
                           this->mStagingBuffer,
                           bufferSize,
                           copyRegion);
}

void
Tensor::recordCopyBuffer(const vk::CommandBuffer& commandBuffer,
                         std::shared_ptr<vk::Buffer> bufferFrom,
                         std::shared_ptr<vk::Buffer> bufferTo,
                         vk::DeviceSize /*bufferSize*/,
                         vk::BufferCopy copyRegion)
{

    commandBuffer.copyBuffer(*bufferFrom, *bufferTo, copyRegion);
}

void
Tensor::recordCopyBufferFromImage(const vk::CommandBuffer& commandBuffer,
                         std::shared_ptr<vk::Image> imageFrom,
                         std::shared_ptr<vk::Buffer> bufferTo,
                         vk::DeviceSize /*bufferSize*/,
                         vk::BufferImageCopy copyRegion)
{
    commandBuffer.copyImageToBuffer(*imageFrom, vk::ImageLayout::eGeneral, *bufferTo, 1, &copyRegion);
}

void
Tensor::recordPrimaryMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                   vk::AccessFlagBits srcAccessMask,
                                   vk::AccessFlagBits dstAccessMask,
                                   vk::PipelineStageFlagBits srcStageMask,
                                   vk::PipelineStageFlagBits dstStageMask)
{
    KP_LOG_DEBUG("Kompute Tensor recording PRIMARY buffer memory barrier");

    this->recordBufferMemoryBarrier(commandBuffer,
                                    *this->mPrimaryBuffer,
                                    srcAccessMask,
                                    dstAccessMask,
                                    srcStageMask,
                                    dstStageMask);
}

void
Tensor::recordStagingMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                   vk::AccessFlagBits srcAccessMask,
                                   vk::AccessFlagBits dstAccessMask,
                                   vk::PipelineStageFlagBits srcStageMask,
                                   vk::PipelineStageFlagBits dstStageMask)
{
    KP_LOG_DEBUG("Kompute Tensor recording STAGING buffer memory barrier");

    this->recordBufferMemoryBarrier(commandBuffer,
                                    *this->mStagingBuffer,
                                    srcAccessMask,
                                    dstAccessMask,
                                    srcStageMask,
                                    dstStageMask);
}

// FIXME: Make this private.
void
Tensor::recordBufferMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                  const vk::Buffer& buffer,
                                  vk::AccessFlagBits srcAccessMask,
                                  vk::AccessFlagBits dstAccessMask,
                                  vk::PipelineStageFlagBits srcStageMask,
                                  vk::PipelineStageFlagBits dstStageMask)
{
    KP_LOG_DEBUG("Kompute Tensor recording buffer memory barrier");

    vk::DeviceSize bufferSize = this->memorySize();

    vk::BufferMemoryBarrier bufferMemoryBarrier;
    bufferMemoryBarrier.buffer = buffer;
    bufferMemoryBarrier.size = bufferSize;
    bufferMemoryBarrier.srcAccessMask = srcAccessMask;
    bufferMemoryBarrier.dstAccessMask = dstAccessMask;
    bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    commandBuffer.pipelineBarrier(srcStageMask,
                                  dstStageMask,
                                  vk::DependencyFlags(),
                                  nullptr,
                                  bufferMemoryBarrier,
                                  nullptr);
}

vk::DescriptorBufferInfo
Tensor::constructDescriptorBufferInfo()
{
    KP_LOG_DEBUG("Kompute Tensor construct descriptor buffer info size {}",
                 this->memorySize());
    vk::DeviceSize bufferSize = this->memorySize();
    return vk::DescriptorBufferInfo(*this->mPrimaryBuffer,
                                    0, // offset
                                    bufferSize);
}

vk::WriteDescriptorSet
Tensor::constructDescriptorSet(vk::DescriptorSet descriptorSet,
                               uint32_t binding)
{
    KP_LOG_DEBUG("Kompute Tensor construct descriptor set for binding {}",
                 binding);

    mDescriptorBufferInfo = this->constructDescriptorBufferInfo();

    vk::WriteDescriptorSet writeDesciptorSet = vk::WriteDescriptorSet(
      descriptorSet,
      binding, // Destination binding
      0,       // Destination array element
      1,       // Descriptor count
      vk::DescriptorType::eStorageBuffer,
      nullptr, // Descriptor image info
      // Using a member variable here means the address is still valid later
      &mDescriptorBufferInfo);

    return writeDesciptorSet;
}

vk::BufferUsageFlags
Tensor::getPrimaryBufferUsageFlags()
{
    switch (this->mMemoryType) {
        case MemoryTypes::eDevice:
        case MemoryTypes::eHost:
        case MemoryTypes::eDeviceAndHost:
            return vk::BufferUsageFlagBits::eStorageBuffer |
                   vk::BufferUsageFlagBits::eTransferSrc |
                   vk::BufferUsageFlagBits::eTransferDst;
            break;
        case MemoryTypes::eStorage:
            return vk::BufferUsageFlagBits::eStorageBuffer;
            break;
        default:
            throw std::runtime_error("Kompute Tensor invalid tensor type");
    }
}

vk::MemoryPropertyFlags
Tensor::getPrimaryMemoryPropertyFlags()
{
    switch (this->mMemoryType) {
        case MemoryTypes::eDevice:
            return vk::MemoryPropertyFlagBits::eDeviceLocal;
            break;
        case MemoryTypes::eHost:
            return vk::MemoryPropertyFlagBits::eHostVisible |
                   vk::MemoryPropertyFlagBits::eHostCoherent;
            break;
        case MemoryTypes::eDeviceAndHost:
            return vk::MemoryPropertyFlagBits::eDeviceLocal |
                   vk::MemoryPropertyFlagBits::eHostVisible |
                   vk::MemoryPropertyFlagBits::eHostCoherent;
        case MemoryTypes::eStorage:
            return vk::MemoryPropertyFlagBits::eDeviceLocal;
            break;
        default:
            throw std::runtime_error("Kompute Tensor invalid tensor type");
    }
}

vk::BufferUsageFlags
Tensor::getStagingBufferUsageFlags()
{
    switch (this->mMemoryType) {
        case MemoryTypes::eDevice:
            return vk::BufferUsageFlagBits::eTransferSrc |
                   vk::BufferUsageFlagBits::eTransferDst;
            break;
        default:
            throw std::runtime_error("Kompute Tensor invalid tensor type");
    }
}

vk::MemoryPropertyFlags
Tensor::getStagingMemoryPropertyFlags()
{
    switch (this->mMemoryType) {
        case MemoryTypes::eDevice:
            return vk::MemoryPropertyFlagBits::eHostVisible |
                   vk::MemoryPropertyFlagBits::eHostCoherent;
            break;
        default:
            throw std::runtime_error("Kompute Tensor invalid tensor type");
    }
}

std::shared_ptr<vk::Buffer> Tensor::getPrimaryBuffer()
{
    return this->mPrimaryBuffer;
}

void
Tensor::allocateMemoryCreateGPUResources()
{
    KP_LOG_DEBUG("Kompute Tensor creating buffer");

    if (!this->mPhysicalDevice) {
        throw std::runtime_error("Kompute Tensor phyisical device is null");
    }
    if (!this->mDevice) {
        throw std::runtime_error("Kompute Tensor device is null");
    }

    KP_LOG_DEBUG("Kompute Tensor creating primary buffer and memory");

    this->mPrimaryBuffer = std::make_shared<vk::Buffer>();
    this->createBuffer(this->mPrimaryBuffer,
                       this->getPrimaryBufferUsageFlags());
    this->mFreePrimaryBuffer = true;
    this->mPrimaryMemory = std::make_shared<vk::DeviceMemory>();
    this->allocateBindMemory(this->mPrimaryBuffer,
                             this->mPrimaryMemory,
                             this->getPrimaryMemoryPropertyFlags());
    this->mFreePrimaryMemory = true;

    if (this->mMemoryType == MemoryTypes::eDevice) {
        KP_LOG_DEBUG("Kompute Tensor creating staging buffer and memory");

        this->mStagingBuffer = std::make_shared<vk::Buffer>();
        this->createBuffer(this->mStagingBuffer,
                           this->getStagingBufferUsageFlags());
        this->mFreeStagingBuffer = true;
        this->mStagingMemory = std::make_shared<vk::DeviceMemory>();
        this->allocateBindMemory(this->mStagingBuffer,
                                 this->mStagingMemory,
                                 this->getStagingMemoryPropertyFlags());
        this->mFreeStagingMemory = true;
    }

    KP_LOG_DEBUG("Kompute Tensor buffer & memory creation successful");
}

void
Tensor::createBuffer(std::shared_ptr<vk::Buffer> buffer,
                     vk::BufferUsageFlags bufferUsageFlags)
{

    vk::DeviceSize bufferSize = this->memorySize();

    if (bufferSize < 1) {
        throw std::runtime_error(
          "Kompute Tensor attempted to create a zero-sized buffer");
    }

    KP_LOG_DEBUG("Kompute Tensor creating buffer with memory size: {}, and "
                 "usage flags: {}",
                 bufferSize,
                 vk::to_string(bufferUsageFlags));

    // TODO: Explore having concurrent sharing mode (with option)
    vk::BufferCreateInfo bufferInfo(vk::BufferCreateFlags(),
                                    bufferSize,
                                    bufferUsageFlags,
                                    vk::SharingMode::eExclusive);

    this->mDevice->createBuffer(&bufferInfo, nullptr, buffer.get());
}

void
Tensor::allocateBindMemory(std::shared_ptr<vk::Buffer> buffer,
                           std::shared_ptr<vk::DeviceMemory> memory,
                           vk::MemoryPropertyFlags memoryPropertyFlags)
{

    KP_LOG_DEBUG("Kompute Tensor allocating and binding memory");

    vk::PhysicalDeviceMemoryProperties memoryProperties =
      this->mPhysicalDevice->getMemoryProperties();

    vk::MemoryRequirements memoryRequirements =
      this->mDevice->getBufferMemoryRequirements(*buffer);

    uint32_t memoryTypeIndex = -1;
    bool memoryTypeIndexFound = false;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (memoryRequirements.memoryTypeBits & (1 << i)) {
            if (((memoryProperties.memoryTypes[i]).propertyFlags &
                 memoryPropertyFlags) == memoryPropertyFlags) {
                memoryTypeIndex = i;
                memoryTypeIndexFound = true;
                break;
            }
        }
    }
    if (!memoryTypeIndexFound) {
        throw std::runtime_error(
          "Memory type index for buffer creation not found");
    }

    KP_LOG_DEBUG(
      "Kompute Tensor allocating memory index: {}, size {}, flags: {}",
      memoryTypeIndex,
      memoryRequirements.size,
      vk::to_string(memoryPropertyFlags));

    vk::MemoryAllocateInfo memoryAllocateInfo(memoryRequirements.size,
                                              memoryTypeIndex);

    this->mDevice->allocateMemory(&memoryAllocateInfo, nullptr, memory.get());

    this->mDevice->bindBufferMemory(*buffer, *memory, 0);
}

void
Tensor::destroy()
{
    KP_LOG_DEBUG("Kompute Tensor started destroy()");

    // Setting raw data to null regardless whether device is available to
    // invalidate Tensor
    this->mRawData = nullptr;
    this->mSize = 0;
    this->mDataTypeMemorySize = 0;

    if (!this->mDevice) {
        KP_LOG_WARN(
          "Kompute Tensor destructor reached with null Device pointer");
        return;
    }

    // Unmap the current memory data
    if (this->memoryType() != Memory::MemoryTypes::eStorage) {
        this->unmapRawData();
    }

    if (this->mFreePrimaryBuffer) {
        if (!this->mPrimaryBuffer) {
            KP_LOG_WARN("Kompose Tensor expected to destroy primary buffer "
                        "but got null buffer");
        } else {
            KP_LOG_DEBUG("Kompose Tensor destroying primary buffer");
            this->mDevice->destroy(
              *this->mPrimaryBuffer,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mPrimaryBuffer = nullptr;
            this->mFreePrimaryBuffer = false;
        }
    }

    if (this->mFreeStagingBuffer) {
        if (!this->mStagingBuffer) {
            KP_LOG_WARN("Kompose Tensor expected to destroy staging buffer "
                        "but got null buffer");
        } else {
            KP_LOG_DEBUG("Kompose Tensor destroying staging buffer");
            this->mDevice->destroy(
              *this->mStagingBuffer,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mStagingBuffer = nullptr;
            this->mFreeStagingBuffer = false;
        }
    }

    if (this->mFreePrimaryMemory) {
        if (!this->mPrimaryMemory) {
            KP_LOG_WARN("Kompose Tensor expected to free primary memory but "
                        "got null memory");
        } else {
            KP_LOG_DEBUG("Kompose Tensor freeing primary memory");
            this->mDevice->freeMemory(
              *this->mPrimaryMemory,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mPrimaryMemory = nullptr;
            this->mFreePrimaryMemory = false;
        }
    }

    if (this->mFreeStagingMemory) {
        if (!this->mStagingMemory) {
            KP_LOG_WARN("Kompose Tensor expected to free staging memory but "
                        "got null memory");
        } else {
            KP_LOG_DEBUG("Kompose Tensor freeing staging memory");
            this->mDevice->freeMemory(
              *this->mStagingMemory,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mStagingMemory = nullptr;
            this->mFreeStagingMemory = false;
        }
    }

    if (this->mDevice) {
        this->mDevice = nullptr;
    }

    KP_LOG_DEBUG("Kompute Tensor successful destroy()");
}

template<>
Tensor::TensorDataTypes
TensorT<bool>::dataType()
{
    return Tensor::TensorDataTypes::eBool;
}

template<>
Tensor::TensorDataTypes
TensorT<int8_t>::dataType()
{
    return Tensor::TensorDataTypes::eChar;
}

template<>
Tensor::TensorDataTypes
TensorT<uint8_t>::dataType()
{
    return Tensor::TensorDataTypes::eUnsignedChar;
}

template<>
Tensor::TensorDataTypes
TensorT<int16_t>::dataType()
{
    return Tensor::TensorDataTypes::eShort;
}

template<>
Tensor::TensorDataTypes
TensorT<uint16_t>::dataType()
{
    return Tensor::TensorDataTypes::eUnsignedShort;
}

template<>
Tensor::TensorDataTypes
TensorT<int32_t>::dataType()
{
    return Tensor::TensorDataTypes::eInt;
}

template<>
Tensor::TensorDataTypes
TensorT<uint32_t>::dataType()
{
    return Tensor::TensorDataTypes::eUnsignedInt;
}

template<>
Tensor::TensorDataTypes
TensorT<float>::dataType()
{
    return Tensor::TensorDataTypes::eFloat;
}

template<>
Tensor::TensorDataTypes
TensorT<double>::dataType()
{
    return Tensor::TensorDataTypes::eDouble;
}

}
