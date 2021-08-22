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

#ifndef __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_MANAGER_H__
#define __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_MANAGER_H__

#include <stdlib.h>
#include <stdbool.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SMARTTHINGS_DEVICE_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration for the SmartThings Device error.
 * @since_ses 9
 */
typedef enum {
	SMARTTHINGS_DEVICE_ERROR_NONE = TIZEN_ERROR_NONE,                                /**< Successful */
	SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,      /**< Invalid parameter */
	SMARTTHINGS_DEVICE_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,              /**< Out of memory */
	SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,      /**< Permission denied */
	SMARTTHINGS_DEVICE_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA,                          /**< No data */
	SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,              /**< Not supported */
	SMARTTHINGS_DEVICE_ERROR_UNKNOWN = TIZEN_ERROR_UNKNOWN,					   /**< Unknown */
	SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED = TIZEN_ERROR_UNKNOWN - 1,             /**< Operation failed */
	SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE = TIZEN_ERROR_UNKNOWN -2,            /**< Service unavailable */
	SMARTTHINGS_DEVICE_ERROR_LOWER_VERSION = TIZEN_ERROR_UNKNOWN -3            /**< Lower version */
} smartthings_device_error_e;

/**
 * @brief Enumeration for SmartThings status.
 * @since_ses 9
 *
 * @remarks In version ses 10, some values and names of the enumeration have been changed.
 */
typedef enum {
	SMARTTHINGS_DEVICE_STATUS_INIT = 0,                         /**< Initial state of SmartThings Device */
	SMARTTHINGS_DEVICE_STATUS_ES_STARTED,                       /**< Easy-setup is started */
	SMARTTHINGS_DEVICE_STATUS_ES_DONE,                          /**< Easy-setup is done */
	SMARTTHINGS_DEVICE_STATUS_REGISTERING_TO_CLOUD,             /**< Trying to sign up, sign in to cloud */
	SMARTTHINGS_DEVICE_STATUS_REGISTERING_TO_CLOUD_FAILED,    /**< Failed to sign up to cloud */
	SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD,	/**< In progress to connect to MQTT server */
	SMARTTHINGS_DEVICE_STATUS_CONNECTED_TO_CLOUD,	/**< connected to MQTT server */
	SMARTTHINGS_DEVICE_STATUS_CONNECTING_TO_CLOUD_FAILED,	/**< Failed to connect to MQTT server */
} smartthings_device_status_e;

/**
 * @brief Enumeration for RPC connection status.
 * @since_ses 9
 */
typedef enum {
	SMARTTHINGS_DEVICE_CONNECTION_STATUS_CONNECTED = 0,            /**< Connection is connected */
	SMARTTHINGS_DEVICE_CONNECTION_STATUS_DISCONNECTED,             /**< Connection is disconnected */
	SMARTTHINGS_DEVICE_CONNECTION_STATUS_REJECTED,                 /**< Connection is rejected */
} smartthings_device_connection_status_e;

/**
 * @brief Enumeration for notification message type.
 * @since_ses 10
 */
typedef enum {
	SMARTTHINGS_DEVICE_NOTI_TYPE_UNKNOWN = -1,     /**< Unknown */
	SMARTTHINGS_DEVICE_NOTI_TYPE_DEV_DELETED,      /**< Device deleted */
	SMARTTHINGS_DEVICE_NOTI_TYPE_RATE_LIMIT,       /**< Rate limited */
	SMARTTHINGS_DEVICE_NOTI_TYPE_QUOTA_REACHED,    /**< Quota Reached */
	SMARTTHINGS_DEVICE_NOTI_TYPE_ERROR             /**< Error */
} smartthings_device_noti_type_e;

/**
 * @brief The Wi-Fi authentication type of access point.
 * @since_ses 10
 */
