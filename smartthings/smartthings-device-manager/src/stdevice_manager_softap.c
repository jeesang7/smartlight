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
#include <softap.h>

#include "stdevice_manager_log.h"
#include "stdevice_manager_softap.h"
#include "stdevice_manager_homeap.h"

static softap_h gSoftAPHandle;
static char* gSoftAPSsid;
static char* gSoftAPPwd;

static void __soft_ap_client_connection_state_changed_cb(softap_client_h client, bool open, void *data)
{
	STDK_LOGI("Connection is [%s]", open ? "opened" : "closed");
	if (open == false) {
		STDK_LOGI("client is disconnected");
	}
}

static void SoftAPEnabledCb(softap_error_e error, bool is_requested, void *data)
{
	STDK_LOGI("[VERDICT] is_requested : [%d]", is_requested);
	if (is_requested && (error != SOFTAP_ERROR_NONE)) {
		STDK_LOGE("Soft AP enable fail!! error code[%d]", error);
	} else if (error == SOFTAP_ERROR_ALREADY_ENABLED) {
		STDK_LOGI("SOFTAP_ERROR_ALREADY_ENABLED");
		softap_set_client_connection_state_changed_cb (gSoftAPHandle, __soft_ap_client_connection_state_changed_cb, NULL);
	} else {
		softap_set_client_connection_state_changed_cb (gSoftAPHandle, __soft_ap_client_connection_state_changed_cb, NULL);
	}

	if (softap_unset_enabled_cb(gSoftAPHandle) != 0)
		STDK_LOGE("softap_unset_enabled_cb() failed");
}

static gboolean _softap_destroy_idler(gpointer data)
{
	STDK_LOG_ENTER;

	STDK_LOGI("[VERDICT] _softap_destroy_idler");

	if (softap_destroy(gSoftAPHandle) != 0)
		STDK_LOGE("softap_destroy() failed");

	gSoftAPHandle = NULL;

	free(gSoftAPSsid);
	gSoftAPSsid = NULL;
	free(gSoftAPPwd);
	gSoftAPPwd = NULL;

	ESConnectHomeAP();

	return G_SOURCE_REMOVE;
}

static void SoftAPDisabledCb(softap_error_e error, softap_disabled_cause_e cause, void *data)
{
	STDK_LOGI("cause : [%d]", cause);
	if ((cause == SOFTAP_DISABLED_BY_REQUEST) && (error != SOFTAP_ERROR_NONE)) {
		STDK_LOGE("[VERDICT] Soft AP disable fail!! error code[0x%X]", error);
		return;
	}

	if (softap_unset_disabled_cb(gSoftAPHandle) != 0)
		STDK_LOGE("softap_unset_disabled_cb() failed");

	g_idle_add(_softap_destroy_idler, NULL);
}

static bool ESEnableSoftAP()
{
	STDK_LOG_ENTER;

	if (softap_set_ssid(gSoftAPHandle, gSoftAPSsid) != 0) {
		STDK_LOGE("softap_set_ssid() failed");
		return false;
	}
	if (softap_set_passphrase(gSoftAPHandle, gSoftAPPwd) != 0) {
		STDK_LOGE("softap_set_passphrase() failed");
		return false;
	}

	if (softap_set_channel(gSoftAPHandle, 5) != 0) {
		STDK_LOGE("softap_set_channel() failed");
		return false;
	}

	if (softap_set_enabled_cb(gSoftAPHandle, SoftAPEnabledCb, NULL) != 0) {
		STDK_LOGE("softap_set_enabled_cb() failed");
		return false;
	}

	if (softap_enable(gSoftAPHandle) != 0) {
		STDK_LOGE("softap_enable() failed");
		return false;
	}
	return true;
}

gboolean _ESEnableSoftApIdler(gpointer data)
{
	ESEnableSoftAP();

	return G_SOURCE_REMOVE;
}

