/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_kp_Algorithm =
R"doc(Abstraction for compute shaders that are run on top of tensors grouped
via ParameterGroups (which group descriptorsets))doc";

static const char *__doc_kp_Algorithm_Algorithm =
R"doc(Main constructor for algorithm with configuration parameters to create
the underlying resources.

Parameter ``device``:
    The Vulkan device to use for creating resources

Parameter ``tensors``:
    (optional) The tensors to use to create the descriptor resources

Parameter ``spirv``:
    (optional) The spirv code to use to create the algorithm

Parameter ``workgroup``:
    (optional) The kp::Workgroup to use for the dispatch which
    defaults to kp::Workgroup(tensor[0].size(), 1, 1) if not set.

Parameter ``specializationConstants``:
    (optional) The templatable param is to be used to initialize the
    specialization constants which cannot be changed once set.

Parameter ``pushConstants``:
    (optional) This templatable param is to be used when initializing
    the pipeline, which set the size of the push constants - these can
    be modified but all new values must have the same data type and
    length as otherwise it will result in errors.)doc";

static const char *__doc_kp_Algorithm_createParameters = R"doc()doc";

static const char *__doc_kp_Algorithm_createPipeline = R"doc()doc";

static const char *__doc_kp_Algorithm_createShaderModule = R"doc()doc";

static const char *__doc_kp_Algorithm_destroy = R"doc()doc";

static const char *__doc_kp_Algorithm_getMemObjects =
R"doc(Gets the current memory objects that are used in the algorithm.

Returns:
    The list of memory objects used in the algorithm.)doc";

static const char *__doc_kp_Algorithm_getPushConstants =
R"doc(Gets the specialization constants of the current algorithm.

Returns:
    The std::vector<float> currently set for push constants)doc";

static const char *__doc_kp_Algorithm_getSpecializationConstants =
R"doc(Gets the specialization constants of the current algorithm.

Returns:
    The std::vector<float> currently set for specialization constants)doc";

static const char *__doc_kp_Algorithm_getWorkgroup =
R"doc(Gets the current workgroup from the algorithm.

Parameter ``The``:
    kp::Constant to use to set the push constants to use in the next
    bindPush(...) calls. The constants provided must be of the same
    size as the ones created during initialization.)doc";

static const char *__doc_kp_Algorithm_isInit =
R"doc(function that checks all the gpu resource components to verify if
these have been created and returns true if all are valid.

Returns:
    returns true if the algorithm is currently initialized.)doc";

static const char *__doc_kp_Algorithm_mDescriptorPool = R"doc()doc";

static const char *__doc_kp_Algorithm_mDescriptorSet = R"doc()doc";

static const char *__doc_kp_Algorithm_mDescriptorSetLayout = R"doc()doc";

static const char *__doc_kp_Algorithm_mDevice = R"doc()doc";

static const char *__doc_kp_Algorithm_mFreeDescriptorPool = R"doc()doc";

static const char *__doc_kp_Algorithm_mFreeDescriptorSet = R"doc()doc";

static const char *__doc_kp_Algorithm_mFreeDescriptorSetLayout = R"doc()doc";

static const char *__doc_kp_Algorithm_mFreePipeline = R"doc()doc";

static const char *__doc_kp_Algorithm_mFreePipelineCache = R"doc()doc";

static const char *__doc_kp_Algorithm_mFreePipelineLayout = R"doc()doc";

static const char *__doc_kp_Algorithm_mFreeShaderModule = R"doc()doc";

static const char *__doc_kp_Algorithm_mMemObjects = R"doc()doc";

static const char *__doc_kp_Algorithm_mPipeline = R"doc()doc";

static const char *__doc_kp_Algorithm_mPipelineCache = R"doc()doc";

static const char *__doc_kp_Algorithm_mPipelineLayout = R"doc()doc";

static const char *__doc_kp_Algorithm_mPushConstantsData = R"doc()doc";

static const char *__doc_kp_Algorithm_mPushConstantsDataTypeMemorySize = R"doc()doc";

static const char *__doc_kp_Algorithm_mPushConstantsSize = R"doc()doc";

static const char *__doc_kp_Algorithm_mShaderModule = R"doc()doc";

static const char *__doc_kp_Algorithm_mSpecializationConstantsData = R"doc()doc";

static const char *__doc_kp_Algorithm_mSpecializationConstantsDataTypeMemorySize = R"doc()doc";

static const char *__doc_kp_Algorithm_mSpecializationConstantsSize = R"doc()doc";

static const char *__doc_kp_Algorithm_mSpirv = R"doc()doc";

static const char *__doc_kp_Algorithm_mWorkgroup = R"doc()doc";

static const char *__doc_kp_Algorithm_rebuild =
R"doc(Rebuild function to reconstruct algorithm with configuration
parameters to create the underlying resources.

Parameter ``tensors``:
    The tensors to use to create the descriptor resources

Parameter ``spirv``:
    The spirv code to use to create the algorithm

Parameter ``workgroup``:
    (optional) The kp::Workgroup to use for the dispatch which
    defaults to kp::Workgroup(tensor[0].size(), 1, 1) if not set.

Parameter ``specializationConstants``:
    (optional) The std::vector<float> to use to initialize the
    specialization constants which cannot be changed once set.

Parameter ``pushConstants``:
    (optional) The std::vector<float> to use when initializing the
    pipeline, which set the size of the push constants - these can be
    modified but all new values must have the same vector size as this
    initial value.)doc";

static const char *__doc_kp_Algorithm_recordBindCore =
R"doc(Records command that binds the "core" algorithm components which
consist of binding the pipeline and binding the descriptorsets.

Parameter ``commandBuffer``:
    Command buffer to record the algorithm resources to)doc";

static const char *__doc_kp_Algorithm_recordBindPush =
R"doc(Records command that binds the push constants to the command buffer
provided - it is required that the pushConstants provided are of the
same size as the ones provided during initialization.

Parameter ``commandBuffer``:
    Command buffer to record the algorithm resources to)doc";

static const char *__doc_kp_Algorithm_recordDispatch =
R"doc(Records the dispatch function with the provided template parameters or
alternatively using the size of the tensor by default.

Parameter ``commandBuffer``:
    Command buffer to record the algorithm resources to)doc";

static const char *__doc_kp_Algorithm_setPushConstants =
R"doc(Sets the push constants to the new value provided to use in the next
bindPush()

Parameter ``pushConstants``:
    The templatable vector is to be used to set the push constants to
    use in the next bindPush(...) calls. The constants provided must
    be of the same size as the ones created during initialization.)doc";

static const char *__doc_kp_Algorithm_setPushConstants_2 =
R"doc(Sets the push constants to the new value provided to use in the next
bindPush() with the raw memory block location and memory size to be
used.

