// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"
#include "kompute/logger/Logger.hpp"

TEST(TestTensor, ConstructorData)
{
    kp::Manager mgr;
    std::vector<float> vec{ 0, 1, 2 };
    std::shared_ptr<kp::TensorT<float>> tensor = mgr.tensor(vec);
    EXPECT_EQ(tensor->size(), vec.size());
    EXPECT_EQ(tensor->dataTypeMemorySize(), sizeof(float));
    EXPECT_EQ(tensor->vector(), vec);
}

TEST(TestTensor, ConstructorNoData)
{
    kp::Manager mgr;
    std::shared_ptr<kp::Tensor> tensor = mgr.tensor(
      nullptr, 3, sizeof(float), kp::Tensor::TensorDataTypes::eFloat);
    EXPECT_EQ(tensor->size(), 3);
    EXPECT_EQ(tensor->dataTypeMemorySize(), sizeof(float));

    std::shared_ptr<kp::Tensor> tensor2 =
      mgr.tensor(3, sizeof(float), kp::Tensor::TensorDataTypes::eFloat);
    EXPECT_EQ(tensor2->size(), 3);
    EXPECT_EQ(tensor2->dataTypeMemorySize(), sizeof(float));

    std::shared_ptr<kp::TensorT<float>> tensor3 = mgr.tensorT<float>(3);
    EXPECT_EQ(tensor3->size(), 3);
    EXPECT_EQ(tensor3->dataTypeMemorySize(), sizeof(float));
}

TEST(TestTensor, DataTypes)
{
    kp::Manager mgr;

    {
        std::vector<float> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<float>> tensor = mgr.tensor(vec);
        EXPECT_EQ(tensor->dataType(), kp::Tensor::TensorDataTypes::eFloat);
    }

    {
        std::vector<int8_t> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<int8_t>> tensor = mgr.tensorT(vec);
        EXPECT_EQ(tensor->dataType(), kp::Tensor::TensorDataTypes::eChar);
    }

    {
        std::vector<uint8_t> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<uint8_t>> tensor = mgr.tensorT(vec);
        EXPECT_EQ(tensor->dataType(),
                  kp::Tensor::TensorDataTypes::eUnsignedChar);
    }

    {
        std::vector<int16_t> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<int16_t>> tensor = mgr.tensorT(vec);
        EXPECT_EQ(tensor->dataType(), kp::Tensor::TensorDataTypes::eShort);
    }

    {
        std::vector<uint16_t> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<uint16_t>> tensor = mgr.tensorT(vec);
        EXPECT_EQ(tensor->dataType(),
                  kp::Tensor::TensorDataTypes::eUnsignedShort);
    }

    {
        std::vector<int32_t> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<int32_t>> tensor = mgr.tensorT(vec);
        EXPECT_EQ(tensor->dataType(), kp::Tensor::TensorDataTypes::eInt);
    }

    {
        std::vector<uint32_t> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<uint32_t>> tensor = mgr.tensorT(vec);
        EXPECT_EQ(tensor->dataType(),
                  kp::Tensor::TensorDataTypes::eUnsignedInt);
    }

    {
        std::vector<double> vec{ 0, 1, 2 };
        std::shared_ptr<kp::TensorT<double>> tensor = mgr.tensorT(vec);
        EXPECT_EQ(tensor->dataType(), kp::Tensor::TensorDataTypes::eDouble);
    }
}
