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

#ifndef __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_CAPABILITY_DATA_H__
#define __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_CAPABILITY_DATA_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file smartthings_device_capability_data.h
 */

/**
 * @addtogroup CAPI_SMARTTHINGS_DEVICE_CAPABILITY_MODULE
 * @{
 */

/**
 * @brief The SmartThings Device Capability data handle.
 * @since_ses 9
 */
typedef struct smartthings_device_cap_data_s *smartthings_device_cap_data_h;

/**
 * @brief Creates a capability data handle.
 * @since_ses 9
 *
 * @remarks The @a data must be released using smartthings_device_cap_data_destroy().
 *
 * @param[out] data The capability data handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see smartthings_device_cap_data_destroy()
 */
int smartthings_device_cap_data_create(smartthings_device_cap_data_h *data);

/**
 * @brief Destroys a capability data handle and releases all its resources.
 * @since_ses 9
 *
 * @remarks If the @a data has child data, its child data will be released as well.
 *
 * @param[in] data The capability data handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_cap_data_create()
 */
int smartthings_device_cap_data_destroy(smartthings_device_cap_data_h data);

/**
 * @brief Sets an integer value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] value The value to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_int(smartthings_device_cap_data_h data, const char *property, int value);

/**
 * @brief Sets a boolean value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] value The value to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_bool(smartthings_device_cap_data_h data, const char *property, bool value);

/**
 * @brief Sets a double value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] value The value to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_double(smartthings_device_cap_data_h data, const char *property, double value);

/**
 * @brief Sets a string value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] value The value to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_string(smartthings_device_cap_data_h data, const char *property, const char *value);

/**
 * @brief Sets an object value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The @a value is capability data handle created using smartthings_device_cap_data_create().
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] value The value to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_object(smartthings_device_cap_data_h data, const char *property, smartthings_device_cap_data_h value);

/**
 * @brief Sets an integer array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] array The array associated with the given property name
 * @param[in] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_int_array(smartthings_device_cap_data_h data, const char *property, const int *array, unsigned int length);

/**
 * @brief Sets an boolean array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] array The array associated with the given property name
 * @param[in] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_bool_array(smartthings_device_cap_data_h data, const char *property, const bool *array, unsigned int length);

/**
 * @brief Sets a double array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] array The array associated with the given property name
 * @param[in] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_double_array(smartthings_device_cap_data_h data, const char *property, const double *array, unsigned int length);

/**
 * @brief Sets a string array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The function replaces any existing value for the given @a property.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[in] array The array associated with the given property name
 * @param[in] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int smartthings_device_cap_data_set_string_array(smartthings_device_cap_data_h data, const char *property, const char **array, unsigned int length);

/**
 * @brief Gets an integer value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] value The value associated with the given property name
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_int(smartthings_device_cap_data_h data, const char *property, int *value);


/**
 * @brief Gets a boolean value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] value The value associated with the given property name
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_bool(smartthings_device_cap_data_h data, const char *property, bool *value);

/**
 * @brief Gets a double value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] value The value associated with the given property name
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_double(smartthings_device_cap_data_h data, const char *property, double *value);

/**
 * @brief Gets a string value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The @a value should be released using free().
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] value The value associated with the given property name
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_string(smartthings_device_cap_data_h data, const char *property, char **value);

/**
 * @brief Gets an object value for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The @a value should be released using smartthings_device_cap_data_destroy().
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] value The value associated with the given property name
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_object(smartthings_device_cap_data_h data, const char *property, smartthings_device_cap_data_h *value);

/**
 * @brief Gets an integer array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The @a array must be released using smartthings_device_cap_data_release_int_array().
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] array The array associated with the given property name
 * @param[out] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_int_array(smartthings_device_cap_data_h data, const char *property, int **array, unsigned int *length);

/**
 * @brief Gets a boolean array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The @a array must be released using smartthings_device_cap_data_release_bool_array().
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] array The array associated with the given property name
 * @param[out] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_bool_array(smartthings_device_cap_data_h data, const char *property, bool **array, unsigned int *length);

/**
 * @brief Gets a double array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The @a array must be released using smartthings_device_cap_data_release_double_array().
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] array The array associated with the given property name
 * @param[out] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_double_array(smartthings_device_cap_data_h data, const char *property, double **array, unsigned int *length);

/**
 * @brief Gets a string array for the property name.
 * @since_ses 9
 *
 * @remarks The @a property is key value, one of several properties of the attribute or argument.
 * @remarks The @a array must be released using smartthings_device_cap_data_release_string_array().
 *
 * @param[in] data The capability data handle
 * @param[in] property The property name
 * @param[out] array The array associated with the given property name
 * @param[out] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NO_DATA No data
 */
int smartthings_device_cap_data_get_string_array(smartthings_device_cap_data_h data, const char *property, char ***array, unsigned int *length);

/**
 * @brief Releases an integer array of the capability data.
 * @since_ses 9
 *
 * @param[in] array The array to release
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_cap_data_get_int_array()
 */
int smartthings_device_cap_data_release_int_array(int *array);

/**
 * @brief Releases a boolean array of the capability data.
 * @since_ses 9
 *
 * @param[in] array The array to release
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_cap_data_get_bool_array()
 */
int smartthings_device_cap_data_release_bool_array(bool *array);

/**
 * @brief Releases a double array of the capability data.
 * @since_ses 9
 *
 * @param[in] array The array to release
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_cap_data_get_double_array()
 */
int smartthings_device_cap_data_release_double_array(double *array);

/**
 * @brief Releases a string array of the capability data.
 * @since_ses 9
 *
 * @param[in] array The array to release
 * @param[in] length The length of @a array
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NONE Successful
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_NOT_SUPPORTED Not supported
 * @retval #SMARTTHINGS_DEVICE_CAP_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see smartthings_device_cap_data_get_string_array()
 */
int smartthings_device_cap_data_release_string_array(char **array, unsigned int length);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SAMSUNG_EXPERIENCE_SERVICE_SMARTTHINGS_DEVICE_CAPABILITY_DATA_H__ */
