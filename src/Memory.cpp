// SPDX-License-Identifier: Apache-2.0

#include "kompute/Memory.hpp"

namespace kp {

std::string
Memory::toString(Memory::MemoryTypes dt)
{
    switch (dt) {
        case MemoryTypes::eDevice:
            return "eDevice";
        case MemoryTypes::eHost:
            return "eHost";
        case MemoryTypes::eStorage:
            return "eStorage";
        default:
            return "unknown";
    }
}

Memory::MemoryTypes
Memory::memoryType()
{
    return this->mMemoryType;
}

uint32_t
Memory::size()
{
    return this->mSize;
}

uint32_t
Memory::dataTypeMemorySize()
{
    return this->mDataTypeMemorySize;
}

uint32_t
Memory::memorySize()
{
    return this->mSize * this->mDataTypeMemorySize;
}

void*
Memory::rawData()
{
    if (!this->mRawData) {
        this->mapRawData();
    }
    return this->mRawData;
}

void
Memory::setRawData(const void* data)
{
    if (!this->mRawData) {
        this->mapRawData();
    }
    memcpy(this->mRawData, data, this->memorySize());
}

void
Memory::mapRawData()
{
    KP_LOG_DEBUG("Kompute Memory mapping data from host buffer");

    std::shared_ptr<vk::DeviceMemory> hostVisibleMemory = nullptr;

    if (this->mMemoryType == MemoryTypes::eHost || this->mMemoryType == MemoryTypes::eDeviceAndHost) {
        hostVisibleMemory = this->mPrimaryMemory;
    } else if (this->mMemoryType == MemoryTypes::eDevice) {
        hostVisibleMemory = this->mStagingMemory;
    } else {
        KP_LOG_WARN("Kompute Memory mapping data not supported on {} memory",
                    Memory::toString(this->memoryType()));
        return;
    }

    vk::DeviceSize size = this->memorySize();

    // Given we request coherent host memory we don't need to invalidate /
    // flush
    this->mRawData = this->mDevice->mapMemory(
      *hostVisibleMemory, 0, size, vk::MemoryMapFlags());

    this->mUnmapMemory = true;
}

void
Memory::unmapRawData()
{
    KP_LOG_DEBUG("Kompute Memory unmapping data from host buffer");
    if (!this->mUnmapMemory) {
        return;
    }

    std::shared_ptr<vk::DeviceMemory> hostVisibleMemory = nullptr;

    if (this->mMemoryType == MemoryTypes::eHost || this->mMemoryType == MemoryTypes::eDeviceAndHost) {
        hostVisibleMemory = this->mPrimaryMemory;
    } else if (this->mMemoryType == MemoryTypes::eDevice) {
        hostVisibleMemory = this->mStagingMemory;
    } else {
        KP_LOG_WARN("Kompute Tensor mapping data not supported on {} memory",
                    Memory::toString(this->memoryType()));
        return;
    }

    vk::DeviceSize size = this->memorySize();
    vk::MappedMemoryRange mappedRange(*hostVisibleMemory, 0, size);
    this->mDevice->flushMappedMemoryRanges(1, &mappedRange);
    this->mDevice->unmapMemory(*hostVisibleMemory);

    this->mUnmapMemory = false;
}

} // end namespace kp