Parameter ``data``:
    The raw data point to copy the data from, without modifying the
    pointer.

Parameter ``size``:
    The number of data elements provided in the data

Parameter ``memorySize``:
    The memory size of each of the data elements in bytes.)doc";

static const char *__doc_kp_Algorithm_setWorkgroup =
R"doc(Sets the work group to use in the recordDispatch

Parameter ``workgroup``:
    The kp::Workgroup value to use to update the algorithm. It must
    have a value greater than 1 on the x value (index 1) otherwise it
    will be initialized on the size of the first tensor (ie.
    this->mTensor[0]->size()))doc";

static const char *__doc_kp_Image =
R"doc(Image data used in GPU operations.

Each image would has a respective Vulkan memory and image, which would
be used to store their respective data. The images can be used for GPU
data storage or transfer.)doc";

static const char *__doc_kp_ImageT = R"doc()doc";

static const char *__doc_kp_ImageT_ImageT = R"doc()doc";

static const char *__doc_kp_ImageT_ImageT_2 = R"doc()doc";

static const char *__doc_kp_ImageT_ImageT_3 = R"doc()doc";

static const char *__doc_kp_ImageT_ImageT_4 = R"doc()doc";

static const char *__doc_kp_ImageT_dataType = R"doc()doc";

static const char *__doc_kp_ImageT_operator_array = R"doc()doc";

static const char *__doc_kp_ImageT_setData = R"doc()doc";

static const char *__doc_kp_ImageT_vector = R"doc()doc";

static const char *__doc_kp_Image_Image =
R"doc(Constructor with data provided which would be used to create the
respective vulkan image and memory.

Parameter ``physicalDevice``:
    The physical device to use to fetch properties

Parameter ``device``:
    The device to use to create the image and memory from

Parameter ``data``:
    Non-zero-sized vector of data that will be used by the image

Parameter ``width``:
    Width of the image in pixels

Parameter ``height``:
    Height of the image in pixels

Parameter ``dataType``:
    Data type for the image which is of type ImageDataTypes

Parameter ``memoryType``:
    Type for the image which is of type MemoryTypes

Parameter ``tiling``:
    Tiling mode to use for the image.)doc";

static const char *__doc_kp_Image_Image_2 =
R"doc(Constructor with no data provided.

Parameter ``physicalDevice``:
    The physical device to use to fetch properties

Parameter ``device``:
    The device to use to create the image and memory from

Parameter ``width``:
    Width of the image in pixels

Parameter ``height``:
    Height of the image in pixels

Parameter ``dataType``:
    Data type for the image which is of type ImageDataTypes

Parameter ``memoryType``:
    Type for the image which is of type MemoryTypes

Parameter ``tiling``:
    Tiling mode to use for the image.)doc";

static const char *__doc_kp_Image_Image_3 =
R"doc(Constructor with data provided which would be used to create the
respective vulkan image and memory. No tiling has been provided so
will be inferred from \p memoryType.

Parameter ``physicalDevice``:
    The physical device to use to fetch properties

Parameter ``device``:
    The device to use to create the image and memory from

Parameter ``data``:
    Non-zero-sized vector of data that will be used by the image

Parameter ``width``:
    Width of the image in pixels

Parameter ``height``:
    Height of the image in pixels

Parameter ``dataType``:
    Data type for the image which is of type ImageDataTypes

Parameter ``memoryType``:
    Type for the image which is of type MemoryTypes

Parameter ``tiling``:
    Tiling mode to use for the image.)doc";

static const char *__doc_kp_Image_Image_4 =
R"doc(Constructor with no data provided. No tiling has been provided so will
be inferred from \p memoryType.

Parameter ``physicalDevice``:
    The physical device to use to fetch properties

Parameter ``device``:
    The device to use to create the image and memory from

Parameter ``width``:
    Width of the image in pixels

Parameter ``height``:
    Height of the image in pixels

Parameter ``dataType``:
    Data type for the image which is of type ImageDataTypes

Parameter ``memoryType``:
    Type for the image which is of type MemoryTypes)doc";

static const char *__doc_kp_Image_ImageDataTypes = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eF16 = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eF32 = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eS16 = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eS32 = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eS8 = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eU16 = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eU32 = R"doc()doc";

static const char *__doc_kp_Image_ImageDataTypes_eU8 = R"doc()doc";

static const char *__doc_kp_Image_allocateBindMemory = R"doc()doc";

static const char *__doc_kp_Image_allocateMemoryCreateGPUResources = R"doc()doc";

static const char *__doc_kp_Image_constructDescriptorImageInfo = R"doc()doc";

static const char *__doc_kp_Image_constructDescriptorSet =
R"doc(Adds this object to a Vulkan descriptor set at \p binding.

Parameter ``descriptorSet``:
    The descriptor set to add to.

Parameter ``binding``:
    The binding number to use.

Returns:
    Add this object to a descriptor set at \p binding.)doc";

static const char *__doc_kp_Image_createImage = R"doc()doc";

static const char *__doc_kp_Image_dataType =
R"doc(Retrieve the data type of the image (host, device, storage)

Returns:
    Data type of image of type kp::image::ImageDataTypes)doc";

static const char *__doc_kp_Image_destroy =
R"doc(Destroys and frees the GPU resources which include the image and
memory.)doc";

static const char *__doc_kp_Image_elementTypeSize = R"doc()doc";

static const char *__doc_kp_Image_getFormat = R"doc()doc";

static const char *__doc_kp_Image_getPrimaryImageUsageFlags = R"doc()doc";

static const char *__doc_kp_Image_getPrimaryMemoryPropertyFlags = R"doc()doc";

static const char *__doc_kp_Image_getStagingImageUsageFlags = R"doc()doc";

static const char *__doc_kp_Image_getStagingMemoryPropertyFlags = R"doc()doc";

static const char *__doc_kp_Image_init = R"doc()doc";

static const char *__doc_kp_Image_isInit =
R"doc(Check whether image is initialized based on the created gpu resources.

Returns:
    Boolean stating whether image is initialized)doc";

static const char *__doc_kp_Image_mDataType = R"doc()doc";

static const char *__doc_kp_Image_mDescriptorImageInfo = R"doc()doc";

static const char *__doc_kp_Image_mFreePrimaryImage = R"doc()doc";

static const char *__doc_kp_Image_mFreeStagingImage = R"doc()doc";

static const char *__doc_kp_Image_mHeight = R"doc()doc";

static const char *__doc_kp_Image_mImageView = R"doc()doc";

static const char *__doc_kp_Image_mNumChannels = R"doc()doc";

static const char *__doc_kp_Image_mPrimaryImage = R"doc()doc";

static const char *__doc_kp_Image_mPrimaryImageLayout = R"doc()doc";

static const char *__doc_kp_Image_mStagingImage = R"doc()doc";

