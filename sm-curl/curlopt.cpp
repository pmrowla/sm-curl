/*
 * Copyright (c) 2013 Peter Rowlands
 *
 * This file is a part of sm-curl.
 *
 * sm-curl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sm-curl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with sm-curl.  If not, see <http://www.gnu.org/licenses/>.
 *
 * vim: set ts=4 et :
 */
/**
 * @file
 * @brief sm-curl CURLoption handling routines
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#include "curlopt.h"

static chainhash_t(ul_ul) *g_curlHandles = NULL;

CURLcode curlopt_init(void)
{
    g_curlHandles = chainhash_init(ul_ul, 10);
    if (NULL == g_curlHandles)
        return CURLE_OUT_OF_MEMORY;

    return CURLE_OK;
}

CURLcode curlopt_fini(void)
{
    if (g_curlHandles)
    {
        chainhash_itr_t(ul_ul) it = chainhash_begin(ul_ul, g_curlHandles);
        while (!chainhash_end(it))
        {
            unsigned long n = chainhash_value(it);
            chainhash_t(ul_ul) *ch = reinterpret_cast<chainhash_t(ul_ul) *>(n);
            chainhash_destroy(ul_ul, ch);
        }
        chainhash_destroy(ul_ul, g_curlHandles);
    }

    return CURLE_OK;
}
