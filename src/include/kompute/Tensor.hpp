// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "Memory.hpp"
#include "kompute/Core.hpp"
#include "logger/Logger.hpp"
#include <memory>
#include <string>

namespace kp {

/**
 * Structured data used in GPU operations.
 *
 * Tensors are the base building block in Kompute to perform operations across
 * GPUs. Each tensor would have a respective Vulkan memory and buffer, which
 * would be used to store their respective data. The tensors can be used for GPU
 * data storage or transfer.
 */
class Tensor : public Memory
{
  public:
    enum class TensorDataTypes
    {
        eBool = 0,
        eInt = 1,
        eUnsignedInt = 2,
        eFloat = 3,
        eDouble = 4,
        eCustom = 5,
        eChar = 6,
        eUnsignedChar = 7,
        eShort = 8,
        eUnsignedShort = 9
    };

    static std::string toString(TensorDataTypes dt);

    /**
     *  Constructor with data provided which would be used to create the
     * respective vulkan buffer and memory.
     *
     *  @param physicalDevice The physical device to use to fetch properties
     *  @param device The device to use to create the buffer and memory from
     *  @param data Non-zero-sized vector of data that will be used by the
     * tensor
     *  @param tensorTypes Type for the tensor which is of type MemoryTypes
     */
    Tensor(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
           std::shared_ptr<vk::Device> device,
           void* data,
           uint32_t elementTotalCount,
           uint32_t elementMemorySize,
           const TensorDataTypes& dataType,
           const MemoryTypes& tensorType = MemoryTypes::eDevice);

    /**
     *  Constructor without data provided which would be used to create the
     * respective vulkan buffer and memory.
     *
     *  @param physicalDevice The physical device to use to fetch properties
     *  @param device The device to use to create the buffer and memory from
     *  @param tensorTypes Type for the tensor which is of type MemoryTypes
     */
    Tensor(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
              std::shared_ptr<vk::Device> device,
              uint32_t elementTotalCount,
              uint32_t elementMemorySize,
              const TensorDataTypes& dataType,
              const MemoryTypes& memoryType) : Tensor(physicalDevice, device, nullptr, elementTotalCount, elementMemorySize, dataType, memoryType) {};

    /**
     * Destructor which is in charge of freeing vulkan resources unless they
     * have been provided externally.
     */
    virtual ~Tensor();

    /**
     * Function to trigger reinitialisation of the tensor buffer and memory with
     * new data as well as new potential device type.
     *
     * @param data Vector of data to use to initialise vector from
     * @param tensorType The type to use for the tensor
     */
    void rebuild(void* data,
                 uint32_t elementTotalCount,
                 uint32_t elementMemorySize);

    /**
     * Destroys and frees the GPU resources which include the buffer and memory.
     */
    void destroy();

    /**
     * Check whether tensor is initialized based on the created gpu resources.
     *
     * @returns Boolean stating whether tensor is initialized
     */
    bool isInit();

    /**
     * Records a copy from the memory of the tensor provided to the current
     * tensor. This is intended to pass memory into a processing, to perform
     * a staging buffer transfer, or to gather output (between others).
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     * @param copyFromTensor Tensor to copy the data from
     */
    void recordCopyFrom(const vk::CommandBuffer& commandBuffer,
                        std::shared_ptr<Tensor> copyFromTensor);

    /**
     * Records a copy from the internal staging memory to the device memory
     * using an optional barrier to wait for the operation. This function would
     * only be relevant for kp::Tensors of type eDevice.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     */
    void recordCopyFromStagingToDevice(const vk::CommandBuffer& commandBuffer);

    /**
     * Records a copy from the internal device memory to the staging memory
     * using an optional barrier to wait for the operation. This function would
     * only be relevant for kp::Tensors of type eDevice.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     */
    void recordCopyFromDeviceToStaging(const vk::CommandBuffer& commandBuffer);

