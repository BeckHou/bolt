// Copyright (C) 2019. Huawei Technologies Co., Ltd. All rights reserved.

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "cpu/arm/tensor_computing_arm.h"
#ifdef _USE_FP32
#include "cpu/arm/fp32/tensor_computing_fp32.h"
#endif
#ifdef _USE_FP16
#include "cpu/arm/fp16/tensor_computing_fp16.h"
#endif

EE scale_arm(TensorDesc inputDesc, void* input,
    I32 axis, void *alpha, void *beta,
    TensorDesc outputDesc, void* output)
{
    UNUSED(outputDesc);
    U32 length = tensorNumElements(inputDesc);
    axis = (axis + inputDesc.nDims) % inputDesc.nDims;
    I32 in = inputDesc.dims[inputDesc.nDims - 1];
    I32 ic = inputDesc.dims[inputDesc.nDims - 1 - axis];
    I32 elements_per_channel = length / (in * ic);
    if (inputDesc.df == DF_NCHWC8)
        axis = inputDesc.nDims;
    EE ret = SUCCESS;
    switch (inputDesc.dt) {
#ifdef _USE_FP32
        case DT_F32: {
            ret = scale_fp32((F32*)input, 
                axis, inputDesc.nDims, (F32*)alpha, (F32*)beta, in, ic, elements_per_channel,
                (F32*)output);
            break;
        }
#endif
#ifdef _USE_FP16
        case DT_F16: {
            ret = scale_fp16((F16*)input, 
                axis, inputDesc.nDims, (F16*)alpha, (F16*)beta, in, ic, elements_per_channel,
                (F16*)output);
            break;
        }
#endif
        default:
            ret = NOT_SUPPORTED;
            break;
    }

    return ret;
}
