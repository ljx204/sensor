/*
    liujian define

		2024/4/24

*/

#include "app_i2c.h"
#include "stk832x.h"
#include "boards.h"

#include "app_util_platform.h"
#include "app_error.h"

#include "nrf_delay.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_drv_twi.h"


void stk832x_mdelay(unsigned int msec)
{
    nrf_delay_ms(msec);
}


//high precision mode 高精度模式
void stk832x_set_reg(void)
{
    stk832x_register_set(0x14, 0xB6); //software reset

    nrf_delay_ms(100);
    /* mode settings */
    uint8_t read_buf;
    stk832x_register_set(0x34, 0x02); // Enable I2C WatchDog  启用  I2C看门狗

    stk832x_register_get(0x34, &read_buf);

    //STK832x_register_set(0x11, 0x76); // high precision mode  高精度模式 0x00
    stk832x_register_set(0x11, 0x00); //0x7c

    stk832x_register_get(0x11, &read_buf);

    stk832x_register_set(0x0F, 0x01); // range = +-2g  0x3

    stk832x_register_get(0x0F, &read_buf);

    stk832x_register_set(0x10, 0x0B); // BW = 62.5Hz or ODR = 125Hz

    stk832x_register_get(0x10, &read_buf);
    /* INT mode */
    stk832x_register_set(0x20, 0x05); // INT1 and INT2 active high INT1和INT2有效高电平

    stk832x_register_get(0x20, &read_buf);

    stk832x_register_set(0x21, 0x0E); // temp-latch 50ms

    stk832x_register_get(0x21, &read_buf);

    /* slope settings */
    stk832x_register_set(0x16, 0x04); // enable slope on XYZ-axis     启用  XYZ轴上的坡度

    stk832x_register_get(0x16, &read_buf);

    stk832x_register_set(0x19, 0x04); // map any_motion(slop) to INT 1 将  ANY_MOTION( 映射任意运动(坡度) 斜率)映射到            INT 1

    stk832x_register_get(0x19, &read_buf);

    stk832x_register_set(0x1a, 0x02);

    stk832x_register_set(0x27, 0x01); // The actual number of samples SLP_DUR[1:0]+1 实际样本数     slp_dur[1：0]+1

    stk832x_register_get(0x27, &read_buf);

    //stk832x_register_set(0x28, 0x14); // SLP_THD[7:0]*3.91_mg at +-2g range    寄存器  0x  28 是門檻值，往上調力道要加大

    stk832x_register_set(0x28, 0x4);

    stk832x_register_get(0x28, &read_buf);

    stk832x_register_set(0x2A, 0x04); // Enable significant and any-motion interrupt

    stk832x_register_get(0x2A, &read_buf);


//		stk832x_register_set(0x13, 0x0);
//		stk832x_register_set(0x3A, 0x0f);
}


void stk832x_fifo_init(void)
{

    uint8_t read_buf;

    stk832x_register_get(STK832x_CHIPID, &read_buf);

    if(read_buf == STK832x_CHIP_ID_VAL)
    {
        //STK832x_register_set(STK832x_INTEN2,0x40);  // enable fifo watermark interrupt
        //STK832x_register_set(STK832x_INTMAP2,0x02);  //fifo interrupt mapping to INT1

        stk832x_register_set(STK832x_FIFOCFG1, ACCD_TOTAL_FIFO_LENGTH);

        stk832x_register_get(STK832x_FIFOCFG2, &read_buf); // read back fifo config
        read_buf = (read_buf & 0x0F) | (STREAM_MODE << 5); // set mode
        stk832x_register_set(STK832x_FIFOCFG2, read_buf); //write back

        /* set sleep timer to equidistant sampling mode for FIFO */
        stk832x_register_get(STK832x_POWER_MODE, &read_buf); // read back power mode
        read_buf = read_buf | 0x20; //rising ESM bit
        stk832x_register_set(STK832x_POWER_MODE, read_buf); //write back

        stk832x_mdelay(10);
    }
}





