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

#include <tizen.h>
#include <service_app.h>
#include <stdio.h>
#include <string.h>

#include "stdevice_manager_log.h"
#include "stdevice_manager_user.h"
#include "stdevice_manager_config.h"
#include "smartthings_device_manager.h"

#define APP_KEY_CMD	"cmd"
#define APP_CMD_START_BY_FORCE	"start_by_force"

void service_app_control(app_control_h app_control, void *data)
{
	STDK_LOG_ENTER;

	char *cmd = NULL;
	int init_mode = INIT_MODE_NORMAL;

	int ret = app_control_get_extra_data(app_control, APP_KEY_CMD, &cmd);
	if (ret != APP_CONTROL_ERROR_NONE) {
		STDK_LOGE("app_control_get_extra_data returns error = %d", ret);
	}

	STDK_LOGD("cmd [%s]", cmd);
	if (cmd != NULL && strncmp(cmd, APP_CMD_START_BY_FORCE, strlen(APP_CMD_START_BY_FORCE)) == 0) {
		STDK_LOGD("start by force is enabled");
		init_mode = INIT_MODE_FORCE;
	}

	init_manager_app(init_mode);

	free(cmd);
	return;
}

bool service_app_create(void *data)
{
	char *res_path = NULL;
	bool ret = true;

	res_path = app_get_resource_path();
	if (!res_path) {
		STDK_LOGE("app_get_resource_path() is failed");
		return false;
	}

	if (std_mgr_config_init(res_path)) {
		STDK_LOGE("std_mgr_config_init() is failed");
		ret = false;
	}
	free(res_path);
	return ret;
}

void service_app_terminate(void *data)
{
	STDK_LOG_ENTER;

	if (deinit_manager_app(true) != 0) {
		STDK_LOGE("deinit_manager_app() is failed");
	}

	std_mgr_config_fini();

	return;
}

int main(int argc, char* argv[])
{
	STDK_LOG_ENTER;
	service_app_lifecycle_callback_s event_callback;

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	return service_app_main(argc, argv, &event_callback, NULL);
}

