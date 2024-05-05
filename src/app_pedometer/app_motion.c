/*
    liujian

        2024/4/30

    ctd_liu@163.com
*/
#include "app_motion.h"
#include "stk832x.h"
#include "nrf_delay.h"
#include "app_i2c.h"

#include "app_scheduler.h"

#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

void STK832x_SetEnable(int en);

void STK832x_Initialization(void);

static uint32_t global_step;
#define G_INT1 0
#define G_INT2 5

unsigned int stk832x_getstep(void)
{
    return global_step;
}
signed char stk_register_read(unsigned char i2cAddr, unsigned char regAddr, unsigned char *regValue, unsigned char count)
{
    stk832x_registers_get(regAddr, regValue, count);
    return 1;
}

signed char stk_register_write(unsigned char i2cAddr, unsigned char regAddr, unsigned char *regValue, unsigned char count)
{
    unsigned char status = stk832x_registers_set(regAddr, regValue, count);
    return status;
}

void stk2x2_mdelay(unsigned int msec)
{
    nrf_delay_ms(msec);
}

static signed char sendEvent(STKMOTION_EVENT event, signed int data)
{
    //  NRF_LOG_INFO("event = %d data = %d \n", event, data);

    if (event == STK_EVENT_STEP_NOTIFY)
    {
      //  NRF_LOG_INFO("step = %d \n", data);
        global_step = data;
    }
    // SEGGER_RTT_printf(0,"stkmotion>> event: %d, data: %d", event, data);
    return 0;
}

signed char stk_printf(char *fmt, ...)
{
    return 0;
}

struct stkMotion_op_s operators_handle =
    {
        sendEvent,
        stk_register_read,
        stk_register_write,
        stk2x2_mdelay,
        NULL,
        NULL,
        stk_printf};

static void STK832x_stkMotion_irq_init(void);

