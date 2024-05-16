// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"
#include "kompute/logger/Logger.hpp"

#include "shaders/Utils.hpp"

TEST(TestOpImageCopy, CopyDeviceToDeviceImage)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, 3, 1, 1);
    std::shared_ptr<kp::Memory> imageB = mgr.image(testVecB, 3, 1, 1);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageA, imageB })
      ->eval<kp::OpImageCopy>({ imageA, imageB })
      ->eval<kp::OpImageSyncLocal>({ imageA, imageB });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(imageA->vector(), imageB->vector());
}

TEST(TestOpImageCopy, CopyDeviceToDeviceImage2D)
{
    kp::Manager mgr;

    std::vector<float> testVecA;
    std::vector<float> testVecB;

    for(int i = 0; i < 256; i++)
    {
      testVecA.push_back(i);
      testVecB.push_back(0);
    }

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, 16, 16, 1);
    std::shared_ptr<kp::Memory> imageB = mgr.image(testVecB, 16, 16, 1);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageA, imageB })
      ->eval<kp::OpImageCopy>({ imageA, imageB })
      ->eval<kp::OpImageSyncLocal>({ imageA, imageB });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(imageA->vector(), imageB->vector());

    // Make sure that the vector matches the input vector
    for(int i = 0; i < 256; i++)
    {
      EXPECT_EQ(imageA->vector()[i], testVecA[i]);
      EXPECT_EQ(imageB->vector()[i], testVecA[i]);
    }
}

TEST(TestOpImageCopy, CopyDeviceToDeviceImageMulti)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 2, 3, 4 };
    std::vector<float> testVecB{ 0, 0, 0 };
    std::vector<float> testVecC{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, 3, 1, 1);
    std::shared_ptr<kp::Memory> imageB = mgr.image(testVecB, 3, 1, 1);
    std::shared_ptr<kp::Memory> tensorC = mgr.image(testVecC, 3, 1, 1);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());
    EXPECT_TRUE(tensorC->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncLocal>({ imageA, imageB, tensorC })
      ->eval<kp::OpImageCopy>({ imageA, imageB, tensorC });

    EXPECT_EQ(imageA->vector(), imageB->vector());
    EXPECT_EQ(imageA->vector(), tensorC->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB, tensorC });

    EXPECT_EQ(imageA->vector(), imageB->vector());
    EXPECT_EQ(imageA->vector(), tensorC->vector());
}

TEST(TestOpImageCopy, CopyDeviceToHostImage)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 3, 4, 5 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, 3, 1, 1);
    std::shared_ptr<kp::Memory> imageB =
      mgr.image(testVecB, 3, 1, 1, kp::Memory::MemoryTypes::eHost);

    //  Only calling sync on device type tensor
    mgr.sequence()->eval<kp::OpImageSyncDevice>({ imageA });

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()->eval<kp::OpImageCopy>({ imageA, imageB });

    EXPECT_EQ(imageA->vector(), imageB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB });
    EXPECT_EQ(imageA->vector(), imageB->vector());
}

TEST(TestOpImageCopy, CopyHostToDeviceImage)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 4, 5, 6 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA =
      mgr.image(testVecA, 3, 1, 1, kp::Memory::MemoryTypes::eHost);
    std::shared_ptr<kp::Memory> imageB = mgr.image(testVecB, 3, 1, 1);

    //  Only calling sync on device type tensor
    mgr.sequence()->eval<kp::OpImageSyncDevice>({ imageA, imageB });

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()->eval<kp::OpImageCopy>({ imageA, imageB });

    EXPECT_EQ(imageA->vector(), imageB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB });
    EXPECT_EQ(imageA->vector(), imageB->vector());
}

TEST(TestOpImageCopy, CopyHostToHostImage)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 5, 6, 7 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA =
      mgr.image(testVecA, 3, 1, 1, kp::Memory::MemoryTypes::eHost);
    std::shared_ptr<kp::Memory> imageB =
      mgr.image(testVecB, 3, 1, 1, kp::Memory::MemoryTypes::eHost);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageA })
      ->eval<kp::OpImageCopy>({ imageA, imageB });

    EXPECT_EQ(imageA->vector(), imageB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB });
    EXPECT_EQ(imageA->vector(), imageB->vector());
}

TEST(TestOpImageCopy, SingleImageShouldFail)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 6, 7, 8 };

    std::shared_ptr<kp::Memory> imageA =
      mgr.image(testVecA, 3, 1, 1, kp::Memory::MemoryTypes::eHost);

    EXPECT_TRUE(imageA->isInit());

    EXPECT_THROW(mgr.sequence()->eval<kp::OpImageCopy>({ imageA }),
                 std::runtime_error);
}

TEST(TestOpImageCopy, CopyThroughStorageImage)
{
    kp::Manager mgr;

    std::vector<float> testVecIn{ 9, 1, 3 };
    std::vector<float> testVecOut{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> ImageIn = mgr.image(testVecIn, 3, 1, 1);
    std::shared_ptr<kp::Memory> ImageOut = mgr.image(testVecOut, 3, 1, 1);
    // Image storage requires a vector to be passed only to reflect size
    std::shared_ptr<kp::Memory> tensorStorage =
      mgr.image({ 0, 0, 0 }, 3, 1, 1, kp::Memory::MemoryTypes::eStorage);

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ ImageIn, ImageOut })
      ->eval<kp::OpImageCopy>({ ImageIn, tensorStorage })
      ->eval<kp::OpImageCopy>({ tensorStorage, ImageOut })
      ->eval<kp::OpImageSyncLocal>({ ImageIn, ImageOut });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(ImageIn->vector(), ImageOut->vector());
}

