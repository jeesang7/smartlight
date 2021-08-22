/*
   Copyright 2021 Samsung Electronics Co.

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

#include <smartthings_device_capability.h>
#include "stdevice_capability_app.h"
#include "stdevice_capability_user.h"
#include <service_app.h>

#include "smartthings_device_manager.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <glib.h>


#define THING_RESOURCE_FILE_NAME	"resource.json"

static smartthings_device_cap_service_h st_handle;
static smartthings_device_h st_h;
static bool g_initialized = false;

static const char* const component_main = "main";
static const char* const cap_switch = "switch";


typedef enum  {
	SMARTLAMP_SWITCH_OFF = 0,
	SMARTLAMP_SWITCH_ON = 1,
}smartlamp_onoff_state_e;

static smartlamp_onoff_state_e smartlamp_switch_state = SMARTLAMP_SWITCH_ON;


/*==============================================================
# **** Example functions for handing capabilities ****
================================================================ */
static void _color_led_onoff(smartlamp_onoff_state_e state)
{
	if (state == SMARTLAMP_SWITCH_OFF) {
		//gpio_set_level(GPIO_OUTPUT_COLORLED_R, 0);
		//gpio_set_level(GPIO_OUTPUT_COLORLED_G, 0);
		//gpio_set_level(GPIO_OUTPUT_COLORLED_B, 0);
	} else {
		//gpio_set_level(GPIO_OUTPUT_COLORLED_R, (smartlamp_color_red > 127));
		//gpio_set_level(GPIO_OUTPUT_COLORLED_G, (smartlamp_color_green > 127));
		//gpio_set_level(GPIO_OUTPUT_COLORLED_B, (smartlamp_color_blue > 127));
	}
}

static void _change_switch_state(smartlamp_onoff_state_e state)
{
	/* change state */
	smartlamp_switch_state = state;
	_color_led_onoff(smartlamp_switch_state);
	if(state == SMARTLAMP_SWITCH_ON) {
		//gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, NOTIFICATION_LED_GPIO_ON);
	} else {
		//gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, NOTIFICATION_LED_GPIO_OFF);
	}
}

static void _publish_result_cb(smartthings_device_cap_error_e result, const char *err_msg, int sequence_no, void *user_data)
{
	STDK_LOGD("result[%d], seq_no[%d]", result, sequence_no);

}

static void _publish_string_event(const char *component, const char *capability, const char* attribute, const char *value, const char *command_id)
{
	smartthings_device_cap_event_h evt_h = NULL;

	smartthings_device_cap_event_create_string(component, capability, attribute, value, NULL, command_id, &evt_h);
	smartthings_device_cap_publish_events(st_handle, &evt_h, 1, _publish_result_cb, NULL);
	smartthings_device_cap_event_destroy(evt_h);
}

static void _switch_cmd_cb(const char *command, smartthings_device_cap_arguments_h arguments, const char *command_id, void *user_data)
{
	STDK_LOG_ENTER;
	RET_IF(NULL == command);

	STDK_LOGD("command [%s], arguments[%p], user_data[%p]", command, arguments, user_data);

	if (strncmp(command, "on", strlen(command)) == 0) {
		_change_switch_state(SMARTLAMP_SWITCH_ON);
		_publish_string_event(component_main, cap_switch, "switch", "on", command_id);
	} else if (strncmp(command, "off", strlen(command)) == 0){
		_change_switch_state(SMARTLAMP_SWITCH_OFF);
		_publish_string_event(component_main, cap_switch, "switch", "off", command_id);
	} else {
		STDK_LOGD("Unexpected command [%s]", command);
	}

	STDK_LOG_END;
}

/*==============================================================
# **** BELOW ARE THE MANDATORY FUNCTIONS ****
================================================================ */

/*
This callback is invoked when the device is connected to SmartThings server
The device should publish current statuses of capabilities in it
*/

void cap_publish_events()
{
	STDK_LOG_ENTER;
	smartthings_device_cap_event_h evt_h[1];
	int evt_num = 1;

	if( smartlamp_switch_state == SMARTLAMP_SWITCH_ON) {
		smartlamp_switch_state = SMARTLAMP_SWITCH_OFF;
		smartthings_device_cap_event_create_string(component_main, cap_switch, "switch", "off", NULL, NULL, &evt_h[0]);
	} else {
		smartlamp_switch_state = SMARTLAMP_SWITCH_ON;
		smartthings_device_cap_event_create_string(component_main, cap_switch, "switch", "on", NULL, NULL, &evt_h[0]);
	}

	smartthings_device_cap_publish_events(st_handle, evt_h, evt_num, _publish_result_cb, NULL);

	for (int i = 0; i < evt_num; i++)
		smartthings_device_cap_event_destroy(evt_h[i]);
}


