// c
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "app_scheduler.h"
// pah
#include "pah800x_main.h"
#include "pah800x_bsp.h"

#include "nrf_delay.h"
// pah factory test
//#include "pah800x_factory_test.h"

#include "app_pah8009.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#ifdef PPG_MEMS_Synchronize
extern uint8_t mems_data_polling_ready;
static uint8_t ppg_data_num=0;
#endif
//============================================================================
// Private function
//============================================================================
void handle_gpio_interrupt(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_status gpio_status);
void rtc_handler(nrf_drv_rtc_int_type_t int_type);


//============================================================================
// Public function
//============================================================================
#if (INTERRUPT_MODE==1)
//-----------------HRD--------------//
    #ifdef touch_only_hrd_en
        void demo_touch_mode_only_hrd(void);
    #endif

    #ifdef ppg_only_hrd_en
        void demo_ppg_mode_only_hrd(void);
    #endif

    #ifdef auto_hrd_en
        void demo_auto_mode_hrd(void);
    #endif

//-----------------SPO2--------------//
    #ifdef touch_only_spo2_en
        void demo_touch_mode_only_spo2(void);
    #endif

    #ifdef ppg_only_spo2_en
        void demo_ppg_mode_only_spo2(void);
    #endif

    #ifdef auto_spo2_en
        void demo_auto_mode_spo2(void);
    #endif

//-----------------Stress--------------//
    #ifdef touch_only_stress_en
        void demo_touch_mode_only_stress(void);
    #endif

    #ifdef ppg_only_stress_en
        void demo_ppg_mode_only_stress(void);
    #endif

    #ifdef auto_stress_en
        void demo_auto_mode_stress(void);
    #endif
//-----------------HRD--PC Cover------------//
    #ifdef touch_only_hrd_en_pc
            void demo_touch_mode_only_hrd_pc(void);
    #endif

    #ifdef ppg_only_hrd_en_pc
           void demo_ppg_mode_only_hrd(_pcvoid);
    #endif

    #ifdef auto_hrd_en_pc
           void demo_auto_mode_hrd_pc(void);
    #endif    
    

#endif


#if (POLLING_MODE==1)
//---------------------HRD----------------------//
    #ifdef touch_only_polling_hrd_en
        void demo_touch_mode_only_polling_hrd(void);
    #endif
    
    #ifdef ppg_only_polling_hrd_en
        void demo_ppg_mode_only_polling_hrd(void);
    #endif
    
    #ifdef auto_polling_hrd_en
        void demo_auto_mode_polling_hrd(void);
    #endif
    
//---------------------SPO2----------------------//
    #ifdef touch_only_polling_spo2_en
        void demo_touch_mode_only_polling_spo2(void);
    #endif
    
    #ifdef ppg_only_polling_spo2_en
        void demo_ppg_mode_only_polling_spo2(void);
    #endif
    
    #ifdef auto_polling_spo2_en
        void demo_auto_mode_polling_spo2(void);
    #endif

//---------------------Stress----------------------//
    #ifdef touch_only_polling_stress_en
        void demo_touch_mode_only_polling_stress(void);
    #endif
    
    #ifdef ppg_only_polling_stress_en
        void demo_ppg_mode_only_polling_stress(void);
    #endif
    
    #ifdef auto_polling_stress_en
        void demo_auto_mode_polling_stress(void);
    #endif
 //---------------------HRD--PC_Cover--------------------//
    #ifdef touch_only_polling_hrd_en_pc
           void demo_touch_mode_only_polling_hrd_pc(void);
    #endif
		
		#ifdef ppg_only_polling_hrd_en_pc
           void demo_ppg_mode_only_polling_hrd_pc(void);
    #endif
		
		#ifdef auto_polling_hrd_en_pc
           void demo_auto_mode_polling_hrd_pc(void);
    #endif   
    

#endif

#if (Test_pattern_mode==1)
//HRD
    #ifdef hrd_testpattern_en
        void test_library_HRD(void);
    #endif
//SPO2
    #ifdef spo2_testpattern_en
        void test_library_SPO2(void);
    #endif

#endif

