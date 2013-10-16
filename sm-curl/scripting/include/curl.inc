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
 * vim: set ts=4 et ft=sourcepawn :
 */
/**
 * @file
 * @brief sm-curl sourcepawn includes
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#if defined _curl_included
 #endinput
#endif
#define _curl_included

#define CURLINFO_STRING   0x100000
#define CURLINFO_LONG     0x200000
#define CURLINFO_DOUBLE   0x300000
#define CURLINFO_SLIST    0x400000
#define CURLINFO_MASK     0x0fffff
#define CURLINFO_TYPEMASK 0xf00000

enum CURLINFO {
  CURLINFO_NONE, /* first, never use this */
  CURLINFO_EFFECTIVE_URL    = CURLINFO_STRING + 1,
  CURLINFO_RESPONSE_CODE    = CURLINFO_LONG   + 2,
  CURLINFO_TOTAL_TIME       = CURLINFO_DOUBLE + 3,
  CURLINFO_NAMELOOKUP_TIME  = CURLINFO_DOUBLE + 4,
  CURLINFO_CONNECT_TIME     = CURLINFO_DOUBLE + 5,
  CURLINFO_PRETRANSFER_TIME = CURLINFO_DOUBLE + 6,
  CURLINFO_SIZE_UPLOAD      = CURLINFO_DOUBLE + 7,
  CURLINFO_SIZE_DOWNLOAD    = CURLINFO_DOUBLE + 8,
  CURLINFO_SPEED_DOWNLOAD   = CURLINFO_DOUBLE + 9,
  CURLINFO_SPEED_UPLOAD     = CURLINFO_DOUBLE + 10,
  CURLINFO_HEADER_SIZE      = CURLINFO_LONG   + 11,
  CURLINFO_REQUEST_SIZE     = CURLINFO_LONG   + 12,
  CURLINFO_SSL_VERIFYRESULT = CURLINFO_LONG   + 13,
  CURLINFO_FILETIME         = CURLINFO_LONG   + 14,
  CURLINFO_CONTENT_LENGTH_DOWNLOAD   = CURLINFO_DOUBLE + 15,
  CURLINFO_CONTENT_LENGTH_UPLOAD     = CURLINFO_DOUBLE + 16,
  CURLINFO_STARTTRANSFER_TIME = CURLINFO_DOUBLE + 17,
  CURLINFO_CONTENT_TYPE     = CURLINFO_STRING + 18,
  CURLINFO_REDIRECT_TIME    = CURLINFO_DOUBLE + 19,
  CURLINFO_REDIRECT_COUNT   = CURLINFO_LONG   + 20,
  CURLINFO_PRIVATE          = CURLINFO_STRING + 21,
  CURLINFO_HTTP_CONNECTCODE = CURLINFO_LONG   + 22,
  CURLINFO_HTTPAUTH_AVAIL   = CURLINFO_LONG   + 23,
  CURLINFO_PROXYAUTH_AVAIL  = CURLINFO_LONG   + 24,
  CURLINFO_OS_ERRNO         = CURLINFO_LONG   + 25,
  CURLINFO_NUM_CONNECTS     = CURLINFO_LONG   + 26,
  CURLINFO_SSL_ENGINES      = CURLINFO_SLIST  + 27,
  CURLINFO_COOKIELIST       = CURLINFO_SLIST  + 28,
  CURLINFO_LASTSOCKET       = CURLINFO_LONG   + 29,
  CURLINFO_FTP_ENTRY_PATH   = CURLINFO_STRING + 30,
  CURLINFO_REDIRECT_URL     = CURLINFO_STRING + 31,
  CURLINFO_PRIMARY_IP       = CURLINFO_STRING + 32,
  CURLINFO_APPCONNECT_TIME  = CURLINFO_DOUBLE + 33,
  CURLINFO_CERTINFO         = CURLINFO_SLIST  + 34,
  CURLINFO_CONDITION_UNMET  = CURLINFO_LONG   + 35,
  CURLINFO_RTSP_SESSION_ID  = CURLINFO_STRING + 36,
  CURLINFO_RTSP_CLIENT_CSEQ = CURLINFO_LONG   + 37,
  CURLINFO_RTSP_SERVER_CSEQ = CURLINFO_LONG   + 38,
  CURLINFO_RTSP_CSEQ_RECV   = CURLINFO_LONG   + 39,
  CURLINFO_PRIMARY_PORT     = CURLINFO_LONG   + 40,
  CURLINFO_LOCAL_IP         = CURLINFO_STRING + 41,
  CURLINFO_LOCAL_PORT       = CURLINFO_LONG   + 42,
  /* Fill in new entries below here! */

  CURLINFO_LASTONE          = 42
};

