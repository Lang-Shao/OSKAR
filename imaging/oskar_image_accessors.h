/*
 * Copyright (c) 2014, The University of Oxford
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

#ifndef OSKAR_IMAGE_ACCESSORS_H_
#define OSKAR_IMAGE_ACCESSORS_H_

/**
 * @file oskar_image_accessors.h
 */

#include <oskar_global.h>
#include <oskar_mem.h>

#ifdef __cplusplus
extern "C" {
#endif

OSKAR_EXPORT
oskar_Mem* oskar_image_data(oskar_Image* image);

OSKAR_EXPORT
const oskar_Mem* oskar_image_data_const(const oskar_Image* image);

OSKAR_EXPORT
oskar_Mem* oskar_image_settings_path(oskar_Image* image);

OSKAR_EXPORT
const oskar_Mem* oskar_image_settings_path_const(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_grid_type(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_coord_frame(const oskar_Image* image);

OSKAR_EXPORT
const int* oskar_image_dimension_order(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_type(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_width(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_height(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_num_pols(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_num_times(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_num_channels(const oskar_Image* image);

OSKAR_EXPORT
int oskar_image_healpix_nside(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_centre_lon_deg(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_centre_lat_deg(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_fov_lon_deg(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_fov_lat_deg(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_time_start_mjd_utc(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_time_inc_sec(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_freq_start_hz(const oskar_Image* image);

OSKAR_EXPORT
double oskar_image_freq_inc_hz(const oskar_Image* image);

OSKAR_EXPORT
void oskar_image_set_type(oskar_Image* image, int image_type);

OSKAR_EXPORT
void oskar_image_set_coord_frame(oskar_Image* image, int coord_frame);

OSKAR_EXPORT
void oskar_image_set_grid_type(oskar_Image* image, int grid_type);

OSKAR_EXPORT
void oskar_image_set_healpix_nside(oskar_Image* image, int nside);

OSKAR_EXPORT
void oskar_image_set_centre(oskar_Image* image, double centre_lon_deg,
        double centre_lat_deg);

OSKAR_EXPORT
void oskar_image_set_fov(oskar_Image* image, double fov_lon_deg,
        double fov_lat_deg);

OSKAR_EXPORT
void oskar_image_set_time(oskar_Image* image, double time_start_mjd_utc,
        double time_inc_sec);

OSKAR_EXPORT
void oskar_image_set_freq(oskar_Image* image, double freq_start_hz,
        double freq_inc_hz);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_IMAGE_ACCESSORS_H_ */