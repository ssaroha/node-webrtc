// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_LIB_AW_CONTENT_BROWSER_CLIENT_H_
#define ANDROID_WEBVIEW_LIB_AW_CONTENT_BROWSER_CLIENT_H_

#include <stddef.h>

#include <memory>

#include "android_webview/browser/aw_web_preferences_populater.h"
#include "base/compiler_specific.h"
#include "base/macros.h"
#include "content/public/browser/content_browser_client.h"

namespace android_webview {

class AwBrowserContext;
class JniDependencyFactory;

class AwContentBrowserClient : public content::ContentBrowserClient {
 public:
  // This is what AwContentBrowserClient::GetAcceptLangs uses.
  static std::string GetAcceptLangsImpl();

  // Deprecated: use AwBrowserContext::GetDefault() instead.
  static AwBrowserContext* GetAwBrowserContext();

  AwContentBrowserClient(JniDependencyFactory* native_factory);
  ~AwContentBrowserClient() override;

  // Allows AwBrowserMainParts to initialize a BrowserContext at the right
  // moment during startup. AwContentBrowserClient owns the result.
  AwBrowserContext* InitBrowserContext();

  content::BrowserMainParts* CreateBrowserMainParts(
      const content::MainFunctionParams& parameters) override;
  content::WebContentsViewDelegate* GetWebContentsViewDelegate(
      content::WebContents* web_contents) override;
  void RenderProcessWillLaunch(content::RenderProcessHost* host) override;
  bool IsHandledURL(const GURL& url) override;
  std::string GetCanonicalEncodingNameByAliasName(
      const std::string& alias_name) override;
  void AppendExtraCommandLineSwitches(base::CommandLine* command_line,
                                      int child_process_id) override;
  std::string GetApplicationLocale() override;
  std::string GetAcceptLangs(content::BrowserContext* context) override;
  const gfx::ImageSkia* GetDefaultFavicon() override;
  bool AllowAppCache(const GURL& manifest_url,
                     const GURL& first_party,
                     content::ResourceContext* context) override;
  bool AllowGetCookie(const GURL& url,
                      const GURL& first_party,
                      const net::CookieList& cookie_list,
                      content::ResourceContext* context,
                      int render_process_id,
                      int render_frame_id) override;
  bool AllowSetCookie(const GURL& url,
                      const GURL& first_party,
                      const std::string& cookie_line,
                      content::ResourceContext* context,
                      int render_process_id,
                      int render_frame_id,
                      const net::CookieOptions& options) override;
  void AllowWorkerFileSystem(
      const GURL& url,
      content::ResourceContext* context,
      const std::vector<std::pair<int, int>>& render_frames,
      base::Callback<void(bool)> callback) override;
  bool AllowWorkerIndexedDB(
      const GURL& url,
      const base::string16& name,
      content::ResourceContext* context,
      const std::vector<std::pair<int, int>>& render_frames) override;
  content::QuotaPermissionContext* CreateQuotaPermissionContext() override;
  void AllowCertificateError(
      content::WebContents* web_contents,
      int cert_error,
      const net::SSLInfo& ssl_info,
      const GURL& request_url,
      content::ResourceType resource_type,
      bool overridable,
      bool strict_enforcement,
      bool expired_previous_decision,
      const base::Callback<void(content::CertificateRequestResultType)>&
          callback) override;
  void SelectClientCertificate(
      content::WebContents* web_contents,
      net::SSLCertRequestInfo* cert_request_info,
      std::unique_ptr<content::ClientCertificateDelegate> delegate) override;
  bool CanCreateWindow(const GURL& opener_url,
                       const GURL& opener_top_level_frame_url,
                       const GURL& source_origin,
                       WindowContainerType container_type,
                       const GURL& target_url,
                       const content::Referrer& referrer,
                       const std::string& frame_name,
                       WindowOpenDisposition disposition,
                       const blink::WebWindowFeatures& features,
                       bool user_gesture,
                       bool opener_suppressed,
                       content::ResourceContext* context,
                       int render_process_id,
                       int opener_render_view_id,
                       int opener_render_frame_id,
                       bool* no_javascript_access) override;
  void ResourceDispatcherHostCreated() override;
  net::NetLog* GetNetLog() override;
  void ClearCache(content::RenderFrameHost* rfh) override;
  void ClearCookies(content::RenderFrameHost* rfh) override;
  base::FilePath GetDefaultDownloadDirectory() override;
  std::string GetDefaultDownloadName() override;
  void DidCreatePpapiPlugin(content::BrowserPpapiHost* browser_host) override;
  bool AllowPepperSocketAPI(
      content::BrowserContext* browser_context,
      const GURL& url,
      bool private_api,
      const content::SocketPermissionRequest* params) override;
  bool IsPepperVpnProviderAPIAllowed(content::BrowserContext* browser_context,
                                     const GURL& url) override;
  content::TracingDelegate* GetTracingDelegate() override;
  void GetAdditionalMappedFilesForChildProcess(
      const base::CommandLine& command_line,
      int child_process_id,
      content::FileDescriptorInfo* mappings,
      std::map<int, base::MemoryMappedFile::Region>* regions) override;
  void OverrideWebkitPrefs(content::RenderViewHost* rvh,
                           content::WebPreferences* web_prefs) override;
  ScopedVector<content::NavigationThrottle> CreateThrottlesForNavigation(
      content::NavigationHandle* navigation_handle) override;
  content::DevToolsManagerDelegate* GetDevToolsManagerDelegate() override;

 private:
  // Android WebView currently has a single global (non-off-the-record) browser
  // context.
  std::unique_ptr<AwBrowserContext> browser_context_;
  std::unique_ptr<AwWebPreferencesPopulater> preferences_populater_;

  JniDependencyFactory* native_factory_;

  DISALLOW_COPY_AND_ASSIGN(AwContentBrowserClient);
};

}  // namespace android_webview

#endif  // ANDROID_WEBVIEW_LIB_AW_CONTENT_BROWSER_CLIENT_H_
