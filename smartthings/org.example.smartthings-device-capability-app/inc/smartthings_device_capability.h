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

#ifndef __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_CAPABILITY_H__
#define __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_CAPABILITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tizen.h>
#include <smartthings_device_capability_data.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file smartthings_device_capability.h
 */

/**
 * @addtogroup CAPI_SMARTTHINGS_DEVICE_CAPABILITY_MODULE
 * @{
 */

/**
 * @brief Enumeration for SmartThings Device Capability error.
 * @since_ses 9
 */
typedef enum {
	SMARTTHINGS_DEVICE_CAP_ERROR_NONE = TIZEN_ERROR_NONE,                                /**< Successful */
	SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,      /**< Invalid parameter */
	SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,              /**< Out of memory */
	SMARTTHINGS_DEVICE_CAP_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,      /**< Permission denied */
	SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA,                          /**< No data */
	SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,			   /**< Not supported */
	SMARTTHINGS_DEVICE_CAP_ERROR_UNKNOWN = TIZEN_ERROR_UNKNOWN,					   /**< Unknown */
	SMARTTHINGS_DEVICE_CAP_ERROR_OPERATION_FAILED = TIZEN_ERROR_UNKNOWN - 1,             /**< Operation failed */
	SMARTTHINGS_DEVICE_CAP_ERROR_SERVICE_UNAVAILABLE = TIZEN_ERROR_UNKNOWN -2,            /**< Service unavailable */
	SMARTTHINGS_DEVICE_CAP_ERROR_LOWER_VERSION = TIZEN_ERROR_UNKNOWN -3            /**< Lower version */
} smartthings_device_cap_error_e;

/**
 * @brief Enumeration for connection status.
 * @since_ses 9
 */
typedef enum {
	SMARTTHINGS_DEVICE_CAP_CONNECTION_STATUS_CONNECTED = 0,          /**< Connection is connected */
	SMARTTHINGS_DEVICE_CAP_CONNECTION_STATUS_DISCONNECTED,           /**< Connection is disconnected */
	SMARTTHINGS_DEVICE_CAP_CONNECTION_STATUS_REJECTED,               /**< Connection is rejected */
} smartthings_device_cap_connection_status_e;

/**
 * @brief The SmartThings Device service handle.
 * @since_ses 9
 */
typedef struct smartthings_device_cap_service_s *smartthings_device_cap_service_h;

/**
 * @brief The SmartThings capability event handle.
 * @since_ses 9
 */
typedef struct smartthings_device_cap_event_s *smartthings_device_cap_event_h;

/**
 * @brief The argument handle of SmartThings capability command.
 * @since_ses 9
 */
typedef struct smartthings_device_cap_arguments_s *smartthings_device_cap_arguments_h;

/**
 * @brief The callback for status of connection to SmartThings Device agent.
  * @details The following error codes can be received: \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_NONE:                               Success \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_PERMISSION_DENIED:      Permission denied \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_SERVICE_UNAVAILABLE:   Service unavailable \n
 * @since_ses 9
 *
 * @remarks The @a handle should not be released.
 * @remarks The @a handle is the same object for which the callback was set/added.
 * @remarks The @a handle will be released when smartthings_device_cap_disconnect() is called.
 * @remarks When the callback is called, user can see result as #smartthings_device_cap_error_e enumeration value.
 * @remarks When the callback is called, user can see connection status as #smartthings_device_cap_connection_status_e enumeration value.
 *
 * @param[in] result The result of connection operation
 * @param[in] handle The SmartThings Device service handle
 * @param[in] status The status of connection
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_cap_connect()
 */
typedef void (*smartthings_device_cap_connection_status_cb)(smartthings_device_cap_error_e result, smartthings_device_cap_service_h handle, smartthings_device_cap_connection_status_e status, void *user_data);