typedef enum {
	SMARTTHINGS_DEVICE_WIFI_AUTHTYPE_NONE = 0, /**< No authentication */
	SMARTTHINGS_DEVICE_WIFI_AUTHTYPE_WEP,      /**< WEP */
	SMARTTHINGS_DEVICE_WIFI_AUTHTYPE_WPA_PSK,  /**< WPA-PSK */
	SMARTTHINGS_DEVICE_WIFI_AUTHTYPE_WPA2_PSK,  /**< WPA2-PSK */
	SMARTTHINGS_DEVICE_WIFI_AUTHTYPE_WPA_WPA2_PSK,  /**< WPA-WPA2-PSK */
	SMARTTHINGS_DEVICE_WIFI_AUTHTYPE_WPA2_ENTERPRISE	/**< WPA2-ENTERPRISE */
} smartthings_device_wifi_authtype_e;

/**
 * @brief The Wi-Fi frequency band.
 * @since_ses 10
 */
typedef enum {
	SMARTTHINGS_DEVICE_WIFI_FREQ_2_4G = 0,       /**< Wi-Fi 2.4GHz */
	SMARTTHINGS_DEVICE_WIFI_FREQ_5G,                  /**< Wi-Fi 5GHz */
	SMARTTHINGS_DEVICE_WIFI_FREQ_2_4G_5G_BOTH,    /**< Wi-Fi 2.4GHz and 5GHz */
} smartthings_device_wifi_freq_e;

/**
 * @brief The SmartThings handle.
 * @since_ses 9
 */
typedef struct smartthings_device_s *smartthings_device_h;

/**
 * @brief The SmartThings setup information handle.
 * @since_ses 10
 */
typedef struct smartthings_device_setup_info_s *smartthings_device_setup_info_h;

/**
 * @brief The access point information handle.
 * @since_ses 10
 */
typedef struct smartthings_device_ap_info_s *smartthings_device_ap_info_h;

/**
 * @brief The access point list handle.
 * @since_ses 10
 */
typedef struct smartthings_device_ap_list_s *smartthings_device_ap_list_h;

/**
 * @brief The callback for status of connection to SmartThings Thing agent.
 * @details The following error codes can be received: \n
	#SMARTTHINGS_DEVICE_ERROR_NONE:      Successful \n
	#SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED:      Permission denied \n
	#SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE:   Service unavailable \n
 * @since_ses 9
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_disconnect() is called.
 * @remarks When the callback is called, user can see result as #smartthings_device_error_e enumeration value.
 * @remarks When the callback is called, user can see connection status as #smartthings_device_connection_status_e enumeration value.
 *
 * @param[in] result The result of connection operation
 * @param[in] handle The SmartThings handle
 * @param[in] status The status of connection
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_connect()
 */
typedef void (*smartthings_device_connection_status_cb)(smartthings_device_error_e result, smartthings_device_h handle, smartthings_device_connection_status_e status, void *user_data);

/**
 * @brief The callback for SmartThings Thing status.
 * @since_ses 9
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_disconnect() is called.
 * @remarks The @a errmsg can be used only in the callback. To use outside, make a copy.
 * @remarks When the callback is called, user can see SmartThings status as #smartthings_device_status_e enumeration value.
 *
 * @param[in] handle The SmartThings handle
 * @param[in] status The status of SmartThings
 * @param[in] errmsg The error message
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_set_status_changed_cb()
 */
typedef void (*smartthings_device_status_changed_cb)(smartthings_device_h handle, smartthings_device_status_e status, const char* errmsg, void *user_data);

/**
 * @brief The callback for informing user-input BUTTON of OTM(ownership transfer method).
 * @since_ses 9
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_disconnect() is called.
 * @remarks When the callback is called, user can send a confirmation for mutual verification as true or false using smartthings_device_send_otm_result().
 *
 * @param[in] handle The SmartThings handle
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_set_otm_for_button_cb()
 * @see smartthings_device_send_otm_result()
 */
typedef void (*smartthings_device_button_cb)(smartthings_device_h handle, void *user_data);

/**
 * @brief The callback for informing user-input PIN of OTM(ownership transfer method).
 * @since_ses 9
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_disconnect() is called.
 * @remarks When the callback is called, user generate PIN information and show generated PIN.
 *
 * @param[in] handle The SmartThings handle
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_set_otm_for_pin_cb()
 */
typedef void (*smartthings_device_show_pin_cb)(smartthings_device_h handle, void *user_data);