void stk832x_init(void)
{
    /* STK832x twi interface init */

  //  stk832x_twi_init();

    uint8_t read_buf = 0xff;

    uint8_t I2CData[5] = {0};

    I2CData[0] = 0x68;

    //I2C0_Write(I2C_ADDS,0xD0,&I2CData[0],1);

    stk832x_register_set(0xD0,  I2CData[0]);

    stk832x_register_get(STK832x_CHIPID, &read_buf);


    NRF_LOG_INFO("STK832x :: Chip ID = 0x%02x \r\n", read_buf);

    if(read_buf == STK832x_CHIP_ID_VAL)
    {
        NRF_LOG_INFO("STK832x :: Chip ID = %d \r\n", read_buf);
        stk832x_set_reg();
    }
    else
    {
        NRF_LOG_INFO("Error read stkchip id");
        return;
    }

    stk832x_fifo_init( );

#if 0
    char ret;
    ret = stkMotion_Init(&operators_handle);
    SEGGER_RTT_printf(0, "stkMotion_Init:%d\r\n", ret);

    if(0 == ret)
    {
        stkMotion_Control(STK_PEDOMETER_X, STK_DISABLE_X);
        stkMotion_Control(STK_RAISE_X, STK_DISABLE_X);
        stkMotion_Control(STK_FALL_X, STK_DISABLE_X);
        stkMotion_Control(STK_SLEEP_X, STK_DISABLE_X);

        stkMotion_Control(STK_ACTION_X, STK_DISABLE_X);
        stkMotion_Control(STK_SEDENTARY_X, STK_DISABLE_X);
        stkMotion_Control(STK_CALORIE_X, STK_DISABLE_X);
        stkMotion_Control(STK_DISTANCE_X, STK_DISABLE_X);
        stkMotion_Control(STK_FLIP_X, STK_DISABLE_X);
        stkMotion_Control(STK_SHAKE_X, STK_DISABLE_X);
    }

    /* initialize algorithm */
    stkMotion_Clear_Pedometer_Value(); // reset and enable pedometer
    stkMotion_Set_Pedometer_Params(10, 3); // sensitivity, default: 3 (range 0~4),  level 0 is most sensitive
    stkMotion_Clear_Step_Calorie_Distance_Value(); // reset calorie / distance data

    stkMotion_Set_Sedentary_Parma(3, 30); //monitor time: 59min (range 1~59min), step: 30
    stkMotion_Set_Calorie_Parma(180, 70, 30, 1); //height: 180cm, weight: 70kg, age: 30 years old, gender: male
    stkMotion_Set_Raise_Parma(6, 5); // dir is 6 (0~7), sensitivity is 5 (1~5) ( 5 is most sensitive)
    stkMotion_Set_Fall_Parma(3); // sensitivity is 5 (1~10), level 1 is most sensitive
//			stkMotion_Set_Sleep_Parma(nrfRTC, 5); // the sensitive setting of sleep, range 1~10,  level 1 is most sensitive
    stkMotion_Set_Shake_Parma(80); // the slope threshold of shake, unit: mg. default is 80mg (75 ~ 250mg)
#endif
#if 0

    if(read_buf == STK832x_CHIP_ID_VAL)
    {

        STK832x_register_set(STK832x_SOFTRESET, 0xB6); //soft-reset
        nrf_delay_ms(10);
        STK832x_register_set(STK832x_POWER_MODE, 0x80); // set suspend before initialized
        STK832x_register_set(STK832x_G_RANGE, 0x05); //range = +-4G
        STK832x_register_set(STK832x_BANDWIDTH, 0x1F);
        STK832x_register_set(STK832x_INTEN1, 0x00);
        STK832x_register_set(STK832x_INTEN2, 0x00);
        STK832x_register_set(STK832x_INTMAP1, 0x00);
        STK832x_register_set(STK832x_INTMAP2, 0x00);
        STK832x_register_set(STK832x_INTCFG1, 0x05); // push-pull, INT1/INT2 active high
        STK832x_register_set(STK832x_INTFCFG, 0x00);
        STK832x_register_set(STK832x_POWER_MODE, 0x56); //low-power mode, ODR~34Hz
        nrf_delay_ms(10);
    }

#endif
}

