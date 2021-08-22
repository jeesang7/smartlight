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

#include <glib.h>
#include <stdio.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

#include "stdevice_manager_log.h"
#include "stdevice_mananger_macro.h"

#define SHA256_LEN 32
#define HASH_SIZE (4)

static char *g_device_info = NULL;
static char *g_onboarding_config = NULL;
static char *g_softap_ssid = NULL;

static char *read_file(const char *filename)
{
	FILE *file = NULL;
	long length = 0;
	char *content = NULL;
	size_t read_chars = 0;

	RETV_IF(!filename, NULL);
	if (!filename)
		return NULL;

	/* open in read binary mode */
	file = fopen(filename, "rb");
	RETV_IF(!file, NULL);

	/* get the length */
	if (fseek(file, 0, SEEK_END) != 0) {
		STDK_LOGE("failed to fseek : %s", filename);
		goto cleanup;
	}

	length = ftell(file);
	if (length < 0) {
		STDK_LOGE("failed to ftell() : %s", filename);
		goto cleanup;
	}

	if (fseek(file, 0, SEEK_SET) != 0) {
		STDK_LOGE("failed to fseek() : %s", filename);
		goto cleanup;
	}
	/* allocate content buffer */
	content = (char*)malloc((size_t)length + sizeof(""));
	if (!content) {
		STDK_LOGE("failed to malloc() : %s, size : %ld", filename, length);
		goto cleanup;
	}
	/* read the file into memory */
	read_chars = fread(content, sizeof(char), (size_t)length, file);
	if ((long)read_chars != length) {
		STDK_LOGE("the size of read chars is different %ld : %ld",
			(long)read_chars, length);
		free(content);
		content = NULL;
		goto cleanup;
	}
	content[read_chars] = '\0';

cleanup:
	if (file)
		fclose(file);

	return content;
}

int std_mgr_config_init(const char *res_path)
{
	char *device_info_path = NULL;
	char *onboarding_config_path = NULL;

	RETV_IF(!res_path, -1);

	device_info_path = g_strdup_printf("%s/%s", res_path, "device_info.json");
	onboarding_config_path = g_strdup_printf("%s/%s", res_path, "onboarding_config.json");

	free(g_device_info);
	free(g_onboarding_config);

	g_device_info = read_file(device_info_path);
	g_onboarding_config = read_file(onboarding_config_path);
	free(device_info_path);
	free(onboarding_config_path);

	STDK_LOGI("device_info[%s]", g_device_info);
	STDK_LOGI("onboarding_config[%s]", g_onboarding_config);

	if (!(g_device_info && g_onboarding_config)) {
		free(g_device_info);
		free(g_onboarding_config);
		return -1;
	}

	return 0;
}

int std_mgr_config_fini(void)
{
	FREE(g_device_info);
	FREE(g_onboarding_config);
	FREE(g_softap_ssid);

	return 0;
}

const char *std_mgr_config_get_device_info(void)
{
	return g_device_info;
}

const char *std_mgr_config_get_onboarding_config(void)
{
	return g_onboarding_config;
}

static int _get_onboarding_detail(char **onboarding_id, char **mnid, char **setup_id, char **identity_type)
{
	JsonParser *parser = NULL;
	JsonNode *root = NULL;
	JsonObject *root_obj = NULL;
	JsonObject *config = NULL;
	const char *onbo_id = NULL;
	const char *mn_id = NULL;
	const char *s_id = NULL;
	const char *id_type = NULL;

	RETV_IF(!g_onboarding_config, -1);
	RETV_IF(!onboarding_id, -1);
	RETV_IF(!mnid, -1);
	RETV_IF(!setup_id, -1);
	RETV_IF(!identity_type, -1);

	parser = json_parser_new();
	json_parser_load_from_data(parser, g_onboarding_config, -1, NULL);

	root = json_parser_get_root(parser);
	GO_IF(!root, ERROR);

	root_obj = json_node_get_object(root);
	GO_IF(!root_obj, ERROR);

	config = json_object_get_object_member(root_obj, "onboardingConfig");
	GO_IF(!config, ERROR);

	onbo_id = json_object_get_string_member(config, "deviceOnboardingId");
	if (strlen(onbo_id) > 13) {
		STDK_LOGE("device_onboarding_id too long");
		goto ERROR;
	}

	mn_id = json_object_get_string_member(config, "mnId");
	if (mn_id && !strcmp(mn_id, "MNID")) {
		STDK_LOGE("wrong mmid");
		goto ERROR;
	}

	s_id = json_object_get_string_member(config, "setupId");
	id_type = json_object_get_string_member(config, "identityType");
	if (!onbo_id || !mn_id || !s_id || !id_type) {
		STDK_LOGE("failed to get data - [%s] [%s] [%s] [%s]",
			onbo_id, mn_id, s_id, id_type);
		goto ERROR;
	}

	*onboarding_id = strdup(onbo_id);
	*mnid = strdup(mn_id);
	*setup_id = strdup(s_id);
	*identity_type = strdup(id_type);

	if (!*onboarding_id || !*mnid || !*setup_id || !*identity_type) {
		STDK_LOGE("strdup() failed");
		FREE(*onboarding_id);
		FREE(*mnid);
		FREE(*setup_id);
		FREE(*identity_type);
		goto ERROR;
	}

	if (parser)
		g_object_unref(parser);

	return 0;

ERROR:
	if (parser)
		g_object_unref(parser);

	return -1;
}

