/*
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of Cypress Semiconductor 
 *  Corporation. All rights reserved. This software, including source code, documentation and  related 
 * materials ("Software"), is owned by Cypress Semiconductor  Corporation or one of its 
 *  subsidiaries ("Cypress") and is protected by and subject to worldwide patent protection  
 * (United States and foreign), United States copyright laws and international treaty provisions. 
 * Therefore, you may use this Software only as provided in the license agreement accompanying the 
 * software package from which you obtained this Software ("EULA"). If no EULA applies, Cypress 
 * hereby grants you a personal, nonexclusive, non-transferable license to  copy, modify, and 
 * compile the Software source code solely for use in connection with Cypress's  integrated circuit 
 * products. Any reproduction, modification, translation, compilation,  or representation of this 
 * Software except as specified above is prohibited without the express written permission of 
 * Cypress. Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO  WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING,  BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE. Cypress reserves the right to make changes to 
 * the Software without notice. Cypress does not assume any liability arising out of the application 
 * or use of the Software or any product or circuit  described in the Software. Cypress does 
 * not authorize its products for use in any products where a malfunction or failure of the 
 * Cypress product may reasonably be expected to result  in significant property damage, injury 
 * or death ("High Risk Product"). By including Cypress's product in a High Risk Product, the 
 *  manufacturer of such system or application assumes  all risk of such use and in doing so agrees 
 * to indemnify Cypress against all liability.
 */

/** @file
 *
 * BLE device's GATT database and device configuration.
 * Database contains mandatory GATT and GAP, plus Device Info which is
 * required by most OSs.
 */

#include "wiced.h"
#include "wiced_bt_dev.h"
#include "gatt_server_db.h"

#include "wiced_bt_uuid.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_hrs.h"

/*************************************************************************************
** GATT server definitions
*************************************************************************************/

const uint8_t gatt_server_db[] = // Define GATT database
{
    // Declare mandatory GATT service
    PRIMARY_SERVICE_UUID16( HDLS_GENERIC_ATTRIBUTE, UUID_SERVICE_GATT ),

    // Declare mandatory GAP service. Device Name and Appearance are mandatory
    PRIMARY_SERVICE_UUID16( HDLS_GENERIC_ACCESS, UUID_SERVICE_GAP ),

        // Declare mandatory GAP service characteristic: Dev Name
        CHARACTERISTIC_UUID16(HDLC_GENERIC_ACCESS_DEVICE_NAME, HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE,
            UUID_CHARACTERISTIC_DEVICE_NAME, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE ),

        // Declare mandatory GAP service characteristic: Appearance
        CHARACTERISTIC_UUID16(HDLC_GENERIC_ACCESS_APPEARANCE, HDLC_GENERIC_ACCESS_APPEARANCE_VALUE,
            UUID_CHARACTERISTIC_APPEARANCE, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE ),

    // Declare mandatory heart rate service. 
    PRIMARY_SERVICE_UUID16( HDLS_HEART_RATE, UUID_SERVICE_HEART_RATE ),

        // Declare mandatory heart rate service characteristic: Heart Rate Measurement
        CHARACTERISTIC_UUID16(HDLC_HEART_RATE_MEASUREMENT, HDLC_HEART_RATE_MEASUREMENT_VALUE,
            UUID_CHARACTERISTIC_HEART_RATE_MEASUREMENT, LEGATTDB_CHAR_PROP_NOTIFY,
            LEGATTDB_PERM_READABLE ),

        // Declare mandatory client characteristic configuration descriptor for Heart Rate Measurement. Setting value to 1 tells this application to send notification
        CHAR_DESCRIPTOR_UUID16_WRITABLE (HDLC_HEART_MEASUREMENT_CLIENT_CONFIG_DESCRIPTOR,
            UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION,
            LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_AUTH_WRITABLE | LEGATTDB_PERM_AUTH_READABLE),

        // Declare optional heart rate service characteristic: Sensor location
        CHARACTERISTIC_UUID16(HDLC_HEART_RATE_SENSOR_LOCATION, HDLC_HEART_RATE_SENSOR_LOCATION_VALUE,
            UUID_CHARACTERISTIC_HEART_RATE_SENSOR_LOCATION, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE ),

        // Declare conditional (mandatory of energy expended field included in hear rate notification) heart rate service characteristic: Heart Rate Control Point
        CHARACTERISTIC_UUID16_WRITABLE( HDLC_HEART_RATE_CONTROL_POINT, HDLC_HEART_RATE_CONTROL_POINT_VALUE,
            UUID_CHARACTERISTIC_HEART_RATE_CONTROL_POINT, LEGATTDB_CHAR_PROP_WRITE,
            LEGATTDB_PERM_READABLE|LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_AUTH_WRITABLE ),

    // Declare mandatory Device information service. 
    PRIMARY_SERVICE_UUID16( HDLS_DEVICE_INFORMATION, UUID_SERVICE_DEVICE_INFORMATION ),

        /* Characteristic 'Manufacturer Name String' */
        CHARACTERISTIC_UUID16 (HDLC_DEVICE_INFORMATION_MANUFACTURER_NAME_STRING, HDLC_DEVICE_INFORMATION_MANUFACTURER_NAME_STRING_VALUE,
            UUID_CHARACTERISTIC_MANUFACTURER_NAME_STRING, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE | LEGATTDB_PERM_AUTH_READABLE),

        /* Characteristic 'Model Number String' */
        CHARACTERISTIC_UUID16 (HDLC_DEVICE_INFORMATION_MODEL_NUMBER_STRING, HDLC_DEVICE_INFORMATION_MODEL_NUMBER_STRING_VALUE,
            UUID_CHARACTERISTIC_MODEL_NUMBER_STRING, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE),

        /* Characteristic 'Firmware Revision String' */
        CHARACTERISTIC_UUID16 (HDLC_DEVICE_INFORMATION_FIRMWARE_REVISION_STRING, HDLC_DEVICE_INFORMATION_FIRMWARE_REVISION_STRING_VALUE,
            UUID_CHARACTERISTIC_FIRMWARE_REVISION_STRING, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE),

        /* Characteristic 'Software Revision String' */
        CHARACTERISTIC_UUID16 (HDLC_DEVICE_INFORMATION_SOFTWARE_REVISION_STRING, HDLC_DEVICE_INFORMATION_SOFTWARE_REVISION_STRING_VALUE,
            UUID_CHARACTERISTIC_SOFTWARE_REVISION_STRING, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE),
};

