// SPDX-License-Identifier: Apache-2.0
#pragma once

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
class Image
{
  public:
    /**
     * Type for images created: Device allows memory to be transferred from
     * staging images. Staging are host memory visible. Storage are device
     * visible but are not set up to transfer or receive data (only for shader
     * storage).
     */
    enum class ImageTypes
    {
        eDevice = 0,  ///< Type is device memory, source and destination
        eHost = 1,    ///< Type is host memory, source and destination
        eStorage = 2, ///< Type is Device memory (only)
    };

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
    static std::string toString(ImageTypes dt);

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
     *  @param imageType Type for the image which is of type ImageTypes
     */
    Image(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
           std::shared_ptr<vk::Device> device,
           void* data,
           uint32_t width,
           uint32_t height,
           const ImageDataTypes& dataType,
           const ImageTypes& imageType = ImageTypes::eDevice);

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
     * Retrieve the image type of the image
     *
     * @return image type of image
     */
    ImageTypes ImageType();

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
    void recordPrimaryImageMemoryBarrier(
      const vk::CommandBuffer& commandBuffer,
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
    void recordStagingImageMemoryBarrier(
      const vk::CommandBuffer& commandBuffer,
      vk::AccessFlagBits srcAccessMask,
      vk::AccessFlagBits dstAccessMask,
      vk::PipelineStageFlagBits srcStageMask,
      vk::PipelineStageFlagBits dstStageMask);

    /**
     * Constructs a vulkan descriptor image info which can be used to specify
     * and reference the underlying image component of the image without
     * exposing it.
     *
     * @return Descriptor image info with own image
     */
    vk::DescriptorImageInfo constructDescriptorImageInfo();

    /**
     * Returns the size/magnitude of the image, which will be the total number
     * of elements across all dimensions
     *
     * @return Unsigned integer representing the total number of elements
     */
    uint32_t size();

    /**
     * Returns the total size of a single element of the respective data type
     * that this image holds.
     *
     * @return Unsigned integer representing the memory of a single element of
     * the respective data type.
     */
    uint32_t dataTypeMemorySize();

    /**
     * Returns the total memory size of the data contained by the image object
     * which would equate to (this->size() * this->dataTypeMemorySize())
     *
     * @return Unsigned integer representing the total memory size of the data
     * contained by the image object.
     */
    uint32_t memorySize();

    /**
     * Retrieve the data type of the image (host, device, storage)
     *
     * @return Data type of image of type kp::image::ImageDataTypes
     */
    ImageDataTypes dataType();

    /**
     * Retrieve the raw data via the pointer to the memory that contains the raw
     * memory of this current image. This image gets changed to a nullptr when
     * the image is removed.
     *
     * @return Pointer to raw memory containing raw bytes data of image.
     */
    void* rawData();

    /**
     * Sets / resets the data of the image which is directly done on the GPU
     * host visible memory available by the image.
     */
    void setRawData(const void* data);

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
    ImageTypes mImageType;
    ImageDataTypes mDataType;
    uint32_t mSize;
    uint32_t mDataTypeMemorySize;
    uint32_t mWidth;
    uint32_t mHeight;
    void* mRawData;

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
                                   vk::PipelineStageFlagBits dstStageMask);

    // Private util functions
    vk::ImageUsageFlags getPrimaryImageUsageFlags();
    vk::MemoryPropertyFlags getPrimaryMemoryPropertyFlags();
    vk::ImageUsageFlags getStagingImageUsageFlags();
    vk::MemoryPropertyFlags getStagingMemoryPropertyFlags();

    void mapRawData();
    void unmapRawData();
};

template<typename T>
class ImageT : public Image
{

  public:
    ImageT(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
            std::shared_ptr<vk::Device> device,
            const std::vector<T>& data,
            const ImageTypes& imageType = ImageTypes::eDevice)
      : Image(physicalDevice,
               device,
               (void*)data.data(),
               data.size(),
               sizeof(T),
               this->dataType(),
               imageType)
    {
        KP_LOG_DEBUG("Kompute imageT constructor with data size {}",
                     data.size());
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