    /**
     * Records the memory barrier into the primary buffer and command
     * buffer which ensures that relevant data transfers are carried out
     * correctly.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     * @param srcAccessMask Access flags for source access mask
     * @param dstAccessMask Access flags for destination access mask
     * @param scrStageMask Pipeline stage flags for source stage mask
     * @param dstStageMask Pipeline stage flags for destination stage mask
     */
    void recordPrimaryMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                    vk::AccessFlagBits srcAccessMask,
                                    vk::AccessFlagBits dstAccessMask,
                                    vk::PipelineStageFlagBits srcStageMask,
                                    vk::PipelineStageFlagBits dstStageMask);
    /**
     * Records the memory barrier into the staging buffer and command
     * buffer which ensures that relevant data transfers are carried out
     * correctly.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     * @param srcAccessMask Access flags for source access mask
     * @param dstAccessMask Access flags for destination access mask
     * @param scrStageMask Pipeline stage flags for source stage mask
     * @param dstStageMask Pipeline stage flags for destination stage mask
     */
    void recordStagingMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                    vk::AccessFlagBits srcAccessMask,
                                    vk::AccessFlagBits dstAccessMask,
                                    vk::PipelineStageFlagBits srcStageMask,
                                    vk::PipelineStageFlagBits dstStageMask);

    /**
     * Adds this object to a Vulkan descriptor set at \p binding.
     *
     * @param descriptorSet The descriptor set to add to.
     * @param binding The binding number to use.
     * @return Add this object to a descriptor set at \p binding.
     */
    vk::WriteDescriptorSet constructDescriptorSet(
      vk::DescriptorSet descriptorSet,
      uint32_t binding);

    /**
     * Retrieve the data type of the tensor (host, device, storage)
     *
     * @return Data type of tensor of type kp::Tensor::TensorDataTypes
     */
    TensorDataTypes dataType();

  protected:
    // -------------- ALWAYS OWNED RESOURCES
    TensorDataTypes mDataType;
    vk::DescriptorBufferInfo mDescriptorBufferInfo;

  private:
    // -------------- NEVER OWNED RESOURCES
    std::shared_ptr<vk::PhysicalDevice> mPhysicalDevice;
    std::shared_ptr<vk::Device> mDevice;

    // -------------- OPTIONALLY OWNED RESOURCES
    std::shared_ptr<vk::Buffer> mPrimaryBuffer;
    bool mFreePrimaryBuffer = false;
    std::shared_ptr<vk::Buffer> mStagingBuffer;
    bool mFreeStagingBuffer = false;
    std::shared_ptr<vk::DeviceMemory> mPrimaryMemory;
    bool mFreePrimaryMemory = false;
    std::shared_ptr<vk::DeviceMemory> mStagingMemory;
    bool mFreeStagingMemory = false;

    void allocateMemoryCreateGPUResources(); // Creates the vulkan buffer
    void createBuffer(std::shared_ptr<vk::Buffer> buffer,
                      vk::BufferUsageFlags bufferUsageFlags);
    void allocateBindMemory(std::shared_ptr<vk::Buffer> buffer,
                            std::shared_ptr<vk::DeviceMemory> memory,
                            vk::MemoryPropertyFlags memoryPropertyFlags);
    void recordCopyBuffer(const vk::CommandBuffer& commandBuffer,
                          std::shared_ptr<vk::Buffer> bufferFrom,
                          std::shared_ptr<vk::Buffer> bufferTo,
                          vk::DeviceSize bufferSize,
                          vk::BufferCopy copyRegion);
    void recordBufferMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                   const vk::Buffer& buffer,
                                   vk::AccessFlagBits srcAccessMask,
                                   vk::AccessFlagBits dstAccessMask,
                                   vk::PipelineStageFlagBits srcStageMask,
                                   vk::PipelineStageFlagBits dstStageMask);

    // Private util functions
    vk::BufferUsageFlags getPrimaryBufferUsageFlags();
    vk::MemoryPropertyFlags getPrimaryMemoryPropertyFlags();
    vk::BufferUsageFlags getStagingBufferUsageFlags();
    vk::MemoryPropertyFlags getStagingMemoryPropertyFlags();

    void mapRawData();
    void unmapRawData();

    vk::DescriptorBufferInfo constructDescriptorBufferInfo();
};

template<typename T>
class TensorT : public Tensor
{

  public:
    TensorT(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
            std::shared_ptr<vk::Device> device,
            const std::vector<T>& data,
            const MemoryTypes& tensorType = MemoryTypes::eDevice)
      : Tensor(physicalDevice,
               device,
               (void*)data.data(),
               data.size(),
               sizeof(T),
               this->dataType(),
               tensorType)
    {
        KP_LOG_DEBUG("Kompute TensorT constructor with data size {}",
                     data.size());
    }

    TensorT(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
            std::shared_ptr<vk::Device> device,
            const size_t size,
            const MemoryTypes& tensorType = MemoryTypes::eDevice)
      : Tensor(physicalDevice,
               device,
               size,
               sizeof(T),
               this->dataType(),
               tensorType)
    {
        KP_LOG_DEBUG("Kompute TensorT constructor with data size {}",
                     size);
    }

    ~TensorT() { KP_LOG_DEBUG("Kompute TensorT destructor"); }

    T* data() { return (T*)this->mRawData; }

    std::vector<T> vector()
    {
        return { (T*)this->mRawData, ((T*)this->mRawData) + this->size() };
    }

    T& operator[](int index) { return *(((T*)this->mRawData) + index); }

    void setData(const std::vector<T>& data)
    {

        KP_LOG_DEBUG("Kompute TensorT setting data with data size {}",
                     data.size());

        if (data.size() != this->mSize) {
            throw std::runtime_error(
              "Kompute TensorT Cannot set data of different sizes");
        }

        Tensor::setRawData(data.data());
    }

    TensorDataTypes dataType();
};

} // End namespace kp
