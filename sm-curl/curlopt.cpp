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

#include "smcurl.h"
#include "curlopt.h"

static size_t smcurl_write_function(char *ptr, size_t size, size_t nmemb, void *userdata);

CURLcode curlopt_set_func(CURL *curl, CURLoption option, IPluginFunction *func)
{
    CURLcode code = CURLE_OK;

    switch (option)
    {
        case CURLOPT_WRITEFUNCTION:
            code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl);
            if (CURLE_OK == code)
                code = curl_easy_setopt(curl, option, &smcurl_write_function);
            break;
    }

    return code;
}

static size_t smcurl_write_function(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    CURL *curl = static_cast<CURL *>(userdata);
    CurlData *data;

    curl_easy_getinfo(curl, CURLINFO_PRIVATE, &data);
    cell_t result = 0;

    if (data->write)
    {
        data->write->PushStringEx(ptr, size * (nmemb + 1), SM_PARAM_STRING_COPY|SM_PARAM_STRING_BINARY, 0);
        data->write->PushCell(size);
        data->write->PushCell(nmemb);
        // TODO
        //data->write->PushCell(data->write_data)
        data->write->Execute(&result);
    }

    return static_cast<size_t>(result);
}
