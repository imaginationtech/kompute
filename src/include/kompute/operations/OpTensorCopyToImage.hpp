// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "kompute/Core.hpp"

#include "kompute/Tensor.hpp"

#include "kompute/operations/OpBase.hpp"

namespace kp {

/**
 * Operation that copies the data from the first tensor to the vector of images
 * provided, using a record command for all the vectors. This operation
 * does not own/manage the memory of the tensor/images passed to it.
 */
class OpTensorCopyToImage : public OpBase
{
  public:
    /**
     * Default constructor with parameters that provides the core vulkan
     * resources and the tensors/images that will be used in the operation.
     *
     * @param tensors Tensors that will be used to create in operation.
     */
    OpTensorCopyToImage(const std::vector<std::shared_ptr<Memory>>& mem_objects);

    /**
     * Default destructor. This class does not manage memory so it won't be
     * expecting the parent to perform a release.
     */
    ~OpTensorCopyToImage() override;

    /**
     * Records the copy commands from the first tensor into all the other
     * tensors provided. Also optionally records a barrier.
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
     * Copies the local vectors for all the tensors to sync the data with the
     * gpu.
     *
     * @param commandBuffer The command buffer to record the command into.
     */
    virtual void postEval(const vk::CommandBuffer& commandBuffer) override;

  private:
    // -------------- ALWAYS OWNED RESOURCES
    std::shared_ptr<Tensor> mTensor;
    std::vector<std::shared_ptr<Image>> mImages;
};

} // End namespace kp
