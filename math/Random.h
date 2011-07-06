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

#ifndef OSKAR_RANDOM_H_
#define OSKAR_RANDOM_H_

/**
 * @file Random.h
 *
 * @brief This file defines functions to generate random numbers.
 */

#include <cmath>
#include <cstdlib>

using std::log;
using std::pow;
using std::exp;
using std::abs;

/**
 * @brief
 * Class containing functions for generating random numbers.
 *
 * @details
 * This class contains functions to generate random numbers from uniform and
 * Gaussian distributions.
 */
class Random
{
public:
    /// Generates a random number from a uniform distribution.
    template<typename T>
    static T uniform(int seed = -1);

    /// Generates a random number from a Gaussian distribution.
    template<typename T>
    static T gaussian(T* out1, T* out2, int seed = -1);

    template<typename T>
    static T power_law(const T min, const T max, const T power,
            const int seed = -1);

    template<typename T>
    static T broken_power_law(const T min, const T max, const T threshold,
            const T power1, const T power2, const int seed = -1);
};

/*=============================================================================
 * Static public members
 *---------------------------------------------------------------------------*/

/**
 * @details
 * Generates a random number from a uniform distribution in the range 0 to 1.
 * If the supplied seed is greater than 0, then the random number generator is
 * seeded first.
 *
 * @param[in] seed If greater than 0, use this as a seed for the generator.
 */
template<typename T>
T Random::uniform(int seed)
{
    // Seed the random number generator if required.
    if (seed > 0) srand(seed);
    return (T)rand() / ((T)RAND_MAX + 1.0);
}

/**
 * @details
 * Generates a random number from a Gaussian distribution with zero mean
 * and unit variance.
 *
 * @param[out] out1 The first random number.
 * @param[out] out2 The second random number.
 * @param[in] seed If greater than 0, use this as a seed for the generator.
 */
template<typename T>
T Random::gaussian(T* x, T* y, int seed)
{
    if (seed > 0) srand(seed);
    T r2;
    do {
        // Choose x and y in a uniform square (-1, -1) to (+1, +1).
        *x = 2.0 * rand() / (RAND_MAX + 1.0) - 1.0;
        *y = 2.0 * rand() / (RAND_MAX + 1.0) - 1.0;

        // Check if this is in the unit circle.
        r2 = (*x)*(*x) + (*y)*(*y);
    } while (r2 >= 1.0 || r2 == 0);

    // Box-Muller transform.
    T fac = std::sqrt(-2.0 * std::log(r2) / r2);
    *x *= fac;
    *y *= fac;

    // Return the first random number.
    return *x;
}


template<typename T>
T Random::power_law(const T min, const T max, const T power,
        const int seed)
{
    if (seed > 0) srand(seed);
    const T r = (T)rand() / ((T)RAND_MAX + 1.0);
    const T b0 = std::pow(min, power + 1);
    const T b1 = std::pow(max, power + 1);
    return std::pow( ((b1 - b0) * r + b0), (1.0 / (power + 1)));
}


template<typename T>
T Random::broken_power_law(const T min, const T max, const T threshold,
        const T power1, const T power2, const int seed)
{
    if (seed > 0) srand(seed);

    const T b0 = std::pow(threshold, (power1 - power2));
    const T pow1 = power1 + 1.0;
    const T pow2 = power2 + 1.0;
    const T powinv1 = 1.0 / pow1;
    const T powinv2 = 1.0 / pow2;

    T b1, b2;

    if (power1 == -1)
        b1 = log(threshold) - log(min);
    else
        b1 = powinv1 * (pow(threshold, pow1) - pow(min, pow2));

    if (power2 == -1)
        b2 = b0 * (log(max) - log(threshold));
    else
        b2 = b0 * powinv2 * (pow(max, pow2) - pow(threshold, pow2));

    const T r = (T)rand() / ((T)RAND_MAX + 1.0);
    const T b = -b1 + r * (b2 + b1);
    if (b > 0)
    {
        if (power2 == -1.0)
            return threshold * exp(b / b0);
        else
            return pow((b * (pow2/b0) + pow(threshold, pow2)), powinv2);
    }
    else
    {
        if (power1 == -1.0)
            return threshold * exp(-abs(b));
        else
            return pow( (pow(threshold, pow1) - abs(b) * pow1) , powinv1);
    }
}



#endif // OSKAR_RANDOM_H_