static const char *__doc_kp_Image_mStagingImageLayout = R"doc()doc";

static const char *__doc_kp_Image_mTiling = R"doc()doc";

static const char *__doc_kp_Image_mWidth = R"doc()doc";

static const char *__doc_kp_Image_rebuild =
R"doc(Function to trigger reinitialisation of the image and memory with new
data as well as new potential device type.

Parameter ``data``:
    Vector of data to use to initialise image from)doc";

static const char *__doc_kp_Image_recordCopyFrom =
R"doc(Records a copy from the memory of the image provided to the current
image. This is intended to pass memory into a processing, to perform a
staging image transfer, or to gather output (between others).

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``copyFromImage``:
    Image to copy the data from)doc";

static const char *__doc_kp_Image_recordCopyFromDeviceToStaging =
R"doc(Records a copy from the internal device memory to the staging memory
using an optional barrier to wait for the operation. This function
would only be relevant for kp::images of type eDevice.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into)doc";

static const char *__doc_kp_Image_recordCopyFromStagingToDevice =
R"doc(Records a copy from the internal staging memory to the device memory
using an optional barrier to wait for the operation. This function
would only be relevant for kp::images of type eDevice.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into)doc";

static const char *__doc_kp_Image_recordCopyImage = R"doc()doc";

static const char *__doc_kp_Image_recordImageMemoryBarrier = R"doc()doc";

static const char *__doc_kp_Image_recordPrimaryMemoryBarrier =
R"doc(Records the image memory barrier into the primary image and command
buffer which ensures that relevant data transfers are carried out
correctly.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``srcAccessMask``:
    Access flags for source access mask

Parameter ``dstAccessMask``:
    Access flags for destination access mask

Parameter ``scrStageMask``:
    Pipeline stage flags for source stage mask

Parameter ``dstStageMask``:
    Pipeline stage flags for destination stage mask)doc";

static const char *__doc_kp_Image_recordStagingMemoryBarrier =
R"doc(Records the image memory barrier into the staging image and command
buffer which ensures that relevant data transfers are carried out
correctly.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``srcAccessMask``:
    Access flags for source access mask

Parameter ``dstAccessMask``:
    Access flags for destination access mask

Parameter ``scrStageMask``:
    Pipeline stage flags for source stage mask

Parameter ``dstStageMask``:
    Pipeline stage flags for destination stage mask)doc";

static const char *__doc_kp_Image_toString = R"doc()doc";

static const char *__doc_kp_Manager =
R"doc(Base orchestrator which creates and manages device and child
components)doc";

static const char *__doc_kp_Manager_Manager =
R"doc(Base constructor and default used which creates the base resources
including choosing the device 0 by default.)doc";

static const char *__doc_kp_Manager_Manager_2 =
R"doc(Similar to base constructor but allows for further configuration to
use when creating the Vulkan resources.

Parameter ``physicalDeviceIndex``:
    The index of the physical device to use

Parameter ``familyQueueIndices``:
    (Optional) List of queue indices to add for explicit allocation

Parameter ``desiredExtensions``:
    The desired extensions to load from physicalDevice)doc";

static const char *__doc_kp_Manager_Manager_3 =
R"doc(Manager constructor which allows your own vulkan application to
integrate with the kompute use.

Parameter ``instance``:
    Vulkan compute instance to base this application

Parameter ``physicalDevice``:
    Vulkan physical device to use for application

Parameter ``device``:
    Vulkan logical device to use for all base resources

Parameter ``physicalDeviceIndex``:
    Index for vulkan physical device used)doc";

static const char *__doc_kp_Manager_algorithm =
R"doc(Default non-template function that can be used to create algorithm
objects which provides default types to the push and spec constants as
floats.

Parameter ``memObjects``:
    (optional) The mem objects to initialise the algorithm with

Parameter ``spirv``:
    (optional) The SPIRV bytes for the algorithm to dispatch

Parameter ``workgroup``:
    (optional) kp::Workgroup for algorithm to use, and defaults to
    (tensor[0].size(), 1, 1)

Parameter ``specializationConstants``:
    (optional) float vector to use for specialization constants, and
    defaults to an empty constant

Parameter ``pushConstants``:
    (optional) float vector to use for push constants, and defaults to
    an empty constant

Returns:
    Shared pointer with initialised algorithm)doc";

static const char *__doc_kp_Manager_algorithm_2 =
R"doc(Create a managed algorithm that will be destroyed by this manager if
it hasn't been destroyed by its reference count going to zero.

Parameter ``memObjects``:
    (optional) The mem objects to initialise the algorithm with

Parameter ``spirv``:
    (optional) The SPIRV bytes for the algorithm to dispatch

Parameter ``workgroup``:
    (optional) kp::Workgroup for algorithm to use, and defaults to
    (tensor[0].size(), 1, 1)

Parameter ``specializationConstants``:
    (optional) templatable vector parameter to use for specialization
    constants, and defaults to an empty constant

Parameter ``pushConstants``:
    (optional) templatable vector parameter to use for push constants,
    and defaults to an empty constant

Returns:
    Shared pointer with initialised algorithm)doc";

static const char *__doc_kp_Manager_clear =
R"doc(Run a pseudo-garbage collection to release all the managed resources
that have been already freed due to these reaching to zero ref count.)doc";

static const char *__doc_kp_Manager_createDevice = R"doc()doc";

static const char *__doc_kp_Manager_createInstance = R"doc()doc";

static const char *__doc_kp_Manager_destroy = R"doc(Destroy the GPU resources and all managed resources by manager.)doc";

static const char *__doc_kp_Manager_getDeviceProperties =
R"doc(Information about the current device.

Returns:
    vk::PhysicalDeviceProperties containing information about the
    device)doc";

static const char *__doc_kp_Manager_getVkInstance =
R"doc(The current Vulkan instance.

Returns:
    a shared pointer to the current Vulkan instance held by this
    object)doc";

static const char *__doc_kp_Manager_image = R"doc()doc";

static const char *__doc_kp_Manager_image_2 = R"doc()doc";

static const char *__doc_kp_Manager_image_3 = R"doc()doc";

static const char *__doc_kp_Manager_image_4 = R"doc()doc";

static const char *__doc_kp_Manager_image_5 = R"doc()doc";

static const char *__doc_kp_Manager_image_6 = R"doc()doc";

static const char *__doc_kp_Manager_image_7 = R"doc()doc";

static const char *__doc_kp_Manager_image_8 = R"doc()doc";

static const char *__doc_kp_Manager_imageT =
R"doc(Create a managed image that will be destroyed by this manager if it
hasn't been destroyed by its reference count going to zero.

Parameter ``data``:
    The data to initialize the image with

Parameter ``tensorType``:
    The type of image to initialize

Returns:
    Shared pointer with initialised image)doc";

