// Copyright (c) 2018 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=52ba75e5a733ed0be9a064b9794c0181e714c090$
//

#ifndef CEF_INCLUDE_CAPI_CEF_REQUEST_HANDLER_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_REQUEST_HANDLER_CAPI_H_
#pragma once

#include "include/capi/cef_auth_callback_capi.h"
#include "include/capi/cef_base_capi.h"
#include "include/capi/cef_browser_capi.h"
#include "include/capi/cef_frame_capi.h"
#include "include/capi/cef_request_capi.h"
#include "include/capi/cef_resource_handler_capi.h"
#include "include/capi/cef_response_capi.h"
#include "include/capi/cef_response_filter_capi.h"
#include "include/capi/cef_ssl_info_capi.h"
#include "include/capi/cef_x509_certificate_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

///
// Callback structure used for asynchronous continuation of url requests.
///
typedef struct _cef_request_callback_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Continue the url request. If |allow| is true (1) the request will be
  // continued. Otherwise, the request will be canceled.
  ///
  void(CEF_CALLBACK* cont)(struct _cef_request_callback_t* self, int allow);

  ///
  // Cancel the url request.
  ///
  void(CEF_CALLBACK* cancel)(struct _cef_request_callback_t* self);
} cef_request_callback_t;

///
// Callback structure used to select a client certificate for authentication.
///
typedef struct _cef_select_client_certificate_callback_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Chooses the specified certificate for client certificate authentication.
  // NULL value means that no client certificate should be used.
  ///
  void(CEF_CALLBACK* select)(
      struct _cef_select_client_certificate_callback_t* self,
      struct _cef_x509certificate_t* cert);
} cef_select_client_certificate_callback_t;

