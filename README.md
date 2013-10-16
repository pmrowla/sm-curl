sm-curl
=======
Sourcemod libcurl wrapper extension

Some effort has been made to maintain API compatibility with raydan's [cURL & Hash](https://forums.alliedmods.net/showthread.php?t=152216) extension, but full compatibility is not guaranteed.

Notes
-----
Calls to `curl_global_init` and `curl_global_cleanup` are performed upon loading/unloading of the extension, so they do not appear in the extension's API.

Currently only the libcurl "easy" interface is implemented.
There are no plans to implement the "multi" or "share" interfaces at this time.

The following functions are marked as deprecated in libcurl are not implemented:
* `curl_escape`
* `curl_unescape`
* `curl_strequal`
* `curl_strunequal`

License
-------
sm-curl is distributed under the GNU General Public License version 3.
See [COPYING.md](https://github.com/pmrowla/sm-curl/blob/master/COPYING.md) for more information.

curl is copyright (c) 1996-2013 Daniel Stenberg and is distributed under an [MIT derivative license](http://curl.haxx.se/docs/copyright.html).
