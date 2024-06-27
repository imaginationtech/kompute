// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"
#include "kompute/logger/Logger.hpp"

#include "shaders/Utils.hpp"

TEST(TestOpTensorCopyToImage, CopyDeviceToDeviceTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensor = mgr.tensor(testVecA);
    std::shared_ptr<kp::Memory> image =
      mgr.image(testVecB, testVecB.size(), 1, 1);

    EXPECT_TRUE(tensor->isInit());
    EXPECT_TRUE(image->isInit());

    mgr.sequence()
      ->eval<kp::OpTensorSyncDevice>({ tensor })
      ->eval<kp::OpImageSyncDevice>({ image })
      ->eval<kp::OpTensorCopyToImage>({ tensor, image })
      ->eval<kp::OpTensorSyncLocal>({ tensor })
      ->eval<kp::OpImageSyncLocal>({ image });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(tensor->vector(), image->vector());
}

TEST(TestOpTensorCopyToImage, CopyDeviceToDeviceTensorMulti)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 2, 3, 4 };
    std::vector<float> testVecB{ 0, 0, 0 };
    std::vector<float> testVecC{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensorA = mgr.tensor(testVecA);
    std::shared_ptr<kp::Memory> imageB =
      mgr.image(testVecB, testVecB.size(), 1, 1);
    std::shared_ptr<kp::Memory> imageC =
      mgr.image(testVecC, testVecC.size(), 1, 1);

    EXPECT_TRUE(tensorA->isInit());
    EXPECT_TRUE(imageB->isInit());
    EXPECT_TRUE(imageC->isInit());

    mgr.sequence()
      ->eval<kp::OpTensorSyncLocal>({ tensorA })
      ->eval<kp::OpImageSyncDevice>({ imageB, imageC })
      ->eval<kp::OpTensorCopyToImage>({ tensorA, imageB, imageC });

    EXPECT_EQ(tensorA->vector(), imageB->vector());
    EXPECT_EQ(tensorA->vector(), imageC->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB, imageC });

    EXPECT_EQ(tensorA->vector(), imageB->vector());
    EXPECT_EQ(tensorA->vector(), imageC->vector());
}

TEST(TestOpTensorCopyToImage, CopyDeviceToHostTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 3, 4, 5 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensorA = mgr.tensor(testVecA);
    std::shared_ptr<kp::Memory> imageB = mgr.image(
      testVecB, testVecB.size(), 1, 1, kp::Memory::MemoryTypes::eHost);

    //  Only calling sync on device type tensor
    mgr.sequence()->eval<kp::OpTensorSyncDevice>({ tensorA });

    EXPECT_TRUE(tensorA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()->eval<kp::OpTensorCopyToImage>({ tensorA, imageB });

    EXPECT_EQ(tensorA->vector(), imageB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB });
    EXPECT_EQ(tensorA->vector(), imageB->vector());
}

TEST(TestOpTensorCopyToImage, CopyHostToDeviceTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 4, 5, 6 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensorA =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);
    std::shared_ptr<kp::Memory> imageB =
      mgr.image(testVecB, testVecB.size(), 1, 1);

    //  Only calling sync on device type tensor
    mgr.sequence()->eval<kp::OpTensorSyncDevice>({ tensorA });
    mgr.sequence()->eval<kp::OpImageSyncDevice>({ imageB });

    EXPECT_TRUE(tensorA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()->eval<kp::OpTensorCopyToImage>({ tensorA, imageB });

    EXPECT_EQ(tensorA->vector(), imageB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB });
    EXPECT_EQ(tensorA->vector(), imageB->vector());
}

TEST(TestOpTensorCopyToImage, CopyHostToHostTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 5, 6, 7 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensorA =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);
    std::shared_ptr<kp::Memory> imageB = mgr.image(
      testVecB, testVecB.size(), 1, 1, kp::Memory::MemoryTypes::eHost);

    EXPECT_TRUE(tensorA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()
      ->eval<kp::OpTensorSyncDevice>({ tensorA })
      ->eval<kp::OpTensorCopyToImage>({ tensorA, imageB });

    EXPECT_EQ(tensorA->vector(), imageB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageB });
    EXPECT_EQ(tensorA->vector(), imageB->vector());
}