int app_pah8009_init(void)
{
    // system
    pxi_nrf_system_clock_init();
#if (POLLING_MODE==1)
    pxi_nrf_rtc_init();
#endif
    
    // uart
 //   pxi_nrf_uart_init();
    
    // gpio
    pxi_nrf_gpio_in_init();
    pxi_nrf_gpio_in_set_interrupt_handler(handle_gpio_interrupt);
    pxi_nrf_gpio_in_pull(PXI_NRF_GPIO_IN_6, PXI_NRF_GPIO_PULL_TYPE_PULLUP);
    // i2c
 //  pxi_nrf_i2c_init();


#if  (INT_OD_EXTERNAL_PULLH==1)
    DEBUG_PRINT(" INT_OD_EXTERNAL_PULLH  \r\n");
#endif

#if (INT_OD_INTERNAL_PULLH==1)
    DEBUG_PRINT(" INT_OD_INTERNAL_PULLH  \r\n");
#endif

#if (INT_PUP_WITHOUT_PULLH==1)
    DEBUG_PRINT(" INT_PUP_WITHOUT_PULLH  \r\n");
#endif

/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_HRD
============================================================================*/
 
#ifdef touch_only_hrd_en 
    DEBUG_PRINT("Run_touch_only_hrd_en_mode\r\n");
    demo_touch_mode_only_hrd();   /* touch detect mode */
#endif

#ifdef ppg_only_hrd_en
    DEBUG_PRINT("Run_ppg_only_hrd_en_mode\r\n"); 
    demo_ppg_mode_only_hrd();    /* ppg_mode always run mode */
#endif

#ifdef auto_hrd_en
    DEBUG_PRINT("Run_auto_hrd_en_mode\r\n");
    demo_auto_mode_hrd();        /* Touch & PPG auto switch  mode */
#endif
/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_HRD
============================================================================*/
#ifdef touch_only_polling_hrd_en
    DEBUG_PRINT("Run_touch_only_polling_hrd_en_mode\r\n");
    demo_touch_mode_only_polling_hrd();  /* touch detect mode   -  polling */
#endif

#ifdef ppg_only_polling_hrd_en
    DEBUG_PRINT("Run_ppg_only_polling_hrd_en_mode\r\n");
    demo_ppg_mode_only_polling_hrd();   /* ppg_mode always run mode  -  polling */ 
#endif

#ifdef auto_polling_hrd_en
    DEBUG_PRINT("Run_auto_polling_hrd_en_mode\r\n");
    demo_auto_mode_polling_hrd();      /* Touch & PPG Auto switch  mode -  polling */
#endif

/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_SPO2
============================================================================*/
#ifdef touch_only_spo2_en
    DEBUG_PRINT("Run_touch_only_spo2_en_mode\r\n");
    demo_touch_mode_only_spo2();  /* touch detect mode SPO2*/
#endif

#ifdef ppg_only_spo2_en
    DEBUG_PRINT("Run_ppg_only_spo2_en_mode\r\n");
    demo_ppg_mode_only_spo2();   /* ppg_mode always run mode SPO2 */
#endif

#ifdef auto_spo2_en
    DEBUG_PRINT("Run_auto_spo2_en_mode\r\n");
    demo_auto_mode_spo2();      /* Touch & PPG auto switch  mode SPO2 */
#endif

/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_SPO2
============================================================================*/
#ifdef touch_only_polling_spo2_en
    DEBUG_PRINT("Run_touch_only_polling_spo2_en_mode\r\n");
    demo_touch_mode_only_polling_spo2();  /* touch detect mode   -  polling */
#endif
 
#ifdef ppg_only_polling_spo2_en
    DEBUG_PRINT("Run_ppg_only_polling_spo2_en_mode\r\n");
    demo_ppg_mode_only_polling_spo2();   /* ppg_mode always run mode  -  polling */ 
#endif
 
#ifdef auto_polling_spo2_en
    DEBUG_PRINT("Run_auto_polling_spo2_en_mode\r\n");
    demo_auto_mode_polling_spo2();      /* Touch & PPG Auto switch  mode -  polling */
#endif

/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_Stress
============================================================================*/
#ifdef touch_only_stress_en
    DEBUG_PRINT("Run_touch_only_sstress_en_mode\r\n");
    demo_touch_mode_only_stress();  /* touch detect mode SPO2*/
#endif

#ifdef ppg_only_stress_en
    DEBUG_PRINT("Run_ppg_only_stress_en_mode\r\n");
    demo_ppg_mode_only_stress();   /* ppg_mode always run mode SPO2 */
#endif

#ifdef auto_stress_en
    DEBUG_PRINT("Run_auto_stress_en_mode\r\n");
    demo_auto_mode_stress();      /* Touch & PPG auto switch  mode SPO2 */
#endif

/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_Stress
============================================================================*/
#ifdef touch_only_polling_stress_en
    DEBUG_PRINT("Run_touch_only_polling_stress_en_mode\r\n");
    demo_touch_mode_only_polling_stress();  /* touch detect mode   -  polling */
#endif
 
#ifdef ppg_only_polling_stress_en
    DEBUG_PRINT("Run_ppg_only_polling_stress_en_mode\r\n");
    demo_ppg_mode_only_polling_stress();   /* ppg_mode always run mode  -  polling */ 
#endif
 

#ifdef auto_polling_stress_en
    DEBUG_PRINT("Run_auto_polling_stress_en_mode\r\n");
    demo_auto_mode_polling_stress();      /* Touch & PPG Auto switch  mode -  polling */
#endif

/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_HRD pc cover
============================================================================*/
	 
    #ifdef touch_only_hrd_en_pc 
        DEBUG_PRINT("Run_touch_only_hrd_en_mode_pc\r\n");
        demo_touch_mode_only_hrd_pc();   /* touch detect mode */
    #endif

    #ifdef ppg_only_hrd_en_pc
        DEBUG_PRINT("Run_ppg_only_hrd_en_mode_pc\r\n"); 
        demo_ppg_mode_only_hrd_pc();    /* ppg_mode always run mode */
    #endif

    #ifdef auto_hrd_en_pc
        DEBUG_PRINT("Run_auto_hrd_en_mode_pc\r\n");
        demo_auto_mode_hrd_pc();        /* Touch & PPG auto switch  mode */
    #endif
/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_HRD pc cover
============================================================================*/
#ifdef touch_only_polling_hrd_en_pc
    DEBUG_PRINT("Run_touch_only_polling_hrd_en_mode_pc\r\n");
    demo_touch_mode_only_polling_hrd_pc();  /* touch detect mode   -  polling */
#endif

#ifdef ppg_only_polling_hrd_en_pc
    DEBUG_PRINT("Run_ppg_only_polling_hrd_en_mode_pc\r\n");
    demo_ppg_mode_only_polling_hrd_pc();   /* ppg_mode always run mode  -  polling */ 
#endif

#ifdef auto_polling_hrd_en_pc
    DEBUG_PRINT("Run_auto_polling_hrd_en_mode_pc\r\n");
    demo_auto_mode_polling_hrd_pc();      /* Touch & PPG Auto switch  mode -  polling */
#endif

/*============================================================================
Test_pattern mode
============================================================================*/
 /* HRD*/
#ifdef hrd_testpattern_en
    test_library_HRD();
#endif

/* SPO2*/
#ifdef  spo2_testpattern_en
    test_library_SPO2();
#endif

/*============================================================================
Factory_Test Function
============================================================================*/
#ifdef factory_test_en
    //factory_test_mode(factory_test_brightness_golden_sample, 1, 100, 100, 40);
    //factory_test_mode(factory_test_brightness_target_sample, 1, 100, 100, 40);
    factory_test_mode(factory_test_leak_light_target_sample, 0, 0, 0, 0);
    //factory_test_mode(factory_test_power_noise, 0, 0, 0, 0);
    //factory_test_mode(factory_test_power_noise, 0, 0, 0, 0);
    
#endif
}


