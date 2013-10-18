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
 * @brief sm-curl extension header
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#ifndef _SMCURL_H_
#define _SMCURL_H_

#include <curl/curl.h>

#include "smsdk_ext.h"

struct CurlData
{
    IPluginFunction *write;
    IPluginFunction *read;
};

/**
 * @brief sm-curl implementation of the SDK Extension.
 * Note: Uncomment one of the pre-defined virtual functions in order to use it.
 */
class SmCurl : public SDKExtension
{
public:
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
	virtual void SDK_OnUnload();
	virtual void SDK_OnAllLoaded();
	virtual bool QueryRunning(char *error, size_t maxlength);
};

class CurlTypeHandler : public IHandleTypeDispatch
{
public:
    void OnHandleDestroy(HandleType_t, void *object);
};

extern HandleType_t g_CurlFileType;
extern HandleType_t g_CurlHandleType;
extern HandleType_t g_CurlWebFormType;
extern HandleType_t g_CurlSlistType;

extern const sp_nativeinfo_t smcurl_natives[];

#endif // ! _SMCURL_H_
