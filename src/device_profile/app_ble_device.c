
#include "app_ble_device.h"

#include "app_error.h"

#include "nrf_log.h"

#define MANUFACTURER_NAME               "112233445566"


void app_device_get_mac(char *name)
{
	 ret_code_t              err_code;
	
	  ble_gap_addr_t   addr;
   // char name[BLE_GAP_DEVNAME_MAX_LEN];
    uint8_t tmp;
	
    err_code = sd_ble_gap_addr_get(&addr);
	
//    memset(name, 0, sizeof(name));
//	
//	  strcpy(name, DEVICE_NAME); 
//	  strcat(name, "_");   
	
	  tmp = (addr.addr[5] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1);
	
	  tmp = addr.addr[5] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
	
	
	  tmp = (addr.addr[4] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1);
	
	  tmp = addr.addr[4] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
		
		tmp = (addr.addr[3] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1);
	
	  tmp = addr.addr[3] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
		
		tmp = (addr.addr[2] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1);
	
	  tmp = addr.addr[2] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
	
	  
	  tmp = (addr.addr[1] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1); 
    
		tmp = addr.addr[1] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
    
		tmp = (addr.addr[0]>>4);   
		tmp = (tmp >= 10)?(tmp+0x37):(tmp+0x30);   
		strncat(name, (char *)&tmp, 1);     
		
		tmp = addr.addr[0] % 0x10;
		tmp = tmp >=10 ? (tmp + 0x37):(tmp + 0x30);    
		strncat(name, (char *)&tmp, 1); 
		
}

char device_id[20] =   { 0x01, 0x02,0x03,0x04,0x05,0x22,0x01,0x01,0x01,0x01, \
                         0x44, 0x44,0x44,0x44,0x44,0x33,0x44,0x44,0x44,0x44};

char device_info[20] = { 0x01, 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01, \
                         0x02, 0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02};

void app_device_info_service_init(void)
	
{
	   ret_code_t          err_code;
	
     ble_device_info_init_t   dis_init_obj;
	
     char mac_name[6];
	   
	   memset(mac_name, 0, sizeof(mac_name));
	
     //app_device_get_mac(mac_name);
	   ble_gap_addr_t   addr;
	
     err_code = sd_ble_gap_addr_get(&addr);
	
	   memcpy(mac_name, addr.addr, 6);
     memset(&dis_init_obj, 0, sizeof(dis_init_obj));
	
     ble_srv_ascii_to_utf8(&dis_init_obj.device_mac, mac_name);
	
	   ble_srv_ascii_to_utf8(&dis_init_obj.device_sn, device_id);
	
	   ble_srv_ascii_to_utf8(&dis_init_obj.device_info, device_info);
	
     dis_init_obj.dis_char_rd_sec = SEC_OPEN;
	
	

     err_code = ble_device_info_init(&dis_init_obj);
     APP_ERROR_CHECK(err_code);
		
}