//============================================================================
// Public function
//============================================================================
#if (INTERRUPT_MODE==1)
//*********** HRD  mode define****************//
#ifdef touch_only_hrd_en
void demo_touch_mode_only_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();
    while (true)
    {
        ppg_sensor_task_interrupt();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();

    }
}
#endif

#ifdef ppg_only_hrd_en
void demo_ppg_mode_only_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    hr_algorithm_enable();
    while (true)
    {
        ppg_sensor_task_interrupt();
        
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif

   

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        /**Please call hr_algorithm_disable() to disable hr algorithm     
         */
        //hr_algorithm_disable();

    }
}
#endif

#ifdef auto_hrd_en
void demo_auto_mode_hrd(void)
{
  pah_sensor_init(diw_4mm_g_ir_hrd);
	
//	pah_sensor_init(pc_4mm_2g_ir_hrd);
	NRF_LOG_PROCESS();
    auto_mode_sensor_start();
	NRF_LOG_PROCESS();

    while (true)
    {
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif
       /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
        * This Function will disable the ppg sensor
        */
       //pah_sensor_stop();
           /**Please call hr_algorithm_disable() to disable hr algorithm     
        */
        //hr_algorithm_disable();
         NRF_LOG_PROCESS();
    }
}
#endif


//*********** SPO2  mode define****************//
#ifdef touch_only_spo2_en
void demo_touch_mode_only_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    touch_only_sensor_start();
    while (true)
    {
        ppg_sensor_task_interrupt();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();

    }
}
#endif

