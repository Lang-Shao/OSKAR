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

#ifndef JONES_JOIN_TEST_H_
#define JONES_JOIN_TEST_H_

/**
 * @file JonesJoinTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include "math/oskar_Jones.h"

/**
 * @brief Unit test class that uses CppUnit.
 *
 * @details
 * This class uses the CppUnit testing framework to perform unit tests
 * on the class it is named after.
 */
class JonesJoinTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(JonesJoinTest);
        CPPUNIT_TEST(test_mat2_f);
        CPPUNIT_TEST(test_mat2_d);
        CPPUNIT_TEST_SUITE_END();

    protected:
        oskar_Jones jonesHost(int type, int n_src, int n_stat);
        oskar_Jones jonesDevice(int type, int n_src, int n_stat);
        void checkResultMatrixMatrix(const oskar_Jones* data);
        void checkResultMatrixScalar(const oskar_Jones* data);
        void checkResultScalarScalar(const oskar_Jones* data);

    public:
        /// Set up context before running a test.
        void setUp();

        /// Clean up after the test run.
        void tearDown();

        /// Test method.
        void test_mat2_f();

        /// Test method.
        void test_mat2_d();
};

// Register the test class.
CPPUNIT_TEST_SUITE_REGISTRATION(JonesJoinTest);

#endif // JONES_JOIN_TEST_H_
