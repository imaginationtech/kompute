// SPDX-License-Identifier: Apache-2.0

#include "kompute/Image.hpp"

namespace kp {

std::string
Image::toString(Image::ImageDataTypes dt)
{
    switch (dt) {
        case ImageDataTypes::eU8:
            return "U8";
        case ImageDataTypes::eS8:
            return "S8";
        case ImageDataTypes::eU16:
            return "U16";
        case ImageDataTypes::eS16:
            return "S16";
        case ImageDataTypes::eU32:
            return "U32";
        case ImageDataTypes::eS32:
            return "S32";
        case ImageDataTypes::eF16:
            return "F16";
        case ImageDataTypes::eF32:
            return "F32";
        default:
            return "unknown";
    }
}

void Image::init(std::shared_ptr<vk::PhysicalDevice> physicalDevice,
             std::shared_ptr<vk::Device> device,
             void* data,
             uint32_t width,
             uint32_t height,
             uint32_t numChannels,
             const ImageDataTypes& dataType,
             vk::ImageTiling tiling,
             const MemoryTypes& memoryType)
{
    KP_LOG_DEBUG(
      "Kompute Image constructor data width: {}, height: {}, and type: {}",
      width,
      height,
      Memory::toString(memoryType));

    if (width == 0 || height == 0 || numChannels == 0) {
        throw std::runtime_error(
          "Kompute Image attempted to create a zero-sized image");
    }

    if (memoryType == MemoryTypes::eStorage && data != nullptr)
    {
        KP_LOG_WARN("Kompute Image of type eStorage do not need to be initialised with data");
    }

    if (tiling == vk::ImageTiling::eOptimal && 
        (memoryType != Memory::MemoryTypes::eDevice && memoryType != Memory::MemoryTypes::eStorage))
    {
        throw std::runtime_error("Kompute Image with optimal tiling is only supported for eDevice and eStorage images");
    }

    this->mPhysicalDevice = physicalDevice;
    this->mDevice = device;
    this->mDataType = dataType;
    this->mMemoryType = memoryType;
    this->mWidth = width;
    this->mHeight = height;
    this->mNumChannels = numChannels;
    this->mDescriptorType = vk::DescriptorType::eStorageImage;
    this->mTiling = tiling;

    this->rebuild(data);
}

Image::~Image()
{
    KP_LOG_DEBUG("Kompute Image destructor started. Type: {}",
                 Memory::toString(this->memoryType()));

    if (this->mDevice) {
        this->destroy();
    }

    KP_LOG_DEBUG("Kompute Image destructor success");
}

void
Image::rebuild(void* data)
{
    KP_LOG_DEBUG("Kompute Image rebuilding with size {} x {} with {} channels",
                 this->mWidth,
                 this->mHeight,
                 this->mNumChannels);

    if (this->mPrimaryImage || this->mPrimaryMemory) {
        KP_LOG_DEBUG(
          "Kompute Image destroying existing resources before rebuild");
        this->destroy();
    }

    this->mSize = this->mWidth * this->mHeight * this->mNumChannels;
    this->mDataTypeMemorySize = elementTypeSize(this->mDataType);
    this->allocateMemoryCreateGPUResources();

    if (this->memoryType() != Image::MemoryTypes::eStorage && data != nullptr) {
        this->mapRawData();
        memcpy(this->mRawData, data, this->memorySize());
    }
}

bool
Image::isInit()
{
    return this->mDevice && this->mPrimaryImage && this->mPrimaryMemory;
}

kp::Image::ImageDataTypes
Image::dataType()
{
    return this->mDataType;
}

void
Image::recordCopyFrom(const vk::CommandBuffer& commandBuffer,
                      std::shared_ptr<Image> copyFromImage)
{
    vk::ImageSubresourceLayers layer = {};
    layer.aspectMask = vk::ImageAspectFlagBits::eColor;
    layer.layerCount = 1;
    vk::Offset3D offset = { 0, 0, 0 };

    // FIXME: Check the size of the dest and source images match
    vk::Extent3D size = { this->mWidth, this->mHeight, 1 };

    vk::ImageCopy copyRegion(layer, offset, layer, offset, size);

    KP_LOG_DEBUG(
      "Kompute Image recordCopyFrom size {},{}.", size.width, size.height);

    if (copyFromImage->mPrimaryImageLayout == vk::ImageLayout::eUndefined) {
        copyFromImage->recordPrimaryMemoryBarrier(
          commandBuffer,
          vk::AccessFlagBits::eMemoryRead,
          vk::AccessFlagBits::eMemoryWrite,
          vk::PipelineStageFlagBits::eTransfer,
          vk::PipelineStageFlagBits::eTransfer);
    }

    if (this->mPrimaryImageLayout == vk::ImageLayout::eUndefined) {
        this->recordPrimaryMemoryBarrier(commandBuffer,
                                         vk::AccessFlagBits::eMemoryRead,
                                         vk::AccessFlagBits::eMemoryWrite,
                                         vk::PipelineStageFlagBits::eTransfer,
                                         vk::PipelineStageFlagBits::eTransfer);
    }

    this->recordCopyImage(commandBuffer,
                          copyFromImage->mPrimaryImage,
                          this->mPrimaryImage,
                          copyRegion);
}

void
Image::recordCopyFromStagingToDevice(const vk::CommandBuffer& commandBuffer)
{
    vk::ImageSubresourceLayers layer = {};
    layer.aspectMask = vk::ImageAspectFlagBits::eColor;
    layer.layerCount = 1;
    vk::Offset3D offset = { 0, 0, 0 };

    // FIXME: Check the size of the dest and source images match
    vk::Extent3D size = { this->mWidth, this->mHeight, 1 };

    vk::ImageCopy copyRegion(layer, offset, layer, offset, size);

    KP_LOG_DEBUG("Kompute Image copying size {},{}.", size.width, size.height);

    if (this->mPrimaryImageLayout == vk::ImageLayout::eUndefined) {
        this->recordPrimaryMemoryBarrier(commandBuffer,
                                         vk::AccessFlagBits::eMemoryRead,
                                         vk::AccessFlagBits::eMemoryWrite,
                                         vk::PipelineStageFlagBits::eTransfer,
                                         vk::PipelineStageFlagBits::eTransfer);
    }

    if (this->mStagingImageLayout == vk::ImageLayout::eUndefined) {
        this->recordStagingMemoryBarrier(commandBuffer,
                                         vk::AccessFlagBits::eMemoryRead,
                                         vk::AccessFlagBits::eMemoryWrite,
                                         vk::PipelineStageFlagBits::eTransfer,
                                         vk::PipelineStageFlagBits::eTransfer);
    }

    this->recordCopyImage(
      commandBuffer, this->mStagingImage, this->mPrimaryImage, copyRegion);
}

void
Image::recordCopyFromDeviceToStaging(const vk::CommandBuffer& commandBuffer)
{
    vk::ImageSubresourceLayers layer;
    layer.aspectMask = vk::ImageAspectFlagBits::eColor;
    layer.layerCount = 1;
    vk::Offset3D offset = { 0, 0, 0 };

    // FIXME: Check the size of the dest and source images match
    vk::Extent3D size = { this->mWidth, this->mHeight, 1 };

    vk::ImageCopy copyRegion(layer, offset, layer, offset, size);

    KP_LOG_DEBUG("Kompute Image copying size {},{}.", size.width, size.height);

    if (this->mPrimaryImageLayout == vk::ImageLayout::eUndefined) {
        this->recordPrimaryMemoryBarrier(commandBuffer,
                                         vk::AccessFlagBits::eMemoryRead,
                                         vk::AccessFlagBits::eMemoryWrite,
                                         vk::PipelineStageFlagBits::eTransfer,
                                         vk::PipelineStageFlagBits::eTransfer);
    }

    if (this->mStagingImageLayout == vk::ImageLayout::eUndefined) {
        this->recordStagingMemoryBarrier(commandBuffer,
                                         vk::AccessFlagBits::eMemoryRead,
                                         vk::AccessFlagBits::eMemoryWrite,
                                         vk::PipelineStageFlagBits::eTransfer,
                                         vk::PipelineStageFlagBits::eTransfer);
    }

    this->recordCopyImage(
      commandBuffer, this->mPrimaryImage, this->mStagingImage, copyRegion);
}

void
Image::recordCopyImage(const vk::CommandBuffer& commandBuffer,
                       std::shared_ptr<vk::Image> imageFrom,
                       std::shared_ptr<vk::Image> imageTo,
                       vk::ImageCopy copyRegion)
{
    commandBuffer.copyImage(*imageFrom,
                            vk::ImageLayout::eGeneral,
                            *imageTo,
                            vk::ImageLayout::eGeneral,
                            1,
                            &copyRegion);
}

void
Image::recordPrimaryMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                  vk::AccessFlagBits srcAccessMask,
                                  vk::AccessFlagBits dstAccessMask,
                                  vk::PipelineStageFlagBits srcStageMask,
                                  vk::PipelineStageFlagBits dstStageMask)
{
    KP_LOG_DEBUG("Kompute Image recording PRIMARY image memory barrier");

    // FIXME: Image layout is used here to transition from eUndefined on
    // creation to eGeneral for the rest of the program. Is there a better
    // way/place to do this?
    this->recordImageMemoryBarrier(commandBuffer,
                                   *this->mPrimaryImage,
                                   srcAccessMask,
                                   dstAccessMask,
                                   srcStageMask,
                                   dstStageMask,
                                   this->mPrimaryImageLayout,
                                   vk::ImageLayout::eGeneral);
}

