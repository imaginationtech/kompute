#pragma once

#include "Algorithm.hpp"
#include "Core.hpp"
#include "Image.hpp"
#include "Manager.hpp"
#include "Sequence.hpp"
#include "Tensor.hpp"

#include "operations/OpAlgoDispatch.hpp"
#include "operations/OpBase.hpp"
#include "operations/OpImageCopy.hpp"
#include "operations/OpImageCopyToTensor.hpp"
#include "operations/OpImageSyncDevice.hpp"
#include "operations/OpImageSyncLocal.hpp"
#include "operations/OpMemoryBarrier.hpp"
#include "operations/OpMult.hpp"
#include "operations/OpTensorCopy.hpp"
#include "operations/OpTensorCopyToImage.hpp"
#include "operations/OpTensorSyncDevice.hpp"
#include "operations/OpTensorSyncLocal.hpp"

// Will be build by CMake and placed inside the build directory
#include "ShaderLogisticRegression.hpp"
#include "ShaderOpMult.hpp"
