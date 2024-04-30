/*
    liujian define

		2024/4/24

*/

#ifndef __STK832X_H__
#define __STK832X_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

#define ACCD_TOTAL_DATA_LENGTH		6
#define ACCD_TOTAL_FIFO_LENGTH		32
#define EACH_TIME_FIFO_FRAMES		  32//10

                                  // nordic ¿ª·¢°å PCAL6408A 
#define STK832x_ADDR      	    	0x0F  //0x0F        /* SA0 pull-low, 7-bit slave address */

//#define STK832x_ADDR        		0x1F        /* SA0 pull-high, 7-bit slave address */

#define STK832x_CHIPID		  		0x00

#define STK832x_ACCD_X_LSB			0x02
#define STK832x_ACCD_X_MSB			0x03
#define STK832x_ACCD_Y_LSB			0x04
#define STK832x_ACCD_Y_MSB			0x05
#define STK832x_ACCD_Z_LSB			0x06
#define STK832x_ACCD_Z_MSB			0x07


#define	STK832x_FIFOSTS			  	0x0C
#define STK832x_G_RANGE			  	0x0F	/* Range Select */
#define STK832x_BANDWIDTH			  0x10    /* Bandwidth Select */
#define STK832x_POWER_MODE			0x11

#define STK832x_SOFTRESET	  		0x14
#define STK832x_INTEN1      		0x16
#define STK832x_INTEN2      		0x17

#define STK832x_INTMAP1     		0x19
#define STK832x_INTMAP2     		0x1A

#define STK832x_INTCFG1     		0x20
#define STK832x_INTCFG2     		0x21
#define STK832x_SLOPEDLY		  	0x27
#define STK832x_SLOPETHD    		0x28
#define STK832x_INTFCFG     		0x34

#define STK832x_FIFOCFG1    		0x3D
#define STK832x_FIFOCFG2    		0x3E
#define STK832x_FIFODATA    		0x3F


    /* ************************************** */
    /* Register Data Information              */
    /* ************************************** */
#define STK832x_CHIP_ID_VAL			  0x26    /* STK8327 CHIP ID = 0x26 */
//#define STK832x_CHIP_ID_VAL			0x25    /* STK8325 CHIP ID = 0x25 */

#define BYPASS_MODE             0x00
#define FIFO_MODE               0x01
#define STREAM_TO_FIFO_MODE     0x03
#define BYPASS_TO_STREAM_MODE   0x04
#define STREAM_MODE             0x06

    void stk832x_init(void);

    void stk832x_get_realtime_data(int16_t *x_data, int16_t *y_data, int16_t *z_data);

    void stk832x_fifo_init(void);

    void stk832x_get_fifo_data(int16_t *x_data_array, int16_t *y_data_array, int16_t *z_data_array, uint8_t* cnt);




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