void
Image::recordStagingMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                  vk::AccessFlagBits srcAccessMask,
                                  vk::AccessFlagBits dstAccessMask,
                                  vk::PipelineStageFlagBits srcStageMask,
                                  vk::PipelineStageFlagBits dstStageMask)
{
    KP_LOG_DEBUG("Kompute Image recording STAGING image memory barrier");

    // FIXME: Image layout is used here to transition from eUndefined on
    // creation to eGeneral for the rest of the program. Is there a better
    // way/place to do this?
    this->recordImageMemoryBarrier(commandBuffer,
                                   *this->mStagingImage,
                                   srcAccessMask,
                                   dstAccessMask,
                                   srcStageMask,
                                   dstStageMask,
                                   this->mStagingImageLayout,
                                   vk::ImageLayout::eGeneral);
}

// FIXME: Make this private.
void
Image::recordImageMemoryBarrier(const vk::CommandBuffer& commandBuffer,
                                const vk::Image& image,
                                vk::AccessFlagBits srcAccessMask,
                                vk::AccessFlagBits dstAccessMask,
                                vk::PipelineStageFlagBits srcStageMask,
                                vk::PipelineStageFlagBits dstStageMask,
                                vk::ImageLayout outLayout,
                                vk::ImageLayout newLayout)
{
    KP_LOG_DEBUG("Kompute Image recording image memory barrier");

    vk::ImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.image = image;

    imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    imageMemoryBarrier.subresourceRange.levelCount = 1;
    imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    imageMemoryBarrier.subresourceRange.layerCount = 1;
    imageMemoryBarrier.subresourceRange.aspectMask =
      vk::ImageAspectFlagBits::eColor;

    imageMemoryBarrier.srcAccessMask = srcAccessMask;
    imageMemoryBarrier.dstAccessMask = dstAccessMask;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    imageMemoryBarrier.oldLayout = outLayout;
    imageMemoryBarrier.newLayout = newLayout;

    commandBuffer.pipelineBarrier(srcStageMask,
                                  dstStageMask,
                                  vk::DependencyFlags(),
                                  nullptr,
                                  nullptr,
                                  imageMemoryBarrier);
}

