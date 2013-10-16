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
 * @brief sm-curl extension code
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <curl/curl.h>

#include "smcurl.h"

HandleType_t g_CurlFileType = 0;
HandleType_t g_CurlHandleType = 0;
HandleType_t g_CurlWebFormType = 0;
HandleType_t g_CurlSlistType = 0;

CurlTypeHandler g_CurlTypeHandler;

bool SmCurl::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
    HandleError err;
    CURLcode cErr;
    bool result = true;
    IdentityToken_t *self = myself->GetIdentity();

    cErr = curl_global_init(CURL_GLOBAL_ALL);
    if (cErr)
    {
        snprintf(error, maxlength, "curl_global_init() failed: %s",
                curl_easy_strerror(cErr));
        return false;
    }

    g_CurlFileType = handlesys->CreateType("CurlFile", &g_CurlTypeHandler, 0,
            NULL, NULL, self, &err);
    if (0 == g_CurlFileType)
    {
        handlesys->RemoveType(g_CurlFileType, self);
        snprintf(error, maxlength, "Could not create CurlFile type (err: %d)", err);
        result = false;
        goto cleanup;
    }

    g_CurlHandleType = handlesys->CreateType("CurlHandle", &g_CurlTypeHandler, 0,
            NULL, NULL, self, &err);
    if (0 == g_CurlHandleType)
    {
        handlesys->RemoveType(g_CurlHandleType, self);
        snprintf(error, maxlength, "Could not create CurlHandle type (err: %d)", err);
        result = false;
        goto cleanup;
    }

    g_CurlWebFormType = handlesys->CreateType("CurlWebForm", &g_CurlTypeHandler, 0,
            NULL, NULL, self, &err);
    if (0 == g_CurlWebFormType)
    {
        handlesys->RemoveType(g_CurlWebFormType, self);
        snprintf(error, maxlength, "Could not create CurlWebForm type (err: %d)", err);
        result = false;
        goto cleanup;
    }

    g_CurlSlistType = handlesys->CreateType("CurlSlist", &g_CurlTypeHandler, 0,
            NULL, NULL, self, &err);
    if (0 == g_CurlSlistType)
    {
        handlesys->RemoveType(g_CurlSlistType, self);
        snprintf(error, maxlength, "Could not create CurlSlist type (err: %d)", err);
        result = false;
        goto cleanup;
    }

cleanup:
    if (result)
    {
        sharesys->RegisterLibrary(myself, "curl");
    }
    else
    {
        if (g_CurlFileType)
        {
            handlesys->RemoveType(g_CurlFileType, self);
            g_CurlFileType = 0;
        }
        if (g_CurlHandleType)
        {
            handlesys->RemoveType(g_CurlHandleType, self);
            g_CurlHandleType = 0;
        }
        if (g_CurlWebFormType)
        {
            handlesys->RemoveType(g_CurlWebFormType, self);
            g_CurlWebFormType = 0;
        }
        if (g_CurlSlistType)
        {
            handlesys->RemoveType(g_CurlSlistType, self);
            g_CurlSlistType = 0;
        }
    }
    return result;
}

void SmCurl::SDK_OnUnload()
{
    IdentityToken_t *self = myself->GetIdentity();

    if (g_CurlFileType)
    {
        handlesys->RemoveType(g_CurlFileType, self);
        g_CurlFileType = 0;
    }
    if (g_CurlHandleType)
    {
        handlesys->RemoveType(g_CurlHandleType, self);
        g_CurlHandleType = 0;
    }
    if (g_CurlWebFormType)
    {
        handlesys->RemoveType(g_CurlWebFormType, self);
        g_CurlWebFormType = 0;
    }
    if (g_CurlSlistType)
    {
        handlesys->RemoveType(g_CurlSlistType, self);
        g_CurlSlistType = 0;
    }

    curl_global_cleanup();
}

void SmCurl::SDK_OnAllLoaded()
{
    sharesys->AddNatives(myself, smcurl_natives);
}

bool SmCurl::QueryRunning(char *error, size_t maxlength)
{
    return true;
}

SmCurl g_SmCurl;	    /**< Global singleton for extension's main interface */
SMEXT_LINK(&g_SmCurl);