// Length of the GATT database
const uint16_t  gatt_server_db_len                                  = sizeof(gatt_server_db);

const uint8_t   gatt_server_db_device_name[]                        = {'H','R','S'};
const uint16_t  gatt_server_db_device_name_len                      = sizeof(gatt_server_db_device_name);

const uint8_t   gatt_server_db_appearance[]                         = { BIT16_TO_8(APPEARANCE_GENERIC_TAG) };
const uint16_t  gatt_server_db_appearance_len                       = sizeof(gatt_server_db_appearance);

uint8_t         hrs_device_information_manufacturer_name_string[]  = {'C','y','p','r','e','s','s'};
uint8_t         hrs_device_information_model_number_string[]       = {'B','L','E','-','1','0','3'};
uint8_t         hrs_device_information_firmware_revision_string[]  = {'1','.','0','.','0'};
uint8_t         hrs_device_information_software_revision_string[]  = {'1','.','0','.','1'};

/* GATT attribute lookup table                                */
/* (attributes externally referenced by GATT server database) */
gatt_db_lookup_table_t hrs_gatt_db_ext_attr_tbl[] =
{
    /* { attribute handle,                                                   maxlen,                                                   curlen,                                                    attribute data } */
    {HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE,                                  4,                                                        4,                                                       (uint8_t *)gatt_server_db_device_name},
    {HDLC_GENERIC_ACCESS_APPEARANCE_VALUE,                                   2,                                                        2,                                                       (uint8_t *)gatt_server_db_appearance},
    {HDLC_DEVICE_INFORMATION_MANUFACTURER_NAME_STRING_VALUE,                 sizeof(hrs_device_information_manufacturer_name_string), sizeof(hrs_device_information_manufacturer_name_string), hrs_device_information_manufacturer_name_string},
    {HDLC_DEVICE_INFORMATION_MODEL_NUMBER_STRING_VALUE,                      sizeof(hrs_device_information_model_number_string),      sizeof(hrs_device_information_model_number_string),      hrs_device_information_model_number_string},
    {HDLC_DEVICE_INFORMATION_FIRMWARE_REVISION_STRING_VALUE,                 sizeof(hrs_device_information_firmware_revision_string), sizeof(hrs_device_information_firmware_revision_string), hrs_device_information_firmware_revision_string},
    {HDLC_DEVICE_INFORMATION_SOFTWARE_REVISION_STRING_VALUE,                 sizeof(hrs_device_information_software_revision_string), sizeof(hrs_device_information_software_revision_string), hrs_device_information_software_revision_string},
};

const uint16_t hrs_gatt_db_ext_attr_tbl_size = (sizeof(hrs_gatt_db_ext_attr_tbl) / sizeof (gatt_db_lookup_table_t));