static int _make_hashed_sn(const char *serial_num, char **hashed_sn)
{
	GChecksum *checksum = NULL;
	guchar digest[SHA256_LEN] = {0};
	unsigned int len_dig = SHA256_LEN;

	RETV_IF(!serial_num, -1);
	RETV_IF(!hashed_sn, -1);

	checksum = g_checksum_new(G_CHECKSUM_SHA256);
	RETV_IF(!checksum, -1);

	g_checksum_update(checksum, (const guchar *)serial_num, -1);
	g_checksum_get_digest(checksum, digest, &len_dig);
	if (len_dig < HASH_SIZE) {
		g_checksum_free(checksum);
		STDK_LOGE("failed to g_checksum_get_digest - %s", digest);
		return -1;
	}

	*hashed_sn = g_base64_encode(digest, len_dig);
	g_checksum_free(checksum);

	RETV_IF(!*hashed_sn, -1);

	return 0;
}

static int _get_serial_number(char **serial_number, char **hashed_sn)
{
	JsonParser *parser = NULL;
	JsonNode *root = NULL;
	JsonObject *root_obj = NULL;
	JsonObject *info = NULL;
	const char *seiral = NULL;

	RETV_IF(!g_device_info, -1);
	RETV_IF(!serial_number, -1);
	RETV_IF(!hashed_sn, -1);

	parser = json_parser_new();
	json_parser_load_from_data(parser, g_device_info, -1, NULL);

	root = json_parser_get_root(parser);
	GO_IF(!root, ERROR);

	root_obj = json_node_get_object(root);
	GO_IF(!root_obj, ERROR);

	info = json_object_get_object_member(root_obj, "deviceInfo");
	GO_IF(!info, ERROR);

	seiral = json_object_get_string_member(info, "serialNumber");
	GO_IF(!seiral, ERROR);

	if (_make_hashed_sn(seiral, hashed_sn)) {
		STDK_LOGE("_make_hashed_sn() failed");
		goto ERROR;
	}

	*serial_number = strdup(seiral);
	if (!*serial_number) {
		STDK_LOGE("strdup() failed");
		FREE(*serial_number);
		FREE(*hashed_sn);
		goto ERROR;
	}

	return 0;

ERROR:
	if (parser)
		g_object_unref(parser);

	return -1;
}

static int _create_softap_ssid(void)
{
	char *serial_number = NULL;
	char *hashed_sn = NULL;
	char *onboarding_id = NULL;
	char *mnid = NULL;
	char *setupid = NULL;
	char *identity_type = NULL;
	unsigned char last_sn[HASH_SIZE + 1] = { 0,};
	unsigned int length;
	int i;
	int ret = 0;

	RETV_IF(!g_onboarding_config, -1);
	RETV_IF(!g_device_info, -1);

	do {
		ret = _get_serial_number(&serial_number, &hashed_sn);
		if (ret != 0) {
			STDK_LOGE("_get_serial_number() failed");
			break;
		}

		ret = _get_onboarding_detail(&onboarding_id, &mnid, &setupid, &identity_type);
		if (ret != 0) {
			STDK_LOGE("_get_onboarding_detail() failed");
			break;
		}

		length = strlen(serial_number);
		for (i = 0; i < HASH_SIZE; i++) {
			if (length < (HASH_SIZE - i))
				last_sn[i] = 0;
			else
				last_sn[i] = serial_number[length - (HASH_SIZE - i)];
		}
		last_sn[HASH_SIZE] = '\0';

		STDK_LOGI(">> %s[%c%c%c%c] <<", onboarding_id,
					last_sn[0], last_sn[1],
					last_sn[2], last_sn[3]);

		hashed_sn[HASH_SIZE] = '\0';

		if (!strcmp(identity_type, "X509")) {
			g_softap_ssid = g_strdup_printf("%s_E4%3s%3s7%4s%4s",
				onboarding_id, mnid, setupid, hashed_sn, last_sn);
		} else if (!strcmp(identity_type, "ED25519")) {
			g_softap_ssid = g_strdup_printf("%s_E4%3s%3s6%4s%4s",
				onboarding_id, mnid, setupid, hashed_sn, last_sn);
		} else {
			STDK_LOGE("unsupported type [%s]", identity_type);
			ret = -1;
			break;
		}
	} while (0);

	STDK_LOGI("ssid : [%s]", g_softap_ssid);

	free(serial_number);
	free(hashed_sn);
	free(onboarding_id);
	free(mnid);
	free(setupid);
	free(identity_type);

	return ret;
}

const char* std_mgr_config_get_softap_ssid(void)
{
	if (g_softap_ssid)
		return g_softap_ssid;

	_create_softap_ssid();

	return g_softap_ssid;
}
