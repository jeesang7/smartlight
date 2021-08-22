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

#ifndef STDK_MANAGER_USER_H_
#define STDK_MANAGER_USER_H_

#include <stdbool.h>
typedef enum {
	INIT_MODE_NORMAL,
	INIT_MODE_FORCE,
	INIT_MODE_SELF_SETUP
} init_mode_e;

int init_manager_app(init_mode_e init_mode);
int deinit_manager_app();

int set_confirm_flag_manager_app(bool confirm);
#endif /* STDK_MANAGER_USER_H_ */
