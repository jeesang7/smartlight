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

#ifndef __STDK_MANAGER_MACRO_H__
#define __STDK_MANAGER_MACRO_H__

#include <stdlib.h>
#include <glib.h>
#include "stdevice_manager_log.h"

/* FREE */
#define FREE(x) \
	do {\
		free(x);	\
		x = NULL;	\
	} while(0)

#define G_FREE(x) \
	do {\
		g_free(x);	\
		x = NULL;	\
	} while(0)

#define RET_IF(expr) \
	do { \
		if (expr) { \
			STDK_LOGE("(%s)", #expr); \
			return; \
		} \
	} while (0)

#define RETV_IF(expr, val) \
	do { \
		if (expr) { \
			STDK_LOGE("(%s)", #expr); \
			return (val); \
		} \
	} while (0)

#define RETM_IF(expr, fmt, arg...) \
	do { \
		if (expr) { \
			STDK_LOGE(fmt, ##arg); \
			return; \
		} \
	} while (0)

#define RETVM_IF(expr, val, fmt, arg...) \
	do { \
		if (expr) { \
			STDK_LOGE(fmt, ##arg); \
			return (val); \
		} \
	} while (0)

#define GO_IF(expr, label) \
	do { \
		if (expr) { \
			STDK_LOGE("(%s)", #expr); \
			goto label; \
		} \
	} while (0)

#endif /* __STDK_MANAGER_MACRO_H__ */
