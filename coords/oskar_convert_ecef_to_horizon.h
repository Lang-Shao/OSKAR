/*
 * Copyright (c) 2013, The University of Oxford
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

#ifndef OSKAR_CONVERT_ECEF_TO_HORIZON_H_
#define OSKAR_CONVERT_ECEF_TO_HORIZON_H_

/**
 * @file oskar_convert_ecef_to_horizon.h
 */

#include <oskar_global.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Convert coordinates from the Earth-Centred-Earth-Fixed (ECEF) to a local
 * horizon system.
 *
 * @details
 * This function converts station positions from the geocentric cartesian
 * (Earth-Centred-Earth-Fixed, or ECEF) system to the local horizon plane
 * (East-North-Up, or ENU).
 *
 * The input coordinates are with respect to the origin at the centre of the
 * Earth, and have the x-axis pointing towards the meridian of zero longitude,
 * the y-axis to +90 degrees East, and the z-axis to the North Celestial Pole.
 *
 * The output coordinates are with respect to the origin at the tangent point,
 * and have the x-axis pointing to the local East, the y-axis to the local
 * North, and the z-axis to the zenith.
 *
 * A single precision version of this function is not provided, because it
 * would be unable to represent points accurately on the Earth's surface
 * (more than 7 decimal digits are required for sub-metre precision).
 *
 * @param[in]  n             Number of points.
 * @param[in]  ecef_x        Vector of ECEF x-positions, in metres.
 * @param[in]  ecef_y        Vector of ECEF y-positions, in metres.
 * @param[in]  ecef_z        Vector of ECEF z-positions, in metres.
 * @param[in]  longitude     Longitude of tangent point, in radians.
 * @param[in]  latitude      Latitude of tangent point, in radians.
 * @param[in]  altitude      Altitude above ellipsoid, in metres.
 * @param[out] horizon_x     Vector of horizontal x-positions, in metres.
 * @param[out] horizon_y     Vector of horizontal y-positions, in metres.
 * @param[out] horizon_z     Vector of horizontal z-positions, in metres.
 */
OSKAR_EXPORT
void oskar_convert_ecef_to_horizon(int n, const double* ecef_x,
        const double* ecef_y, const double* ecef_z, double longitude,
        double latitude, double altitude, double* horizon_x,
        double* horizon_y, double* horizon_z);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_CONVERT_ECEF_TO_HORIZON_H_ */