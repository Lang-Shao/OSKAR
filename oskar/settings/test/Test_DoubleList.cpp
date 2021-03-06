/*
 * Copyright (c) 2015, The University of Oxford
 * All rights reserved.
 *
 * This file is part of the OSKAR package.
 * Contact: oskar at oerc.ox.ac.uk
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

#include <gtest/gtest.h>
#include "settings/oskar_settings_types.h"

using namespace oskar;

TEST(settings_types, DoubleList)
{
    DoubleList l;
    {
        ASSERT_TRUE(l.set_default("0.0,1.1,2.2,3.3,4.4,5.5"));
        ASSERT_TRUE(l.is_default());
        ASSERT_EQ(6, l.size());
        for (int i = 0; i < l.size(); ++i) {
            ASSERT_EQ(double(i)+double(i)/10., l.values()[i]);
        }
        ASSERT_STREQ("0.0,1.1,2.2,3.3,4.4,5.5", l.get_value());
    }
    {
        ASSERT_TRUE(l.set_value("0.01234567891011,  666.6"));
        ASSERT_FALSE(l.is_default());
        ASSERT_EQ(2, l.size());
        ASSERT_DOUBLE_EQ(0.01234567891011, l.values()[0]);
        ASSERT_EQ(666.6, l.values()[1]);
        // FIXME(BM) string printing to not enough decimal places!
        EXPECT_STREQ("0.01234567891011,666.6", l.get_value());
    }
}