/**
 * @brief The callback for publishing current status of the device when connected to SmartThings server.
 * @since_ses 9
 *
 * @remarks The callback will be invoked when the device connects to SmartThings server.
 * @remarks The device should publish its current status to SmartThings server in this callback.
 *
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see smartthings_device_cap_set_cbs()
 */
typedef void (*smartthings_device_cap_init_cb)(void *user_data );

/**
 * @brief The callback for handling capability command.
 * @since_ses 9
 *
 * @remarks The callback will be invoked when receiving capability commands from SmartThings server.
 * @remarks The @a component, @a capability, @a command and @a command_id can be used only in the callback. To use outside, make a copy.
 * @remarks The @a arguments should not be released and can be used only in the callback.
 *
 * @param[in] component The component name
 * @param[in] capability The capability name
 * @param[in] command The command for capability
 * @param[in] arguments The array handle of command arguments
 * @param[in] command_id The command ID
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @see smartthings_device_cap_set_cbs()
 */
typedef void (*smartthings_device_cap_command_cb)(const char *component, const char *capability, const char *command, smartthings_device_cap_arguments_h arguments, const char *command_id, void *user_data );

/**
 * @brief The callback for receiving the result of event publishing.
 * @details The following error codes can be received: \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_NONE:                            Successful \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER:           Invalid parameter \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_PERMISSION_DENIED:       Permission denied \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY:      Out of memory \n
	#SMARTTHINGS_DEVICE_CAP_ERROR_OPERATION_FAILED:            Operation failed \n
 *
 * @since_ses 10
 *
 * @remarks The @a err_mag should not be released and can be used only in the callback.
 *
 * @param[in] result The result code
 * @param[in] err_mag The optional text explaining the error. it can be NULL
 * @param[in] sequence_no The sequence number of the events sent.
 * @param[in] user_data The user data passed from the callback function
 *
 * @see smartthings_device_cap_publish_events()
 */
typedef void (*smartthings_device_cap_publish_result_cb)(smartthings_device_cap_error_e result, const char *err_mag, int sequence_no, void *user_data);

/**
 * @brief Initializes a capability service handle and connects to the agent.
 * @since_ses 9
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.capability \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @remarks The @a cap_svc_h must be released using smartthings_device_cap_disconnect().
 * @remarks Ths function returns #SMARTTHINGS_DEVICE_CAP_ERROR_PERMISSION_DENIED\n
 *          if the application has no app-defined privilege for 'http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.capability'.
 *
 * @param[out] cap_svc_h The capability service handle to be newly created on success
 * @param[in] connection_status_cb The connection status callback to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 * @see smartthings_device_cap_disconnect()
 */
int smartthings_device_cap_connect(smartthings_device_cap_service_h *cap_svc_h,
		smartthings_device_cap_connection_status_cb connection_status_cb, void *user_data);

/**
 * @brief Deinitializes a capability service handle and disconnects from the agent.
 * @since_ses 9
 *
 * @param[in] cap_svc_h The capability service handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_cap_connect()
 */
int smartthings_device_cap_disconnect(smartthings_device_cap_service_h cap_svc_h);

/**
 * @brief Sets a init callback and a command callback.
 * @since_ses 9
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.capability \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 *
 * @param[in] cap_svc_h The capability service handle
 * @param[in] init_cb The init callback to register
 * @param[in] cmd_cb The command callback to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_SERVICE_UNAVAILABLE Service unavailable
 *
 */
int smartthings_device_cap_set_cbs(smartthings_device_cap_service_h cap_svc_h,
		smartthings_device_cap_init_cb init_cb, smartthings_device_cap_command_cb cmd_cb, void *user_data);

/**
 * @brief Gets the number of argments of the capability command.
 * @since_ses 9
 *
 * @param[in] arguments The arguments handle of the capability command
 * @param[out] count The number of argments
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_cap_command_cb()
 */
int smartthings_device_cap_arguments_get_count(smartthings_device_cap_arguments_h arguments, unsigned int *count);