static const char *__doc_kp_Manager_imageT_2 = R"doc()doc";

static const char *__doc_kp_Manager_imageT_3 = R"doc()doc";

static const char *__doc_kp_Manager_imageT_4 = R"doc()doc";

static const char *__doc_kp_Manager_listDevices =
R"doc(List the devices available in the current vulkan instance.

Returns:
    vector of physical devices containing their respective properties)doc";

static const char *__doc_kp_Manager_mComputeQueueFamilyIndices = R"doc()doc";

static const char *__doc_kp_Manager_mComputeQueues = R"doc()doc";

static const char *__doc_kp_Manager_mDebugDispatcher = R"doc()doc";

static const char *__doc_kp_Manager_mDebugReportCallback = R"doc()doc";

static const char *__doc_kp_Manager_mDevice = R"doc()doc";

static const char *__doc_kp_Manager_mFreeDevice = R"doc()doc";

static const char *__doc_kp_Manager_mFreeInstance = R"doc()doc";

static const char *__doc_kp_Manager_mInstance = R"doc()doc";

static const char *__doc_kp_Manager_mManageResources = R"doc()doc";

static const char *__doc_kp_Manager_mManagedAlgorithms = R"doc()doc";

static const char *__doc_kp_Manager_mManagedImages = R"doc()doc";

static const char *__doc_kp_Manager_mManagedSequences = R"doc()doc";

static const char *__doc_kp_Manager_mManagedTensors = R"doc()doc";

static const char *__doc_kp_Manager_mPhysicalDevice = R"doc()doc";

static const char *__doc_kp_Manager_sequence =
R"doc(Create a managed sequence that will be destroyed by this manager if it
hasn't been destroyed by its reference count going to zero.

Parameter ``queueIndex``:
    The queue to use from the available queues

Parameter ``nrOfTimestamps``:
    The maximum number of timestamps to allocate. If zero (default),
    disables latching of timestamps.

Returns:
    Shared pointer with initialised sequence)doc";

static const char *__doc_kp_Manager_tensor = R"doc()doc";

static const char *__doc_kp_Manager_tensor_2 = R"doc()doc";

static const char *__doc_kp_Manager_tensor_3 = R"doc()doc";

static const char *__doc_kp_Manager_tensorT =
R"doc(Create a managed tensor that will be destroyed by this manager if it
hasn't been destroyed by its reference count going to zero.

Parameter ``data``:
    The data to initialize the tensor with

Parameter ``tensorType``:
    The type of tensor to initialize

Returns:
    Shared pointer with initialised tensor)doc";

static const char *__doc_kp_Manager_tensorT_2 =
R"doc(Create a managed tensor that will be destroyed by this manager if it
hasn't been destroyed by its reference count going to zero.

Parameter ``size``:
    The number of element in this tensor

Parameter ``tensorType``:
    The type of tensor to initialize

Returns:
    Shared pointer with initialised tensor)doc";

static const char *__doc_kp_Memory = R"doc()doc";

static const char *__doc_kp_Memory_MemoryTypes =
R"doc(Type for memory created: Device allows memory to be transferred from
staging memory. Staging are host memory visible. Storage are device
visible but are not set up to transfer or receive data (only for
shader storage).)doc";

static const char *__doc_kp_Memory_MemoryTypes_eDevice = R"doc(< Type is device memory, source and destination)doc";

static const char *__doc_kp_Memory_MemoryTypes_eDeviceAndHost = R"doc(< Type is host-visible and host-coherent device memory)doc";

static const char *__doc_kp_Memory_MemoryTypes_eHost = R"doc(< Type is host memory, source and destination)doc";

static const char *__doc_kp_Memory_MemoryTypes_eStorage = R"doc(< Type is Device memory (only))doc";

static const char *__doc_kp_Memory_constructDescriptorSet =
R"doc(Adds this object to a Vulkan descriptor set at \p binding.

Parameter ``descriptorSet``:
    The descriptor set to add to.

Parameter ``binding``:
    The binding number to use.

Returns:
    Add this object to a descriptor set at \p binding.)doc";

static const char *__doc_kp_Memory_data =
R"doc(Template to return the pointer data converted by specific type, which
would be any of the supported types including float, double, int32,
uint32 and bool.

Returns:
    Pointer to raw memory containing raw bytes data of Tensor/Image.)doc";

static const char *__doc_kp_Memory_data_2 =
R"doc(Return the pointer data cast to float.

Returns:
    Pointer to raw memory containing raw bytes data of Tensor/Image.
    This is the default, for convenience.)doc";

static const char *__doc_kp_Memory_dataTypeMemorySize =
R"doc(Returns the total size of a single element of the respective data type
that this memory object holds.

Returns:
    Unsigned integer representing the memory of a single element of
    the respective data type.)doc";

static const char *__doc_kp_Memory_getDescriptorType = R"doc()doc";

static const char *__doc_kp_Memory_isInit =
R"doc(Check whether tensor/image is initialized based on the created gpu
resources.

Returns:
    Boolean stating whether tensor is initialized)doc";

static const char *__doc_kp_Memory_mDataTypeMemorySize = R"doc()doc";

static const char *__doc_kp_Memory_mDescriptorType = R"doc()doc";

static const char *__doc_kp_Memory_mDevice = R"doc()doc";

static const char *__doc_kp_Memory_mFreePrimaryMemory = R"doc()doc";

static const char *__doc_kp_Memory_mFreeStagingMemory = R"doc()doc";

static const char *__doc_kp_Memory_mMemoryType = R"doc()doc";

static const char *__doc_kp_Memory_mPhysicalDevice = R"doc()doc";

static const char *__doc_kp_Memory_mPrimaryMemory = R"doc()doc";

static const char *__doc_kp_Memory_mRawData = R"doc()doc";

static const char *__doc_kp_Memory_mSize = R"doc()doc";

static const char *__doc_kp_Memory_mStagingMemory = R"doc()doc";

static const char *__doc_kp_Memory_mUnmapMemory = R"doc()doc";

static const char *__doc_kp_Memory_mapRawData = R"doc()doc";

static const char *__doc_kp_Memory_memorySize =
R"doc(Returns the total memory size of the data contained by the memory
object which would equate to (this->size() *
this->dataTypeMemorySize())

Returns:
    Unsigned integer representing the total memory size of the data
    contained by the image object.)doc";

static const char *__doc_kp_Memory_memoryType =
R"doc(Retrieve the image type of the image

Returns:
    image type of image)doc";

static const char *__doc_kp_Memory_rawData =
R"doc(Retrieve the raw data via the pointer to the memory that contains the
raw memory of this current tensor. This tensor gets changed to a
nullptr when the Tensor is removed.

Returns:
    Pointer to raw memory containing raw bytes data of Tensor/Image.)doc";

