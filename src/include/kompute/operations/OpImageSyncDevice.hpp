// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "kompute/Core.hpp"
#include "kompute/Image.hpp"
#include "kompute/operations/OpBase.hpp"

namespace kp {

/**
 * Operation that syncs image's device by mapping local data into the device
 * memory. For ImageTypes::eDevice it will use a record operation for the
 * memory to be syncd into GPU memory which means that the operation will be
 * done in sync with GPU commands. For ImageTypes::eHost it will only map the
 * data into host memory which will happen during preEval before the recorded
 * commands are dispatched.
 */
class OpImageSyncDevice : public OpBase
{
  public:
    /**
     * Default constructor with parameters that provides the core vulkan
     * resources and the images that will be used in the operation. The tensos
     * provided cannot be of type ImageTypes::eStorage.
     *
     * @param images Images that will be used to create in operation.
     */
    OpImageSyncDevice(const std::vector<std::shared_ptr<Memory>>& images);

    /**
     * Default destructor. This class does not manage memory so it won't be
     * expecting the parent to perform a release.
     */
    ~OpImageSyncDevice() override;

    /**
     * For device images, it records the copy command for the image to copy
     * the data from its staging to device memory.
     *
     * @param commandBuffer The command buffer to record the command into.
     */
    void record(const vk::CommandBuffer& commandBuffer) override;

    /**
     * Does not perform any preEval commands.
     *
     * @param commandBuffer The command buffer to record the command into.
     */
    virtual void preEval(const vk::CommandBuffer& commandBuffer) override;

    /**
     * Does not perform any postEval commands.
     *
     * @param commandBuffer The command buffer to record the command into.
     */
    virtual void postEval(const vk::CommandBuffer& commandBuffer) override;

  private:
    // -------------- ALWAYS OWNED RESOURCES
    std::vector<std::shared_ptr<Image>> mImages;
};

} // End namespace kp