TEST(TestOpTensorCopyToImage, CopyDeviceAndHostToDeviceAndHostTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensorA =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eDeviceAndHost);
    std::shared_ptr<kp::Memory> imageB = mgr.image(
      testVecB, testVecB.size(), 1, 1, kp::Memory::MemoryTypes::eDeviceAndHost);

    EXPECT_TRUE(tensorA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()
      ->eval<kp::OpTensorSyncDevice>({ tensorA })
      ->eval<kp::OpImageSyncDevice>({ imageB })
      ->eval<kp::OpTensorCopyToImage>({ tensorA, imageB })
      ->eval<kp::OpTensorSyncLocal>({ tensorA })
      ->eval<kp::OpImageSyncLocal>({ imageB });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(tensorA->vector(), imageB->vector());
}

TEST(TestOpTensorCopyToImage, SingleTensorShouldFail)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 6, 7, 8 };

    std::shared_ptr<kp::Memory> tensorA =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);

    EXPECT_TRUE(tensorA->isInit());

    EXPECT_THROW(mgr.sequence()->eval<kp::OpTensorCopyToImage>({ tensorA }),
                 std::runtime_error);
}

TEST(TestOpTensorCopyToImage, TensorsShouldFail)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 6, 7, 8 };

    std::shared_ptr<kp::Memory> tensorA =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);

    std::shared_ptr<kp::Memory> tensorB =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);

    EXPECT_THROW(
      mgr.sequence()->eval<kp::OpTensorCopyToImage>({ tensorA, tensorB }),
      std::runtime_error);
}

TEST(TestOpTensorCopyToImage, ImagesShouldFail)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 6, 7, 8 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(
      testVecA, testVecA.size(), 1, 1, kp::Memory::MemoryTypes::eHost);

    std::shared_ptr<kp::Memory> imageB = mgr.image(
      testVecA, testVecA.size(), 1, 1, kp::Memory::MemoryTypes::eHost);

    EXPECT_THROW(
      mgr.sequence()->eval<kp::OpTensorCopyToImage>({ imageA, imageB }),
      std::runtime_error);
}

TEST(TestOpTensorCopyToImage, CopyThroughStorageTensor)
{
    kp::Manager mgr;

    std::vector<float> testVecIn{ 9, 1, 3 };
    std::vector<float> testVecOut{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensorIn = mgr.tensor(testVecIn);
    std::shared_ptr<kp::Memory> imageOut =
      mgr.image(testVecOut, testVecOut.size(), 1, 1);
    // Tensor storage requires a vector to be passed only to reflect size
    std::shared_ptr<kp::Memory> tensorStorage =
      mgr.tensor({ 0, 0, 0 }, kp::Memory::MemoryTypes::eStorage);

    mgr.sequence()
      ->eval<kp::OpTensorSyncDevice>({ tensorIn })
      ->eval<kp::OpImageSyncDevice>({ imageOut })
      ->eval<kp::OpTensorCopy>({ tensorIn, tensorStorage })
      ->eval<kp::OpTensorCopyToImage>({ tensorStorage, imageOut })
      ->eval<kp::OpTensorSyncLocal>({ tensorIn })
      ->eval<kp::OpImageSyncLocal>({ imageOut });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(tensorIn->vector(), imageOut->vector());
}

TEST(TestOpTensorCopyToImage, CopyDeviceToDeviceImageUninitialised)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };

    std::shared_ptr<kp::Memory> tensorA = mgr.tensor(testVecA);
    std::shared_ptr<kp::Memory> imageB =
      mgr.imageT<float>(testVecA.size(), 1, 1);

    EXPECT_TRUE(tensorA->isInit());
    EXPECT_TRUE(imageB->isInit());

    mgr.sequence()
      ->eval<kp::OpTensorSyncDevice>({ tensorA })
      ->eval<kp::OpImageSyncDevice>({ imageB })
      ->eval<kp::OpTensorCopyToImage>({ tensorA, imageB })
      ->eval<kp::OpTensorSyncLocal>({
        tensorA,
      })
      ->eval<kp::OpImageSyncLocal>({ imageB });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(tensorA->vector(), imageB->vector());
}
