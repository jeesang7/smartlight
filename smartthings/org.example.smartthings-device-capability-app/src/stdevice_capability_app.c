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
#include <tizen.h>
#include <service_app.h>
#include "stdevice_capability_app.h"
#include "stdevice_capability_user.h"
#define APP_KEY_CMD	"cmd"
#define APP_CMD_DELETE_URI "delete_uri"
#define APP_CMD_SEND_PUSH "send_push"
#define APP_CMD_PUBLISH "publish"
#define APP_KEY_URI "uri"

extern void cap_publish_events();

bool service_app_create(void *data)
{
	return true;
}

void service_app_terminate(void *data)
{
	if (deinit_capability_app() != 0) {
		STDK_LOGE("deinit_capability_app() is failed");
	}

	return;
}


void service_app_control(app_control_h app_control, void *data)
{
	STDK_LOG_ENTER;

	char *cmd = NULL;
	char *uri = NULL;

	if (init_capability_app(app_control) != 0) {
		STDK_LOGE("init_capability_app() is failed");
	}

	app_control_get_extra_data(app_control, APP_KEY_CMD, &cmd);
	if (!cmd) {
		return;
	}

	STDK_LOGD("cmd [%s]", cmd);
	if (strncmp(cmd, APP_CMD_DELETE_URI, strlen(APP_CMD_DELETE_URI)) == 0) {
		app_control_get_extra_data(app_control, APP_KEY_URI, &uri);
		if (uri != NULL) {
			STDK_LOGD("uri [%s]", uri);

//			delete_uri(uri);
		} else {
			STDK_LOGE("uri is NULL");
		}
	} else if (strncmp(cmd, APP_CMD_SEND_PUSH, strlen(APP_CMD_SEND_PUSH)) == 0) {
//		send_push_msg();
	} else if (strncmp(cmd, APP_CMD_PUBLISH, strlen(APP_CMD_PUBLISH)) == 0) {
		cap_publish_events();
	}

	free(cmd);
	free(uri);
}

static void
service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void
service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
	char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}