static const char *__doc_kp_Memory_recordCopyFromDeviceToStaging =
R"doc(Records a copy from the internal device memory to the staging memory
using an optional barrier to wait for the operation. This function
would only be relevant for kp::Tensors of type eDevice.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into)doc";

static const char *__doc_kp_Memory_recordCopyFromStagingToDevice =
R"doc(Records a copy from the internal staging memory to the device memory
using an optional barrier to wait for the operation. This function
would only be relevant for kp::Tensors of type eDevice.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into)doc";

static const char *__doc_kp_Memory_recordPrimaryMemoryBarrier =
R"doc(Records the buffer memory barrier into the primary buffer and command
buffer which ensures that relevant data transfers are carried out
correctly.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``srcAccessMask``:
    Access flags for source access mask

Parameter ``dstAccessMask``:
    Access flags for destination access mask

Parameter ``scrStageMask``:
    Pipeline stage flags for source stage mask

Parameter ``dstStageMask``:
    Pipeline stage flags for destination stage mask)doc";

static const char *__doc_kp_Memory_recordStagingMemoryBarrier =
R"doc(Records the buffer memory barrier into the staging buffer and command
buffer which ensures that relevant data transfers are carried out
correctly.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``srcAccessMask``:
    Access flags for source access mask

Parameter ``dstAccessMask``:
    Access flags for destination access mask

Parameter ``scrStageMask``:
    Pipeline stage flags for source stage mask

Parameter ``dstStageMask``:
    Pipeline stage flags for destination stage mask)doc";

static const char *__doc_kp_Memory_setRawData =
R"doc(Sets / resets the data of the tensor/image which is directly done on
the GPU host visible memory available by the tensor.)doc";

static const char *__doc_kp_Memory_size =
R"doc(Returns the size/magnitude of the Tensor/Image, which will be the
total number of elements across all dimensions

Returns:
    Unsigned integer representing the total number of elements)doc";

static const char *__doc_kp_Memory_toString = R"doc()doc";

static const char *__doc_kp_Memory_unmapRawData = R"doc()doc";

static const char *__doc_kp_Memory_vector =
R"doc(Template to get the data of the current tensor/image as a vector of
specific type, which would be any of the supported types including
float, double, int32, uint32 and bool.

Returns:
    Vector of type provided by template.)doc";

static const char *__doc_kp_Memory_vector_2 =
R"doc(Get the data of the current tensor/image as a vector of float. This is
the default, for convenience.

Returns:
    Vector of floats.)doc";

static const char *__doc_kp_OpAlgoDispatch =
R"doc(Operation that provides a general abstraction that simplifies the use
of algorithm and parameter components which can be used with shaders.
By default it enables the user to provide a dynamic number of tensors
which are then passed as inputs.)doc";

static const char *__doc_kp_OpAlgoDispatch_OpAlgoDispatch =
R"doc(Constructor that stores the algorithm to use as well as the relevant
push constants to override when recording.

Parameter ``algorithm``:
    The algorithm object to use for dispatch

Parameter ``pushConstants``:
    The push constants to use for override)doc";

static const char *__doc_kp_OpAlgoDispatch_mAlgorithm = R"doc()doc";

static const char *__doc_kp_OpAlgoDispatch_mPushConstantsData = R"doc()doc";

static const char *__doc_kp_OpAlgoDispatch_mPushConstantsDataTypeMemorySize = R"doc()doc";

static const char *__doc_kp_OpAlgoDispatch_mPushConstantsSize = R"doc()doc";

static const char *__doc_kp_OpAlgoDispatch_postEval =
R"doc(Does not perform any postEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpAlgoDispatch_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpAlgoDispatch_record =
R"doc(This records the commands that are to be sent to the GPU. This
includes the barriers that ensure the memory has been copied before
going in and out of the shader, as well as the dispatch operation that
sends the shader processing to the gpu. This function also records the
GPU memory copy of the output data for the staging buffer so it can be
read by the host.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpBase =
R"doc(Base Operation which provides the high level interface that Kompute
operations implement in order to perform a set of actions in the GPU.

Operations can perform actions on tensors, and optionally can also own
an Algorithm with respective parameters. kp::Operations with
kp::Algorithms would inherit from kp::OpBaseAlgo.)doc";

static const char *__doc_kp_OpBase_postEval =
R"doc(Post eval is called after the Sequence has called eval and submitted
the commands to the GPU for processing, and can be used to perform any
tear-down steps required as the computation iteration finishes. It's
worth noting that there are situations where eval can be called
multiple times, so the resources that are destroyed should not require
a re-init unless explicitly provided by the user.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpBase_preEval =
R"doc(Pre eval is called before the Sequence has called eval and submitted
the commands to the GPU for processing, and can be used to perform any
per-eval setup steps required as the computation iteration begins.
It's worth noting that there are situations where eval can be called
multiple times, so the resources that are created should be idempotent
in case it's called multiple times in a row.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpBase_record =
R"doc(The record function is intended to only send a record command or run
commands that are expected to record operations that are to be
submitted as a batch into the GPU.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageCopy =
R"doc(Operation that copies the data from the first image to the rest of the
images provided, using a record command for all the vectors. This
operation does not own/manage the memory of the images passed to it.
The operation must only receive images of type)doc";

static const char *__doc_kp_OpImageCopy_OpImageCopy =
R"doc(Default constructor with parameters that provides the core vulkan
resources and the images that will be used in the operation.

Parameter ``images``:
    Images that will be used to create in operation.)doc";

static const char *__doc_kp_OpImageCopy_mImages = R"doc()doc";

static const char *__doc_kp_OpImageCopy_postEval =
R"doc(Copies the local vectors for all the images to sync the data with the
gpu.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageCopy_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageCopy_record =
R"doc(Records the copy commands from the first image into all the other
images provided. Also optionally records a barrier.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageSyncDevice =
R"doc(Operation that syncs image's device by mapping local data into the
device memory. For ImageTypes::eDevice it will use a record operation
for the memory to be syncd into GPU memory which means that the
operation will be done in sync with GPU commands. For
ImageTypes::eHost it will only map the data into host memory which
will happen during preEval before the recorded commands are
dispatched.)doc";

static const char *__doc_kp_OpImageSyncDevice_OpImageSyncDevice =
R"doc(Default constructor with parameters that provides the core vulkan
resources and the images that will be used in the operation. The
tensos provided cannot be of type ImageTypes::eStorage.

Parameter ``images``:
    Images that will be used to create in operation.)doc";

static const char *__doc_kp_OpImageSyncDevice_mImages = R"doc()doc";

