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
 * @brief sm-curl CURLoption handling header
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#include <curl/curl.h>
#include <ulib/chainhash_tpl.h>

DEFINE_CHAINHASH(ul_ul, unsigned long, unsigned long,
        chainhash_hashfn, chainhash_cmpfn);

CURLcode curlopt_init(void);
CURLcode curlopt_fini(void);
