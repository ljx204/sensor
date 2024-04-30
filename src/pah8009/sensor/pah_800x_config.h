#ifndef __pah_800x_config_h__
#define __pah_800x_config_h__

// pah
#include "pah800x_bsp.h"

/*
 * Select LED Supplier
 * If Use new Supplier LED , Need to define LED_SUPPLIER_NEW
*/
#define LED_SUPPLIER_NEW


/*
 * Select Cover Type
*/
#define  DI_WALL_4mm
//#define  PC_4mm

/*
 * Select HW and ALG configuration
 * PC_4MM_2G_IR_HRD : Printing Cover with 4mm 2 Green and 1 IR LED for HRD algorithm use (PAH8009ES)
 * DIW_4MM_G_IR_HRD : DI+Wall Cover with 4mm 1 Green and 1 IR LED for HRD algorithm use (PAH8009ES)
 * DIW_4MM_IR_R_IR_SPO2 : DI+Wall Cover with 4mm 1 IR LED and 1 RED for SpO2 algorithm use (PAH8009ES)
 * This definition will enable the PPG register setting corresponding to different HW and ALG configuration
 */

#define PC_4MM_2G_IR_HRD    1
#define DIW_4MM_G_IR_HRD          1
#define DIW_4MM_IR_R_IR_SPO2    1

//#endif

//============================================================================
// Debug Option
//============================================================================
#define ENABLE_AE 1
#define ENABLE_AE_RECONVERGENCE 1


//#define PAH_800x_ENABLE_DEBUG_PRINT
#define PAH_800x_ENABLE_LOG_PRINT

#ifdef PAH_800x_ENABLE_LOG_PRINT
#define LOG_PRINT(...) pah_print(__LINE__, __func__, __FILENAME__, __VA_ARGS__)
#else
#define LOG_PRINT(...) do {} while (0)
#endif
#ifdef PAH_800x_ENABLE_DEBUG_PRINT
#define DEBUG_PRINT(...) pah_print(__LINE__, __func__, __FILENAME__, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif

/*
 * Select Interrupt or Polling Mode
 * This definition will enable the corresponding demo functionin main.c
 * If select INT mode, then slect following INT configuration type
 */
#define INTERRUPT_MODE      1
#define POLLING_MODE        0
#define Test_pattern_mode   0

/*
 * Select INT HW configuration type
 * INT_OD_EXTERNAL_PULLH : INT open-drain type with external pull-high resistor
 * INT_OD_INTERNAL_PULLH : INT open-drain type with internal pull-high resistor
 * INT_PUP_WITHOUT_PULLH : INT push-pull type without any pull-high resistor
 * If user select POLLING_MODE, all define need to set 0
 * This definition will enable the GPIO register setting corresponding to different INT HW configuration
 */
#define INT_OD_EXTERNAL_PULLH   1
#define INT_OD_INTERNAL_PULLH   0
#define INT_PUP_WITHOUT_PULLH   0


/*
* Configure VBUS Type
 * Define VBUS_18V=1, compatible with VBUS=1.8V or 3.3V (default)
 * Define VBUS_18V=0, only support VBUS=3.3V
 * If user want to define VBUS_18V=0, or VBUS not 1.8V/3.3V, please contact with PixArt for evaluation
 */
#define VBUS_18V    1
#if (VBUS_18V)
    #define R_GPIO_0_OD_EnH        (0x20)
    #define R_GPIO_SLEW            (0x01)
    #define R_I2C_SLEW             (0x40)
    #define R_SCL_Sel            (0x00)
    #define R_SDA_Sel            (0x00)
#else
    #define R_GPIO_0_OD_EnH        (0x20)
    #define R_GPIO_SLEW             0
    #define R_I2C_SLEW              0
    #define R_SCL_Sel            (0x40)
    #define R_SDA_Sel            (0x04)
#endif

#define Time_Interval             400
/*
If Customer platform use G-Sensor with FIFO,they can refer this flow to run pxi alg to get alg output
*/
//#define PPG_MEMS_Synchronize
#define Optimize_SPO2_flow

//****** Demo 800X mode Select ******//
//============HRD-INT================//
//#define touch_only_hrd_en
//#define ppg_only_hrd_en
#define auto_hrd_en

//============HRD-Polling================//
//#define touch_only_polling_hrd_en
//#define ppg_only_polling_hrd_en
//#define auto_polling_hrd_en


//============SPO2-INT================//
//#define touch_only_spo2_en
//#define ppg_only_spo2_en
//#define auto_spo2_en

//============SPO2-Polling================//
//#define touch_only_polling_spo2_en
//#define ppg_only_polling_spo2_en
//#define auto_polling_spo2_en


//============Stress-INT================//
//#define touch_only_stress_en
//#define ppg_only_stress_en
//#define auto_stress_en

//============Stress-Polling================//
//#define touch_only_polling_stress_en
//#define ppg_only_polling_stress_en
//#define auto_polling_stress_en

//=========== Test_pattern======== //
//#define hrd_testpattern_en
//#define spo2_testpattern_en

//=========== Factory _Test=========//
//#define factory_test_en


//============HRD-INT====PC Cover============//
//#define touch_only_hrd_en_pc
//#define ppg_only_hrd_en_pc
//#define auto_hrd_en_pc

//============HRD-Polling======PC Cover==========//
//#define touch_only_polling_hrd_en_pc
//#define ppg_only_polling_hrd_en_pc
//#define auto_polling_hrd_en_pc

#endif  // header guard
