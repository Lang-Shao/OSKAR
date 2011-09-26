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

#include "ms/test/MeasurementSetTest.h"
#include "ms/oskar_MeasurementSet.h"
#include "ms/oskar_ms_create_meta1.h"
#include "ms/oskar_ms_append_vis1.h"
#include <vector>

#define TIMER_ENABLE 1
#include "utility/timer.h"

// Register the test class.
CPPUNIT_TEST_SUITE_REGISTRATION(MeasurementSetTest);

/**
 * @details
 * Sets up the context before running each test method.
 */
void MeasurementSetTest::setUp()
{
}

/**
 * @details
 * Clean up routine called after each test is run.
 */
void MeasurementSetTest::tearDown()
{
}

/**
 * @details
 * Tests creation of simple measurement set.
 */
void MeasurementSetTest::test_create_simple()
{
    // Create the Measurement Set.
    oskar_MeasurementSet ms(90, 90);
    ms.create("simple.ms");

    // Add some dummy antenna positions.
    double ax[] = {0, 0, 0};
    double ay[] = {0, 0, 0};
    double az[] = {0, 0, 0};
    int na = sizeof(ax) / sizeof(double);
    ms.addAntennas(na, ax, ay, az);

    // Add the Right Ascension & Declination of field centre.
    ms.addField(0, 1.570796);

    // Add frequency band.
    ms.addBand(0, 1, 400e6, 1.0);

    // Add test visibilities (don't include conjugated versions).
    double u[] = {1000.0, 2000.01, 156.03};
    double v[] = {0.0, -241.02, 1678.04};
    double w[] = {0.0, -56.0, 145.0};
    double vis[] = {1.0, 0.0, 0.00, 0.0, 0.00, 0.0};
    double times[] = {1.0, 2.0, 3.0};
    int ant1[] = {0, 0, 1};
    int ant2[] = {1, 2, 2};
    int nv = sizeof(u) / sizeof(double);
    ms.addVisibilities(1, 1, nv, u, v, w, vis, ant1, ant2, times);
}

/**
 * @details
 * Tests appending to a measurement set using the C binding.
 */
void MeasurementSetTest::test_append_c()
{
    // Define filename and metadata.
    const char filename[] = "append_c.ms";
    double exposure = 90;
    double interval = 90;
    double ra = 0;
    double dec = 1.570796;
    double freq = 400e6;

    // Define antenna positions.
    double ax[] = {0, 0, 0};
    double ay[] = {0, 0, 0};
    double az[] = {0, 0, 0};
    int na = sizeof(ax) / sizeof(double);

    // Define visibilities.
    double u[] = {1000.0, 2000.01, 156.03};
    double v[] = {0.0, -241.02, 1678.04};
    double w[] = {0.0, -56.0, 145.0};
    double vis[] = {1.0, 0.0, 0.00, 0.0, 0.00, 0.0};
    int ant1[] = {0, 0, 1};
    int ant2[] = {1, 2, 2};
    int nv = sizeof(u) / sizeof(double);

    std::vector<double> times(nv, 0.0f);
    for (int i = 0; i < nv; ++i)
        times[i] = i * 0.01;

    oskar_ms_create_meta1(filename, ra, dec, na, ax, ay, az, freq);
    oskar_ms_append_vis1(filename, exposure, interval,
            nv, u, v, w, vis, ant1, ant2, &times[0]);
}

/**
 * @details
 * Tests appending to a large measurement set using the C binding.
 */