static void _init_cb(void *user_data )
{
	STDK_LOG_ENTER;

	smartthings_device_cap_event_h evt_h[1];
	int evt_num = 1;

	if( smartlamp_switch_state == SMARTLAMP_SWITCH_ON)
		smartthings_device_cap_event_create_string(component_main, cap_switch, "switch", "on", NULL, NULL, &evt_h[0]);
	else
		smartthings_device_cap_event_create_string(component_main, cap_switch, "switch", "off", NULL, NULL, &evt_h[0]);

	smartthings_device_cap_publish_events(st_handle, evt_h, evt_num, _publish_result_cb, NULL);

	for (int i = 0; i < evt_num; i++)
		smartthings_device_cap_event_destroy(evt_h[i]);
}

/*
This callback is invoked when the device receives a command of the capability from SmartThings server
The device should perform the command and publish the result
*/
static void _cmd_cb(const char *component, const char *capability, const char *command, smartthings_device_cap_arguments_h arguments, const char *command_id, void *user_data)
{
	if (strcmp(component, component_main) == 0) {
		if (strcmp(capability, cap_switch) == 0) {
			_switch_cmd_cb(command, arguments, command_id, user_data);
		} else {
			STDK_LOGD("not supported capability [%s]", capability);
		}
	} else {
		STDK_LOGD("not supported component [%s]", component);
	}
}

/*
This is for replying to the launch request of SmartThins device agent
*/
static void _app_control_launch_done_to_agent(void* user_data)
{
	app_control_h app_control = (app_control_h) user_data;
	app_control_h reply = NULL;
	app_control_create(&reply);
	int ret = app_control_reply_to_launch_request(reply, app_control, APP_CONTROL_RESULT_SUCCEEDED);
	if (ret != APP_CONTROL_ERROR_NONE) {
		STDK_LOGE("app_control_reply_to_launch_request error [%d]", ret);
	}
	app_control_destroy(reply);
	app_control_destroy(app_control);
}

static void _capability_connection_status_cb(smartthings_device_cap_error_e result, smartthings_device_cap_service_h handle,
							smartthings_device_cap_connection_status_e status, void *user_data)
{
	STDK_LOGI("smartthings resource connection result=[%d], status=[%d]", result, status);

	if (status == SMARTTHINGS_DEVICE_CAP_CONNECTION_STATUS_REJECTED) {
		STDK_LOGE("REJECTED");
		service_app_exit();
		return;
	} else if (status == SMARTTHINGS_DEVICE_CAP_CONNECTION_STATUS_DISCONNECTED) {
		if (result == SMARTTHINGS_DEVICE_CAP_ERROR_SERVICE_UNAVAILABLE) {
			STDK_LOGI("disconnected by updating");
			return;
		}
		STDK_LOGW("DISCONNECTED");
		service_app_exit();
		return;
	}

	/*
	The device needs to set a init callback and a command callback to handle the capabilities
	*/
	int ret = smartthings_device_cap_set_cbs(st_handle, _init_cb, _cmd_cb, NULL);
	if ( ret != SMARTTHINGS_DEVICE_CAP_ERROR_NONE) {
		STDK_LOGE("smartthings_device_cap_set_cbs() failed");
		return;
	}

	if (user_data)
		_app_control_launch_done_to_agent(user_data);
}

