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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>

#include "stdevice_manager_log.h"
#include "stdevice_mananger_macro.h"
#include "stdevice_manager_user.h"
#include "stdevice_manager_softap.h"
#include "stdevice_manager_homeap.h"
#include "stdevice_manager_config.h"
#include "smartthings_device_manager.h"

static smartthings_device_h st_h;
static bool is_init;
static bool gConfirmFlag = true;
static guint gConfirmEventId;

#define SSID_PWD "1111122222"

static void _connection_status_cb(smartthings_device_error_e result, smartthings_device_h handle, smartthings_device_connection_status_e status, void *user_data);

static void _st_device_disconnect(void)
{
	smartthings_device_disconnect(st_h);
	st_h = NULL;
	is_init = false;
}

gboolean _send_user_confirm(gpointer user_data)
{
	STDK_LOGI("[VERDICT] Received User Confirm after timeout 60 secs");

	STDK_LOGI("gConfirmFlag %d", gConfirmFlag);

	if (smartthings_device_send_otm_result(st_h, gConfirmFlag) != 0) {
		STDK_LOGE("smartthings_device_send_user_confirm() is failed");
	}

	gConfirmEventId = 0;

	return G_SOURCE_REMOVE;
}

static void _button_cb(smartthings_device_h handle, void *user_data)
{
	STDK_LOGD("\n\n\n");
	STDK_LOGD("**********************");
	STDK_LOGD("[VERDICT] Will check user confirm after 10 secs !!!!!");
	STDK_LOGD("**********************");

	/* timeout test */
	gConfirmEventId = g_timeout_add_seconds(10, _send_user_confirm, NULL);
}

static int g_pin_num;

static void _show_pin_cb(smartthings_device_h handle, void *user_data)
{
	STDK_LOG_ENTER;

	g_pin_num = 12345678; /* application should select a pin and display it */

	STDK_LOGD("\n\n\n");
	STDK_LOGD("**********************");
	STDK_LOGD("Pin: [%d] ", g_pin_num);
	STDK_LOGD("**********************");
}

static void _pin_cb(smartthings_device_h handle, int pin_num, void *user_data)
{
	STDK_LOG_ENTER;
	gConfirmFlag = (g_pin_num == pin_num)? true : false;

	/* timeout test */
	gConfirmEventId = g_timeout_add_seconds(10, _send_user_confirm, NULL);
}

static void _stdevice_status_cb(smartthings_device_h handle, smartthings_device_status_e status, const char* errmsg, void *user_data)
{
	STDK_LOGI("smartthings status is called!!");

	switch (status) {
	case SMARTTHINGS_DEVICE_STATUS_INIT:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_INIT", errmsg);

		break;
	case SMARTTHINGS_DEVICE_STATUS_ES_STARTED:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_ES_STARTED", errmsg);
		break;
	case SMARTTHINGS_DEVICE_STATUS_ES_DONE:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_ES_DONE", errmsg);
		break;

	case SMARTTHINGS_DEVICE_STATUS_REGISTERING_TO_CLOUD:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_REGISTERING_TO_CLOUD", errmsg);
		break;

	case SMARTTHINGS_DEVICE_STATUS_REGISTERING_TO_CLOUD_FAILED:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_REGISTERING_TO_CLOUD_FAILED", errmsg);
		_st_device_disconnect();
		break;

	case SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD", errmsg);
		break;

	case SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD_FAILED:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD_FAILED", errmsg);
		_st_device_disconnect();
		break;

	case SMARTTHINGS_DEVICE_STATUS_CONNECTED_TO_CLOUD:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_CONNECTED_TO_CLOUD", errmsg);
		break;

	default:
		STDK_LOGE("status: [%d][%s]", status, "Unknown Status");
		break;
	}
}

