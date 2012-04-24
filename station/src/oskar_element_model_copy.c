/*
 * Copyright (c) 2012, The University of Oxford
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

#include "station/oskar_element_model_copy.h"
#include "math/oskar_spline_data_copy.h"
#include "utility/oskar_mem_copy.h"

#ifdef __cplusplus
extern "C" {
#endif

int oskar_element_model_copy(oskar_ElementModel* dst,
        const oskar_ElementModel* src)
{
    int err;

    err = oskar_mem_copy(&dst->filename_port1, &src->filename_port1);
    if (err) return err;
    err = oskar_mem_copy(&dst->filename_port2, &src->filename_port2);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port1_phi_re, &src->port1_phi_re);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port1_phi_im, &src->port1_phi_im);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port1_theta_re, &src->port1_theta_re);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port1_theta_im, &src->port1_theta_im);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port2_phi_re, &src->port2_phi_re);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port2_phi_im, &src->port2_phi_im);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port2_theta_re, &src->port2_theta_re);
    if (err) return err;
    err = oskar_spline_data_copy(&dst->port2_theta_im, &src->port2_theta_im);
    if (err) return err;

    return 0;
}

#ifdef __cplusplus
}
#endif