void STK832x_stkMotion_init(void)
{
    char res;

    res = stkMotion_Init(&operators_handle);

    if (0 == res)
    {
        stkMotion_Control(STK_PEDOMETER_X, STK_ENABLE_X);

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

    // reset and enable pedometer ���ò����üƲ���
    stkMotion_Clear_Pedometer_Value();

    // level, default: 10 (1~50), window, default: 2 (range 0~4),
    // window 0 is most sensitive ��ƽ��Ĭ��ֵ��10(1~50)�����ڣ�Ĭ��ֵ��2(��Χ0~4)������0�������е�
    stkMotion_Set_Pedometer_Params(10, 2);

    // reset calorie / distance data ���ÿ�·��/��������
    stkMotion_Clear_Step_Calorie_Distance_Value();

    // monitor time: 59min (range 1~59min), step: 30 ���ʱ�䣺59min(��Χ1~59min)��������30
    stkMotion_Set_Sedentary_Parma(3, 30);

    // height: 180cm, weight: 70kg, age: 30 years old, gender: male ���ߣ�180���ף����أ�70������䣺30�꣬�Ա�����
    stkMotion_Set_Calorie_Parma(180, 70, 30, 1);

    // dir is 6 (0~7), sensitivity is 5 (1~5) ( 5 is most sensitive) DIRΪ6(0~7)��������Ϊ5(1~5)(5Ϊ������)
    stkMotion_Set_Raise_Parma(6, 5);

    // sensitivity is 5 (1~10), level 1 is most sensitive ������Ϊ5(1~10)������1������
    stkMotion_Set_Fall_Parma(3);

#ifdef stkMotion_TIMER_MODE
    stkMotion_Set_Sleep_Parma(STK832x_RTC, 5); // the sensitive setting of sleep, range 1~10, level 1 is most sensitive
#endif

    // the slope threshold of shake, unit: mg. default is 80mg (75 ~ 250mg)
    // ������б����ֵ����λ��mg��Ĭ��ֵΪ80����(75~250����)
    stkMotion_Set_Shake_Parma(80);

    //
    uint8_t chipid = 0xff;

    stk832x_register_get(STK832x_CHIPID, &chipid);

    NRF_LOG_INFO("stk832x_id = 0x%02X\n", chipid);

    if (chipid == STK832x_CHIP_ID_VAL)
    {
        STK832x_Initialization();
#if 0
				stk832x_register_set(STK832x_SOFTRESET,  0xB6);  //soft-reset
			
				nrf_delay_ms(10);
				
				stk832x_register_set(STK832x_POWER_MODE, 0x80);  // set suspend before initialized
				
				stk832x_register_set(STK832x_G_RANGE,    0x05);  //range = +-4G
				
				stk832x_register_set(STK832x_BANDWIDTH,  0x1F);
				
				stk832x_register_set(STK832x_INTEN1,     0x00);
				
				stk832x_register_set(STK832x_INTEN2,     0x00);
				
				stk832x_register_set(STK832x_INTMAP1,    0x00);
				
				stk832x_register_set(STK832x_INTMAP2,    0x00);
				
				stk832x_register_set(STK832x_INTCFG1,    0x05);  // push-pull, INT1/INT2 active high
				
				stk832x_register_set(STK832x_INTFCFG,    0x00);
				
				stk832x_register_set(STK832x_POWER_MODE, 0x56);  //low-power mode, ODR~34Hz
				
				nrf_delay_ms(10);
#endif

        STK832x_stkMotion_irq_init();

        STK832x_SetEnable(1);
    }
}

void STK832x_SetEnable(int en)
{
    unsigned char RegAddr, RegWriteValue;

    if (en)
    {
        RegAddr = 0x11; // set Register Adrress
        // RegWriteValue = 0x00;   //NORMAL
        RegWriteValue = 0x56; // Low_Power, sleep duration = 25ms
        stk832x_register_set(RegAddr, RegWriteValue);
    }
    else
    {
        RegAddr = 0x11;
        RegWriteValue = 0x80; // SUSPEND
        stk832x_register_set(RegAddr, RegWriteValue);
    }
}

void STK832x_Initialization()
{
    unsigned char RegAddr, RegWriteValue;
    //   twi_deivce_type_set(I2C_DEVICE_TYPE_GSENSOR);

    //---------------------
    // Reset
    //---------------------
    // Register 0x14?Reset Register
    // D7            D6          D5          D4          D3          D2          D1          D0
    //
    // Writing 0xB6 to this Register will reset IC
    //
    RegAddr = 0x14;
    RegWriteValue = 0xB6;
    stk832x_register_set(RegAddr, RegWriteValue);

    RegAddr = 0x11;
    RegWriteValue = 0x80; // SUSPEND
    stk832x_register_set(RegAddr, RegWriteValue);

    //---------------
    // Set Measure Range, Resolution
    //---------------
    // 0x0F: Range Selection (Read/Write)
    // D7             D6           D5           D4           D3          D2          D1          D0
    // Reserved       Reserved     Reserved     Reserved     Range<3>    Range<2>    Range<1>    Range<0>
    // *** RNG[3:0]     MEASUREMENT RANGE   ***
    //     0011           ?2 g
    //     0101           ?4 g
    //     1000           ?8 g
    //     1100           ?16 g

    RegAddr = 0x0F;
    RegWriteValue = 0x05; // range = 4g
    stk832x_register_set(RegAddr, RegWriteValue);

    //---------------
    // Output Data Rate Settings :
    //---------------
    // 0x10: Bandwidth
    // D7            D6          D5          D4      D3      D2      D1      D0
    // Reserved      Reserved    Reserved    bw<4>   bw<3>   bw<2>   bw<1>   bw<0>

    RegAddr = 0x10;
    RegWriteValue = 0x0a; // ODR=32.5Hz
    stk832x_register_set(RegAddr, RegWriteValue);

    //---------------
    // I2C WatchDog
    //---------------
    // 0x34: I2C WatchDog
    // D7        D6          D5          D4          D3          D2          D1              D0
    // Reserved  Reserved    Reserved    Reserved    Reserved    i2c_wdt_en  i2c_wdt_sel     spi3
    //
    // Enable I2C WatchDog
    //
    RegAddr = 0x34;
    RegWriteValue = 0x04;
    stk832x_register_set(RegAddr, RegWriteValue);

    //---------------
    // FIFO
    //---------------
    // 0x3D: FIFOCFG1
    // D7        D6          D5          D4          D3          D2          D1              D0
    // Reserved  ---------------------------------- FIFO_WM_LV[6:0] ----------------------------------
    //
    // set watermark as 32 frames
    //
    RegAddr = 0x3D;
    RegWriteValue = 0x20;
    stk832x_register_set(RegAddr, RegWriteValue);

    // 0x3E: FIFOCFG2
    // D7          D6           D5          D4          D3                D2              D1               D0
    //-------- FIFO_MODE[2:0] --------     reserved    ---- FIFO_INTERVAL[1:0] ----      ---- FIFO_DATA_SEL[1:0] ----
    //
    // set as stream mode, fifo interval = 1, 3 axis data stored in FIFO
    //
    RegAddr = 0x3E;
    RegWriteValue = 0xC0;
    stk832x_register_set(RegAddr, RegWriteValue);

    //---------------
    // Set Mode as interrupt and measurement operation mode
    //---------------
    // 0x19: INT Mapping
    // D7          D6        D5           D4           D3           D2           D1        D0
    // SIGMOT2INT2 STEP2INT2 ANYMOT2INT2  STEPOVR2INT2 STEPOVR2INT1 ANYMOT2INT1  STEP2INT1 SIGMOT2INT1
    //
    // Map sigmotion interrupt to INT1
    //
    RegAddr = 0x19;
    RegWriteValue = 0x01;
    stk832x_register_set(RegAddr, RegWriteValue);

    // 0x1A: INT Mapping
    // D7         D6        D5           D4           D3          D2              D1          D0
    // DATA2INT2  FWM2INT2  FFULL2INT2   reserved     reserved    FFULL2INT1      FWM2INT1    DATA2INT1
    //
    // Map watermark interrupt to INT2
    //
    RegAddr = 0x1A;
    RegWriteValue = 0x40;
    stk832x_register_set(RegAddr, RegWriteValue);

    // 0x21: INT Latch Setting
    // D7          D6        D5          D4          D3              D2              D1              D0
    // reset_int   Reserved  Reserved    Reserved    latch_int<3>    latch_int<2>    latch_int<1>    latch_int<0>
    //
    // Set INT pin as non-latched mode for new data interrupt, and 1ms latch for sigmotion
    //
    RegAddr = 0x21;
    RegWriteValue = 0x0B;
    stk832x_register_set(RegAddr, RegWriteValue);

    // 0x20: INT PP-OD, Active High/Low
    // D7         D6        D5           D4              D3          D2              D1              D0
    // Reserved   Reserved  Reserved     Reserved        int2_od     int2_|v|        int1_od         int1_|v|
    //
    // Set INT as Push-Pull, Active-High
    //
    RegAddr = 0x20;
    RegWriteValue = 0x05;
    stk832x_register_set(RegAddr, RegWriteValue);

    // 0x17: INT Setting
    // D7          D6        D5          D4          D3          D2          D1          D0
    // Reserved    FWM_EN    FFULL_EN    data_en     reserved    reserved    reserved    reserved
    //
    // Enable watermark interrupt.
    //
    RegAddr = 0x17;
    RegWriteValue = 0x40;
    stk832x_register_set(RegAddr, RegWriteValue);
}

static void STK832x_IRQ_Any_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{

    NRF_LOG_INFO("STK832x_IRQ_Any_handler");
}

// uint8_t array_buf[ACCD_TOTAL_DATA_LENGTH*EACH_TIME_FIFO_FRAMES+1];

void stk832_sensor_interrupt_process(void *p_event_data, uint16_t event_size)
{
    // stk832x_registers_get(STK832x_FIFODATA, array_buf, 16*ACCD_TOTAL_DATA_LENGTH);
    stkMotion_INT_Process_Data();
}

static void STK832x_IRQ_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{

  //  NRF_LOG_INFO("stk");
    app_sched_event_put(NULL, 0, stk832_sensor_interrupt_process);

    // stk832x_registers_get(STK832x_FIFODATA, array_buf, 16*ACCD_TOTAL_DATA_LENGTH);
}

static void STK832x_stkMotion_irq_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    in_config.sense = NRF_GPIOTE_POLARITY_TOGGLE;

    err_code = nrf_drv_gpiote_in_init(G_INT1, &in_config, STK832x_IRQ_Any_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(G_INT1, true);

    nrf_drv_gpiote_in_config_t in1_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in1_config.pull = NRF_GPIO_PIN_PULLDOWN;
    in1_config.sense = NRF_GPIOTE_POLARITY_TOGGLE;

    err_code = nrf_drv_gpiote_in_init(G_INT2, &in_config, STK832x_IRQ_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(G_INT2, true);
}
