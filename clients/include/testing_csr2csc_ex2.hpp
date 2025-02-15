/* ************************************************************************
 * Copyright (C) 2022 Advanced Micro Devices, Inc. All rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ************************************************************************ */

#pragma once
#ifndef TESTING_CSR2CSC_EX2_HPP
#define TESTING_CSR2CSC_EX2_HPP

#include "hipsparse.hpp"
#include "hipsparse_test_unique_ptr.hpp"
#include "unit.hpp"
#include "utility.hpp"

#include <algorithm>
#include <hipsparse.h>
#include <string>

using namespace hipsparse;
using namespace hipsparse_test;

template <typename T>
void testing_csr2csc_ex2_bad_arg(void)
{
#if(!defined(CUDART_VERSION))
    int               m           = 100;
    int               n           = 100;
    int               nnz         = 100;
    int               safe_size   = 100;
    size_t            buffer_size = 0;
    hipsparseStatus_t status;

    std::unique_ptr<handle_struct> unique_ptr_handle(new handle_struct);
    hipsparseHandle_t              handle = unique_ptr_handle->handle;

    auto csr_row_ptr_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(int) * safe_size), device_free};
    auto csr_col_ind_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(int) * safe_size), device_free};
    auto csr_val_managed = hipsparse_unique_ptr{device_malloc(sizeof(T) * safe_size), device_free};
    auto csc_row_ind_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(int) * safe_size), device_free};
    auto csc_col_ptr_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(int) * safe_size), device_free};
    auto csc_val_managed = hipsparse_unique_ptr{device_malloc(sizeof(T) * safe_size), device_free};
    auto buffer_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(char) * safe_size), device_free};

    int*  csr_row_ptr = (int*)csr_row_ptr_managed.get();
    int*  csr_col_ind = (int*)csr_col_ind_managed.get();
    T*    csr_val     = (T*)csr_val_managed.get();
    int*  csc_row_ind = (int*)csc_row_ind_managed.get();
    int*  csc_col_ptr = (int*)csc_col_ptr_managed.get();
    T*    csc_val     = (T*)csc_val_managed.get();
    void* buffer      = (void*)buffer_managed.get();

    if(!csr_row_ptr || !csr_col_ind || !csr_val || !csc_row_ind || !csc_col_ptr || !csc_val
       || !buffer)
    {
        PRINT_IF_HIP_ERROR(hipErrorOutOfMemory);
        return;
    }

    // Testing hipsparseCsr2cscEx2_bufferSize()

    // Testing for (handle == nullptr)
    {
        hipsparseHandle_t handle_null = nullptr;

        status = hipsparseCsr2cscEx2_bufferSize(handle_null,
                                                m,
                                                n,
                                                nnz,
                                                csr_val,
                                                csr_row_ptr,
                                                csr_col_ind,
                                                csc_val,
                                                csc_col_ptr,
                                                csc_row_ind,
                                                HIP_R_32F,
                                                HIPSPARSE_ACTION_NUMERIC,
                                                HIPSPARSE_INDEX_BASE_ZERO,
                                                HIPSPARSE_CSR2CSC_ALG1,
                                                &buffer_size);
        verify_hipsparse_status_invalid_handle(status);
    }

    // Testing for (csr_row_ptr == nullptr)
    {
        int* csr_row_ptr_null = nullptr;

        status = hipsparseCsr2cscEx2_bufferSize(handle,
                                                m,
                                                n,
                                                nnz,
                                                csr_val,
                                                csr_row_ptr_null,
                                                csr_col_ind,
                                                csc_val,
                                                csc_col_ptr,
                                                csc_row_ind,
                                                HIP_R_32F,
                                                HIPSPARSE_ACTION_NUMERIC,
                                                HIPSPARSE_INDEX_BASE_ZERO,
                                                HIPSPARSE_CSR2CSC_ALG1,
                                                &buffer_size);
        verify_hipsparse_status_invalid_pointer(status, "Error: csr_row_ptr is nullptr");
    }

    // Testing for (csr_col_ind == nullptr)
    {
        int* csr_col_ind_null = nullptr;

        status = hipsparseCsr2cscEx2_bufferSize(handle,
                                                m,
                                                n,
                                                nnz,
                                                csr_val,
                                                csr_row_ptr,
                                                csr_col_ind_null,
                                                csc_val,
                                                csc_col_ptr,
                                                csc_row_ind,
                                                HIP_R_32F,
                                                HIPSPARSE_ACTION_NUMERIC,
                                                HIPSPARSE_INDEX_BASE_ZERO,
                                                HIPSPARSE_CSR2CSC_ALG1,
                                                &buffer_size);
        verify_hipsparse_status_invalid_pointer(status, "Error: csr_col_ind is nullptr");
    }

    // Testing for (buffer_size == nullptr)
    {
        size_t* buffer_size_null = nullptr;

        status = hipsparseCsr2cscEx2_bufferSize(handle,
                                                m,
                                                n,
                                                nnz,
                                                csr_val,
                                                csr_row_ptr,
                                                csr_col_ind,
                                                csc_val,
                                                csc_col_ptr,
                                                csc_row_ind,
                                                HIP_R_32F,
                                                HIPSPARSE_ACTION_NUMERIC,
                                                HIPSPARSE_INDEX_BASE_ZERO,
                                                HIPSPARSE_CSR2CSC_ALG1,
                                                buffer_size_null);
        verify_hipsparse_status_invalid_pointer(status, "Error: buffer_size is nullptr");
    }

    // Testing hipsparseCsr2cscEx2()

    // Testing for (handle == nullptr)
    {
        hipsparseHandle_t handle_null = nullptr;

        status = hipsparseCsr2cscEx2(handle_null,
                                     m,
                                     n,
                                     nnz,
                                     csr_val,
                                     csr_row_ptr,
                                     csr_col_ind,
                                     csc_val,
                                     csc_col_ptr,
                                     csc_row_ind,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer);
        verify_hipsparse_status_invalid_handle(status);
    }

    // Testing for (csr_row_ptr == nullptr)
    {
        int* csr_row_ptr_null = nullptr;

        status = hipsparseCsr2cscEx2(handle,
                                     m,
                                     n,
                                     nnz,
                                     csr_val,
                                     csr_row_ptr_null,
                                     csr_col_ind,
                                     csc_val,
                                     csc_col_ptr,
                                     csc_row_ind,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer);
        verify_hipsparse_status_invalid_pointer(status, "Error: csr_row_ptr is nullptr");
    }

    // Testing for (csr_col_ind == nullptr)
    {
        int* csr_col_ind_null = nullptr;

        status = hipsparseCsr2cscEx2(handle,
                                     m,
                                     n,
                                     nnz,
                                     csr_val,
                                     csr_row_ptr,
                                     csr_col_ind_null,
                                     csc_val,
                                     csc_col_ptr,
                                     csc_row_ind,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer);
        verify_hipsparse_status_invalid_pointer(status, "Error: csr_col_ind is nullptr");
    }

    // Testing for (csr_val == nullptr)
    {
        T* csr_val_null = nullptr;

        status = hipsparseCsr2cscEx2(handle,
                                     m,
                                     n,
                                     nnz,
                                     csr_val_null,
                                     csr_row_ptr,
                                     csr_col_ind,
                                     csc_val,
                                     csc_col_ptr,
                                     csc_row_ind,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer);
        verify_hipsparse_status_invalid_pointer(status, "Error: csr_val is nullptr");
    }

    // Testing for (csc_col_ptr == nullptr)
    {
        int* csc_col_ptr_null = nullptr;

        status = hipsparseCsr2cscEx2(handle,
                                     m,
                                     n,
                                     nnz,
                                     csr_val,
                                     csr_row_ptr,
                                     csr_col_ind,
                                     csc_val,
                                     csc_col_ptr_null,
                                     csc_row_ind,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer);
        verify_hipsparse_status_invalid_pointer(status, "Error: csc_col_ptr is nullptr");
    }

    // Testing for (csc_row_ind == nullptr)
    {
        int* csc_row_ind_null = nullptr;

        status = hipsparseCsr2cscEx2(handle,
                                     m,
                                     n,
                                     nnz,
                                     csr_val,
                                     csr_row_ptr,
                                     csr_col_ind,
                                     csc_val,
                                     csc_col_ptr,
                                     csc_row_ind_null,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer);
        verify_hipsparse_status_invalid_pointer(status, "Error: csc_row_ind is nullptr");
    }

    // Testing for (csc_val == nullptr)
    {
        T* csc_val_null = nullptr;

        status = hipsparseCsr2cscEx2(handle,
                                     m,
                                     n,
                                     nnz,
                                     csr_val,
                                     csr_row_ptr,
                                     csr_col_ind,
                                     csc_val_null,
                                     csc_col_ptr,
                                     csc_row_ind,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer);
        verify_hipsparse_status_invalid_pointer(status, "Error: csc_val is nullptr");
    }

    // Testing for (buffer == nullptr)
    {
        T* buffer_null = nullptr;

        status = hipsparseCsr2cscEx2(handle,
                                     m,
                                     n,
                                     nnz,
                                     csr_val,
                                     csr_row_ptr,
                                     csr_col_ind,
                                     csc_val,
                                     csc_col_ptr,
                                     csc_row_ind,
                                     HIP_R_32F,
                                     HIPSPARSE_ACTION_NUMERIC,
                                     HIPSPARSE_INDEX_BASE_ZERO,
                                     HIPSPARSE_CSR2CSC_ALG1,
                                     buffer_null);
        verify_hipsparse_status_invalid_pointer(status, "Error: buffer is nullptr");
    }
