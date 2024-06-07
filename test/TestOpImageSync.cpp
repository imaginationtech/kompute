// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"
#include "kompute/logger/Logger.hpp"

TEST(TestOpImageSync, SyncToDeviceMemorySingleImage)
{
    kp::Manager mgr;

    std::vector<float> testVecPreA{ 0, 0, 0 };
    std::vector<float> testVecPostA{ 9, 8, 7 };

    std::shared_ptr<kp::ImageT<float>> imageA = mgr.image(testVecPreA, 3, 1, 1);

    EXPECT_TRUE(imageA->isInit());

    imageA->setData(testVecPostA);

    mgr.sequence()->eval<kp::OpImageSyncDevice>({ imageA });

    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageA });

    EXPECT_EQ(imageA->vector(), testVecPostA);
}

TEST(TestOpImageSync, SyncToDeviceMemoryMultiImage)
{

    kp::Manager mgr;

    std::vector<float> testVec{ 9, 8, 7 };

    std::shared_ptr<kp::ImageT<float>> imageA = mgr.image({ 0, 0, 0 }, 3, 1, 1);
    std::shared_ptr<kp::ImageT<float>> imageB = mgr.image({ 0, 0, 0 }, 3, 1, 1);
    std::shared_ptr<kp::ImageT<float>> imageC = mgr.image({ 0, 0, 0 }, 3, 1, 1);

    EXPECT_TRUE(imageA->isInit());
    EXPECT_TRUE(imageB->isInit());
    EXPECT_TRUE(imageC->isInit());

    imageA->setData(testVec);

    mgr.sequence()->eval<kp::OpImageSyncDevice>({ imageA });

    mgr.sequence()->eval<kp::OpImageCopy>({ imageA, imageB, imageC });

    mgr.sequence()->eval<kp::OpImageSyncLocal>({ imageA, imageB, imageC });

    EXPECT_EQ(imageA->vector(), testVec);
    EXPECT_EQ(imageB->vector(), testVec);
    EXPECT_EQ(imageC->vector(), testVec);
}

TEST(TestOpImageSync, NegativeUnrelatedImageSync)
{
    kp::Manager mgr;

    std::vector<float> testVecIn{ 9, 1, 3 };
    std::vector<float> testVecOut{ 0, 0, 0 };

    std::shared_ptr<kp::Memory> ImageIn = mgr.image(testVecIn, 3, 1, 1);
    std::shared_ptr<kp::Memory> ImageOut = mgr.image(testVecOut, 3, 1, 1);

    EXPECT_TRUE(ImageIn->isInit());
    EXPECT_TRUE(ImageOut->isInit());

    // Syncing one image to the device should not update an unrelated one
    mgr.sequence()
      ->eval<kp::OpImageSyncDevice>({ ImageIn })
      ->eval<kp::OpImageSyncLocal>({ ImageOut });

    // Making sure the GPU holds the same vector
    EXPECT_NE(ImageIn->vector(), ImageOut->vector());
}

TEST(TestOpImageSync, TensorShouldFail)
{
    kp::Manager mgr;

    std::vector<float> testVecPreA{ 0, 0, 0 };

    std::shared_ptr<kp::TensorT<float>> tensor = mgr.tensor(testVecPreA);

    EXPECT_THROW(mgr.sequence()->eval<kp::OpImageSyncDevice>({ tensor }),
                 std::runtime_error);

    EXPECT_THROW(mgr.sequence()->eval<kp::OpImageSyncLocal>({ tensor }),
                 std::runtime_error);
}