TEST(TestOpImageCopy, CopyImageThroughStorageViaAlgorithms)
{
    kp::Manager mgr;

    std::vector<float> testVecIn{ 9, 1, 3 };
    std::vector<float> testVecOut{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> ImageIn = mgr.image(testVecIn, 3, 1, 1);
    std::shared_ptr<kp::Memory> ImageOut = mgr.image(testVecOut, 3, 1, 1);
    // Image storage requires a vector to be passed only to reflect size
    std::shared_ptr<kp::Memory> tensorStorage =
      mgr.image({ 0, 0, 0 }, 3, 1, 1, kp::Memory::MemoryTypes::eStorage);

    EXPECT_TRUE(ImageIn->isInit());
    EXPECT_TRUE(ImageOut->isInit());

    // Copy to storage tensor through algorithm
    std::string shaderA = (R"(
        #version 450

        layout (local_size_x = 1) in;

        // The input tensors bind index is relative to index in parameter passed
        layout(set = 0, binding = 0, r32f) uniform image2D image_in;
        layout(set = 0, binding = 1, r32f) uniform image2D image_out;

        void main() {
            uint index = gl_GlobalInvocationID.x;
            imageStore(image_out, ivec2(index, 0), imageLoad(image_in, ivec2(index, 0))) ;
        }
    )");

    auto algoA =
      mgr.algorithm({ ImageIn, tensorStorage }, compileSource(shaderA));

    // Copy from storage tensor to output tensor
    std::string shaderB = (R"(
        #version 450

        layout (local_size_x = 1) in;

        // The input tensors bind index is relative to index in parameter passed
        layout(set = 0, binding = 0, r32f) uniform image2D image_in;
        layout(set = 0, binding = 1, r32f) uniform image2D image_out;

        void main() {
            uint index = gl_GlobalInvocationID.x;
            imageStore(image_out, ivec2(index, 0), imageLoad(image_in, ivec2(index, 0))) ;
        }
    )");

    auto algoB =
      mgr.algorithm({ tensorStorage, ImageOut }, compileSource(shaderB));

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ ImageIn })
      ->eval<kp::OpAlgoDispatch>(algoA)
      ->eval<kp::OpAlgoDispatch>(algoB)
      ->eval<kp::OpImageSyncLocal>({ ImageOut });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(ImageIn->vector(), ImageOut->vector());
}

TEST(TestOpImageCopy, CopyDeviceToDeviceImageUninitialised)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, 3, 1, 1);
    std::shared_ptr<kp::Memory> imageB = mgr.image(3, 1, 1);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageA, imageB })
      ->eval<kp::OpImageCopy>({ imageA, imageB })
      ->eval<kp::OpImageSyncLocal>({ imageA, imageB });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(imageA->vector(), imageB->vector());
}

TEST(TestOpImageCopy, CopyImageThroughStorageViaAlgorithmsUninitialisedOutput)
{
    kp::Manager mgr;

    std::vector<float> testVecIn{ 9, 1, 3 };

    std::shared_ptr<kp::Memory> ImageIn = mgr.image(testVecIn, 3, 1, 1);
    std::shared_ptr<kp::Memory> ImageOut = mgr.imageT<float>(3, 1, 1);

    std::shared_ptr<kp::Memory> tensorStorage =
      mgr.image(3, 1, 1, kp::Memory::MemoryTypes::eStorage);

    EXPECT_TRUE(ImageIn->isInit());
    EXPECT_TRUE(ImageOut->isInit());

    // Copy to storage tensor through algorithm
    std::string shaderA = (R"(
        #version 450

        layout (local_size_x = 1) in;

        // The input tensors bind index is relative to index in parameter passed
        layout(set = 0, binding = 0, r32f) uniform image2D image_in;
        layout(set = 0, binding = 1, r32f) uniform image2D image_out;

        void main() {
            uint index = gl_GlobalInvocationID.x;
            imageStore(image_out, ivec2(index, 0), imageLoad(image_in, ivec2(index, 0))) ;
        }
    )");

    auto algoA =
      mgr.algorithm({ ImageIn, tensorStorage }, compileSource(shaderA));

    // Copy from storage tensor to output tensor
    std::string shaderB = (R"(
        #version 450

        layout (local_size_x = 1) in;

        // The input tensors bind index is relative to index in parameter passed
        layout(set = 0, binding = 0, r32f) uniform image2D image_in;
        layout(set = 0, binding = 1, r32f) uniform image2D image_out;

        void main() {
            uint index = gl_GlobalInvocationID.x;
            imageStore(image_out, ivec2(index, 0), imageLoad(image_in, ivec2(index, 0))) ;
        }
    )");

    auto algoB =
      mgr.algorithm({ tensorStorage, ImageOut }, compileSource(shaderB));

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ ImageIn })
      ->eval<kp::OpAlgoDispatch>(algoA)
      ->eval<kp::OpAlgoDispatch>(algoB)
      ->eval<kp::OpImageSyncLocal>({ ImageOut });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(ImageIn->vector(), ImageOut->vector());
}