/**
 * @brief The callback for comparing user-input PIN of OTM(ownership transfer method).
 * @since_ses 9
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_disconnect() is called.
 * @remarks When the callback is called, user can compare generated PIN with inserted PIN.
 * @remarks When the callback is called, user can send a comparison result for mutual verification as true or false using smartthings_device_send_otm_result().
 *
 * @param[in] handle The SmartThings handle
 * @param[in] pin_value The PIN data in integer format
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_set_otm_for_pin_cb()
 * @see smartthings_device_send_otm_result()
 */
typedef void (*smartthings_device_pin_cb)(smartthings_device_h handle, int pin_value, void *user_data);


/**
 * @brief The callback for receiving notifications from the server.
 * @since_ses 9
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_disconnect() is called.
 * @remarks The @a noti_msg can be used only in the callback. To use outside, make a copy.
 * @remarks When the callback is called, user can see SmartThings notification as #smartthings_device_noti_type_e enumeration value.
 *
 * @param[in] handle The SmartThings handle
 * @param[in] type The type of notification
 * @param[in] noti_msg message of notification in JSON formatted
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_set_notification_cb()
 */
typedef void (*smartthings_device_notification_cb)(smartthings_device_h handle, smartthings_device_noti_type_e type, const char *noti_msg, void *user_data);

/**
 * @brief The callback for informing Wi-Fi AP information to connect.
 * @since_ses 10
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_disconnect() is called.
 * @remarks The @a ap_info_h can be used only in the callback and should not be released.
 * @remarks When the callback is called, user can get Wi-Fi provisioning information.
 *
 * @param[in] handle The SmartThings handle
 * @param[in] ap_info_h The AP information handle
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_set_wifi_ap_provisioning_cb()
 */
typedef void(* smartthings_device_wifi_ap_provisioning_cb)(smartthings_device_h handle, smartthings_device_ap_info_h ap_info_h, void *user_data);

/**
 * @brief Creates a handle and connects to agent.
 * @since_ses 9
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @remarks The @a handle must be released using smartthings_device_disconnect().
 * @remarks Ths function returns #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED\n
 *          if the application has no app-defined privilege for 'http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager'.
 *
 * @param[out] handle The SmartThings handle to be newly created on success
 * @param[in] connection_status_cb The RPC connection status callback to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 * @see smartthings_device_disconnect()
 */
int smartthings_device_connect(smartthings_device_h *handle,
						smartthings_device_connection_status_cb connection_status_cb,
						void *user_data);

/**
 * @brief Deinitializes a handle and disconnects from the agent.
 * @since_ses 9
 *
 * @param[in] handle The SmartThings handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @see smartthings_device_connect()
 */
int smartthings_device_disconnect(smartthings_device_h handle);

/**
 * @platform
 * @brief Starts SmartThings Device.
 * @since_ses 10
 * @privlevel platform
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/datasharing \n
 *            %http://tizen.org/privilege/network.get \n
 *            %http://tizen.org/privilege/network.set \n
 *            %http://tizen.org/privilege/network.profile \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/systemsettings.admin
 *
 * @remarks Refer to 'Direct-connected devices' chapter of SmartThings Developer Documentation on the SmartThings Developer site for 'device_info.json' and 'onboarding_config.json'.
 *
 * @param[in] handle The SmartThings Device handle
 * @param[in] device_info JSON string from 'device_info.json'
 * @param[in] onboarding_config JSON string from 'onboarding_config.json'
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_start(smartthings_device_h handle, const char *device_info, const char *onboarding_config);

/**
 * @brief Sets a thing status changed callback.
 * @since_ses 9
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @remarks Only one callback function can be set with this function.
 * @remarks If multiple callbacks are set, the last one is registered.
 * @remarks The callback is called when SmartThings status is changed.
 * @remarks When the callback is called, user can get SmartThings status as #smartthings_device_status_e enumeration value.
 *
 * @param[in] handle The SmartThings Device handle
 * @param[in] status_cb The status changed callback to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_set_status_changed_cb(smartthings_device_h handle,
								smartthings_device_status_changed_cb status_cb,
								void *user_data);

/**
 * @platform
 * @brief Sends a OTM (ownership transfer method) confirmation after getting user's input regarding mutual verification.
 * @since_ses 9
 * @privlevel platform
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing \n
 *            %http://tizen.org/privilege/systemsettings.admin (Since API level 10)
 *
 * @param[in] handle The SmartThings handle
 * @param[in] confirm The confirm things on OTM (ownership transfer method)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 */