void stk832x_power(void)
{
    uint8_t read_buf;

    stk832x_register_set(0x2A, 0x00); // Enable significant and any-motion interrupt

    stk832x_register_get(0x2A, &read_buf);
}

void stk832x_on(void)
{
    uint8_t read_buf;

    uint8_t buf[3];

    stk832x_register_set(0x2A, 0x04);        // Enable significant and any-motion interrupt
    stk832x_register_get(0x2A, &read_buf);


    stk832x_registers_get(0x06, buf, 3);

    //	int16_t cc = (int16_t)((((int32_t)((int8_t)buf[2])) << 8) | (buf[1] & 0xF0)) >> 4;
}


static uint8_t  array_buf[ACCD_TOTAL_DATA_LENGTH * EACH_TIME_FIFO_FRAMES + 1];

int16_t   m_dataA[16];
int16_t   m_dataB[16];
int16_t   m_dataC[16];

void stk832x_get_fifo_data(int16_t *x_data_array, int16_t *y_data_array, int16_t *z_data_array, uint8_t *cnt)
{
//    uint8_t fifo_read_times;
//
//    uint8_t remained_frames;

//    uint16_t len = 12;

//    stk832x_register_get((STK832x_FIFOSTS), cnt);

//    *cnt = *cnt & 0x7F;

    //SEGGER_RTT_printf(0, "*cnt = %d\r\n", *cnt);

    // fifo_read_times = (*cnt) / EACH_TIME_FIFO_FRAMES;


    stk832x_registers_get(STK832x_FIFODATA, array_buf, 16 * ACCD_TOTAL_DATA_LENGTH);

    for(uint8_t i = 0; i < 16; i++)
    {
        m_dataA[i] = (int16_t)((((int16_t)array_buf[i * ACCD_TOTAL_DATA_LENGTH + 2]) << 8) | (array_buf[i * ACCD_TOTAL_DATA_LENGTH + 1] & 0xF0)) >> 4;
        m_dataB[i] = (int16_t)((((int16_t)array_buf[i * ACCD_TOTAL_DATA_LENGTH + 4]) << 8) | (array_buf[i * ACCD_TOTAL_DATA_LENGTH + 3] & 0xF0)) >> 4;
        m_dataC[i] = (int16_t)((((int16_t)array_buf[i * ACCD_TOTAL_DATA_LENGTH + 6]) << 8) | (array_buf[i * ACCD_TOTAL_DATA_LENGTH + 5] & 0xF0)) >> 4;
    }

}

static int16_t dataA;
static int16_t dataB;
static int16_t dataC;

void stk832x_get_realtime_data(int16_t *x_data, int16_t *y_data, int16_t *z_data)
{

    uint8_t array[ACCD_TOTAL_DATA_LENGTH];

    memset(array, 0, ACCD_TOTAL_DATA_LENGTH);


    stk832x_registers_get(STK832x_ACCD_X_LSB, &array[0], ACCD_TOTAL_DATA_LENGTH);

    dataA = *x_data = (int16_t)((((int32_t)((int8_t)array[1])) << 8) | (array[0] & 0xF0)) >> 4;
    dataB = *y_data = (int16_t)((((int32_t)((int8_t)array[3])) << 8) | (array[2] & 0xF0)) >> 4;
    dataC = *z_data = (int16_t)((((int32_t)((int8_t)array[5])) << 8) | (array[4] & 0xF0)) >> 4;

//	NRF_LOG_INFO("X :%d  Y:%d Z:%d ",dataA,dataB,dataC );
//	SEGGER_RTT_printf(0,"X :%d  Y:%d Z:%d\r\n",dataA, dataB, dataC);
}

