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
 * @brief sm-curl natives
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#include <cstdlib>
#include <curl/curl.h>

#include "smcurl.h"

#define CURL_INIT()                                                         \
    Handle_t handle = static_cast<Handle_t>(params[1]);                     \
    HandleError err;                                                        \
    HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity);       \
    CURL *curl;                                                             \
    err = handlesys->ReadHandle(handle, g_CurlType, &sec, (void **)&curl);  \
    if (HandleError_None != err)                                            \
        return pCtx->ThrowNativeError("Invalid CURL * handle %x (error %d)", handle, err);

static cell_t smcurl_easy_escape(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_init(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_cleanup(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_duphandle(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_getinfo(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_pause(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_perform(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_recv(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_reset(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_send(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_setopt(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_strerror(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_easy_unescape(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_formadd(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_formfree(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_free(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_getdate(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_slist_append(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_slist_free_all(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_version(IPluginContext *pCtx, const cell_t *params);
static cell_t smcurl_version_info(IPluginContext *pCtx, const cell_t *params);

sp_nativeinfo_t smcurl_natives[] =
{
    {"curl_easy_escape", smcurl_easy_escape},
    {"curl_easy_init", smcurl_easy_init},
    {"curl_easy_cleanup", smcurl_easy_cleanup},
    {"curl_easy_duphandle", smcurl_easy_duphandle},
    {"curl_easy_getinfo", smcurl_easy_getinfo},
    {"curl_easy_pause", smcurl_easy_pause},
    {"curl_easy_perform", smcurl_easy_perform},
    {"curl_easy_recv", smcurl_easy_recv},
    {"curl_easy_reset", smcurl_easy_reset},
    {"curl_easy_send", smcurl_easy_send},
    {"curl_easy_setopt", smcurl_easy_setopt},
    {"curl_easy_strerror", smcurl_easy_strerror},
    {"curl_easy_unescape", smcurl_easy_unescape},
    {NULL, NULL}
};

static cell_t smcurl_easy_escape(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    char *url;
    pCtx->LocalToString(params[2], &url);
    char *buffer = curl_easy_escape(curl, url, 0);
    if (NULL == buffer)
        return 0;

    pCtx->StringToLocalUTF8(params[3], params[4], buffer, NULL);
    curl_free(buffer);
    return 1;
}

static cell_t smcurl_easy_init(IPluginContext *pCtx, const cell_t *params)
{
    CURL *curl = curl_easy_init();
    if (NULL == curl)
        return BAD_HANDLE;

    Handle_t handle = handlesys->CreateHandle(g_CurlHandleType, curl,
            pCtx->GetIdentity(), myself->GetIdentity(), NULL);
    if (!handle)
    {
        curl_easy_cleanup(curl);
        return BAD_HANDLE;
    }

    return handle;
}

static cell_t smcurl_easy_cleanup(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    curl_easy_cleanup(curl);
}

static cell_t smcurl_easy_duphandle(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    CURL *newcurl = curl_easy_duphandle(curl);
    if (NULL == newcurl)
        return BAD_HANDLE;

    Handle_t handle = handlesys->CreateHandle(g_CurlHandleType, newcurl,
            pCtx->GetIdentity(), myself->GetIdentity(), NULL);
    if (!handle)
    {
        curl_easy_cleanup(newcurl);
        return BAD_HANDLE;
    }

    return handle;
}

static cell_t smcurl_easy_getinfo(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();
    CURLcode code;

    int type = (CURLINFO_TYPEMASK & static_cast<int>(params[2]));
    CURLINFO info = static_cast<CURLINFO>params[2];
    switch (type)
    {
        case CURLINFO_LONG:
            cell_t *addr;
            pCtx->LocalToPhysAddr(params[3], &addr);
            long n;
            code = curl_easy_getinfo(curl, info, &n);
            if (CURLE_OK == code)
                *addr = static_cast<cell_t>(n);
            break;
        case CURLINFO_DOUBLE:
            cell_t *addr;
            pCtx->LocalToPhysAddr(params[3], &addr);
            double n;
            code = curl_easy_getinfo(curl, info, &n);
            if (CURLE_OK == code)
                *addr = static_cast<cell_t>(n);
            break;
        case CURLINFO_STRING:
            if (params[0] < 4)
                return CURLE_BAD_FUNCTION_ARGUMENT;

            char *str;
            code = curl_easy_getinfo(curl, info, &str);
            if (CURLE_OK == code)
                pCtx->StringToLocalUTF8(params[3], params[4], str, NULL);
            break;
    }

    return code;
}

static cell_t smcurl_easy_pause(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    return curl_easy_pause(curl, static_cast<int>(params[2]));
}

static cell_t smcurl_easy_perform(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    return curl_easy_perform(curl);
}

static cell_t smcurl_easy_recv(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    size_t buflen = static_cast<size_t>(params[3]);
    char *buffer = malloc(buflen);
    if (NULL == buffer)
        return CURLE_OUT_OF_MEMORY;
    size_t n = 0;
    cell_t *addr;
    pCtx->LocalToPhysAddr(params[4], &addr);

    CURLcode code = curl_easy_recv(curl, buffer, buflen, &n);
    if (CURLE_OK == code)
    {
        *addr = static_cast<cell_t>(n);
        pCtx->StringToLocalUTF8(params[3], *addr, buffer, NULL);
    }
    free(buffer);

    return code;
}

static cell_t smcurl_easy_reset(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    curl_easy_reset(curl);

    return 1;
}

static cell_t smcurl_easy_send(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    char *buffer;
    pCtx->LocalToString(params[2], &buffer);
    size_t buflen = static_cast<size_t>(params[3]);
    size_t n;
    cell_t *addr;
    pCtx->LocalToPhysAddr(params[4], &addr);

    CURLcode code = curl_easy_send(curl, buffer, buflen, &n);
    if (CURLE_OK == code)
        *addr = static_cast<cell_t>(n);

    return code;
}

static cell_t smcurl_easy_setopt(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    CURLcode code = CURLE_OK;
    CURLoption option = static_cast<CURLoption>(params[2]);

    if (option > CURLOPTTYPE_OFF_T)
    {
        char *buffer;
        pCtx->LocalToString(params[3], &buffer);

#ifdef WIN32
        curl_off_t offset = static_cast<curl_off_t>(_atoi64(buffer));
#else
        curl_off_t offset = static_cast<curl_off_t>(atoll(buffer));
#endif
        code = curl_easy_setopt(curl, option, offset);
    }
    else if (option > CURLOPTTYPE_FUNCTIONPOINT)
    {
        // TODO
    }
    else if (option > CURLOPTTYPE_OBJECTPOINT)
    {
        // TODO
    }
    else // CURLOPTTYPE_LONG
    {
        long parameter = static_cast<long>(params[3]);
        code = curl_easy_setopt(curl, option, parameter);
    }

    return code;
}

static cell_t smcurl_easy_strerror(IPluginContext *pCtx, const cell_t *params)
{
    const char *str = curl_easy_strerror(static_cast<CURLcode>(params[1]));
    pCtx->StringToLocalUTF8(params[2], params[3], str, NULL);
    return 1;
}

static cell_t smcurl_easy_unescape(IPluginContext *pCtx, const cell_t *params)
{
    CURL_INIT();

    int outlength;
    char *url;
    pCtx->LocalToString(params[2], &url);
    char *buffer = curl_easy_unescape(curl, url, 0 &outlength);
    if (NULL == buffer)
        return 0;

    pCtx->StringToLocalUTF8(params[3], params[4], buffer, NULL);
    curl_free(buffer);
    return static_cast<cell_t>(outlength);
}
