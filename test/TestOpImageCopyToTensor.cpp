// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"
#include "kompute/logger/Logger.hpp"

#include "shaders/Utils.hpp"

TEST(TestOpImageCopyToTensor, CopyDeviceToDeviceTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensor = mgr.tensor(testVecA);
    std::shared_ptr<kp::Memory> image = mgr.image(testVecB, testVecB.size(), 1, 1);

    EXPECT_TRUE(tensor->isInit());
    EXPECT_TRUE(image->isInit());

    mgr.sequence()
      ->eval<kp::OpTensorSyncDevice>({tensor})
      ->eval<kp::OpImageSyncDevice>({image})
      ->eval<kp::OpImageCopyToTensor>({ image, tensor })
      ->eval<kp::OpTensorSyncLocal>({tensor})
      ->eval<kp::OpImageSyncLocal>({image});

    // Making sure the GPU holds the same vector
    EXPECT_EQ(tensor->vector(), image->vector());
}

TEST(TestOpImageCopyToTensor, CopyDeviceToDeviceTensorMulti)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 2, 3, 4 };
    std::vector<float> testVecB{ 0, 0, 0 };
    std::vector<float> testVecC{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, testVecB.size(), 1, 1);
    std::shared_ptr<kp::Memory> tensorB = mgr.tensor(testVecB);
    std::shared_ptr<kp::Memory> tensorC = mgr.tensor(testVecC);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(tensorB->isInit());
    EXPECT_TRUE(tensorC->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncLocal>({ imageA })
      ->eval<kp::OpTensorSyncDevice>({ tensorB, tensorC })
      ->eval<kp::OpImageCopyToTensor>({ imageA, tensorB, tensorC });

    EXPECT_EQ(imageA->vector(), tensorB->vector());
    EXPECT_EQ(imageA->vector(), tensorC->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpTensorSyncLocal>({ tensorB, tensorC });

    EXPECT_EQ(imageA->vector(), tensorB->vector());
    EXPECT_EQ(imageA->vector(), tensorC->vector());
}

TEST(TestOpImageCopyToTensor, CopyDeviceToHostTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 3, 4, 5 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, testVecA.size(), 1, 1);
    std::shared_ptr<kp::Memory> tensorB =
      mgr.tensor(testVecB, kp::Memory::MemoryTypes::eHost);

    //  Only calling sync on device type tensor
    mgr.sequence()->eval<kp::OpImageSyncDevice>({ imageA });

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(tensorB->isInit());

    mgr.sequence()->eval<kp::OpImageCopyToTensor>({ imageA, tensorB });

    EXPECT_EQ(imageA->vector(), tensorB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpTensorSyncLocal>({ tensorB });
    EXPECT_EQ(imageA->vector(), tensorB->vector());
}

TEST(TestOpImageCopyToTensor, CopyHostToDeviceTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 4, 5, 6 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA =
      mgr.image(testVecA, testVecA.size(), 1, 1, kp::Memory::MemoryTypes::eHost);
    std::shared_ptr<kp::Memory> tensorB = mgr.tensor(testVecB);

    //  Only calling sync on device type tensor
    mgr.sequence()->eval<kp::OpImageSyncDevice>({ imageA });
    mgr.sequence()->eval<kp::OpTensorSyncDevice>({ tensorB });

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(tensorB->isInit());

    mgr.sequence()->eval<kp::OpImageCopyToTensor>({ imageA, tensorB });

    EXPECT_EQ(imageA->vector(), tensorB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpTensorSyncLocal>({ tensorB });
    EXPECT_EQ(imageA->vector(), tensorB->vector());
}

TEST(TestOpImageCopyToTensor, CopyHostToHostTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 5, 6, 7 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA =
      mgr.image(testVecA, testVecA.size(), 1, 1, kp::Memory::MemoryTypes::eHost);
    std::shared_ptr<kp::Memory> tensorB =
      mgr.tensor(testVecB, kp::Memory::MemoryTypes::eHost);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(tensorB->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageA })
      ->eval<kp::OpImageCopyToTensor>({ imageA, tensorB });

    EXPECT_EQ(imageA->vector(), tensorB->vector());

    // Making sure the GPU holds the same vector
    mgr.sequence()->eval<kp::OpTensorSyncLocal>({ tensorB });
    EXPECT_EQ(imageA->vector(), tensorB->vector());
}

TEST(TestOpImageCopyToTensor, CopyDeviceAndHostToDeviceAndHostTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };
    std::vector<float> testVecB{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, testVecA.size(), 1, 1, kp::Memory::MemoryTypes::eDeviceAndHost);
    std::shared_ptr<kp::Memory> tensorB = mgr.tensor(testVecB, kp::Memory::MemoryTypes::eDeviceAndHost);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(tensorB->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageA })
      ->eval<kp::OpTensorSyncDevice>({ tensorB })
      ->eval<kp::OpImageCopyToTensor>({ imageA, tensorB })
      ->eval<kp::OpImageSyncLocal>({ imageA })
      ->eval<kp::OpTensorSyncLocal>({ tensorB });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(imageA->vector(), tensorB->vector());
}

