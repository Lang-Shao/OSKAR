/*
 * Copyright (c) 2011, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef OSKAR_EVALUATE_E_JONES_2D_HORIZONTAL_H_
#define OSKAR_EVALUATE_E_JONES_2D_HORIZONTAL_H_

/**
 * @file oskar_cuda_bp2hc.h
 */

#include "oskar_windows.h"


#ifdef __cplusplus
extern "C" {
#endif

struct oskar_StationModel;
struct oskar_SkyModelLocal_d;

/**
 * @brief
 *
 * @details
 *
 * @return
 */
DllExport
int oskar_evaluate_e_jones_2d_horizontal_f();


/**
 * @brief
 *
 * @details
 *
 * @return
 */
DllExport
int oskar_evaluate_e_jones_2d_horizontal_d(
        const oskar_StationModel* hd_station,
        const double h_beam_l,
        const double h_beam_m,
        const oskar_SkyModelLocal_d* hd_sky,
        double2* d_weights_work, // FIXME memory for weights. (num_antennas * sizeof(double2))
        double2* d_e_jones);


#ifdef __cplusplus
}
#endif

#endif // OSKAR_EVALUATE_E_JONES_2D_HORIZONTAL_H_