static const char *__doc_kp_OpImageSyncDevice_postEval =
R"doc(Does not perform any postEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageSyncDevice_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageSyncDevice_record =
R"doc(For device images, it records the copy command for the image to copy
the data from its staging to device memory.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageSyncLocal =
R"doc(Operation that syncs image's local memory by mapping device data into
the local CPU memory. For ImageTypes::eDevice it will use a record
operation for the memory to be syncd into GPU memory which means that
the operation will be done in sync with GPU commands. For
ImageTypes::eHost it will only map the data into host memory which
will happen during preEval before the recorded commands are
dispatched.)doc";

static const char *__doc_kp_OpImageSyncLocal_OpImageSyncLocal =
R"doc(Default constructor with parameters that provides the core vulkan
resources and the images that will be used in the operation. The
images provided cannot be of type ImageTypes::eStorage.

Parameter ``images``:
    Images that will be used to create in operation.)doc";

static const char *__doc_kp_OpImageSyncLocal_mImages = R"doc()doc";

static const char *__doc_kp_OpImageSyncLocal_postEval =
R"doc(For host images it performs the map command from the host memory into
local memory.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageSyncLocal_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpImageSyncLocal_record =
R"doc(For device images, it records the copy command for the image to copy
the data from its device to staging memory.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpMemoryBarrier =
R"doc(Operation that provides a general abstraction that simplifies the use
of algorithm and parameter components which can be used with shaders.
It exposes the pipeline barrier functionality specifically for memory
barriers that can be configured through the respective source and
destination masks)doc";

static const char *__doc_kp_OpMemoryBarrier_OpMemoryBarrier =
R"doc(Constructor that stores mem objects as well as memory barrier
parameters to be used to create a pipeline barrier on the respective
primary or staging tensor.

Parameter ``memObjects``:
    The mem objects to apply the memory barriers on

Parameter ``srcAccessMask``:
    The kp::AccessFlagBits for the source access mask

Parameter ``dstAccessMask``:
    The kp::AccessFlagBits for the destination access mask

Parameter ``srcStageMask``:
    The kp::PipelineStageFlagBits for the source stage mask

Parameter ``dstStageMask``:
    The kp::PipelineStageFlagBits for the destination stage mask

Parameter ``barrierOnPrimary``:
    Boolean to select primary or secondary buffers on mem objects)doc";

static const char *__doc_kp_OpMemoryBarrier_mBarrierOnPrimary = R"doc()doc";

static const char *__doc_kp_OpMemoryBarrier_mDstAccessMask = R"doc()doc";

static const char *__doc_kp_OpMemoryBarrier_mDstStageMask = R"doc()doc";

static const char *__doc_kp_OpMemoryBarrier_mMemObjects = R"doc()doc";

static const char *__doc_kp_OpMemoryBarrier_mSrcAccessMask = R"doc()doc";

static const char *__doc_kp_OpMemoryBarrier_mSrcStageMask = R"doc()doc";

static const char *__doc_kp_OpMemoryBarrier_postEval =
R"doc(Does not perform any postEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpMemoryBarrier_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpMemoryBarrier_record =
R"doc(This records the memory barrier with the access and stage masks
provided across all relevant tensors.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpMult =
R"doc(Operation that performs multiplication on two tensors and outpus on
third tensor.)doc";

static const char *__doc_kp_OpMult_OpMult =
R"doc(Default constructor with parameters that provides the bare minimum
requirements for the operations to be able to create and manage their
sub-components.

Parameter ``memObjects``:
    Memory objects that are to be used in this operation

Parameter ``algorithm``:
    An algorithm that will be overridden with the OpMult shader data
    and the tensors provided which are expected to be 3)doc";

static const char *__doc_kp_OpTensorCopy =
R"doc(Operation that copies the data from the first tensor to the rest of
the tensors provided, using a record command for all the vectors. This
operation does not own/manage the memory of the tensors passed to it.
The operation must only receive tensors of type)doc";

static const char *__doc_kp_OpTensorCopy_OpTensorCopy =
R"doc(Default constructor with parameters that provides the core vulkan
resources and the tensors that will be used in the operation.

Parameter ``tensors``:
    Tensors that will be used to create in operation.)doc";

static const char *__doc_kp_OpTensorCopy_mTensors = R"doc()doc";

static const char *__doc_kp_OpTensorCopy_postEval =
R"doc(Copies the local vectors for all the tensors to sync the data with the
gpu.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorCopy_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorCopy_record =
R"doc(Records the copy commands from the first tensor into all the other
tensors provided. Also optionally records a barrier.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorSyncDevice =
R"doc(Operation that syncs tensor's device by mapping local data into the
device memory. For TensorTypes::eDevice it will use a record operation
for the memory to be syncd into GPU memory which means that the
operation will be done in sync with GPU commands. For
TensorTypes::eHost it will only map the data into host memory which
will happen during preEval before the recorded commands are
dispatched.)doc";

static const char *__doc_kp_OpTensorSyncDevice_OpTensorSyncDevice =
R"doc(Default constructor with parameters that provides the core vulkan
resources and the tensors that will be used in the operation. The
tensos provided cannot be of type TensorTypes::eStorage.

Parameter ``tensors``:
    Tensors that will be used to create in operation.)doc";

static const char *__doc_kp_OpTensorSyncDevice_mTensors = R"doc()doc";

static const char *__doc_kp_OpTensorSyncDevice_postEval =
R"doc(Does not perform any postEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorSyncDevice_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorSyncDevice_record =
R"doc(For device tensors, it records the copy command for the tensor to copy
the data from its staging to device memory.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorSyncLocal =
R"doc(Operation that syncs mem object's local memory by mapping device data
into the local CPU memory. For MemoryTypes::eDevice it will use a
record operation for the memory to be syncd into GPU memory which
means that the operation will be done in sync with GPU commands. For
MemoryTypes::eHost it will only map the data into host memory which
will happen during preEval before the recorded commands are
dispatched.)doc";

static const char *__doc_kp_OpTensorSyncLocal_OpTensorSyncLocal =
R"doc(Default constructor with parameters that provides the core vulkan
resources and the memory that will be used in the operation. The
memory provided cannot be of type MemoryTypes::eStorage.

Parameter ``tensors``:
    Tensors that will be used to create in operation.)doc";

static const char *__doc_kp_OpTensorSyncLocal_mTensors = R"doc()doc";

static const char *__doc_kp_OpTensorSyncLocal_postEval =
R"doc(For host tensors it performs the map command from the host memory into
local memory.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorSyncLocal_preEval =
R"doc(Does not perform any preEval commands.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_OpTensorSyncLocal_record =
R"doc(For device tensors, it records the copy command for the tensor to copy
the data from its device to staging memory.

Parameter ``commandBuffer``:
    The command buffer to record the command into.)doc";

static const char *__doc_kp_Sequence = R"doc(Container of operations that can be sent to GPU as batch)doc";

static const char *__doc_kp_Sequence_Sequence =
R"doc(Main constructor for sequence which requires core vulkan components to
generate all dependent resources.