TEST(TestOpImageCopyToTensor, SingleTensorShouldFail)
{

    kp::Manager mgr;

    std::vector<float> testVecA{ 6, 7, 8 };

    std::shared_ptr<kp::Memory> imageA =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);

    EXPECT_TRUE(imageA->isInit());

    EXPECT_THROW(mgr.sequence()->eval<kp::OpImageCopyToTensor>({ imageA }),
                 std::runtime_error);
}

TEST(TestOpImageCopyToTensor, TensorsShouldFail)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 6, 7, 8 };

    std::shared_ptr<kp::Memory> tensorA =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);

    std::shared_ptr<kp::Memory> tensorB =
      mgr.tensor(testVecA, kp::Memory::MemoryTypes::eHost);

    EXPECT_THROW(mgr.sequence()->eval<kp::OpTensorCopyToImage>({ tensorA, tensorB }),
                 std::runtime_error);
}

TEST(TestOpImageCopyToTensor, ImagesShouldFail)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 6, 7, 8 };

    std::shared_ptr<kp::Memory> imageA =
      mgr.image(testVecA, testVecA.size(), 1, 1, kp::Memory::MemoryTypes::eHost);

    std::shared_ptr<kp::Memory> imageB =
      mgr.image(testVecA, testVecA.size(), 1, 1, kp::Memory::MemoryTypes::eHost);

    EXPECT_THROW(mgr.sequence()->eval<kp::OpTensorCopyToImage>({ imageA, imageB }),
                 std::runtime_error);
}

TEST(TestOpImageCopyToTensor, CopyThroughStorageTensor)
{
    kp::Manager mgr;

    std::vector<float> testVecIn{ 9, 1, 3 };
    std::vector<float> testVecOut{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> tensorOut = mgr.tensor(testVecOut);
    std::shared_ptr<kp::Memory> imageIn = mgr.image(testVecIn, testVecIn.size(), 1, 1);
    // Image storage requires a vector to be passed only to reflect size
    std::shared_ptr<kp::Memory> imageStorage =
      mgr.image(testVecIn.size(), 1, 1, kp::Memory::MemoryTypes::eStorage);

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageIn })
      ->eval<kp::OpTensorSyncDevice>({ tensorOut })
      ->eval<kp::OpImageCopy>({ imageIn, imageStorage })
      ->eval<kp::OpImageCopyToTensor>({ imageStorage, tensorOut })
      ->eval<kp::OpImageSyncLocal>({ imageIn })
      ->eval<kp::OpTensorSyncLocal>({ tensorOut });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(imageIn->vector(), tensorOut->vector());
}

TEST(TestOpImageCopyToTensor, CopyDeviceToDeviceImageUninitialised)
{
    kp::Manager mgr;

    std::vector<float> testVecA{ 1, 2, 3 };

    std::shared_ptr<kp::Memory> imageA = mgr.image(testVecA, testVecA.size(), 1, 1);
    std::shared_ptr<kp::Memory> tensorB = mgr.tensorT<float>(testVecA.size());

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(tensorB->isInit());

    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ imageA })
      ->eval<kp::OpTensorSyncDevice>({ tensorB })
      ->eval<kp::OpImageCopyToTensor>({ imageA, tensorB })
      ->eval<kp::OpImageSyncLocal>({ imageA, })
      ->eval<kp::OpTensorSyncLocal>({ tensorB });

    // Making sure the GPU holds the same vector
    EXPECT_EQ(imageA->vector(), tensorB->vector());
}
