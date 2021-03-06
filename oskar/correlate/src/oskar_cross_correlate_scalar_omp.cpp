/*
 * Copyright (c) 2014-2018, The University of Oxford
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

#include <cmath>
#include "correlate/private_correlate_functions_inline.h"
#include "correlate/oskar_cross_correlate_scalar_omp.h"

template<typename T1, typename T2>
struct is_same
{
    enum { value = false }; // is_same represents a bool.
    typedef is_same<T1,T2> type; // to qualify as a metafunction.
};

template<typename T>
struct is_same<T,T>
{
    enum { value = true };
    typedef is_same<T,T> type;
};

template
<
// Compile-time parameters.
bool BANDWIDTH_SMEARING, bool TIME_SMEARING, bool GAUSSIAN,
typename REAL, typename REAL2
>
void oskar_xcorr_scalar_omp(
        const int                   num_sources,
        const int                   num_stations,
        const REAL2* const restrict jones,
        const REAL*  const restrict source_I,
        const REAL*  const restrict source_l,
        const REAL*  const restrict source_m,
        const REAL*  const restrict source_n,
        const REAL*  const restrict source_a,
        const REAL*  const restrict source_b,
        const REAL*  const restrict source_c,
        const REAL*  const restrict station_u,
        const REAL*  const restrict station_v,
        const REAL*  const restrict station_w,
        const REAL*  const restrict station_x,
        const REAL*  const restrict station_y,
        const REAL                  uv_min_lambda,
        const REAL                  uv_max_lambda,
        const REAL                  inv_wavelength,
        const REAL                  frac_bandwidth,
        const REAL                  time_int_sec,
        const REAL                  gha0_rad,
        const REAL                  dec0_rad,
        REAL2*             restrict vis)
{
    // Loop over stations.
#pragma omp parallel for schedule(dynamic, 1)
    for (int SQ = 0; SQ < num_stations; ++SQ)
    {
        // Pointer to source vector for station q.
        const REAL2* const station_q = &jones[SQ * num_sources];

        // Loop over baselines for this station.
        for (int SP = SQ + 1; SP < num_stations; ++SP)
        {
            REAL uv_len, uu, vv, ww, uu2, vv2, uuvv, du, dv, dw;
            REAL2 t1, t2, sum, guard;
            sum.x = sum.y = (REAL) 0;
            guard.x = guard.y = (REAL) 0;

            // Pointer to source vector for station p.
            const REAL2* const station_p = &jones[SP * num_sources];

            // Get common baseline values.
            OSKAR_BASELINE_TERMS(REAL, station_u[SP], station_u[SQ],
                    station_v[SP], station_v[SQ], station_w[SP], station_w[SQ],
                    uu, vv, ww, uu2, vv2, uuvv, uv_len);

            // Apply the baseline length filter.
            if (uv_len < uv_min_lambda || uv_len > uv_max_lambda) continue;

            // Compute the deltas for time-average smearing.
            if (TIME_SMEARING)
                OSKAR_BASELINE_DELTAS(REAL, station_x[SP], station_x[SQ],
                        station_y[SP], station_y[SQ], du, dv, dw);

            // Loop over sources.
            for (int i = 0; i < num_sources; ++i)
            {
                REAL smearing;
                if (GAUSSIAN)
                {
                    const REAL t = source_a[i] * uu2 + source_b[i] * uuvv +
                            source_c[i] * vv2;
                    smearing = exp((REAL) -t);
                }
                else
                {
                    smearing = (REAL) 1;
                }
                smearing *= source_I[i];
                if (BANDWIDTH_SMEARING || TIME_SMEARING)
                {
                    const REAL l = source_l[i];
                    const REAL m = source_m[i];
                    const REAL n = source_n[i] - (REAL) 1;
                    if (BANDWIDTH_SMEARING)
                    {
                        const REAL t = uu * l + vv * m + ww * n;
                        smearing *= oskar_sinc<REAL>(t);
                    }
                    if (TIME_SMEARING)
                    {
                        const REAL t = du * l + dv * m + dw * n;
                        smearing *= oskar_sinc<REAL>(t);
                    }
                }

                // Multiply Jones scalars.
                t1 = station_p[i];
                t2 = station_q[i];
                OSKAR_MUL_COMPLEX_CONJUGATE_IN_PLACE(REAL2, t1, t2)

                // Multiply result by smearing term and accumulate.
                if (is_same<REAL, float>::value)
                {
                    OSKAR_KAHAN_SUM_MULTIPLY_COMPLEX(
                            REAL, sum, t1, smearing, guard)
                }
                else
                {
                    sum.x += t1.x * smearing;
                    sum.y += t1.y * smearing;
                }
            }

            // Add result to the baseline visibility.
            int i = oskar_evaluate_baseline_index_inline(num_stations, SP, SQ);
            vis[i].x += sum.x;
            vis[i].y += sum.y;
        }
    }
}


void oskar_cross_correlate_scalar_point_omp_f(
        int num_sources, int num_stations, const float2* d_jones,
        const float* d_I, const float* d_l,
        const float* d_m, const float* d_n,
        const float* d_station_u, const float* d_station_v,
        const float* d_station_w, const float* d_station_x,
        const float* d_station_y, float uv_min_lambda, float uv_max_lambda,
        float inv_wavelength, float frac_bandwidth, const float time_int_sec,
        const float gha0_rad, const float dec0_rad, float2* d_vis)
{
    if (frac_bandwidth == 0.0f && time_int_sec == 0.0f)
        oskar_xcorr_scalar_omp<false, false, false, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0f && time_int_sec == 0.0f)
        oskar_xcorr_scalar_omp<true, false, false, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth == 0.0f && time_int_sec != 0.0f)
        oskar_xcorr_scalar_omp<false, true, false, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0f && time_int_sec != 0.0f)
        oskar_xcorr_scalar_omp<true, true, false, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
}

void oskar_cross_correlate_scalar_point_omp_d(
        int num_sources, int num_stations, const double2* d_jones,
        const double* d_I, const double* d_l,
        const double* d_m, const double* d_n,
        const double* d_station_u, const double* d_station_v,
        const double* d_station_w, const double* d_station_x,
        const double* d_station_y, double uv_min_lambda, double uv_max_lambda,
        double inv_wavelength, double frac_bandwidth, const double time_int_sec,
        const double gha0_rad, const double dec0_rad, double2* d_vis)
{
    if (frac_bandwidth == 0.0 && time_int_sec == 0.0)
        oskar_xcorr_scalar_omp<false, false, false, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0 && time_int_sec == 0.0)
        oskar_xcorr_scalar_omp<true, false, false, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth == 0.0 && time_int_sec != 0.0)
        oskar_xcorr_scalar_omp<false, true, false, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0 && time_int_sec != 0.0)
        oskar_xcorr_scalar_omp<true, true, false, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, 0, 0, 0,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
}

void oskar_cross_correlate_scalar_gaussian_omp_f(
        int num_sources, int num_stations, const float2* d_jones,
        const float* d_I, const float* d_l,
        const float* d_m, const float* d_n,
        const float* d_a, const float* d_b,
        const float* d_c, const float* d_station_u,
        const float* d_station_v, const float* d_station_w,
        const float* d_station_x, const float* d_station_y,
        float uv_min_lambda, float uv_max_lambda, float inv_wavelength,
        float frac_bandwidth, float time_int_sec, float gha0_rad,
        float dec0_rad, float2* d_vis)
{
    if (frac_bandwidth == 0.0f && time_int_sec == 0.0f)
        oskar_xcorr_scalar_omp<false, false, true, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0f && time_int_sec == 0.0f)
        oskar_xcorr_scalar_omp<true, false, true, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth == 0.0f && time_int_sec != 0.0f)
        oskar_xcorr_scalar_omp<false, true, true, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0f && time_int_sec != 0.0f)
        oskar_xcorr_scalar_omp<true, true, true, float, float2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
}

void oskar_cross_correlate_scalar_gaussian_omp_d(
        int num_sources, int num_stations, const double2* d_jones,
        const double* d_I, const double* d_l,
        const double* d_m, const double* d_n,
        const double* d_a, const double* d_b,
        const double* d_c, const double* d_station_u,
        const double* d_station_v, const double* d_station_w,
        const double* d_station_x, const double* d_station_y,
        double uv_min_lambda, double uv_max_lambda, double inv_wavelength,
        double frac_bandwidth, double time_int_sec, double gha0_rad,
        double dec0_rad, double2* d_vis)
{
    if (frac_bandwidth == 0.0 && time_int_sec == 0.0)
        oskar_xcorr_scalar_omp<false, false, true, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0 && time_int_sec == 0.0)
        oskar_xcorr_scalar_omp<true, false, true, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth == 0.0 && time_int_sec != 0.0)
        oskar_xcorr_scalar_omp<false, true, true, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
    else if (frac_bandwidth != 0.0 && time_int_sec != 0.0)
        oskar_xcorr_scalar_omp<true, true, true, double, double2>
        (num_sources, num_stations, d_jones, d_I, d_l, d_m, d_n, d_a, d_b, d_c,
                d_station_u, d_station_v, d_station_w, d_station_x, d_station_y,
                uv_min_lambda, uv_max_lambda, inv_wavelength,
                frac_bandwidth, time_int_sec, gha0_rad, dec0_rad, d_vis);
}