///
// Implement this structure to handle events related to browser requests. The
// functions of this structure will be called on the thread indicated.
///
typedef struct _cef_request_handler_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Called on the UI thread before browser navigation. Return true (1) to
  // cancel the navigation or false (0) to allow the navigation to proceed. The
  // |request| object cannot be modified in this callback.
  // cef_load_handler_t::OnLoadingStateChange will be called twice in all cases.
  // If the navigation is allowed cef_load_handler_t::OnLoadStart and
  // cef_load_handler_t::OnLoadEnd will be called. If the navigation is canceled
  // cef_load_handler_t::OnLoadError will be called with an |errorCode| value of
  // ERR_ABORTED.
  ///
  int(CEF_CALLBACK* on_before_browse)(struct _cef_request_handler_t* self,
                                      struct _cef_browser_t* browser,
                                      struct _cef_frame_t* frame,
                                      struct _cef_request_t* request,
                                      int is_redirect);

  ///
  // Called on the UI thread before OnBeforeBrowse in certain limited cases
  // where navigating a new or different browser might be desirable. This
  // includes user-initiated navigation that might open in a special way (e.g.
  // links clicked via middle-click or ctrl + left-click) and certain types of
  // cross-origin navigation initiated from the renderer process (e.g.
  // navigating the top-level frame to/from a file URL). The |browser| and
  // |frame| values represent the source of the navigation. The
  // |target_disposition| value indicates where the user intended to navigate
  // the browser based on standard Chromium behaviors (e.g. current tab, new
  // tab, etc). The |user_gesture| value will be true (1) if the browser
  // navigated via explicit user gesture (e.g. clicking a link) or false (0) if
  // it navigated automatically (e.g. via the DomContentLoaded event). Return
  // true (1) to cancel the navigation or false (0) to allow the navigation to
  // proceed in the source browser's top-level frame.
  ///
  int(CEF_CALLBACK* on_open_urlfrom_tab)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      const cef_string_t* target_url,
      cef_window_open_disposition_t target_disposition,
      int user_gesture);

  ///
  // Called on the IO thread before a resource request is loaded. The |request|
  // object may be modified. Return RV_CONTINUE to continue the request
  // immediately. Return RV_CONTINUE_ASYNC and call cef_request_tCallback::
  // cont() at a later time to continue or cancel the request asynchronously.
  // Return RV_CANCEL to cancel the request immediately.
  //
  ///
  cef_return_value_t(CEF_CALLBACK* on_before_resource_load)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      struct _cef_request_t* request,
      struct _cef_request_callback_t* callback);

  ///
  // Called on the IO thread before a resource is loaded. To allow the resource
  // to load normally return NULL. To specify a handler for the resource return
  // a cef_resource_handler_t object. The |request| object should not be
  // modified in this callback.
  ///
  struct _cef_resource_handler_t*(CEF_CALLBACK* get_resource_handler)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      struct _cef_request_t* request);

  ///
  // Called on the IO thread when a resource load is redirected. The |request|
  // parameter will contain the old URL and other request-related information.
  // The |response| parameter will contain the response that resulted in the
  // redirect. The |new_url| parameter will contain the new URL and can be
  // changed if desired. The |request| object cannot be modified in this
  // callback.
  ///
  void(CEF_CALLBACK* on_resource_redirect)(struct _cef_request_handler_t* self,
                                           struct _cef_browser_t* browser,
                                           struct _cef_frame_t* frame,
                                           struct _cef_request_t* request,
                                           struct _cef_response_t* response,
                                           cef_string_t* new_url);

  ///
  // Called on the IO thread when a resource response is received. To allow the
  // resource to load normally return false (0). To redirect or retry the
  // resource modify |request| (url, headers or post body) and return true (1).
  // The |response| object cannot be modified in this callback.
  ///
  int(CEF_CALLBACK* on_resource_response)(struct _cef_request_handler_t* self,
                                          struct _cef_browser_t* browser,
                                          struct _cef_frame_t* frame,
                                          struct _cef_request_t* request,
                                          struct _cef_response_t* response);

  ///
  // Called on the IO thread to optionally filter resource response content.
  // |request| and |response| represent the request and response respectively
  // and cannot be modified in this callback.
  ///
  struct _cef_response_filter_t*(CEF_CALLBACK* get_resource_response_filter)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      struct _cef_request_t* request,
      struct _cef_response_t* response);

  ///
  // Called on the IO thread when a resource load has completed. |request| and
  // |response| represent the request and response respectively and cannot be
  // modified in this callback. |status| indicates the load completion status.
  // |received_content_length| is the number of response bytes actually read.
  ///
  void(CEF_CALLBACK* on_resource_load_complete)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      struct _cef_request_t* request,
      struct _cef_response_t* response,
      cef_urlrequest_status_t status,
      int64 received_content_length);

  ///
  // Called on the IO thread when the browser needs credentials from the user.
  // |isProxy| indicates whether the host is a proxy server. |host| contains the
  // hostname and |port| contains the port number. |realm| is the realm of the
  // challenge and may be NULL. |scheme| is the authentication scheme used, such
  // as "basic" or "digest", and will be NULL if the source of the request is an
  // FTP server. Return true (1) to continue the request and call
  // cef_auth_callback_t::cont() either in this function or at a later time when
  // the authentication information is available. Return false (0) to cancel the
  // request immediately.
  ///
  int(CEF_CALLBACK* get_auth_credentials)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      int isProxy,
      const cef_string_t* host,
      int port,
      const cef_string_t* realm,
      const cef_string_t* scheme,
      struct _cef_auth_callback_t* callback);

  ///
  // Called on the IO thread when JavaScript requests a specific storage quota
  // size via the webkitStorageInfo.requestQuota function. |origin_url| is the
  // origin of the page making the request. |new_size| is the requested quota
  // size in bytes. Return true (1) to continue the request and call
  // cef_request_tCallback::cont() either in this function or at a later time to
  // grant or deny the request. Return false (0) to cancel the request
  // immediately.
  ///
  int(CEF_CALLBACK* on_quota_request)(struct _cef_request_handler_t* self,
                                      struct _cef_browser_t* browser,
                                      const cef_string_t* origin_url,
                                      int64 new_size,
                                      struct _cef_request_callback_t* callback);

  ///
  // Called on the UI thread to handle requests for URLs with an unknown
  // protocol component. Set |allow_os_execution| to true (1) to attempt
  // execution via the registered OS protocol handler, if any. SECURITY WARNING:
  // YOU SHOULD USE THIS METHOD TO ENFORCE RESTRICTIONS BASED ON SCHEME, HOST OR
  // OTHER URL ANALYSIS BEFORE ALLOWING OS EXECUTION.
  ///
  void(CEF_CALLBACK* on_protocol_execution)(struct _cef_request_handler_t* self,
                                            struct _cef_browser_t* browser,
                                            const cef_string_t* url,
                                            int* allow_os_execution);

  ///
  // Called on the UI thread to handle requests for URLs with an invalid SSL
  // certificate. Return true (1) and call cef_request_tCallback::cont() either
  // in this function or at a later time to continue or cancel the request.
  // Return false (0) to cancel the request immediately. If
  // CefSettings.ignore_certificate_errors is set all invalid certificates will
  // be accepted without calling this function.
  ///
  int(CEF_CALLBACK* on_certificate_error)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      cef_errorcode_t cert_error,
      const cef_string_t* request_url,
      struct _cef_sslinfo_t* ssl_info,
      struct _cef_request_callback_t* callback);

  ///
  // Called on the UI thread when a client certificate is being requested for
  // authentication. Return false (0) to use the default behavior and
  // automatically select the first certificate available. Return true (1) and
  // call cef_select_client_certificate_callback_t::Select either in this
  // function or at a later time to select a certificate. Do not call Select or
  // call it with NULL to continue without using any certificate. |isProxy|
  // indicates whether the host is an HTTPS proxy or the origin server. |host|
  // and |port| contains the hostname and port of the SSL server. |certificates|
  // is the list of certificates to choose from; this list has already been
  // pruned by Chromium so that it only contains certificates from issuers that
  // the server trusts.
  ///
  int(CEF_CALLBACK* on_select_client_certificate)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      int isProxy,
      const cef_string_t* host,
      int port,
      size_t certificatesCount,
      struct _cef_x509certificate_t* const* certificates,
      struct _cef_select_client_certificate_callback_t* callback);

  ///
  // Called on the browser process UI thread when a plugin has crashed.
  // |plugin_path| is the path of the plugin that crashed.
  ///
  void(CEF_CALLBACK* on_plugin_crashed)(struct _cef_request_handler_t* self,
                                        struct _cef_browser_t* browser,
                                        const cef_string_t* plugin_path);

  ///
  // Called on the browser process UI thread when the render view associated
  // with |browser| is ready to receive/handle IPC messages in the render
  // process.
  ///
  void(CEF_CALLBACK* on_render_view_ready)(struct _cef_request_handler_t* self,
                                           struct _cef_browser_t* browser);

  ///
  // Called on the browser process UI thread when the render process terminates
  // unexpectedly. |status| indicates how the process terminated.
  ///
  void(CEF_CALLBACK* on_render_process_terminated)(
      struct _cef_request_handler_t* self,
      struct _cef_browser_t* browser,
      cef_termination_status_t status);
} cef_request_handler_t;

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_REQUEST_HANDLER_CAPI_H_