/* CURLINFO_RESPONSE_CODE is the new name for the option previously known as
   CURLINFO_HTTP_CODE */
#define CURLINFO_HTTP_CODE CURLINFO_RESPONSE_CODE

/**
 * URL encode the given string
 *
 * @param[in] curl a curl session handle
 * @param[in] url the URL to encode
 * @param[out] buffer   the buffer to store the URL escaped string
 * @param[in] maxlen maximum length of \p buffer
 *
 * @return true on success
 */
native bool:curl_easy_escape(Handle:curl, const String:url[], String:buffer[], maxlen);

/**
 * Start a libcurl easy session
 *
 * @return a curl easy handle to be used with other easy-functions
 */
native Handle:curl_easy_init();

/**
 * End a libcurl easy session
 */
native Handle:curl_easy_cleanup();

/**
 * Clone a libcurl easy session handle
 *
 * @param[in] handle the session handle to clone
 * @return a new session handle
 */
native Handle:curl_easy_duphandle(Handle:handle);

/**
 * Retrieve internal information from a curl session
 *
 * Note that for string data you must also pass a 4th parameter for the
 * maximum length of the string buffer.
 *
 * @param[in] curl a curl session handle
 * @param[in] info the CURLINFO code for the data you wish to retrieve
 * @param[out] ...  variable to store contain the retrieved data
 * @return #CURLE_OK on success
 */
native CURLcode:curl_easy_getinfo(Handle:curl, CURLINFO:info, ...);

/**
 * Explicitly mark a running connection to be paused, or unpause a connection
 * that was previously paused
 *
 * @param[in] curl a curl session handle
 * @param[in] bitmask   a set of bits that sets the new state of the
 *                      connection, see the curl documentation for more
 *                      information.
 * @return #CURLE_OK on success
 */
native CURLcode:curl_easy_pause(Handle:curl, bitmask);

/**
 * Perform a file transfer
 *
 * @param[in] curl a curl session handle
 * @return #CURLE_OK on success
 */
native CURLcode:curl_easy_perform(Handle:curl);

/**
 * Receive raw data on an easy connection
 *
 * @param[in] curl  a curl session handle
 * @param[out] buffer   string buffer to store any received data
 * @param[in] buflen    max length of \p buffer
 * @param[out] n    will contain the actual number of bytes received on success
 * @return #CURLE_OK on success
 */
native CURLcode:curl_easy_recv(Handle:curl, String[]:buffer, buflen, n);

/**
 * Reset all options of a curl session handle
 *
 * @param[in] handle    a curl session handle
 */
native curl_easy_reset(Handle:handle);

/**
 * Send raw data on an easy connection
 *
 * @param[in] curl  a curl session handle
 * @param[in] buffer    string buffer containing data to send
 * @param[in] buflen    length of \p buffer
 * @param[out] n    will contain the actual number of bytes sent on success
 * @return #CURLE_OK on success
 */
native CURLcode:curl_easy_send(Handle:curl, String[]:buffer, buflen, n);

/**
 * Set options for a curl easy session handle
 *
 * @param[in] handle    a curl session handle
 * @param[in] option    the option to set
 * @param[in] ...       the data to set \p option
 * @return #CURLE_OK on success
 *
 * \note Options that take curl_off_t types must be set using a String[]
 * representation of the number, since sourcemod does not support 64-bit
 * integers.
 */
native CURLcode:curl_easy_setopt(Handle:handle, CURLoption:option, ...);

/**
 * Retrieve string describing error code
 *
 * @param[in] errornum  the error number
 * @param[out] buffer   string to store the error message
 * @param[in] maxlen    max length of \p buffer
 */
native curl_easy_strerror(CURLcode:errornum, String[]:buffer, maxlen);

/**
 * URL decode the given string
 *
 * @param[in] curl a curl session handle
 * @param[in] url the URL to decode
 * @param[out] buffer   the buffer to store the unescaped URL string
 * @param[in] maxlen maximum length of \p buffer
 *
 * @return the length of the decoded url string
 */
native curl_easy_escape(Handle:curl, const String:url[], String:buffer[], maxlen);

/**
 * Do not edit below this line!
 */
public Extension:__ext_zip =
{
    name = "Curl",
    file = "smcurl.ext",
#if defined AUTOLOAD_EXTENSIONS
    autoload = 1,
#else
    autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
    required = 1,
#else
    required = 0,
#endif
};
