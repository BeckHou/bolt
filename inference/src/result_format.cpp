// Copyright (C) 2019. Huawei Technologies Co., Ltd. All rights reserved.

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include <algorithm>
#include "result_format.hpp"

Vec<int> topK_index(Tensor data, U32 topK){
    TensorDesc desc = data.get_desc();
    U32 len = tensorNumElements(desc);

    Vec<int> index(len);
    for (U32 i = 0; i < index.size(); i++) {
        index[i] = i;
    }

    U8* res = NULL;
    auto mem = data.get_memory();
#ifdef _USE_MALI
    if(mem->get_mem_type() == OCLMem) {
        std::shared_ptr<GCLMem> oclMem = mem->get_shared_ptr_caster();
        if(!oclMem->desc.use_map) {
            std::cout << "Error: Not support check unmapped gcl memory value" << std::endl;
            exit(1);
        }
        res = oclMem->desc.map_ptr;
    } else {
#endif           
        res = (U8*)mem->get_val();
#ifdef _USE_MALI
    }
#endif           

    switch (desc.dt) {
#ifdef __aarch64__
        case DT_F16: {
            F16* dataPtr = (F16 *)res;
            sort(index.begin(), index.end(),
                [&](const int& a, const int& b) {
                    return (dataPtr[a] > dataPtr[b]);
                }
            );
            break;
        }
#endif
        case DT_F32: {
            F32* dataPtr = (F32 *)res;
            sort(index.begin(), index.end(),
                [&](const int& a, const int& b) {
                    return (dataPtr[a] > dataPtr[b]);
                }
            );
            break;
        }
        default:
            break;
    }

    Vec<int>::const_iterator first = index.begin() + 0;
    Vec<int>::const_iterator last = index.begin() + topK;
    Vec<int> indexesTopK(first, last);

    return indexesTopK;
}