#endif
}

template <typename T>
hipsparseStatus_t testing_csr2csc_ex2(Arguments argus)
{
    int                   m        = argus.M;
    int                   n        = argus.N;
    hipsparseIndexBase_t  idx_base = argus.idx_base;
    hipsparseAction_t     action   = argus.action;
    std::string           filename = argus.filename;
    hipsparseCsr2CscAlg_t alg      = HIPSPARSE_CSR2CSC_ALG1;

    hipDataType dataType = getDataType<T>();

    std::unique_ptr<handle_struct> unique_ptr_handle(new handle_struct);
    hipsparseHandle_t              handle = unique_ptr_handle->handle;

    srand(12345ULL);

    // Host structures
    std::vector<int> hcsr_row_ptr;
    std::vector<int> hcsr_col_ind;
    std::vector<T>   hcsr_val;

    // Read or construct CSR matrix
    int nnz = 0;
    if(!generate_csr_matrix(filename, m, n, nnz, hcsr_row_ptr, hcsr_col_ind, hcsr_val, idx_base))
    {
        fprintf(stderr, "Cannot open [read] %s\ncol", filename.c_str());
        return HIPSPARSE_STATUS_INTERNAL_ERROR;
    }

    // Allocate memory on the device
    auto dcsr_row_ptr_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(int) * (m + 1)), device_free};
    auto dcsr_col_ind_managed = hipsparse_unique_ptr{device_malloc(sizeof(int) * nnz), device_free};
    auto dcsr_val_managed     = hipsparse_unique_ptr{device_malloc(sizeof(T) * nnz), device_free};
    auto dcsc_row_ind_managed = hipsparse_unique_ptr{device_malloc(sizeof(int) * nnz), device_free};
    auto dcsc_col_ptr_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(int) * (n + 1)), device_free};
    auto dcsc_val_managed = hipsparse_unique_ptr{device_malloc(sizeof(T) * nnz), device_free};

    int* dcsr_row_ptr = (int*)dcsr_row_ptr_managed.get();
    int* dcsr_col_ind = (int*)dcsr_col_ind_managed.get();
    T*   dcsr_val     = (T*)dcsr_val_managed.get();
    int* dcsc_row_ind = (int*)dcsc_row_ind_managed.get();
    int* dcsc_col_ptr = (int*)dcsc_col_ptr_managed.get();
    T*   dcsc_val     = (T*)dcsc_val_managed.get();

    // Reset CSC arrays
    CHECK_HIP_ERROR(hipMemset(dcsc_row_ind, 0, sizeof(int) * nnz));
    CHECK_HIP_ERROR(hipMemset(dcsc_col_ptr, 0, sizeof(int) * (n + 1)));
    CHECK_HIP_ERROR(hipMemset(dcsc_val, 0, sizeof(T) * nnz));

    // Copy data from host to device
    CHECK_HIP_ERROR(
        hipMemcpy(dcsr_row_ptr, hcsr_row_ptr.data(), sizeof(int) * (m + 1), hipMemcpyHostToDevice));
    CHECK_HIP_ERROR(
        hipMemcpy(dcsr_col_ind, hcsr_col_ind.data(), sizeof(int) * nnz, hipMemcpyHostToDevice));
    CHECK_HIP_ERROR(hipMemcpy(dcsr_val, hcsr_val.data(), sizeof(T) * nnz, hipMemcpyHostToDevice));

    size_t bufferSize;
    CHECK_HIPSPARSE_ERROR(hipsparseCsr2cscEx2_bufferSize(handle,
                                                         m,
                                                         n,
                                                         nnz,
                                                         dcsr_val,
                                                         dcsr_row_ptr,
                                                         dcsr_col_ind,
                                                         dcsc_val,
                                                         dcsc_col_ptr,
                                                         dcsc_row_ind,
                                                         dataType,
                                                         action,
                                                         idx_base,
                                                         alg,
                                                         &bufferSize));

    // Allocate buffer on the device
    auto dbuffer_managed
        = hipsparse_unique_ptr{device_malloc(sizeof(char) * bufferSize), device_free};

    void* dbuffer = (void*)dbuffer_managed.get();

    if(argus.unit_check)
    {
        CHECK_HIPSPARSE_ERROR(hipsparseCsr2cscEx2(handle,
                                                  m,
                                                  n,
                                                  nnz,
                                                  dcsr_val,
                                                  dcsr_row_ptr,
                                                  dcsr_col_ind,
                                                  dcsc_val,
                                                  dcsc_col_ptr,
                                                  dcsc_row_ind,
                                                  dataType,
                                                  action,
                                                  idx_base,
                                                  alg,
                                                  dbuffer));

        // Copy output from device to host
        std::vector<int> hcsc_row_ind(nnz);
        std::vector<int> hcsc_col_ptr(n + 1);
        std::vector<T>   hcsc_val(nnz);

        CHECK_HIP_ERROR(
            hipMemcpy(hcsc_row_ind.data(), dcsc_row_ind, sizeof(int) * nnz, hipMemcpyDeviceToHost));
        CHECK_HIP_ERROR(hipMemcpy(
            hcsc_col_ptr.data(), dcsc_col_ptr, sizeof(int) * (n + 1), hipMemcpyDeviceToHost));
        CHECK_HIP_ERROR(
            hipMemcpy(hcsc_val.data(), dcsc_val, sizeof(T) * nnz, hipMemcpyDeviceToHost));

        // Host csr2csc conversion
        std::vector<int> hcsc_row_ind_gold(nnz);
        std::vector<int> hcsc_col_ptr_gold(n + 1, 0);
        std::vector<T>   hcsc_val_gold(nnz);

        // Determine nnz per column
        for(int i = 0; i < nnz; ++i)
        {
            ++hcsc_col_ptr_gold[hcsr_col_ind[i] + 1 - idx_base];
        }

        // Scan
        for(int i = 0; i < n; ++i)
        {
            hcsc_col_ptr_gold[i + 1] += hcsc_col_ptr_gold[i];
        }

        // Fill row indices and values
        for(int i = 0; i < m; ++i)
        {
            for(int j = hcsr_row_ptr[i]; j < hcsr_row_ptr[i + 1]; ++j)
            {
                int col = hcsr_col_ind[j - idx_base] - idx_base;
                int idx = hcsc_col_ptr_gold[col];

                hcsc_row_ind_gold[idx] = i + idx_base;
                hcsc_val_gold[idx]     = hcsr_val[j - idx_base];

                ++hcsc_col_ptr_gold[col];
            }
        }

        // Shift column pointer array
        for(int i = n; i > 0; --i)
        {
            hcsc_col_ptr_gold[i] = hcsc_col_ptr_gold[i - 1] + idx_base;
        }

        hcsc_col_ptr_gold[0] = idx_base;

        // Unit check
        unit_check_general(1, nnz, 1, hcsc_row_ind_gold.data(), hcsc_row_ind.data());
        unit_check_general(1, n + 1, 1, hcsc_col_ptr_gold.data(), hcsc_col_ptr.data());

        // If action == HIPSPARSE_ACTION_NUMERIC also check values
        if(action == HIPSPARSE_ACTION_NUMERIC)
        {
            unit_check_general(1, nnz, 1, hcsc_val_gold.data(), hcsc_val.data());
        }
    }

    return HIPSPARSE_STATUS_SUCCESS;
}

#endif // TESTING_CSR2CSC_EX2_HPP