int smartthings_device_send_otm_result(smartthings_device_h handle, bool confirm);

/**
 * @brief Sets a callback for informing the application of user-input BUTTON.
 * @since_ses 9
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @remarks Only one callback function can be set with this function.
 * @remarks If multiple callbacks are set, the last one is registered.
 * @remarks The callback is called when it needs user's confirm for mutual verification based BUTTON.
 * @remarks When the callback is called, user can send a confirmation for mutual verification as true or false using smartthings_device_send_otm_result().
 *
 * @param[in] handle The SmartThings handle
 * @param[in] button_cb The callback for informing user-input BUTTON of OTM(ownership transfer method)
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_set_otm_for_button_cb(smartthings_device_h handle,
						smartthings_device_button_cb button_cb,
						void *user_data);


/**
 * @brief Sets a callback for informing user-input PIN of OTM(ownership transfer method).
 * @since_ses 9
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @remarks Only one callback function can be set with this function.
 * @remarks If multiple callbacks are set, the last one is registered.
 * @remarks @a show_pin_cb callback is called when PIN needs to be generated.
 * @remarks @a pin_cb callback is called when it needs user's confirm for mutual verification based PIN.
 * @remarks When @a show_pin_cb callback is called, user generates PIN information and show generated PIN.
 * @remarks When @a pin_cb callback is called, user can compare generated PIN with inserted PIN.
 *
 * @param[in] handle The SmartThings handle
 * @param[in] show_pin_cb The callback for informing user-input PIN of OTM(ownership transfer method)
 * @param[in] pin_cb The callback for comparing user-input PIN of OTM(ownership transfer method)
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_set_otm_for_pin_cb(smartthings_device_h handle,
						smartthings_device_show_pin_cb show_pin_cb,
						smartthings_device_pin_cb pin_cb,
						void *user_data);


/**
 * @brief Sets a callback for receiving notifications from the server.
 * @since_ses 9
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @remarks Only one callback function can be set with this function.
 * @remarks If multiple callbacks are set, the last one is registered.
 * @remarks The callback is called when it inform notification as #smartthings_device_noti_type_e enumeration value from server.
 *
 * @param[in] handle The SmartThings handle
 * @param[in] noti_cb The callback for notification from server
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_set_notification_cb(smartthings_device_h handle, smartthings_device_notification_cb noti_cb, void *user_data);

/**
 * @brief Creates a SmartThings setup information handle.
 * @since_ses 10
 * @privlevel public
 *
 * @param[out] setup_info_h The SmartThings setup information handle to be newly created on success
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int smartthings_device_setupinfo_create(smartthings_device_setup_info_h *setup_info_h);

/**
 * @brief Destroys a SmartThings setup information handle.
 * @since_ses 10
 * @privlevel public
 *
 * @param[in] setup_info_h The SmartThings setup information handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int smartthings_device_setupinfo_destroy(smartthings_device_setup_info_h setup_info_h);

/**
 * @brief Sets broker URL of setup information.
 * @since_ses 10
 * @privlevel public
 *
 * @param[in] setup_info_h The SmartThings setup information handle
 * @param[in] url The broker URL
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int smartthings_device_setupinfo_set_broker_url(smartthings_device_setup_info_h setup_info_h, const char *url);

/**
 * @brief Sets location of setup information.
 * @since_ses 10
 * @privlevel public
 *
 * @param[in] setup_info_h The SmartThings setup information handle
 * @param[in] location_id The location ID
 * @param[in] room_id The room ID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int smartthings_device_setupinfo_set_location(smartthings_device_setup_info_h setup_info_h, const char *location_id, const char *room_id);

/**
 * @platform
 * @brief Starts SmartThings self setup.
 * @since_ses 10
 * @privlevel platform
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/datasharing \n
 *            %http://tizen.org/privilege/network.get \n
 *            %http://tizen.org/privilege/network.set \n
 *            %http://tizen.org/privilege/network.profile \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/systemsettings.admin
 *
 * @param[in] handle The SmartThings Device handle
 * @param[in] device_info JSON string from 'device_info.json'
 * @param[in] onboarding_config JSON string from 'onboarding_config.json'
 * @param[in] setup_info_h The SmartThings setup information handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_start_with_setupinfo(smartthings_device_h handle, const char *device_info, const char *onboarding_config, smartthings_device_setup_info_h setup_info_h);

/**
 * @brief Gets an Easy-setup status.
 * @since_ses 10
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @param[in] handle The SmartThings handle
 * @param[out] is_completed The status of Easy-setup whether it is completed or not
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 */
int smartthings_device_get_easysetup_status(smartthings_device_h handle, bool *is_completed);

