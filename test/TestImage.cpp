// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"
#include "kompute/logger/Logger.hpp"

TEST(TestImage, ConstructorData)
{
    kp::Manager mgr;
    std::vector<float> vec{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    std::shared_ptr<kp::ImageT<float>> image = mgr.image(vec, 3, 3, 1);
    EXPECT_EQ(image->size(), vec.size());
    EXPECT_EQ(image->dataTypeMemorySize(), sizeof(float));
    EXPECT_EQ(image->vector(), vec);
}

TEST(TestImage, ConstructorNoData)
{
    kp::Manager mgr;

    std::shared_ptr<kp::Image> image = mgr.image(nullptr, 3, 3, 1, kp::Image::ImageDataTypes::eF32);
    EXPECT_EQ(image->size(), 9);
    EXPECT_EQ(image->dataTypeMemorySize(), sizeof(float));

    std::shared_ptr<kp::Image> image2 = mgr.image(3, 3, 1, kp::Image::ImageDataTypes::eF32);
    EXPECT_EQ(image2->size(), 9);
    EXPECT_EQ(image2->dataTypeMemorySize(), sizeof(float));

    std::shared_ptr<kp::ImageT<float>> image3 = mgr.imageT<float>(3, 3, 1);
    EXPECT_EQ(image3->size(), 9);
    EXPECT_EQ(image3->dataTypeMemorySize(), sizeof(float));

    std::shared_ptr<kp::ImageT<float>> image4 = mgr.image(3, 3, 1);
    EXPECT_EQ(image4->size(), 9);
    EXPECT_EQ(image4->dataTypeMemorySize(), sizeof(float));
}

// FIXME: Could use a parameterised and/or templated test here to reduce code
// duplication.
TEST(TestImage, DataTypes)
{
    kp::Manager mgr;
    const int width = 3;
    const int height = 3;

    for (int numChannels = 1; numChannels <= 4; numChannels++) {
        {
            std::vector<float> vec(width * height * numChannels);
            std::shared_ptr<kp::ImageT<float>> image =
              mgr.image(vec, width, height, numChannels);
            EXPECT_EQ(image->dataType(), kp::Image::ImageDataTypes::eF32);
        }

        {
            std::vector<int32_t> vec(width * height * numChannels);
            std::shared_ptr<kp::ImageT<int32_t>> image =
              mgr.imageT(vec, width, height, numChannels);
            EXPECT_EQ(image->dataType(), kp::Image::ImageDataTypes::eS32);
        }

        {
            std::vector<uint32_t> vec(width * height * numChannels);
            std::shared_ptr<kp::ImageT<uint32_t>> image =
              mgr.imageT(vec, width, height, numChannels);
            EXPECT_EQ(image->dataType(), kp::Image::ImageDataTypes::eU32);
        }

        {
            std::vector<int16_t> vec(width * height * numChannels);
            std::shared_ptr<kp::ImageT<int16_t>> image =
              mgr.imageT(vec, width, height, numChannels);
            EXPECT_EQ(image->dataType(), kp::Image::ImageDataTypes::eS16);
        }

        {
            std::vector<uint16_t> vec(width * height * numChannels);
            std::shared_ptr<kp::ImageT<uint16_t>> image =
              mgr.imageT(vec, width, height, numChannels);
            EXPECT_EQ(image->dataType(), kp::Image::ImageDataTypes::eU16);
        }

        {
            std::vector<int8_t> vec(width * height * numChannels);
            std::shared_ptr<kp::ImageT<int8_t>> image =
              mgr.imageT(vec, width, height, numChannels);
            EXPECT_EQ(image->dataType(), kp::Image::ImageDataTypes::eS8);
        }

        {
            std::vector<uint8_t> vec(width * height * numChannels);
            std::shared_ptr<kp::ImageT<uint8_t>> image =
              mgr.imageT(vec, width, height, numChannels);
            EXPECT_EQ(image->dataType(), kp::Image::ImageDataTypes::eU8);
        }
    }
}

TEST(TestImage, InvalidVectorSize)
{
    kp::Manager mgr;
    std::vector<float> vec{ 0, 1, 2, 3 };

    // The vector is too small to initialise all the data in the image.
    EXPECT_THROW(mgr.image(vec, 3, 3, 1), std::runtime_error);
}

TEST(TestImage, LargeVectorSize)
{
    kp::Manager mgr;
    std::vector<float> vec{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

    // The same vector but only the first 4 elements
    std::vector<float> result(vec.begin(), vec.begin() + 4);

    // It's OK to initilaise an image from a vector which is larger than the
    // image.
    std::shared_ptr<kp::ImageT<float>> image = mgr.image(vec, 2, 2, 1);
    EXPECT_EQ(image->size(), 2 * 2 * 1);

    // The output should be equal to the result vector, not the input vector.
    EXPECT_EQ(image->vector(), result);
}
