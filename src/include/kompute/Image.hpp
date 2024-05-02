// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "Memory.hpp"
#include "kompute/Core.hpp"
#include "logger/Logger.hpp"
#include <memory>
#include <string>

namespace kp {

/**
 * Image data used in GPU operations.
 *
 * Each image would has a respective Vulkan memory and image, which
 * would be used to store their respective data. The images can be used for GPU
 * data storage or transfer.
 */
class Image : public Memory
{
  public:
    // FIXME: For now let's just assume 4 channels
    // We'll see later what you actually pass to Vulkan to set this up.
    enum class ImageDataTypes
    {
        eS8 = 0,
        eU8 = 1,
        eS16 = 2,
        eU16 = 3,
        eS32 = 4,
        eU32 = 5,
        eF16 = 6,
        eF32 = 7,
    };

    static std::string toString(ImageDataTypes dt);

    /**
     *  Constructor with data provided which would be used to create the
     * respective vulkan image and memory.
     *
     *  @param physicalDevice The physical device to use to fetch properties
     *  @param device The device to use to create the image and memory from
     *  @param data Non-zero-sized vector of data that will be used by the
     * image
     *  @param width Width of the image in pixels
     *  @param height Height of the image in pixels
     *  @param dataType Data type for the image which is of type ImageDataTypes
     *  @param imageType Type for the image which is of type MemoryTypes
     */
    Image(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
          std::shared_ptr<vk::Device> device,
          void* data,
          uint32_t width,
          uint32_t height,
          uint32_t numChannels,
          const ImageDataTypes& dataType,
          const MemoryTypes& imageType = MemoryTypes::eDevice);

    /**
     * Destructor which is in charge of freeing vulkan resources unless they
     * have been provided externally.
     */
    virtual ~Image();

    /**
     * Function to trigger reinitialisation of the image and memory with
     * new data as well as new potential device type.
     *
     * @param data Vector of data to use to initialise image from
     */
    void rebuild(void* data);

    /**
     * Destroys and frees the GPU resources which include the image and memory.
     */
    void destroy();

    /**
     * Check whether image is initialized based on the created gpu resources.
     *
     * @returns Boolean stating whether image is initialized
     */
    bool isInit();

    /**
     * Records a copy from the memory of the image provided to the current
     * image. This is intended to pass memory into a processing, to perform
     * a staging image transfer, or to gather output (between others).
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     * @param copyFromImage Image to copy the data from
     */
    void recordCopyFrom(const vk::CommandBuffer& commandBuffer,
                        std::shared_ptr<Image> copyFromImage);

    /**
     * Records a copy from the internal staging memory to the device memory
     * using an optional barrier to wait for the operation. This function would
     * only be relevant for kp::images of type eDevice.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     */
    void recordCopyFromStagingToDevice(const vk::CommandBuffer& commandBuffer);

    /**
     * Records a copy from the internal device memory to the staging memory
     * using an optional barrier to wait for the operation. This function would
     * only be relevant for kp::images of type eDevice.
     *
     * @param commandBuffer Vulkan Command Buffer to record the commands into
     */
    void recordCopyFromDeviceToStaging(const vk::CommandBuffer& commandBuffer);

    /**
     * Records the image memory barrier into the primary image and command
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
     * Records the image memory barrier into the staging image and command
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
     * Retrieve the data type of the image (host, device, storage)
     *
     * @return Data type of image of type kp::image::ImageDataTypes
     */
    ImageDataTypes dataType();

    /**
     * Template to return the pointer data converted by specific type, which
     * would be any of the supported types.
     *
     * @return Pointer to raw memory containing raw bytes data of image.
     */
    template<typename T>
    T* data()
    {
        return (T*)this->mRawData;
    }

    /**
     * Template to get the data of the current image as a vector of specific
     * type, which would be any of the supported types.
     *
     * @return Vector of type provided by template.
     */
    template<typename T>
    std::vector<T> vector()
    {
        return { (T*)this->mRawData, ((T*)this->mRawData) + this->size() };
    }