/**
 * @brief Gets the SSID of the access point.
 * @since_ses 10
 *
 * @remarks The @a ssid should be released using free().
 *
 * @param[in] ap_info_h The SmartThings AP information handle
 * @param[out] ssid The SSID name
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @see smartthings_device_set_wifi_ap_provisioning_cb()
 */
int smartthings_device_apinfo_get_ssid(smartthings_device_ap_info_h ap_info_h, char **ssid);

/**
 * @brief Gets the password of the access point.
 * @since_ses 10
 *
 * @remarks The @a pwd should be released using free().
 *
 * @param[in] ap_info_h The SmartThings AP information handle
 * @param[out] pwd The password
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @see smartthings_device_set_wifi_ap_provisioning_cb()
 */
int smartthings_device_apinfo_get_password(smartthings_device_ap_info_h ap_info_h, char **pwd);

/**
 * @brief Gets the authentication type of the access point.
 * @since_ses 10
 *
 * @param[in] ap_info_h The SmartThings AP information handle
 * @param[out] authtype The authentication type
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @see smartthings_device_set_wifi_ap_provisioning_cb()
 */
int smartthings_device_apinfo_get_authtype(smartthings_device_ap_info_h ap_info_h, smartthings_device_wifi_authtype_e *authtype);

/**
 * @brief Gets the BSSID of the access point.
 * @since_ses 10
 *
 * @remarks The @a bssid should be released using free().
 *
 * @param[in] ap_info_h The SmartThings AP information handle
 * @param[out] bssid The BSSID
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @see smartthings_device_set_wifi_ap_provisioning_cb()
 */
int smartthings_device_apinfo_get_bssid (smartthings_device_ap_info_h ap_info_h, char **bssid);

/**
 * @brief Sets a callback for getting Wi-Fi AP information during Easy-setup.
 * @since_ses 10
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @remarks Only one callback function can be set with this function.
 * @remarks If multiple callbacks are set, the last one is registered.
 * @remarks The callback is called when Wi-Fi provisioning event occurs.
 * @remarks When the callback is called, user can get Wi-Fi provisioning information.
 *
 * @param[in] handle The SmartThings handle
 * @param[in] wifi_ap_cb The Wi-Fi AP callback to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 * @see smartthings_device_set_device_property()
 */
int smartthings_device_set_wifi_ap_provisioning_cb(smartthings_device_h handle, smartthings_device_wifi_ap_provisioning_cb wifi_ap_cb, void *user_data);

/**
 * @platform
 * @brief Sets the supported Wi-Fi frequency for Easy-setup.
 * @since_ses 10
 * @privlevel platform
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing \n
 *            %http://tizen.org/privilege/systemsettings.admin
 *
 * @param[in] handle The SmartThings handle
 * @param[in] wifi_freq The supported Wi-Fi frequency
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_set_wifi_frequency(smartthings_device_h handle, smartthings_device_wifi_freq_e wifi_freq);

/**
 * @platform
 * @brief Sets the scanned AP list for Easy-setup.
 * @since_ses 10
 * @privlevel platform
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing \n
 *            %http://tizen.org/privilege/systemsettings.admin
 *
 * @remarks One of the APs in the list is picked up by the controller and passed by a callback registered using smartthings_device_set_wifi_ap_provisioning_cb().
 *
 * @param[in] handle The SmartThings handle
 * @param[in] ap_list_h The handle of the AP list scanned
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 * @see smartthings_device_aplist_create()
 * @see smartthings_device_aplist_destroy()
 * @see smartthings_device_set_wifi_ap_provisioning_cb()
 */