vk::DescriptorImageInfo
Image::constructDescriptorImageInfo()
{
    KP_LOG_DEBUG("Kompute Image construct descriptor image info size {}",
                 this->memorySize());

    // RQ: FIXME: Initilaise these correctly.
    vk::ImageViewCreateInfo viewInfo;
    viewInfo.image = *this->mPrimaryImage;
    viewInfo.format = this->getFormat();
    viewInfo.flags = vk::ImageViewCreateFlags();
    viewInfo.viewType = vk::ImageViewType::e2D;

    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    // This image object owns the image view
    if (!this->mImageView) {
        mImageView = std::make_shared<vk::ImageView>(
          this->mDevice->createImageView(viewInfo));
    }

    vk::DescriptorImageInfo descriptorInfo;

    descriptorInfo.imageView = *(mImageView.get());
    descriptorInfo.imageLayout = vk::ImageLayout::eGeneral;
    return descriptorInfo;
}

vk::WriteDescriptorSet
Image::constructDescriptorSet(vk::DescriptorSet descriptorSet, uint32_t binding)
{
    KP_LOG_DEBUG("Kompute Image construct descriptor set for binding {}",
                 binding);

    mDescriptorImageInfo = this->constructDescriptorImageInfo();

    return vk::WriteDescriptorSet(descriptorSet,
                                  binding, // Destination binding
                                  0,       // Destination array element
                                  1,       // Descriptor count
                                  vk::DescriptorType::eStorageImage,
                                  &mDescriptorImageInfo,
                                  nullptr); // Descriptor buffer info
}