static void _ESEnableSoftAP(softap_error_e error, softap_disabled_cause_e cause, void *data)
{
	STDK_LOGI("cause : [%d], error [0x%X]", cause, error);

	if (softap_unset_disabled_cb(gSoftAPHandle) != 0)
		STDK_LOGE("softap_unset_disabled_cb() failed");

	g_timeout_add(1000, (GSourceFunc)_ESEnableSoftApIdler, NULL);
}

void ESStartSoftAP(void)
{
	STDK_LOG_ENTER;

	bool enabled = false;
	char *temp_ssid = NULL;

	if (gSoftAPHandle == NULL) {
		if (softap_create(&gSoftAPHandle) != 0) {
			STDK_LOGE("softap_create() failed");
			goto _CREATE_SOFTAP_OUT;
		}
	}

	softap_is_enabled(gSoftAPHandle, &enabled);
	if (!enabled) {
		if(ESEnableSoftAP() != true)
			STDK_LOGE("ESEnableSoftAP() failed");

		goto _CREATE_SOFTAP_OUT;
	}

	if (softap_get_ssid(gSoftAPHandle, &temp_ssid) != 0) {
		STDK_LOGE("softap_get_ssid() failed");
		goto _CREATE_SOFTAP_OUT;
	}

	if(strncmp(temp_ssid, gSoftAPSsid, strlen(gSoftAPSsid)) == 0) {
		STDK_LOGI("[VERDICT] already enabled.. skip it");
		softap_set_client_connection_state_changed_cb (gSoftAPHandle, __soft_ap_client_connection_state_changed_cb, NULL);
		goto _CREATE_SOFTAP_OUT;
	}

	STDK_LOGI("other ssid is enabled.. firstly disable and then enable it");
	if (softap_set_disabled_cb(gSoftAPHandle, _ESEnableSoftAP, NULL) != 0) {
		STDK_LOGE("softap_set_disabled_cb() failed");
		goto _CREATE_SOFTAP_OUT;
	}

	if (softap_disable(gSoftAPHandle) != 0) {
		STDK_LOGE("softap_disable() failed");
		goto _CREATE_SOFTAP_OUT;
	}

_CREATE_SOFTAP_OUT:
	if (temp_ssid) {
		free(temp_ssid);
		temp_ssid = NULL;
	}
}

bool ESCreateSoftAP(const char *ssid, const char *pwd)
{
	STDK_LOG_ENTER;
	STDK_LOGI("ssid : [%s]", ssid);

	free(gSoftAPSsid);
	gSoftAPSsid = strdup(ssid);
	free(gSoftAPPwd);
	gSoftAPPwd = strdup(pwd);

	if (ESWifiIsActivated() == false) {
		ESStartSoftAP();
		return true;
	}

	if (ESDisconnectHomeAP() != true) {
		STDK_LOGE("ESDisconnectHomeAP is failed");
		return false;
	}

	return true;
}

bool ESDestroySoftAP()
{
	STDK_LOG_ENTER;

	if (gSoftAPHandle == NULL) {
		STDK_LOGE("gSoftAPHandle will be created");
		if (softap_create(&gSoftAPHandle) != 0) {
			STDK_LOGE("softap_create() failed");
			return false;
		}
	}

	if (softap_set_disabled_cb(gSoftAPHandle, SoftAPDisabledCb, NULL) != 0) {
		STDK_LOGE("softap_set_disabled_cb() failed");
		return false;
	}

	if (softap_disable(gSoftAPHandle) != 0) {
		STDK_LOGE("softap_disable() failed");
		return false;
	}

	return true;
}

bool ESIsSoftAPRunning()
{
	STDK_LOG_ENTER;

	bool enabled = false;
	if (!gSoftAPHandle) {
		STDK_LOGI("gSoftAPHandle will be created");
		if (softap_create(&gSoftAPHandle) != 0) {
			STDK_LOGE("softap_create() failed");
			return false;
		}
	}

	if (softap_is_enabled(gSoftAPHandle, &enabled) != 0) {
		STDK_LOGE("softap_is_enabled() failed");
		return false;
	}

	STDK_LOGI("enabled :[%d]", enabled);

	return enabled;
}

