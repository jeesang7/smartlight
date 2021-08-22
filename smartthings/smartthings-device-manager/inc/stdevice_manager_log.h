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

#ifndef __STDK_MANAGER_LOG_H__
#define __STDK_MANAGER_LOG_H__

#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif

#define LOG_TAG "STDK-MANAGER-APP"

#define STDK_LOGE(fmt, args...) dlog_print(DLOG_ERROR, LOG_TAG, "%s: %s(%d) "fmt, rindex(__FILE__, '/') + 1, __func__, __LINE__, ##args)
#define STDK_LOGW(fmt, args...) dlog_print(DLOG_WARN, LOG_TAG, "%s: %s(%d) "fmt, rindex(__FILE__, '/') + 1, __func__, __LINE__, ##args)
#define STDK_LOGI(fmt, args...) dlog_print(DLOG_INFO, LOG_TAG, "%s: %s(%d) "fmt, rindex(__FILE__, '/') + 1, __func__, __LINE__, ##args)
#define STDK_LOGD(fmt, args...) dlog_print(DLOG_INFO, LOG_TAG, "%s: %s(%d) "fmt, rindex(__FILE__, '/') + 1, __func__, __LINE__, ##args)

#define STDK_LOG_ENTER dlog_print(DLOG_INFO, LOG_TAG, "%s: %s(%d) >>>> called", rindex(__FILE__, '/') + 1, __func__, __LINE__)
#define STDK_LOG_END dlog_print(DLOG_INFO, LOG_TAG, "%s: %s(%d) <<<< ended", rindex(__FILE__, '/') + 1, __func__, __LINE__)

#endif /* __STDK_MANAGER_LOG_H__ */