#ifdef ppg_only_spo2_en
void demo_ppg_mode_only_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    ppg_only_sensor_start();
    spo2_algorithm_enable();
    while (true)
    {
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif

               


        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        /**Please call spo2_algorithm_disable() to disable spo2 algorithm     
         */
        //spo2_algorithm_disable();

    }
}
#endif

#ifdef auto_spo2_en
void demo_auto_mode_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
  auto_mode_sensor_start();
    while (true)
    {
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call spo2_algorithm_disable() to disable spo2 algorithm     
         */
        //spo2_algorithm_disable();

    }
}
#endif



//*********** Stress  mode define****************//
#ifdef touch_only_stress_en
void demo_touch_mode_only_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();
    while (true)
    {
        ppg_sensor_task_interrupt();
        
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();

    }
}
#endif

#ifdef ppg_only_stress_en
void demo_ppg_mode_only_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    stress_algorithm_enable();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;
    
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score );
    
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }
    

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        /**Please call stress_algorithm_disable() to disable stress algorithm     
         */
        //stress_algorithm_disable();

    }
}
#endif

#ifdef auto_stress_en
void demo_auto_mode_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    auto_mode_sensor_start();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score);
     
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call stress_algorithm_disable() to disable stress algorithm     
         */
        //stress_algorithm_disable();

    }
}
#endif

//*********** HRD  mode define***PC COVER*************//
#ifdef touch_only_hrd_en_pc
void demo_touch_mode_only_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    touch_only_sensor_start();
    while (true)
    {
        ppg_sensor_task_interrupt();
        //hr_alg_task();

		/**if customer want to stop PPG Sensor , please call pah_sensor_stop()
	     * This Function will disable the ppg sensor
		 */
		//pah_sensor_stop();

    }
}
#endif

#ifdef ppg_only_hrd_en_pc
void demo_ppg_mode_only_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
	ppg_only_sensor_start();
    hr_algorithm_enable();
    while (true)
    {
        ppg_sensor_task_interrupt();
        hr_alg_task();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
        * This Function will disable the ppg sensor
        */
        //pah_sensor_stop();
        /**Please call hr_algorithm_disable() to disable hr algorithm	     
        */
        //hr_algorithm_disable();

    }
}
#endif

#ifdef auto_hrd_en_pc
void demo_auto_mode_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    auto_mode_sensor_start();
    while (true)
    {
        ppg_sensor_task_interrupt();
        hr_alg_task();
		/**if customer want to stop PPG Sensor , please call pah_sensor_stop()
	     * This Function will disable the ppg sensor
		 */
		//pah_sensor_stop();
        /**Please call hr_algorithm_disable() to disable hr algorithm	     
		 */
        //hr_algorithm_disable();

    }
}
#endif
//*********** Stress  mode define****************//
#ifdef touch_only_stress_en_pc
void demo_touch_mode_only_stress_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
	touch_only_sensor_start();
    while (true)
    {
				ppg_sensor_task_interrupt();
				//hr_alg_task();

		/**if customer want to stop PPG Sensor , please call pah_sensor_stop()
	     * This Function will disable the ppg sensor
		 */
		//pah_sensor_stop();

    }
}
#endif

#ifdef ppg_only_stress_en_pc
void demo_ppg_mode_only_stress_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    ppg_only_sensor_start();
    stress_algorithm_enable();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;
    
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score );
    
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }
    

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        /**Please call stress_algorithm_disable() to disable stress algorithm     
         */
        //stress_algorithm_disable();

    }
}
#endif

