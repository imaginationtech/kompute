// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "kompute/Core.hpp"
#include "logger/Logger.hpp"
#include <memory>
#include <string>

namespace kp {

class Memory
{
    // This is the base class for Tensors and Images.
    // It's required so that algorithms and sequences can mix tensors and
    // images.
    // FIXME: Common-ise lots of the code that is the same between images and
    // tensors into this base class.
  public:
    /**
     * Type for memory created: Device allows memory to be transferred from
     * staging memory. Staging are host memory visible. Storage are device
     * visible but are not set up to transfer or receive data (only for shader
     * storage).
     */
    enum class MemoryTypes
    {
        eDevice = 0,  ///< Type is device memory, source and destination
        eHost = 1,    ///< Type is host memory, source and destination
        eStorage = 2, ///< Type is Device memory (only)
    };

    static std::string toString(MemoryTypes dt);

    /**
     * Destructor which is in charge of freeing vulkan resources unless they
     * have been provided externally.
     */
    virtual ~Memory(){};

    /**
     * Retrieve the image type of the image
     *
     * @return image type of image
     */
    MemoryTypes memoryType();

    /**
     * Check whether tensor/image is initialized based on the created gpu
     * resources.
     *
     * @returns Boolean stating whether tensor is initialized
     */
    virtual bool isInit() = 0;

    /**
     * Records a copy from the internal staging memory to the device memory
     * using an optional barrier to wait for the operation. This function would
     * only be relevant for kp::Tensors of type eDevice.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     */
    virtual void recordCopyFromStagingToDevice(
      const vk::CommandBuffer& commandBuffer) = 0;

    /**
     * Records a copy from the internal device memory to the staging memory
     * using an optional barrier to wait for the operation. This function would
     * only be relevant for kp::Tensors of type eDevice.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     */
    virtual void recordCopyFromDeviceToStaging(
      const vk::CommandBuffer& commandBuffer) = 0;
    /**
     * Records the buffer memory barrier into the primary buffer and command
     * buffer which ensures that relevant data transfers are carried out
     * correctly.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     * @param srcAccessMask Access flags for source access mask
     * @param dstAccessMask Access flags for destination access mask
     * @param scrStageMask Pipeline stage flags for source stage mask
     * @param dstStageMask Pipeline stage flags for destination stage mask
     */
    virtual void recordPrimaryMemoryBarrier(
      const vk::CommandBuffer& commandBuffer,
      vk::AccessFlagBits srcAccessMask,
      vk::AccessFlagBits dstAccessMask,
      vk::PipelineStageFlagBits srcStageMask,
      vk::PipelineStageFlagBits dstStageMask) = 0;
    /**
     * Records the buffer memory barrier into the staging buffer and command
     * buffer which ensures that relevant data transfers are carried out
     * correctly.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     * @param srcAccessMask Access flags for source access mask
     * @param dstAccessMask Access flags for destination access mask
     * @param scrStageMask Pipeline stage flags for source stage mask
     * @param dstStageMask Pipeline stage flags for destination stage mask
     */
    virtual void recordStagingMemoryBarrier(
      const vk::CommandBuffer& commandBuffer,
      vk::AccessFlagBits srcAccessMask,
      vk::AccessFlagBits dstAccessMask,
      vk::PipelineStageFlagBits srcStageMask,
      vk::PipelineStageFlagBits dstStageMask) = 0;

    /**
     * Adds this object to a Vulkan descriptor set at \p binding.
     *
     * @param descriptorSet The descriptor set to add to.
     * @param binding The binding number to use.
     * @return Add this object to a descriptor set at \p binding.
     */
    virtual vk::WriteDescriptorSet constructDescriptorSet(
      vk::DescriptorSet descriptorSet,
      uint32_t binding) = 0;

    /**
     * Returns the size/magnitude of the Tensor/Image, which will be the total
     * number of elements across all dimensions
     *
     * @return Unsigned integer representing the total number of elements
     */
    uint32_t size();

    /**
     * Returns the total size of a single element of the respective data type
     * that this memory object holds.
     *
     * @return Unsigned integer representing the memory of a single element of
     * the respective data type.
     */
    uint32_t dataTypeMemorySize();

    /**
     * Returns the total memory size of the data contained by the memory object
     * which would equate to (this->size() * this->dataTypeMemorySize())
     *
     * @return Unsigned integer representing the total memory size of the data
     * contained by the image object.
     */
    uint32_t memorySize();

    vk::DescriptorType getDescriptorType() { return mDescriptorType; }

    /**
     * Retrieve the raw data via the pointer to the memory that contains the raw
     * memory of this current tensor. This tensor gets changed to a nullptr when
     * the Tensor is removed.
     *
     * @return Pointer to raw memory containing raw bytes data of Tensor/Image.
     */
    void* rawData();

    /**
     * Sets / resets the data of the tensor/image which is directly done on the
     * GPU host visible memory available by the tensor.
     */
    void setRawData(const void* data);

    /**
     * Template to return the pointer data converted by specific type, which
     * would be any of the supported types including float, double, int32,
     * uint32 and bool.
     *
     * @return Pointer to raw memory containing raw bytes data of Tensor/Image.
     */
    template<typename T>
    T* data()
    {
        return (T*)this->mRawData;
    }

    /**
     * Return the pointer data cast to float.
     *
     * @return Pointer to raw memory containing raw bytes data of Tensor/Image.
     * This is the default, for convenience.
     */
    float* data() { return this->data<float>(); }

    /**
     * Template to get the data of the current tensor/image as a vector of
     * specific type, which would be any of the supported types including float,
     * double, int32, uint32 and bool.
     *
     * @return Vector of type provided by template.
     */
    template<typename T>
    std::vector<T> vector()
    {
        return { (T*)this->mRawData, ((T*)this->mRawData) + this->size() };
    }

    /**
     * Get the data of the current tensor/image as a vector of float.
     * This is the default, for convenience.
     *
     * @return Vector of floats.
     */
    std::vector<float> vector() { return this->vector<float>(); }

  protected:
    // -------------- ALWAYS OWNED RESOURCES
    MemoryTypes mMemoryType;
    uint32_t mSize;
    uint32_t mDataTypeMemorySize;
    void* mRawData = nullptr;
    vk::DescriptorType mDescriptorType;
    bool mUnmapMemory = false;
};

} // End namespace kp
