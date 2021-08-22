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

#ifndef STDK_MANAGER_CONFIG_H_
#define STDK_MANAGER_CONFIG_H_

int std_mgr_config_init(const char *res_path);
int std_mgr_config_fini(void);
const char *std_mgr_config_get_device_info(void);
const char *std_mgr_config_get_onboarding_config(void);
const char* std_mgr_config_get_softap_ssid(void);

#endif /* STDK_MANAGER_CONFIG_H_ */