Parameter ``physicalDevice``:
    Vulkan physical device

Parameter ``device``:
    Vulkan logical device

Parameter ``computeQueue``:
    Vulkan compute queue

Parameter ``queueIndex``:
    Vulkan compute queue index in device

Parameter ``totalTimestamps``:
    Maximum number of timestamps to allocate)doc";

static const char *__doc_kp_Sequence_begin =
R"doc(Begins recording commands for commands to be submitted into the
command buffer.)doc";

static const char *__doc_kp_Sequence_clear =
R"doc(Clear function clears all operations currently recorded and starts
recording again.)doc";

static const char *__doc_kp_Sequence_createCommandBuffer = R"doc()doc";

static const char *__doc_kp_Sequence_createCommandPool = R"doc()doc";

static const char *__doc_kp_Sequence_createTimestampQueryPool = R"doc()doc";

static const char *__doc_kp_Sequence_destroy =
R"doc(Destroys and frees the GPU resources which include the buffer and
memory and sets the sequence as init=False.)doc";

static const char *__doc_kp_Sequence_end =
R"doc(Ends the recording and stops recording commands when the record
command is sent.)doc";

static const char *__doc_kp_Sequence_eval =
R"doc(Eval sends all the recorded and stored operations in the vector of
operations into the gpu as a submit job synchronously (with a
barrier).

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_eval_2 =
R"doc(Resets all the recorded and stored operations, records the operation
provided and submits into the gpu as a submit job synchronously (with
a barrier).

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_eval_3 =
R"doc(Eval sends all the recorded and stored operations in the vector of
operations into the gpu as a submit job with a barrier.

Parameter ``memObjects``:
    Vector of memory objects to use for the operation

Parameter ``TArgs``:
    Template parameters that are used to initialise operation which
    allows for extensible configurations on initialisation.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_eval_4 =
R"doc(Eval sends all the recorded and stored operations in the vector of
operations into the gpu as a submit job with a barrier.

Parameter ``algorithm``:
    Algorithm to use for the record often used for OpAlgo operations

Parameter ``TArgs``:
    Template parameters that are used to initialise operation which
    allows for extensible configurations on initialisation.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_evalAsync =
R"doc(Eval Async sends all the recorded and stored operations in the vector
of operations into the gpu as a submit job without a barrier.
EvalAwait() must ALWAYS be called after to ensure the sequence is
terminated correctly.

Returns:
    Boolean stating whether execution was successful.)doc";

static const char *__doc_kp_Sequence_evalAsync_2 =
R"doc(Clears currnet operations to record provided one in the vector of
operations into the gpu as a submit job without a barrier. EvalAwait()
must ALWAYS be called after to ensure the sequence is terminated
correctly.

Returns:
    Boolean stating whether execution was successful.)doc";

static const char *__doc_kp_Sequence_evalAsync_3 =
R"doc(Eval sends all the recorded and stored operations in the vector of
operations into the gpu as a submit job with a barrier.

Parameter ``memObjects``:
    Vector of memory objects to use for the operation

Parameter ``TArgs``:
    Template parameters that are used to initialise operation which
    allows for extensible configurations on initialisation.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_evalAsync_4 =
R"doc(Eval sends all the recorded and stored operations in the vector of
operations into the gpu as a submit job with a barrier.

Parameter ``algorithm``:
    Algorithm to use for the record often used for OpAlgo operations

Parameter ``TArgs``:
    Template parameters that are used to initialise operation which
    allows for extensible configurations on initialisation.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_evalAwait =
R"doc(Eval Await waits for the fence to finish processing and then once it
finishes, it runs the postEval of all operations.

Parameter ``waitFor``:
    Number of milliseconds to wait before timing out.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_getTimestamps =
R"doc(Return the timestamps that were latched at the beginning and after
each operation during the last eval() call.)doc";

static const char *__doc_kp_Sequence_isInit =
R"doc(Returns true if the sequence has been initialised, and it's based on
the GPU resources being referenced.

Returns:
    Boolean stating if is initialized)doc";

static const char *__doc_kp_Sequence_isRecording =
R"doc(Returns true if the sequence is currently in recording activated.

Returns:
    Boolean stating if recording ongoing.)doc";

static const char *__doc_kp_Sequence_isRunning =
R"doc(Returns true if the sequence is currently running - mostly used for
async workloads.

Returns:
    Boolean stating if currently running.)doc";

static const char *__doc_kp_Sequence_mCommandBuffer = R"doc()doc";

static const char *__doc_kp_Sequence_mCommandPool = R"doc()doc";

static const char *__doc_kp_Sequence_mComputeQueue = R"doc()doc";

static const char *__doc_kp_Sequence_mDevice = R"doc()doc";

static const char *__doc_kp_Sequence_mFence = R"doc()doc";

static const char *__doc_kp_Sequence_mFreeCommandBuffer = R"doc()doc";

static const char *__doc_kp_Sequence_mFreeCommandPool = R"doc()doc";

static const char *__doc_kp_Sequence_mIsRunning = R"doc()doc";

static const char *__doc_kp_Sequence_mOperations = R"doc()doc";

static const char *__doc_kp_Sequence_mPhysicalDevice = R"doc()doc";

static const char *__doc_kp_Sequence_mQueueIndex = R"doc()doc";

static const char *__doc_kp_Sequence_mRecording = R"doc()doc";

static const char *__doc_kp_Sequence_record =
R"doc(Record function for operation to be added to the GPU queue in batch.
This template requires classes to be derived from the OpBase class.
This function also requires the Sequence to be recording, otherwise it
will not be able to add the operation.

Parameter ``op``:
    Object derived from kp::BaseOp that will be recoreded by the
    sequence which will be used when the operation is evaluated.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_record_2 =
R"doc(Record function for operation to be added to the GPU queue in batch.
This template requires classes to be derived from the OpBase class.
This function also requires the Sequence to be recording, otherwise it
will not be able to add the operation.

Parameter ``memObjects``:
    Vector of mem objects to use for the operation

Parameter ``TArgs``:
    Template parameters that are used to initialise operation which
    allows for extensible configurations on initialisation.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_record_3 =
R"doc(Record function for operation to be added to the GPU queue in batch.
This template requires classes to be derived from the OpBase class.
This function also requires the Sequence to be recording, otherwise it
will not be able to add the operation.

Parameter ``algorithm``:
    Algorithm to use for the record often used for OpAlgo operations

Parameter ``TArgs``:
    Template parameters that are used to initialise operation which
    allows for extensible configurations on initialisation.

Returns:
    shared_ptr<Sequence> of the Sequence class itself)doc";

static const char *__doc_kp_Sequence_rerecord =
R"doc(Clears command buffer and triggers re-record of all the current
operations saved, which is useful if the underlying kp::Memorys or
kp::Algorithms are modified and need to be re-recorded.)doc";