#ifdef auto_stress_en_pc
void demo_auto_mode_stress_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    auto_mode_sensor_start();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score);
     
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call stress_algorithm_disable() to disable stress algorithm     
         */
        //stress_algorithm_disable();

    }
}
#endif

//static void gpio_in_interrupt_handler(GPIO_IN gpio)
//{
//    if (gpio == GPIO_IN_1)
//    {
//        ppg_interrupt_handle();
//    ppg_sensor_task_interrupt();
//    
//    }
//}
#endif
void handle_gpio_interrupt(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_status gpio_status)
{
#ifdef INTERRUPT_MODE
    if (gpio_in == PXI_NRF_GPIO_IN_6
        && gpio_status == PXI_NRF_GPIO_STATUS_LOW)
    {
        ppg_interrupt_handle();
        //ppg_sensor_task_interrupt();
    }
#endif

}


#if (POLLING_MODE==1)
#ifdef touch_only_polling_hrd_en
void demo_touch_mode_only_polling_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();
    while (true)
    {

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        
    }
}
#endif

#ifdef ppg_only_polling_hrd_en
void demo_ppg_mode_only_polling_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    hr_algorithm_enable();
    while (true)
    {
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call hr_algorithm_disable() to disable hr algorithm     
         */
        //hr_algorithm_disable();

    }
}
#endif


#ifdef auto_polling_hrd_en
void demo_auto_mode_polling_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    auto_mode_sensor_start();
    while (true)
    {
 #ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call hr_algorithm_disable() to disable hr algorithm
             * This Function will disable the hr algorithm
        */
        //hr_algorithm_disable();

    }
}
#endif

#ifdef touch_only_polling_spo2_en
void demo_touch_mode_only_polling_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    touch_only_sensor_start();
    while (true)
    {

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        
    }
}
#endif

#ifdef ppg_only_polling_spo2_en
void demo_ppg_mode_only_polling_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    ppg_only_sensor_start();
    spo2_algorithm_enable();
    while (true)
    {
#ifdef PPG_MEMS_Synchronize
        /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
         */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }
#else
        spo2_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call spo2_algorithm_disable() to disable spo2 algorithm     
         */
        //spo2_algorithm_disable();

    }
}
#endif

#ifdef auto_polling_spo2_en
void demo_auto_mode_polling_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    auto_mode_sensor_start();
    while (true)
    {
#ifdef PPG_MEMS_Synchronize
        /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
         */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(mems_int_data_in,mems_num,ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call spo2_algorithm_disable() to disable spo2 algorithm
             * This Function will disable the spo2 algorithm
         */
        //spo2_algorithm_disable();

    }
}
#endif



#ifdef touch_only_polling_stress_en
void demo_touch_mode_only_polling_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();
    while (true)
    {

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        
    }
}
#endif

#ifdef ppg_only_polling_stress_en
void demo_ppg_mode_only_polling_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    stress_algorithm_enable();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    uint8_t stress_alg_enable = 1 ;
    int score = 100 ;
    while (true)
    {
      
        stress_alg_task(score);
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
        /**Please call stress_algorithm_disable() to disable stress algorithm     
         */
        //stress_algorithm_disable();

    }
}
#endif


#ifdef auto_polling_stress_en
void demo_auto_mode_polling_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    auto_mode_sensor_start();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    uint8_t stress_alg_enable = 1 ;
    int score = 100 ;
    while (true)
    {
        stress_alg_task(score);
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         */
        //pah_sensor_stop();
            /**Please call stress_algorithm_disable() to disable stress algorithm
             * This Function will disable the stress algorithm
         */
        //stress_algorithm_disable();

    }
}
#endif

#ifdef touch_only_polling_hrd_en_pc
void demo_touch_mode_only_polling_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
		touch_only_sensor_start();
    while (true)
    {

		/**if customer want to stop PPG Sensor , please call pah_sensor_stop()
	     * This Function will disable the ppg sensor
		 */
		//pah_sensor_stop();
        
    }
}
#endif

