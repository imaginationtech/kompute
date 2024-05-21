// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"
#include "kompute/logger/Logger.hpp"

TEST(TestOpImageCreate, CreateSingleImageSingleOp)
{
    std::vector<float> testVecA{ 9, 8, 7 };
    std::shared_ptr<kp::ImageT<float>> imageA = nullptr;

    {
        kp::Manager mgr;

        imageA = mgr.image(testVecA, 3, 1, 1);

        EXPECT_TRUE(imageA->isInit());
        EXPECT_EQ(imageA->vector(), testVecA);
    }

    EXPECT_FALSE(imageA->isInit());
}

TEST(TestOpImageCreate, NoErrorIfImageFreedBefore)
{
    std::vector<float> testVecA{ 9, 8, 7 };
    std::vector<float> testVecB{ 6, 5, 4 };

    kp::Manager mgr;

    std::shared_ptr<kp::ImageT<float>> imageA = mgr.image(testVecA, 1, 3, 1);
    std::shared_ptr<kp::ImageT<float>> imageB = mgr.image(testVecB, 3, 1, 1);

    EXPECT_EQ(imageA->vector(), testVecA);
    EXPECT_EQ(imageB->vector(), testVecB);

    imageA->destroy();
    imageB->destroy();

    EXPECT_FALSE(imageA->isInit());
    EXPECT_FALSE(imageB->isInit());
}

TEST(TestOpImageCreate, ExceptionOnZeroSizeImage)
{
    std::vector<float> testVecA;

    kp::Manager mgr;

    try {
        std::shared_ptr<kp::ImageT<float>> imageA =
          mgr.image(testVecA, 1, 1, 1);
    } catch (const std::runtime_error& err) {
        // check exception
        ASSERT_TRUE(std::string(err.what()).find("zero-sized") !=
                    std::string::npos);
    }
}

TEST(TestOpImageCreate, ExceptionOnInvalidLinearImage)
{
    std::vector<float> testVecA;

    kp::Manager mgr;

    try {
        std::shared_ptr<kp::ImageT<float>> imageA =
          mgr.image(testVecA, 1, 1, 1, kp::Memory::MemoryTypes::eDeviceAndHost, vk::ImageTiling::eLinear);
    } catch (const std::runtime_error& err) {
        // check exception
        ASSERT_TRUE(std::string(err.what()).find("linear tiling is only supported for") !=
                    std::string::npos);
    }

    try {
        std::shared_ptr<kp::ImageT<float>> imageA =
          mgr.image(testVecA, 1, 1, 1, kp::Memory::MemoryTypes::eHost, vk::ImageTiling::eLinear);
    } catch (const std::runtime_error& err) {
        // check exception
        ASSERT_TRUE(std::string(err.what()).find("linear tiling is only supported for") !=
                    std::string::npos);
    }
}