static void _stdevice_noti_cb(smartthings_device_h handle, int type, const char *json_data, void *user_data)
{
	STDK_LOG_ENTER;

	JsonParser *json_parser = NULL;
	JsonNode *root = NULL;
	JsonObject *root_object = NULL;

	if (json_data) {
		/** Create a new JSON parser */
		json_parser = json_parser_new();
		json_parser_load_from_data(json_parser, json_data, strlen(json_data), NULL);

		root = json_parser_get_root(json_parser);
		if (NULL == root) {
			/** Free the JSON parser if root node NULL */
			STDK_LOGE("root is NULL");
			g_object_unref(json_parser);
			json_parser = NULL;
		}
		root_object = json_node_get_object(root);
	}

	switch(type) {
	case SMARTTHINGS_DEVICE_NOTI_TYPE_DEV_DELETED:
		STDK_LOGI("got device deleted noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		break;

	case SMARTTHINGS_DEVICE_NOTI_TYPE_RATE_LIMIT:
		STDK_LOGI("got rate limit noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		if (root_object && json_object_has_member(root_object, "count")) {
			int count = (int)json_object_get_int_member(root_object, "count");
			STDK_LOGD("count [%d]", count);
		}
		if (root_object && json_object_has_member(root_object, "threshold")) {
			int threshold = (int)json_object_get_int_member(root_object, "threshold");
			STDK_LOGD("threshold [%d]", threshold);
		}
		if (root_object && json_object_has_member(root_object, "remainingTime")) {
			int remaining_time = (int)json_object_get_int_member(root_object, "remainingTime");
			STDK_LOGD("remainingTime [%d]", remaining_time);
		}
		if (root_object && json_object_has_member(root_object, "sequenceNumber")) {
			int seq_num = (int)json_object_get_int_member(root_object, "sequenceNumber");
			STDK_LOGD("sequenceNumber [%d]", seq_num);
		}

		break;

	case SMARTTHINGS_DEVICE_NOTI_TYPE_QUOTA_REACHED:
		STDK_LOGI("got quota reached noti: [%d]", type);
		STDK_LOGI("json_data:%s", json_data);
		if (root_object && json_object_has_member(root_object, "used")) {
			int used = (int)json_object_get_int_member(root_object, "used");
			STDK_LOGD("used [%d]", used);
		}
		if (root_object && json_object_has_member(root_object, "limit")) {
			int limit = (int)json_object_get_int_member(root_object, "limit");
			STDK_LOGD("limit [%d]", limit);
		}

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

	if (json_parser)
		g_object_unref(json_parser);
	STDK_LOG_END;
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
		break;

	case SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD", errmsg);
		break;

	case SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD_FAILED:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD_FAILED", errmsg);
		break;

	case SMARTTHINGS_DEVICE_STATUS_CONNECTED_TO_CLOUD:
		STDK_LOGI("status: [%d] [%s] errmsg[%s]", status, "SMARTTHINGS_DEVICE_STATUS_CONNECTED_TO_CLOUD", errmsg);
		break;

	default:
		STDK_LOGE("status: [%d][%s]", status, "Unknown Status");
		break;
	}
}


static gboolean _reconnect(gpointer user_data);

static void _connection_status_cb(smartthings_device_error_e result, smartthings_device_h handle, smartthings_device_connection_status_e status, void *user_data)
{
	STDK_LOGI("smartthings connection result=[%d], status=[%d]", result, status);

	int err = SMARTTHINGS_DEVICE_ERROR_NONE;

	if (status == SMARTTHINGS_DEVICE_CONNECTION_STATUS_CONNECTED) {
		STDK_LOGI("smartthings connected!!");
		err = smartthings_device_set_notification_cb(st_h, _stdevice_noti_cb, NULL);
		if (err != SMARTTHINGS_DEVICE_ERROR_NONE) {
			STDK_LOGE("smartthings_device_set_notification_cb() is failed [%d]", err);
			return;
		}

		err = smartthings_device_set_status_changed_cb(st_h, _stdevice_status_cb, NULL);
		if (err != SMARTTHINGS_DEVICE_ERROR_NONE) {
			STDK_LOGE("smartthings_device_set_notification_cb() is failed [%d]", err);
			return;
		}

	} else if (status == SMARTTHINGS_DEVICE_CONNECTION_STATUS_DISCONNECTED) {
		if (result == SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE) {
			// You can find whether it is restarted or not with _stdevice_status_cb()
			STDK_LOGI("It may result from agent updating, so wait until agent is restarted");
			return;
		}

		STDK_LOGI("smartthings disconnected!! will be re-connect!!!");
		/* will be reconnect after some interval*/
		g_timeout_add(10000, _reconnect, NULL);
	}
}


static gboolean _reconnect(gpointer user_data)
{
	STDK_LOG_ENTER;

	smartthings_device_disconnect(st_h);

	int err = smartthings_device_connect(&st_h, _connection_status_cb, NULL);

	if (SMARTTHINGS_DEVICE_ERROR_NONE != err)
		STDK_LOGE("smartthings_device_connect() failed [%d]", err);

	return false;
}



int init_capability_app(app_control_h app_control)
{
	STDK_LOG_ENTER;

	app_control_h cloned = NULL;

	if (g_initialized) {
		STDK_LOGI("Already initialized!");
		return 0;
	} else {
		char* callerId = NULL;

		app_control_get_caller(app_control, &callerId);
		if (callerId && 0 == strcmp("com.samsung.tizen.smartthings-device", callerId)) {
			app_control_clone(&cloned, app_control);
		}
		free(callerId);
	}

	if (smartthings_device_cap_connect(&st_handle, _capability_connection_status_cb, cloned) != SMARTTHINGS_DEVICE_CAP_ERROR_NONE) {
		STDK_LOGE("smartthings_device_cap_connect() failed");
		goto _out;
	}

	if (smartthings_device_connect(&st_h, _connection_status_cb, cloned) != SMARTTHINGS_DEVICE_ERROR_NONE) {
		STDK_LOGE("smartthings_device_cap_connect() is failed");
		goto _out;
	}

	g_initialized = true;

	return 0;

_out :

	return -1;
}

int deinit_capability_app()
{
	STDK_LOG_ENTER;

	if (!st_handle)
		return 0;

	if (smartthings_device_cap_disconnect(st_handle) != 0)
		return -1;

	g_initialized = false;

	st_handle = NULL;

	return 0;
}