int smartthings_device_set_ap_list(smartthings_device_h handle, smartthings_device_ap_list_h ap_list_h);

/**
 * @brief Creates a SmartThings AP list handle.
 * @since_ses 10
 *
 * @remarks The @a ap_list_h must be released using smartthings_device_aplist_destroy().
 *
 * @param[in] count The count of AP
 * @param[out] ap_list_h The SmartThings AP list handle to be newly created on success
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see smartthings_device_aplist_destroy()
 */
int smartthings_device_aplist_create(unsigned int count, smartthings_device_ap_list_h *ap_list_h);

/**
 * @brief Destroys a SmartThings AP list handle.
 * @since_ses 10
 *
 * @param[in] ap_list_h The SmartThings AP list handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_aplist_create()
 */
int smartthings_device_aplist_destroy(smartthings_device_ap_list_h ap_list_h);

/**
 * @brief Sets SSID at the specific index of AP list handle.
 * @since_ses 10
 *
 * @param[in] ap_list_h The SmartThings AP list handle
 * @param[in] idx The index
 * @param[in] ssid The SSID of AP
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_aplist_create()
 * @see smartthings_device_aplist_destroy()
 */
int smartthings_device_aplist_set_ssid(smartthings_device_ap_list_h ap_list_h, int idx, const char *ssid);

/**
 * @brief Sets BSSID at the specific index of AP list handle.
 * @since_ses 10
 *
 * @param[in] ap_list_h The SmartThings AP list handle
 * @param[in] idx The index
 * @param[in] bssid The BSSID of AP
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_aplist_create()
 * @see smartthings_device_aplist_destroy()
 */
int smartthings_device_aplist_set_bssid(smartthings_device_ap_list_h ap_list_h, int idx, const char *bssid);

/**
 * @brief Sets signal strength (RSSI) at the specific index of AP list handle.
 * @since_ses 10
 *
 * @param[in] ap_list_h The SmartThings AP list handle
 * @param[in] idx The index
 * @param[in] signal_strength The signal strength of AP
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_aplist_create()
 * @see smartthings_device_aplist_destroy()
 */
int smartthings_device_aplist_set_signal_strength(smartthings_device_ap_list_h ap_list_h, int idx, int signal_strength);

/**
 * @brief Sets frequency band at the specific index of AP list handle.
 * @since_ses 10
 *
 * @param[in] ap_list_h The SmartThings AP list handle
 * @param[in] idx The index
 * @param[in] band The frequency band (MHz) of AP
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_aplist_create()
 * @see smartthings_device_aplist_destroy()
 */
int smartthings_device_aplist_set_frequency_band(smartthings_device_ap_list_h ap_list_h, int idx, int band);

/**
 * @brief Sets authentication type at the specific index of AP list handle.
 * @since_ses 10
 *
 * @param[in] ap_list_h The SmartThings AP list handle
 * @param[in] idx The index
 * @param[in] authtype The authentication type of AP
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_aplist_create()
 * @see smartthings_device_aplist_destroy()
 */
int smartthings_device_aplist_set_authtype(smartthings_device_ap_list_h ap_list_h, int idx, smartthings_device_wifi_authtype_e authtype);

/**
 * @platform
 * @brief Resets SmartThings Device.
 * @details This function clears the configuration stored on the device. \n
 *    After calling this function, the device will disconnect from SmartThings Cloud and the setup process will be required to restore the device \n
 * @since_ses 10
 * @privlevel platform
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.manager \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing \n
 *            %http://tizen.org/privilege/systemsettings.admin
 *
 * @param[in] handle The SmartThings Device handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_ERROR_SERVICE_UNAVAILABLE Service unavailable
 */
int smartthings_device_reset(smartthings_device_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_MANAGER_H__ */