/**
 * @brief Gets an integer type argument.
 * @since_ses 9
 *
 * @remarks For the index and data type of the argument, see the capability reference.
 *
 * @param[in] arguments The arguments handle of the capability command
 * @param[in] index The index of the argument to be fetched
 * @param[out] value The value of the argument
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 *
 * @see smartthings_device_cap_command_cb()
 */
int smartthings_device_cap_arguments_get_int(smartthings_device_cap_arguments_h arguments, unsigned int index, int *value);

/**
 * @brief Gets a double type argument.
 * @since_ses 9
 *
 * @remarks For the index and data type of the argument, see the capability reference.
 *
 * @param[in] arguments The arguments handle of the capability command
 * @param[in] index The index of the argument to be fetched
 * @param[out] value The value of the argument
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 *
 * @see smartthings_device_cap_command_cb()
 */
int smartthings_device_cap_arguments_get_double(smartthings_device_cap_arguments_h arguments, unsigned int index, double *value);

/**
 * @brief Gets an string type argument.
 * @since_ses 9
 *
 * @remarks For the index and data type of the argument, see the capability reference.
 * @remarks The @a value should be released using free().
 *
 * @param[in] arguments The arguments handle of the capability command
 * @param[in] index The index of the argument to be fetched
 * @param[out] value The value of the argument
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 *
 * @see smartthings_device_cap_command_cb()
 */
int smartthings_device_cap_arguments_get_string(smartthings_device_cap_arguments_h arguments, unsigned int index, char **value);

/**
 * @brief Gets an object type argument.
 * @since_ses 9
 *
 * @remarks For the index and data type of the argument, see the capability reference.
 * @remarks The @a value should be released using smartthings_device_cap_data_destroy().
 *
 * @param[in] arguments The arguments handle of the capability command
 * @param[in] index The index of the argument to be fetched
 * @param[out] value The value of the argument
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 *
 * @see smartthings_device_cap_command_cb()
 */
int smartthings_device_cap_arguments_get_object(smartthings_device_cap_arguments_h arguments, unsigned int index, smartthings_device_cap_data_h *value);

/**
 * @brief Creates a capability event handle with integer type attribute.
 * @since_ses 9
 *
 * @remarks The @a event must be released using smartthings_device_cap_event_destroy().
 *
 * @param[in] component The component name
 * @param[in] capability The capability name
 * @param[in] attribute The attribute name
 * @param[in] value The attribute value to set
 * @param[in] unit The unit if needed. Otherwise NULL.
 * @param[in] command_id The command ID for this event if needed. Otherwise NULL.
 * @param[out] event The capability event handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see smartthings_device_cap_event_destroy()
 * @see smartthings_device_cap_publish_events()
 */
int smartthings_device_cap_event_create_int(const char *component, const char *capability,
		const char *attribute, int value, const char *unit, const char *command_id, smartthings_device_cap_event_h *event);

/**
 * @brief Creates a capability event handle with double type attribute.
 * @since_ses 9
 *
 * @remarks The @a event must be released using smartthings_device_cap_event_destroy().
 *
 * @param[in] component The component name
 * @param[in] capability The capability name
 * @param[in] attribute The attribute name
 * @param[in] value The attribute value to set
 * @param[in] unit The unit if needed. Otherwise NULL.
 * @param[in] command_id The command ID for this event if needed. Otherwise NULL.
 * @param[out] event The capability event handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see smartthings_device_cap_event_destroy()
 * @see smartthings_device_cap_publish_events()
 */
int smartthings_device_cap_event_create_double(const char *component, const char *capability,
		const char *attribute, double value, const char *unit, const char *command_id, smartthings_device_cap_event_h *event);

