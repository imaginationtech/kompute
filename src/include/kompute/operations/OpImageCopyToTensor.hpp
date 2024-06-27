// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "kompute/Core.hpp"

#include "kompute/Image.hpp"

#include "kompute/operations/OpBase.hpp"

namespace kp {

/**
 * Operation that copies the data from the first image to the rest of the
 * images provided, using a record command for all the vectors. This operation
 * does not own/manage the memory of the images passed to it. The operation
 * must only receive images of type
 */
class OpImageCopyToTensor : public OpBase
{
  public:
    /**
     * Default constructor with parameters that provides the core vulkan
     * resources and the images that will be used in the operation.
     *
     * @param images Images that will be used to create in operation.
     */
    OpImageCopyToTensor(const std::vector<std::shared_ptr<Memory>>& mem_objects);

    /**
     * Default destructor. This class does not manage memory so it won't be
     * expecting the parent to perform a release.
     */
    ~OpImageCopyToTensor() override;

    /**
     * Records the copy commands from the first image into all the other
     * images provided. Also optionally records a barrier.
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
     * Copies the local vectors for all the images to sync the data with the
     * gpu.
     *
     * @param commandBuffer The command buffer to record the command into.
     */
    virtual void postEval(const vk::CommandBuffer& commandBuffer) override;

  private:
    // -------------- ALWAYS OWNED RESOURCES
    std::shared_ptr<Image> mImage;
    std::vector<std::shared_ptr<Tensor>> mTensors;
};

} // End namespace kp