#ifdef ppg_only_polling_hrd_en_pc
void demo_ppg_mode_only_polling_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
//		ppg_only_sensor_start();
    hr_algorithm_enable();
    while (true)
    {
				hr_alg_task();

		/**if customer want to stop PPG Sensor , please call pah_sensor_stop()
	     * This Function will disable the ppg sensor
		 */
		//pah_sensor_stop();
        /**Please call hr_algorithm_disable() to disable hr algorithm	     
		 */
        //hr_algorithm_disable();

    }
}
#endif


#ifdef auto_polling_hrd_en_pc
void demo_auto_mode_polling_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
		auto_mode_sensor_start();
    while (true)
    {
				hr_alg_task();

		/**if customer want to stop PPG Sensor , please call pah_sensor_stop()
	     * This Function will disable the ppg sensor
		 */
		//pah_sensor_stop();
        /**Please call hr_algorithm_disable() to disable hr algorithm
         * This Function will disable the hr algorithm
		 */
        //hr_algorithm_disable();

    }
}
#endif



#endif

void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
#ifdef POLLING_MODE
    if (int_type == NRF_DRV_RTC_INT_TICK)
    {
        ppg_sensor_task_polling();
    }
#endif
}

//////////////////////////////////////////////////////////////////
#include "app_timer.h"

APP_TIMER_DEF(ppg_process_timer);

extern void ppg_sensor_interrupt_process(void *p_event_data, uint16_t event_size);

void ppg_handle_gpio_interrupt(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_status gpio_status)
{

    if (gpio_in == PXI_NRF_GPIO_IN_6&& gpio_status == PXI_NRF_GPIO_STATUS_LOW)
    {
#if PPG_SCHEDULER_ENABLE	
         app_sched_event_put(NULL, 0,  ppg_sensor_interrupt_process);
#else			
          ppg_interrupt_handle();
#endif			
    }

}





void ppg_loop(void)
{
	   ppg_sensor_task_interrupt();
	   
	   hr_alg_task();
}

void app_pah8009_process(void)
{
	   ret_code_t err_code;
	
	   int ret;
	
	   pxi_nrf_system_clock_init();

     pxi_nrf_gpio_in_init();
     pxi_nrf_gpio_in_set_interrupt_handler(ppg_handle_gpio_interrupt);
     pxi_nrf_gpio_in_pull(PXI_NRF_GPIO_IN_6, PXI_NRF_GPIO_PULL_TYPE_PULLUP);
	
	   ret = pah_sensor_init(diw_4mm_g_ir_hrd);
	
	   if(!ret)
		 {
			   return;
		 }
	
     auto_mode_sensor_start();
		 
		// touch_only_sensor_start();
	
//	   err_code = app_timer_create(&ppg_process_timer, APP_TIMER_MODE_SINGLE_SHOT, ppg_algorithm_func);
//	
//     APP_ERROR_CHECK(err_code);
	
   //  err_code = app_timer_start(ppg_process_timer, APP_TIMER_TICKS(20), NULL);
	  
}


void pah_write_data(unsigned char reg, const unsigned char data)
{
	 pah_comm_i2c_write(reg, &data, 1);
}


/*
  led_type = 0  //infrared
  led_type = 1  //green
  led_type = 2;  //red
*/
void pah8009_set_led(LED_TYPE led_type)
{

	  pah_write_data(0xE8, 0x00);
	  nrf_delay_ms(20);
	  pah_write_data(0xEE, 0x1f);
	  nrf_delay_ms(20);
	  pah_write_data(0x03, 0x40);
	  pah_write_data(0x04, 0x01);
	  pah_write_data(0x05, 0x00);
	  pah_write_data(0x11, 0x11);
	
	  if(led_type == LED_INFRARED)
		{
	     pah_write_data(0x1d, 0x1f);
		}
		else if(led_type == LED_GREEN)
		{
			 pah_write_data(0x1d, 0x2f);
		}
		else if(led_type == LED_RED)
		{
			  pah_write_data(0x1d, 0x4f);
		}
	
	  
	  pah_write_data(0xEB, 0x80);
	  nrf_delay_ms(20);
	  pah_write_data(0xE8, 0x00);
	  nrf_delay_ms(20);
	  pah_write_data(0x07, 0x01);

}

void pah8009_led_off(void)
{
	
	   pah_write_data(0xE8, 0x00);
	   pah_write_data(0xEE, 0x1F);
}