void MeasurementSetTest::test_append_large()
{
    // Define filename and metadata.
    const char filename[] = "append_large.ms";
    double exposure = 90;
    double interval = 90;
    double ra = 0;
    double dec = 1.570796;
    double freq = 400e6;

    // Define antenna positions.
    double ax[] = {0, 0, 0};
    double ay[] = {0, 0, 0};
    double az[] = {0, 0, 0};
    int na = sizeof(ax) / sizeof(double);

    // Create the MS with the metadata
    oskar_ms_create_meta1(filename, ra, dec, na, ax, ay, az, freq);

    // Define visibilities.
    int nv = 1000;
    std::vector<double> u(nv, 0.0f), v(nv, 0.0f), w(nv, 0.0f);
    std::vector<double> vis(2*nv, 0.0f);
    std::vector<int> ant1(nv, 0), ant2(nv, 0);
    std::vector<double> times(nv, 0.0f);
    for (int i = 0; i < nv; ++i)
        times[i] = i * 0.01;

    // Append to MS.
    TIMER_START
    int blocks = 100;
    for (int b = 0; b < blocks; ++b) {
        oskar_ms_append_vis1(filename, exposure, interval,
                nv, &u[0], &v[0], &w[0], &vis[0], &ant1[0], &ant2[0], &times[0]);
    }
    TIMER_STOP("Finished creating measurement set (%d visibilities)",
            nv * blocks)
}

/**
 * @details
 * Tests writing multiple channels to a measurement set.
 */
void MeasurementSetTest::test_multi_channel()
{
    // Define the data dimensions.
    int n_ant = 3;   // Number of antennas.
    int n_pol = 4;   // Number of polarisations.
    int n_chan = 10; // Number of channels.

    // Create the Measurement Set.
    oskar_MeasurementSet ms(90, 90);
    ms.create("multi_channel.ms");

    // Add some dummy antenna positions.
    std::vector<double> ax(n_ant, 0.0), ay(n_ant, 0.0), az(n_ant, 0.0);
    for (int i = 0; i < n_ant; ++i)
    {
        ax[i] = i / 10.0;
        ay[i] = i / 20.0;
        az[i] = i / 30.0;
    }
    ms.addAntennas(n_ant, &ax[0], &ay[0], &az[0]);

    // Add the Right Ascension & Declination of field centre.
    ms.addField(0, 1.570796, "Test field");

    // Add polarisations.
    ms.addPolarisation(n_pol);

    // Add frequency band.
    ms.addBand(0, n_chan, 400e6, 25e3);

    // Create test data (without complex conjugate).
    int n_dumps = 2;
    int n_baselines = n_ant * (n_ant - 1) / 2;
    int n_rows = n_baselines * n_dumps;
    std::vector<double> u(n_rows), v(n_rows), w(n_rows), times(n_rows);
    std::vector< std::complex<double> > vis_data(n_pol * n_chan * n_rows);
    std::vector<int> ant1(n_rows), ant2(n_rows);

    // Fill the vectors.
    for (int d = 0, r = 0; d < n_dumps; ++d)
    {
        for (int ai = 0; ai < n_ant; ++ai)
        {
            for (int aj = ai+1; aj < n_ant; ++aj)
            {
                // Create the u,v,w coordinates.
                u[r] = 10.0 * r;
                v[r] = 50.0 * r;
                w[r] = 100.0 * r;

                // Create the visibility data.
                for (int c = 0; c < n_chan; ++c)
                {
                    for (int p = 0; p < n_pol; ++p)
                    {
                        int vi = r * n_pol * n_chan + c * n_pol + p;
                        double re = (p + 1) * (c + 1) * 10.0;
                        double im = (double)r;
                        vis_data[vi] = std::complex<double>(re, im);
                    }
                }

                // Create the antenna index pairs.
                ant1[r] = ai;
                ant2[r] = aj;

                // Create the time stamps.
                times[r] = (double)r;

                // Increment the row index.
                ++r;
            }
        }
    }

    TIMER_START
    ms.addVisibilities(n_pol, n_chan, n_rows, &u[0], &v[0], &w[0],
            (double*)(&vis_data[0]), &ant1[0], &ant2[0], &times[0]);
    TIMER_STOP("Finished creating measurement set (%d rows, %d visibilities)",
            n_rows, n_pol * n_chan * n_rows)
}
