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

#include "apps/lib/oskar_set_up_visibilities.h"
#include "interferometry/oskar_visibilities_init.h"
#include "utility/oskar_mem_type_check.h"
#include "utility/oskar_mem_copy.h"

#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

int oskar_set_up_visibilities(oskar_Visibilities* vis,
        const oskar_Settings* settings, const oskar_TelescopeModel* telescope,
        int type)
{
    int error, num_stations, num_channels, num_baselines;
    const oskar_SettingsTime* times;

    /* Sanity check on inputs. */
    if (!vis || !settings || !telescope)
        return OSKAR_ERR_INVALID_ARGUMENT;

    /* Check the type. */
    if (!oskar_mem_is_complex(type))
        return OSKAR_ERR_BAD_DATA_TYPE;

    /* Initialise the global visibility structure on the CPU. */
    times = &settings->obs.time;
    num_stations = telescope->num_stations;
    num_channels = settings->obs.num_channels;
    num_baselines = num_stations * (num_stations - 1) / 2;
    error = oskar_visibilities_init(vis, type, OSKAR_LOCATION_CPU,
            num_channels, times->num_time_steps, num_baselines);
    if (error) return error;

    /* Add meta-data. */
    vis->freq_start_hz = settings->obs.start_frequency_hz;
    vis->freq_inc_hz = settings->obs.frequency_inc_hz;
    vis->time_start_mjd_utc = times->obs_start_mjd_utc;
    vis->time_inc_seconds = times->dt_dump_days * 86400.0;
    vis->channel_bandwidth_hz = settings->obs.start_frequency_hz;
    vis->phase_centre_ra_deg = settings->obs.ra0_rad * 180.0 / M_PI;
    vis->phase_centre_dec_deg = settings->obs.dec0_rad * 180.0 / M_PI;

    /* Add settings file path. */
    error = oskar_mem_copy(&vis->settings_path, &settings->settings_path);
    if (error) return error;

    return OSKAR_SUCCESS;
}

#ifdef __cplusplus
}
#endif