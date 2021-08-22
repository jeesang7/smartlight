/*
   Copyright 2021 Samsung Electronics Co., Ltd

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <wifi-manager.h>

#include "stdevice_manager_log.h"
#include "stdevice_manager_softap.h"
#include "stdevice_manager_homeap.h"

#define ES_WIFI_SCAN_RETRY_CNT_MAX (3)

static char* gHomeAPSsid;
static char* gHomeAPPwd;
static int gWifiScanRetryCnt = 0;
static bool gWifiFound = false;

static void ESWifiScanSpecificAPCb(wifi_manager_error_e error_code, void* user_data);

static gboolean _wifi_deinitialize(gpointer data)
{
	STDK_LOG_ENTER;

	wifi_manager_h wifi_h = (wifi_manager_h)data;

	if (wifi_manager_deinitialize(wifi_h) != 0)
		STDK_LOGE( "wifi_manager_deinitialize() failed");

	return G_SOURCE_REMOVE;
}

static gboolean _wifi_scan_specific(gpointer user_data)
{
	STDK_LOGI( "Wifi scan retry [count=%d]", gWifiScanRetryCnt);

	wifi_manager_h wifi_h = (wifi_manager_h)user_data;

	if (wifi_manager_scan_specific_ap(wifi_h, gHomeAPSsid, ESWifiScanSpecificAPCb, user_data) != 0)
		STDK_LOGE( "wifi_manager_scan_specific_ap() failed");

	return G_SOURCE_REMOVE;
}

static gboolean _wifi_deactivate_cb(gpointer user_data)
{
	STDK_LOG_ENTER;

	ESStartSoftAP();

	g_idle_add(_wifi_deinitialize, user_data);

	return G_SOURCE_REMOVE;
}

static void ESWifiConnectedCb(wifi_manager_error_e result, void* user_data)
{
	STDK_LOG_ENTER;

	STDK_LOGI("[VERDICT] ESWifiConnectedCb");

	if (result != WIFI_MANAGER_ERROR_NONE) {
		STDK_LOGE( "result error [%d]", result);
		return;
	}

	g_idle_add(_wifi_deinitialize, user_data);
}

static void __wifi_connect_again_cb(wifi_manager_connection_state_e state, wifi_manager_ap_h param_ap, void *user_data)
{
	STDK_LOGI( "__wifi_connect_again_cb state [%d]", state);
}

static bool ESWifiFoundSpecificAPCb(wifi_manager_ap_h ap, void *user_data)
{
	STDK_LOG_ENTER;
	char *ap_name = NULL;
	wifi_manager_h wifi_h = (wifi_manager_h)user_data;

	if (wifi_manager_ap_get_essid(ap, &ap_name) != 0) {
		STDK_LOGE( "wifi_manager_ap_get_essid() failed");
		return false;
	}
	STDK_LOGI( "AP : [%s]", ap_name);
	free(ap_name);

	if (wifi_manager_ap_set_passphrase(ap, gHomeAPPwd) != 0) {
		STDK_LOGE( "wifi_manager_ap_set_passphrase() failed");
		return false;
	}

	if (wifi_manager_set_connection_state_changed_cb(wifi_h, __wifi_connect_again_cb, NULL) != WIFI_MANAGER_ERROR_NONE) {
		STDK_LOGE("wifi_manager_set_connection_changed_cb error");
	}

	if (wifi_manager_connect(wifi_h, ap, ESWifiConnectedCb, user_data) != 0) {
		STDK_LOGE( "wifi_manager_connect() failed");
	}

	gWifiFound = true;

	return false;
}

static void ESWifiScanSpecificAPCb(wifi_manager_error_e error_code, void* user_data)
{
	STDK_LOG_ENTER;

	if (error_code != WIFI_MANAGER_ERROR_NONE) {
		STDK_LOGE( "Wi-Fi Activation failed [%d]", error_code);
		return;
	}

	wifi_manager_h wifi_h = (wifi_manager_h)user_data;

	if (wifi_manager_foreach_found_specific_ap(wifi_h, ESWifiFoundSpecificAPCb, user_data) != 0) {
		STDK_LOGE( "wifi_manager_foreach_found_specific_ap() failed");
		return;
	}

	if (gWifiFound == false && gWifiScanRetryCnt < ES_WIFI_SCAN_RETRY_CNT_MAX) {
		gWifiScanRetryCnt++;
		STDK_LOGD( "gWifiScanRetryCnt [%d]", gWifiScanRetryCnt);
		g_timeout_add(3000, _wifi_scan_specific, user_data);
	}
}

static gboolean _wifi_connect(gpointer user_data)
{
	STDK_LOG_ENTER;
	wifi_manager_h wifi_h = (wifi_manager_h)user_data;

	if (wifi_manager_scan_specific_ap(wifi_h, gHomeAPSsid, ESWifiScanSpecificAPCb, user_data) != 0) {
		STDK_LOGE( "wifi_manager_scan_specific_ap() failed");
	}

	return G_SOURCE_REMOVE;
}

static void ESWifiActivateCb(wifi_manager_error_e result, void* user_data)
{
	STDK_LOG_ENTER;

	if (result != WIFI_MANAGER_ERROR_NONE) {
		STDK_LOGE( "Wi-Fi Activation failed [%d]", result);
		return;
	}

	STDK_LOGI( "gHomeAPSsid [%s]", gHomeAPSsid);
	g_idle_add(_wifi_connect, user_data);
}

static void ESWifiDeactivatedCb(wifi_manager_error_e result, void *user_data)
{
	STDK_LOGI( "Wi-Fi Deactivation result [%d]", result);

	STDK_LOGI( "will start softap...");
	g_timeout_add(1000, (GSourceFunc)_wifi_deactivate_cb, user_data);

	STDK_LOG_END;
}

bool ESWifiIsActivated()
{
	wifi_manager_h wifi_h = NULL;
	bool state = false;

	if (wifi_manager_initialize(&wifi_h) != 0) {
		STDK_LOGE( "wifi_manager_initialize() failed");
		return false;
	}

	if (wifi_manager_is_activated(wifi_h, &state) != 0) {
		STDK_LOGE( "wifi_manager_is_activated() failed");
		return false;
	}

	if (wifi_manager_deinitialize(wifi_h) != 0) {
		STDK_LOGE( "wifi_manager_deinitialize() failed");
		return false;
	}

	STDK_LOGI( "Wifi Activate state = [%d]", state);
	return state;
}

bool ESConnectHomeAP()
{
	STDK_LOG_ENTER;
	if (gHomeAPSsid == NULL) {
		STDK_LOGE( "gHomeAPSsid is NULL");
		return false;
	}

	gWifiFound = false;
	gWifiScanRetryCnt = 0;

	int ret = 0;

	wifi_manager_h wifi_h = NULL;

	if ((ret = wifi_manager_initialize(&wifi_h)) != 0) {
		STDK_LOGE( "wifi_manager_initialize() failed [%d]", ret);
		return false;
	}

	if (wifi_manager_activate(wifi_h, ESWifiActivateCb, (void *)wifi_h) != 0) {
		STDK_LOGE( "wifi_manager_activate() failed");
		return false;
	}

	STDK_LOG_END;
	return true;
}

bool ESDisconnectHomeAP()
{
	STDK_LOG_ENTER;
	wifi_manager_h wifi_h = NULL;

	if (wifi_manager_initialize(&wifi_h) != 0) {
		STDK_LOGE( "wifi_manager_initialize() failed");
		return false;
	}

	STDK_LOGI( "wifi will be deactivated");
	if (wifi_manager_deactivate(wifi_h, ESWifiDeactivatedCb, (void *)wifi_h) != 0) {
		STDK_LOGE( "wifi_manager_deactivate() failed");
		goto _OUT;
	}

	return true;

_OUT:
	STDK_LOG_END;
	if (wifi_manager_deinitialize(wifi_h) != 0)
		STDK_LOGE( "wifi_manager_deinitialize() failed");

	return false;
}

int ESSetAPConnectionCb(char* ssid, char* pwd)
{
	STDK_LOG_ENTER;

	STDK_LOGI("[VERDICT] ESSetAPConnectionCb");

	if (ssid == NULL || pwd == NULL) {
		STDK_LOGE( "invalid parameter");
		return 0;
	}

	free(gHomeAPSsid);
	gHomeAPSsid = strdup(ssid);
	free(gHomeAPPwd);

	gHomeAPPwd = strdup(pwd);

	if (ESIsSoftAPRunning()) {
		STDK_LOGI( "SoftAP is running.. will be destroyed");
		ESDestroySoftAP();
	} else {
		ESConnectHomeAP();
	}

	return 1;
}