vk::ImageUsageFlags
Image::getPrimaryImageUsageFlags()
{
    switch (this->mMemoryType) {
        case MemoryTypes::eDevice:
        case MemoryTypes::eHost:
        case MemoryTypes::eDeviceAndHost:
            return vk::ImageUsageFlagBits::eStorage |
                   vk::ImageUsageFlagBits::eTransferSrc |
                   vk::ImageUsageFlagBits::eTransferDst;
            break;
        case MemoryTypes::eStorage:
            return vk::ImageUsageFlagBits::eStorage |
                   // You can still copy image-copy to/from storage memory
                   // (or at least TestOpImageCopy.CopyThroughStorageImage tests
                   // you can) so set the transfer usage flags here.
                   vk::ImageUsageFlagBits::eTransferSrc |
                   vk::ImageUsageFlagBits::eTransferDst;
            break;
        default:
            throw std::runtime_error("Kompute Image invalid image type");
    }
}

vk::MemoryPropertyFlags
Image::getPrimaryMemoryPropertyFlags()
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
            throw std::runtime_error("Kompute Image invalid image type");
    }
}

vk::ImageUsageFlags
Image::getStagingImageUsageFlags()
{
    switch (this->mMemoryType) {
        case MemoryTypes::eDevice:
            return vk::ImageUsageFlagBits::eTransferSrc |
                   vk::ImageUsageFlagBits::eTransferDst;
            break;
        default:
            throw std::runtime_error("Kompute Image invalid image type");
    }
}

vk::MemoryPropertyFlags
Image::getStagingMemoryPropertyFlags()
{
    switch (this->mMemoryType) {
        case MemoryTypes::eDevice:
            return vk::MemoryPropertyFlagBits::eHostVisible |
                   vk::MemoryPropertyFlagBits::eHostCoherent;
            break;
        default:
            throw std::runtime_error("Kompute Image invalid image type");
    }
}

void
Image::allocateMemoryCreateGPUResources()
{
    KP_LOG_DEBUG("Kompute Image creating image");

    if (!this->mPhysicalDevice) {
        throw std::runtime_error("Kompute Image phyisical device is null");
    }
    if (!this->mDevice) {
        throw std::runtime_error("Kompute Image device is null");
    }

    KP_LOG_DEBUG("Kompute Image creating primary image and memory");

    this->mPrimaryImage = std::make_shared<vk::Image>();
    this->createImage(this->mPrimaryImage, this->getPrimaryImageUsageFlags(), this->mTiling);
    this->mFreePrimaryImage = true;
    this->mPrimaryMemory = std::make_shared<vk::DeviceMemory>();
    this->allocateBindMemory(this->mPrimaryImage,
                             this->mPrimaryMemory,
                             this->getPrimaryMemoryPropertyFlags());
    this->mFreePrimaryMemory = true;

    if (this->mMemoryType == MemoryTypes::eDevice) {
        KP_LOG_DEBUG("Kompute Image creating staging image and memory");

        this->mStagingImage = std::make_shared<vk::Image>();
        this->createImage(this->mStagingImage,
                          this->getStagingImageUsageFlags(),
                          vk::ImageTiling::eLinear);
        this->mFreeStagingImage = true;
        this->mStagingMemory = std::make_shared<vk::DeviceMemory>();
        this->allocateBindMemory(this->mStagingImage,
                                 this->mStagingMemory,
                                 this->getStagingMemoryPropertyFlags());
        this->mFreeStagingMemory = true;
    }

    KP_LOG_DEBUG("Kompute Image image & memory creation successful");
}

void
Image::createImage(std::shared_ptr<vk::Image> image,
                   vk::ImageUsageFlags imageUsageFlags,
                   vk::ImageTiling imageTiling)
{
    vk::DeviceSize imageSize = this->memorySize();

    if (imageSize < 1) {
        throw std::runtime_error(
          "Kompute Image attempted to create a zero-sized image");
    }

    KP_LOG_DEBUG("Kompute Image creating image with memory size: {}, and "
                 "usage flags: {}",
                 imageSize,
                 vk::to_string(imageUsageFlags));

    // TODO: Explore having concurrent sharing mode (with option)
    vk::ImageCreateInfo imageInfo;

    imageInfo.flags = vk::ImageCreateFlags();
    imageInfo.imageType = vk::ImageType::e2D;
    imageInfo.format = this->getFormat();
    imageInfo.extent = vk::Extent3D(this->mWidth, this->mHeight, 1);
    imageInfo.usage = imageUsageFlags;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.initialLayout = vk::ImageLayout::eUndefined;
    imageInfo.tiling = imageTiling;

    this->mDevice->createImage(&imageInfo, nullptr, image.get());
}