static void _stdevice_noti_cb(smartthings_device_h handle, int type, const char *json_data, void *user_data)
{
	STDK_LOG_ENTER;

	switch(type) {
	case SMARTTHINGS_DEVICE_NOTI_TYPE_DEV_DELETED:
		STDK_LOGI("got device deleted noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		ESCreateSoftAP(std_mgr_config_get_softap_ssid(), SSID_PWD);
		if (smartthings_device_start(st_h, std_mgr_config_get_device_info(), std_mgr_config_get_onboarding_config()) != SMARTTHINGS_DEVICE_ERROR_NONE) {
			STDK_LOGE("smartthings_device_start() is failed");
		}
		break;

	case SMARTTHINGS_DEVICE_NOTI_TYPE_RATE_LIMIT:
		STDK_LOGI("got rate limit noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		break;

	case SMARTTHINGS_DEVICE_NOTI_TYPE_QUOTA_REACHED:
		STDK_LOGI("got quota reached noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		break;

	case SMARTTHINGS_DEVICE_NOTI_TYPE_ERROR:
		STDK_LOGI("got error noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		break;

	default:
		STDK_LOGE("undefined noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		break;
	}
}

static void _stdevice_wifi_ap_provisioning_cb(smartthings_device_h handle, smartthings_device_ap_info_h ap_info_h, void *user_data)
{
	STDK_LOG_ENTER;

	char *ssid = NULL;
	char *pwd = NULL;
	char *bssid = NULL;

	smartthings_device_apinfo_get_ssid(ap_info_h, &ssid);
	smartthings_device_apinfo_get_password(ap_info_h, &pwd);
	smartthings_device_apinfo_get_bssid(ap_info_h, &bssid);

	STDK_LOGI("ssid : [%s]", ssid);
	STDK_LOGI("pwd : [%s]", pwd);
	STDK_LOGI("bssid : [%s]", bssid);

	ESSetAPConnectionCb(ssid, pwd);

	free(ssid);
	free(pwd);
	free(bssid);
}


gboolean _reconnect(gpointer user_data)
{
	STDK_LOG_ENTER;

	smartthings_device_disconnect(st_h);

	if (SMARTTHINGS_DEVICE_ERROR_NONE != smartthings_device_connect(&st_h, _connection_status_cb, NULL))
		STDK_LOGE("smartthings_device_connect() failed");

	return false;
}


static void _connection_status_cb(smartthings_device_error_e result, smartthings_device_h handle, smartthings_device_connection_status_e status, void *user_data)
{
	STDK_LOGI("smartthings connection result=[%d], status=[%d]", result, status);

	bool is_completed = false;
	init_mode_e mode = (init_mode_e)user_data;

	if (status == SMARTTHINGS_DEVICE_CONNECTION_STATUS_CONNECTED) {
		STDK_LOGI("smartthings connected!!");

		if (smartthings_device_set_status_changed_cb(st_h, _stdevice_status_cb, NULL) != SMARTTHINGS_DEVICE_ERROR_NONE) {
			STDK_LOGE("smartthings_device_set_status_changed_callback() is failed");
			return;
		}

		if (smartthings_device_set_notification_cb(st_h, _stdevice_noti_cb, NULL) != SMARTTHINGS_DEVICE_ERROR_NONE) {
			STDK_LOGE("smartthings_device_set_notification_cb() is failed");
			return;
		}

		smartthings_device_get_easysetup_status(st_h, &is_completed);

		if (mode == INIT_MODE_SELF_SETUP || (is_completed && mode != INIT_MODE_FORCE)) {
			// turn on WiFi
		} else {
			if (mode == INIT_MODE_FORCE)
				smartthings_device_reset(st_h);

			ESCreateSoftAP(std_mgr_config_get_softap_ssid(), SSID_PWD);

			if (smartthings_device_set_otm_for_pin_cb(st_h, _show_pin_cb, _pin_cb, NULL) != SMARTTHINGS_DEVICE_ERROR_NONE) {
				STDK_LOGE("smartthings_device_set_pin_cb() is failed");
				return;
			}

			if (smartthings_device_set_otm_for_button_cb(st_h, _button_cb, NULL) != SMARTTHINGS_DEVICE_ERROR_NONE) {
				STDK_LOGE("smartthings_device_set_user_confirm_cb() is failed");
				return;
			}

			if (smartthings_device_set_wifi_ap_provisioning_cb(st_h, _stdevice_wifi_ap_provisioning_cb, NULL) != SMARTTHINGS_DEVICE_ERROR_NONE) {
				STDK_LOGE("smartthings_device_set_wifi_ap_provisioning_cb() is failed");
				return;
			}

			if (smartthings_device_set_wifi_frequency(st_h, SMARTTHINGS_DEVICE_WIFI_FREQ_2_4G_5G_BOTH) != SMARTTHINGS_DEVICE_ERROR_NONE) {
				STDK_LOGE("smartthings_device_set_wifi_frequency() is failed");
				return;
			}
		}

		/* smartthings_device_start */
		switch (mode)
		{
		case INIT_MODE_SELF_SETUP:
			STDK_LOGE("Not yet supported");
			break;

		default:
			if (smartthings_device_start(st_h, std_mgr_config_get_device_info(), std_mgr_config_get_onboarding_config()) != SMARTTHINGS_DEVICE_ERROR_NONE) {
				STDK_LOGE("smartthings_device_start() is failed");
			}
			break;
		}
	} else if (status == SMARTTHINGS_DEVICE_CONNECTION_STATUS_DISCONNECTED) {
		if (result == SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE) {
			// You can find whether it is restarted or not with _smartthings_device_status_cb()
			STDK_LOGI("It may result from agent updating, so wait until agent is restarted");
			return;
		}

		STDK_LOGI("smartthings disconnected!! will be re-connect!!!");
		/* will be reconnect after some interval*/
		g_timeout_add(10000, _reconnect, NULL);
	} else if (status == SMARTTHINGS_DEVICE_CONNECTION_STATUS_REJECTED) {
		_st_device_disconnect();
	}
}

int init_manager_app(init_mode_e init_mode)
{
	STDK_LOG_ENTER;

	if (is_init) {
		STDK_LOGI("Already initialized!");
		return 0;
	}

	if (smartthings_device_connect(&st_h, _connection_status_cb, (void*)init_mode) != SMARTTHINGS_DEVICE_ERROR_NONE) {
		STDK_LOGE("smartthings_device_connect() is failed");
		goto _out;
	}

	is_init = true;

	return 0;

_out :
	return -1;
}

int deinit_manager_app(void)
{
	STDK_LOG_ENTER;

	is_init = false;

	if (gConfirmEventId > 0) {
		STDK_LOGI("g_source_remove()");
		g_source_remove(gConfirmEventId);
		gConfirmEventId = 0;
	}

	if (ESIsSoftAPRunning()) {
		STDK_LOGI( "SoftAP is running.. will be destroyed");
		ESDestroySoftAP();
	}

	if (!st_h) {
		STDK_LOGI("handle is already NULL");
		return 0;
	}

	if (smartthings_device_disconnect(st_h) != SMARTTHINGS_DEVICE_ERROR_NONE)  {
		STDK_LOGE("smartthings_device_disconnect() is failed");
		return -1;
	}

	st_h = NULL;

	return 0;
}

int set_confirm_flag_manager_app(bool confirm)
{
	STDK_LOG_ENTER;

	gConfirmFlag = confirm;
	STDK_LOGI("gConfirmFlag [%d]", gConfirmFlag);
	return 0;
}
