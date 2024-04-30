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
    return this->mRawData;
}

void
Memory::setRawData(const void* data)
{
    memcpy(this->mRawData, data, this->memorySize());
}

} // end namespace kp