  protected:
    // -------------- ALWAYS OWNED RESOURCES
    ImageDataTypes mDataType;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mNumChannels;
    vk::DescriptorImageInfo mDescriptorImageInfo;
    vk::ImageLayout mPrimaryImageLayout = vk::ImageLayout::eUndefined;
    vk::ImageLayout mStagingImageLayout = vk::ImageLayout::eUndefined;
    std::shared_ptr<vk::ImageView> mImageView = nullptr;

  private:
    // -------------- NEVER OWNED RESOURCES
    std::shared_ptr<vk::PhysicalDevice> mPhysicalDevice;
    std::shared_ptr<vk::Device> mDevice;

    // -------------- OPTIONALLY OWNED RESOURCES
    std::shared_ptr<vk::Image> mPrimaryImage;
    bool mFreePrimaryImage = false;
    std::shared_ptr<vk::Image> mStagingImage;
    bool mFreeStagingImage = false;
    std::shared_ptr<vk::DeviceMemory> mPrimaryMemory;
    bool mFreePrimaryMemory = false;
    std::shared_ptr<vk::DeviceMemory> mStagingMemory;
    bool mFreeStagingMemory = false;

    void allocateMemoryCreateGPUResources(); // Creates the vulkan image
    void createImage(std::shared_ptr<vk::Image> image,
                     vk::ImageUsageFlags imageUsageFlags);
    void allocateBindMemory(std::shared_ptr<vk::Image> image,
                            std::shared_ptr<vk::DeviceMemory> memory,
                            vk::MemoryPropertyFlags memoryPropertyFlags);
    void recordCopyImage(const vk::CommandBuffer& commandBuffer,
                         std::shared_ptr<vk::Image> imageFrom,
                         std::shared_ptr<vk::Image> imageTo,
                         vk::ImageCopy copyRegion);
    void recordImageMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                  const vk::Image& image,
                                  vk::AccessFlagBits srcAccessMask,
                                  vk::AccessFlagBits dstAccessMask,
                                  vk::PipelineStageFlagBits srcStageMask,
                                  vk::PipelineStageFlagBits dstStageMask,
                                  vk::ImageLayout oldLayout,
                                  vk::ImageLayout newLayout);

    // Private util functions
    vk::ImageUsageFlags getPrimaryImageUsageFlags();
    vk::MemoryPropertyFlags getPrimaryMemoryPropertyFlags();
    vk::ImageUsageFlags getStagingImageUsageFlags();
    vk::MemoryPropertyFlags getStagingMemoryPropertyFlags();

    void mapRawData();
    void unmapRawData();

    constexpr size_t elementTypeSize(ImageDataTypes type);
    vk::Format getFormat();

    vk::DescriptorImageInfo constructDescriptorImageInfo();
};

template<typename T>
class ImageT : public Image
{

  public:
    ImageT(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
           std::shared_ptr<vk::Device> device,
           const std::vector<T>& data,
           uint32_t width,
           uint32_t height,
           uint32_t numChannels,
           const MemoryTypes& imageType = MemoryTypes::eDevice)
      : Image(physicalDevice,
              device,
              (void*)data.data(),
              width,
              height,
              numChannels,
              this->dataType(),
              imageType)
    {
        KP_LOG_DEBUG("Kompute imageT constructor with data size {}, width {}, "
                     "height {}, and num channels {}",
                     data.size(),
                     width,
                     height,
                     numChannels);
        if (data.size() < width * height * numChannels) {
            throw std::runtime_error(
              "Kompute ImageT vector is smaller than the requested image size");
        }
    }

    ~ImageT() { KP_LOG_DEBUG("Kompute imageT destructor"); }

    T* data() { return (T*)this->mRawData; }

    std::vector<T> vector()
    {
        return { (T*)this->mRawData, ((T*)this->mRawData) + this->size() };
    }

    T& operator[](int index) { return *(((T*)this->mRawData) + index); }

    void setData(const std::vector<T>& data)
    {

        KP_LOG_DEBUG("Kompute imageT setting data with data size {}",
                     data.size());

        if (data.size() != this->mSize) {
            throw std::runtime_error(
              "Kompute imageT Cannot set data of different sizes");
        }

        Image::setRawData(data.data());
    }

    ImageDataTypes dataType();
};

} // End namespace kp