/**
 * @brief Creates a capability event handle with string type attribute.
 * @since_ses 9
 *
 * @remarks The @a event must be released using smartthings_device_cap_event_destroy().
 *
 * @param[in] component The component name
 * @param[in] capability The capability name
 * @param[in] attribute The attribute name
 * @param[in] value The attribute value to set
 * @param[in] unit The unit if needed. Otherwise NULL.
 * @param[in] command_id The command ID for this event if needed. Otherwise NULL.
 * @param[out] event The capability event handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see smartthings_device_cap_event_destroy()
 * @see smartthings_device_cap_publish_events()
 */
int smartthings_device_cap_event_create_string(const char *component, const char *capability,
		const char *attribute, const char *value, const char *unit, const char *command_id, smartthings_device_cap_event_h *event);

/**
 * @brief Creates a capability event handle with string array type attribute.
 * @since_ses 9
 *
 * @remarks The @a event must be released using smartthings_device_cap_event_destroy().
 *
 * @param[in] component The component name
 * @param[in] capability The capability name
 * @param[in] attribute The attribute name
 * @param[in] string_array The attribute value to set
 * @param[in] length The length of @a string_array
 * @param[in] unit The unit if needed. Otherwise NULL.
 * @param[in] command_id The command ID for this event if needed. Otherwise NULL.
 * @param[out] event The capability event handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see smartthings_device_cap_event_destroy()
 * @see smartthings_device_cap_publish_events()
 */
int smartthings_device_cap_event_create_string_array(const char *component, const char *capability,
		const char *attribute, const char *string_array[], unsigned int length, const char *unit, const char *command_id, smartthings_device_cap_event_h *event);

/**
 * @brief Creates a capability event handle with object type attribute.
 * @since_ses 9
 *
 * @remarks The @a event must be released using smartthings_device_cap_event_destroy().
 * @remarks The @a value is capability data handle created using smartthings_device_cap_data_create().
 *
 * @param[in] component The component name
 * @param[in] capability The capability name
 * @param[in] attribute The attribute name
 * @param[in] value The attribute value to set
 * @param[in] unit The unit if needed. Otherwise NULL.
 * @param[in] command_id The command ID for this event if needed. Otherwise NULL.
 * @param[out] event The capability event handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see smartthings_device_cap_event_destroy()
 * @see smartthings_device_cap_publish_events()
 */
int smartthings_device_cap_event_create_object(const char *component, const char *capability,
		const char *attribute, smartthings_device_cap_data_h value, const char *unit, const char *command_id, smartthings_device_cap_event_h *event);

/**
 * @brief Sets a data property of the attribute.
 * @since_ses 9
 *
 * @remarks The @a data is capability data handle created using smartthings_device_cap_data_create().
 *
 * @param[in] event The capability event handle
 * @param[in] data The value of the data property to set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int smartthings_device_cap_event_set_data(smartthings_device_cap_event_h event, smartthings_device_cap_data_h data);

/**
 * @brief Destroys a capability event handle and releases all its resources.
 * @since_ses 9
 *
 * @param[in] event The capability event handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int smartthings_device_cap_event_destroy(smartthings_device_cap_event_h event);

/**
 * @brief Publishes events of the capabilities.
 * @since_ses 10
 * @privlevel public
 * @privilege %http://com.samsung.tizen.smartthings-device/appdefined/smartthings-device.capability \n
 *            %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing \n
 *            %http://tizen.org/privilege/internet
 *
 * @param[in] cap_svc_h The capability service handle
 * @param[in] event_array The array of events of the capabilities
 * @param[in] length The length of @a event_array
 * @param[in] callback The callback function to receive the result
 * @param[in] user_data The user data passed from the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OPERATION_FAILED Operation failed
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_SERVICE_UNAVAILABLE Service unavailable
 */
int smartthings_device_cap_publish_events(smartthings_device_cap_service_h cap_svc_h,
	smartthings_device_cap_event_h event_array[], unsigned int length, smartthings_device_cap_publish_result_cb callback, void *user_data);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_CAPABILITY_H__ */