void
Image::allocateBindMemory(std::shared_ptr<vk::Image> image,
                          std::shared_ptr<vk::DeviceMemory> memory,
                          vk::MemoryPropertyFlags memoryPropertyFlags)
{

    KP_LOG_DEBUG("Kompute Image allocating and binding memory");

    vk::PhysicalDeviceMemoryProperties memoryProperties =
      this->mPhysicalDevice->getMemoryProperties();

    vk::MemoryRequirements memoryRequirements =
      this->mDevice->getImageMemoryRequirements(*image);

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
          "Memory type index for image creation not found");
    }

    KP_LOG_DEBUG(
      "Kompute Image allocating memory index: {}, size {}, flags: {}",
      memoryTypeIndex,
      memoryRequirements.size,
      vk::to_string(memoryPropertyFlags));

    vk::MemoryAllocateInfo memoryAllocateInfo(memoryRequirements.size,
                                              memoryTypeIndex);

    this->mDevice->allocateMemory(&memoryAllocateInfo, nullptr, memory.get());

    this->mDevice->bindImageMemory(*image, *memory, 0);
}

void
Image::destroy()
{
    KP_LOG_DEBUG("Kompute Image started destroy()");

    // Setting raw data to null regardless whether device is available to
    // invalidate Image
    this->mRawData = nullptr;
    this->mSize = 0;
    this->mDataTypeMemorySize = 0;

    if (!this->mDevice) {
        KP_LOG_WARN(
          "Kompute Image destructor reached with null Device pointer");
        return;
    }

    // Unmap the current memory data
    if (this->memoryType() != Image::MemoryTypes::eStorage) {
        this->unmapRawData();
    }

    if (this->mFreePrimaryImage) {
        if (!this->mPrimaryImage) {
            KP_LOG_WARN("Kompose Image expected to destroy primary image "
                        "but got null image");
        } else {
            KP_LOG_DEBUG("Kompose Image destroying primary image");
            this->mDevice->destroy(
              *this->mPrimaryImage,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mPrimaryImage = nullptr;
            this->mFreePrimaryImage = false;
        }
    }

    if (this->mFreeStagingImage) {
        if (!this->mStagingImage) {
            KP_LOG_WARN("Kompose Image expected to destroy staging image "
                        "but got null image");
        } else {
            KP_LOG_DEBUG("Kompose Image destroying staging image");
            this->mDevice->destroy(
              *this->mStagingImage,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mStagingImage = nullptr;
            this->mFreeStagingImage = false;
        }
    }

    if (this->mFreePrimaryMemory) {
        if (!this->mPrimaryMemory) {
            KP_LOG_WARN("Kompose Image expected to free primary memory but "
                        "got null memory");
        } else {
            KP_LOG_DEBUG("Kompose Image freeing primary memory");
            this->mDevice->freeMemory(
              *this->mPrimaryMemory,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mPrimaryMemory = nullptr;
            this->mFreePrimaryMemory = false;
        }
    }

    if (this->mFreeStagingMemory) {
        if (!this->mStagingMemory) {
            KP_LOG_WARN("Kompose Image expected to free staging memory but "
                        "got null memory");
        } else {
            KP_LOG_DEBUG("Kompose Image freeing staging memory");
            this->mDevice->freeMemory(
              *this->mStagingMemory,
              (vk::Optional<const vk::AllocationCallbacks>)nullptr);
            this->mStagingMemory = nullptr;
            this->mFreeStagingMemory = false;
        }
    }

    if (this->mImageView) {
        KP_LOG_DEBUG("Kompose Image freeing image view");
        this->mDevice->destroyImageView(*this->mImageView);
        this->mImageView = nullptr;
    }

    if (this->mDevice) {
        this->mDevice = nullptr;
    }

    KP_LOG_DEBUG("Kompute Image successful destroy()");
}

constexpr size_t
Image::elementTypeSize(Image::ImageDataTypes type)
{
    switch (type) {
        case Image::ImageDataTypes::eS8:
            return sizeof(int8_t);
        case Image::ImageDataTypes::eU8:
            return sizeof(uint8_t);
        case Image::ImageDataTypes::eS16:
            return sizeof(int16_t);
        case Image::ImageDataTypes::eU16:
            return sizeof(uint16_t);
        case Image::ImageDataTypes::eS32:
            return sizeof(int32_t);
        case Image::ImageDataTypes::eU32:
            return sizeof(uint32_t);
        case Image::ImageDataTypes::eF16:
            return sizeof(int16_t);
        case Image::ImageDataTypes::eF32:
            return sizeof(float);
        default:
            throw std::runtime_error("Kompute Image invalid image data type");
            break;
    }

    return -1;
}

vk::Format
Image::getFormat()
{
    switch (this->mDataType) {
        case Image::ImageDataTypes::eS8: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR8Sint;
                case 2:
                    return vk::Format::eR8G8Sint;
                case 3:
                    return vk::Format::eR8G8B8Sint;
                case 4:
                    return vk::Format::eR8G8B8A8Sint;
                default:
                    return vk::Format::eUndefined;
            }
        }
        case Image::ImageDataTypes::eU8: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR8Uint;
                case 2:
                    return vk::Format::eR8G8Uint;
                case 3:
                    return vk::Format::eR8G8B8Uint;
                case 4:
                    return vk::Format::eR8G8B8A8Uint;
                default:
                    return vk::Format::eUndefined;
            }
        }
        case Image::ImageDataTypes::eU16: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR16Uint;
                case 2:
                    return vk::Format::eR16G16Uint;
                case 3:
                    return vk::Format::eR16G16B16Uint;
                case 4:
                    return vk::Format::eR16G16B16A16Uint;
                default:
                    return vk::Format::eUndefined;
            }
        }
        case Image::ImageDataTypes::eS16: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR16Sint;
                case 2:
                    return vk::Format::eR16G16Sint;
                case 3:
                    return vk::Format::eR16G16B16Sint;
                case 4:
                    return vk::Format::eR16G16B16A16Sint;
                default:
                    return vk::Format::eUndefined;
            }
        }
        case Image::ImageDataTypes::eU32: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR32Uint;
                case 2:
                    return vk::Format::eR32G32Uint;
                case 3:
                    return vk::Format::eR32G32B32Uint;
                case 4:
                    return vk::Format::eR32G32B32A32Uint;
                default:
                    return vk::Format::eUndefined;
            }
        }
        case Image::ImageDataTypes::eS32: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR32Sint;
                case 2:
                    return vk::Format::eR32G32Sint;
                case 3:
                    return vk::Format::eR32G32B32Sint;
                case 4:
                    return vk::Format::eR32G32B32A32Sint;
                default:
                    return vk::Format::eUndefined;
            }
        }
        case Image::ImageDataTypes::eF16: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR16Sfloat;
                case 2:
                    return vk::Format::eR16G16Sfloat;
                case 3:
                    return vk::Format::eR16G16B16Sfloat;
                case 4:
                    return vk::Format::eR16G16B16A16Sfloat;
                default:
                    return vk::Format::eUndefined;
            }
        }
        case Image::ImageDataTypes::eF32: {
            switch (this->mNumChannels) {
                case 1:
                    return vk::Format::eR32Sfloat;
                case 2:
                    return vk::Format::eR32G32Sfloat;
                case 3:
                    return vk::Format::eR32G32B32Sfloat;
                case 4:
                    return vk::Format::eR32G32B32A32Sfloat;
                default:
                    return vk::Format::eUndefined;
            }
        }
        default:
            return vk::Format::eUndefined;
    }
}

template<>
Image::ImageDataTypes
ImageT<int8_t>::dataType()
{
    return Image::ImageDataTypes::eS8;
}

template<>
Image::ImageDataTypes
ImageT<uint8_t>::dataType()
{
    return Image::ImageDataTypes::eU8;
}

template<>
Image::ImageDataTypes
ImageT<int16_t>::dataType()
{
    return Image::ImageDataTypes::eS16;
}

template<>
Image::ImageDataTypes
ImageT<uint16_t>::dataType()
{
    return Image::ImageDataTypes::eU16;
}

template<>
Image::ImageDataTypes
ImageT<int32_t>::dataType()
{
    return Image::ImageDataTypes::eS32;
}

template<>
Image::ImageDataTypes
ImageT<uint32_t>::dataType()
{
    return Image::ImageDataTypes::eU32;
}

template<>
Image::ImageDataTypes
ImageT<float>::dataType()
{
    return Image::ImageDataTypes::eF32;
}

}
