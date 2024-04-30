/*
    liujian define
		
		2024/4/28

*/

#include "sdk_common.h"
#include "ble.h"
#include "ble_device_info.h"
#include "nrf_log.h"
#include "ble_srv_common.h"
#include "app_error.h"

static uint16_t                   service_handle;

static ble_gatts_char_handles_t   device_mac_handles;
static ble_gatts_char_handles_t   device_sn_handles;
static ble_gatts_char_handles_t   device_info_handles;

static uint32_t char_add(uint16_t                        uuid,
                         uint8_t                       * p_char_value,
                         uint16_t                        char_len,
                         security_req_t const            rd_sec,
                         ble_gatts_char_handles_t      * p_handles)
{
    ble_add_char_params_t add_char_params;

    APP_ERROR_CHECK_BOOL(p_char_value != NULL);
    APP_ERROR_CHECK_BOOL(char_len > 0);

    memset(&add_char_params, 0, sizeof(add_char_params));

    add_char_params.uuid            = uuid;
    add_char_params.max_len         = char_len;
    add_char_params.init_len        = char_len;
    add_char_params.p_init_value    = p_char_value;
    add_char_params.char_props.read = 1;
    add_char_params.read_access     = rd_sec;

    return characteristic_add(service_handle, &add_char_params, p_handles);
}


uint32_t ble_device_info_init(ble_device_info_init_t const * p_device_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

//    // Add service
//    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_DEVICE_INFORMATION_SERVICE);
	
	  ble_uuid.type = BLE_UUID_TYPE_BLE;
    ble_uuid.uuid = DEVICE_UUID_SERVICE;

  
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add characteristics
    if (p_device_init->device_mac.length > 0)
    {
        err_code = char_add(DEVICE_UUID_READ_MAC,
                            p_device_init->device_mac.p_str,
                            p_device_init->device_mac.length,
                            p_device_init->dis_char_rd_sec,
                            &device_mac_handles);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
		
		    // Add characteristics
    if (p_device_init->device_sn.length > 0)
    {
        err_code = char_add(DEVICE_UUID_READ_SN,
                            p_device_init->device_sn.p_str,
                            p_device_init->device_sn.length,
                            p_device_init->dis_char_rd_sec,
                            &device_sn_handles);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
		
				    // Add characteristics
    if (p_device_init->device_info.length > 0)
    {
        err_code = char_add(DEVICE_UUID_READ_INFO,
                            p_device_init->device_info.p_str,
                            p_device_init->device_info.length,
                            p_device_init->dis_char_rd_sec,
                            &device_info_handles);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
		
		
		
//    if (p_dis_init->model_num_str.length > 0)
//    {
//        err_code = char_add(BLE_UUID_MODEL_NUMBER_STRING_CHAR,
//                            p_dis_init->model_num_str.p_str,
//                            p_dis_init->model_num_str.length,
//                            p_dis_init->dis_char_rd_sec,
//                            &model_num_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }
//    if (p_dis_init->serial_num_str.length > 0)
//    {
//        err_code = char_add(BLE_UUID_SERIAL_NUMBER_STRING_CHAR,
//                            p_dis_init->serial_num_str.p_str,
//                            p_dis_init->serial_num_str.length,
//                            p_dis_init->dis_char_rd_sec,
//                            &serial_num_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }
//    if (p_dis_init->hw_rev_str.length > 0)
//    {
//        err_code = char_add(BLE_UUID_HARDWARE_REVISION_STRING_CHAR,
//                            p_dis_init->hw_rev_str.p_str,
//                            p_dis_init->hw_rev_str.length,
//                            p_dis_init->dis_char_rd_sec,
//                            &hw_rev_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }
//    if (p_dis_init->fw_rev_str.length > 0)
//    {
//        err_code = char_add(BLE_UUID_FIRMWARE_REVISION_STRING_CHAR,
//                            p_dis_init->fw_rev_str.p_str,
//                            p_dis_init->fw_rev_str.length,
//                            p_dis_init->dis_char_rd_sec,
//                            &fw_rev_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }
//    if (p_dis_init->sw_rev_str.length > 0)
//    {
//        err_code = char_add(BLE_UUID_SOFTWARE_REVISION_STRING_CHAR,
//                            p_dis_init->sw_rev_str.p_str,
//                            p_dis_init->sw_rev_str.length,
//                            p_dis_init->dis_char_rd_sec,
//                            &sw_rev_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }
//    if (p_dis_init->p_sys_id != NULL)
//    {
//        uint8_t encoded_sys_id[BLE_DIS_SYS_ID_LEN];

//        sys_id_encode(encoded_sys_id, p_dis_init->p_sys_id);
//        err_code = char_add(BLE_UUID_SYSTEM_ID_CHAR,
//                            encoded_sys_id,
//                            BLE_DIS_SYS_ID_LEN,
//                            p_dis_init->dis_char_rd_sec,
//                            &sys_id_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }
//    if (p_dis_init->p_reg_cert_data_list != NULL)
//    {
//        err_code = char_add(BLE_UUID_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_CHAR,
//                            p_dis_init->p_reg_cert_data_list->p_list,
//                            p_dis_init->p_reg_cert_data_list->list_len,
//                            p_dis_init->dis_char_rd_sec,
//                            &reg_cert_data_list_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }
//    if (p_dis_init->p_pnp_id != NULL)
//    {
//        uint8_t encoded_pnp_id[BLE_DIS_PNP_ID_LEN];

//        pnp_id_encode(encoded_pnp_id, p_dis_init->p_pnp_id);
//        err_code = char_add(BLE_UUID_PNP_ID_CHAR,
//                            encoded_pnp_id,
//                            BLE_DIS_PNP_ID_LEN,
//                            p_dis_init->dis_char_rd_sec,
//                            &pnp_id_handles);
//        if (err_code != NRF_SUCCESS)
//        {
//            return err_code;
//        }
//    }

    return NRF_SUCCESS;
}