static const char *__doc_kp_Sequence_timestampQueryPool = R"doc()doc";

static const char *__doc_kp_Tensor =
R"doc(Structured data used in GPU operations.

Tensors are the base building block in Kompute to perform operations
across GPUs. Each tensor would have a respective Vulkan memory and
buffer, which would be used to store their respective data. The
tensors can be used for GPU data storage or transfer.)doc";

static const char *__doc_kp_TensorT = R"doc()doc";

static const char *__doc_kp_TensorT_TensorT = R"doc()doc";

static const char *__doc_kp_TensorT_TensorT_2 = R"doc()doc";

static const char *__doc_kp_TensorT_dataType = R"doc()doc";

static const char *__doc_kp_TensorT_operator_array = R"doc()doc";

static const char *__doc_kp_TensorT_setData = R"doc()doc";

static const char *__doc_kp_TensorT_vector = R"doc()doc";

static const char *__doc_kp_Tensor_Tensor =
R"doc(Constructor with data provided which would be used to create the
respective vulkan buffer and memory.

Parameter ``physicalDevice``:
    The physical device to use to fetch properties

Parameter ``device``:
    The device to use to create the buffer and memory from

Parameter ``data``:
    Non-zero-sized vector of data that will be used by the tensor

Parameter ``tensorTypes``:
    Type for the tensor which is of type MemoryTypes)doc";

static const char *__doc_kp_Tensor_Tensor_2 =
R"doc(Constructor without data provided which would be used to create the
respective vulkan buffer and memory.

Parameter ``physicalDevice``:
    The physical device to use to fetch properties

Parameter ``device``:
    The device to use to create the buffer and memory from

Parameter ``tensorTypes``:
    Type for the tensor which is of type MemoryTypes)doc";

static const char *__doc_kp_Tensor_TensorDataTypes = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eBool = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eChar = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eCustom = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eDouble = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eFloat = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eInt = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eShort = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eUnsignedChar = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eUnsignedInt = R"doc()doc";

static const char *__doc_kp_Tensor_TensorDataTypes_eUnsignedShort = R"doc()doc";

static const char *__doc_kp_Tensor_allocateBindMemory = R"doc()doc";

static const char *__doc_kp_Tensor_allocateMemoryCreateGPUResources = R"doc()doc";

static const char *__doc_kp_Tensor_constructDescriptorBufferInfo = R"doc()doc";

static const char *__doc_kp_Tensor_constructDescriptorSet =
R"doc(Adds this object to a Vulkan descriptor set at \p binding.

Parameter ``descriptorSet``:
    The descriptor set to add to.

Parameter ``binding``:
    The binding number to use.

Returns:
    Add this object to a descriptor set at \p binding.)doc";

static const char *__doc_kp_Tensor_createBuffer = R"doc()doc";

static const char *__doc_kp_Tensor_dataType =
R"doc(Retrieve the data type of the tensor (host, device, storage)

Returns:
    Data type of tensor of type kp::Tensor::TensorDataTypes)doc";

static const char *__doc_kp_Tensor_destroy =
R"doc(Destroys and frees the GPU resources which include the buffer and
memory.)doc";

static const char *__doc_kp_Tensor_getPrimaryBufferUsageFlags = R"doc()doc";

static const char *__doc_kp_Tensor_getPrimaryMemoryPropertyFlags = R"doc()doc";

static const char *__doc_kp_Tensor_getStagingBufferUsageFlags = R"doc()doc";

static const char *__doc_kp_Tensor_getStagingMemoryPropertyFlags = R"doc()doc";

static const char *__doc_kp_Tensor_isInit =
R"doc(Check whether tensor is initialized based on the created gpu
resources.

Returns:
    Boolean stating whether tensor is initialized)doc";

static const char *__doc_kp_Tensor_mDataType = R"doc()doc";

static const char *__doc_kp_Tensor_mDescriptorBufferInfo = R"doc()doc";

static const char *__doc_kp_Tensor_mFreePrimaryBuffer = R"doc()doc";

static const char *__doc_kp_Tensor_mFreeStagingBuffer = R"doc()doc";

static const char *__doc_kp_Tensor_mPrimaryBuffer = R"doc()doc";

static const char *__doc_kp_Tensor_mStagingBuffer = R"doc()doc";

static const char *__doc_kp_Tensor_rebuild =
R"doc(Function to trigger reinitialisation of the tensor buffer and memory
with new data as well as new potential device type.

Parameter ``data``:
    Vector of data to use to initialise vector from

Parameter ``tensorType``:
    The type to use for the tensor)doc";

static const char *__doc_kp_Tensor_recordBufferMemoryBarrier = R"doc()doc";

static const char *__doc_kp_Tensor_recordCopyBuffer = R"doc()doc";

static const char *__doc_kp_Tensor_recordCopyFrom =
R"doc(Records a copy from the memory of the tensor provided to the current
tensor. This is intended to pass memory into a processing, to perform
a staging buffer transfer, or to gather output (between others).

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``copyFromTensor``:
    Tensor to copy the data from)doc";

static const char *__doc_kp_Tensor_recordCopyFromDeviceToStaging =
R"doc(Records a copy from the internal device memory to the staging memory
using an optional barrier to wait for the operation. This function
would only be relevant for kp::Tensors of type eDevice.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into)doc";

static const char *__doc_kp_Tensor_recordCopyFromStagingToDevice =
R"doc(Records a copy from the internal staging memory to the device memory
using an optional barrier to wait for the operation. This function
would only be relevant for kp::Tensors of type eDevice.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into)doc";

static const char *__doc_kp_Tensor_recordPrimaryMemoryBarrier =
R"doc(Records the memory barrier into the primary buffer and command buffer
which ensures that relevant data transfers are carried out correctly.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``srcAccessMask``:
    Access flags for source access mask

Parameter ``dstAccessMask``:
    Access flags for destination access mask

Parameter ``scrStageMask``:
    Pipeline stage flags for source stage mask

Parameter ``dstStageMask``:
    Pipeline stage flags for destination stage mask)doc";

static const char *__doc_kp_Tensor_recordStagingMemoryBarrier =
R"doc(Records the memory barrier into the staging buffer and command buffer
which ensures that relevant data transfers are carried out correctly.

Parameter ``commandBuffer``:
    Vulkan Command Buffer to record the commands into

Parameter ``srcAccessMask``:
    Access flags for source access mask

Parameter ``dstAccessMask``:
    Access flags for destination access mask

Parameter ``scrStageMask``:
    Pipeline stage flags for source stage mask

Parameter ``dstStageMask``:
    Pipeline stage flags for destination stage mask)doc";

static const char *__doc_kp_Tensor_toString = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

