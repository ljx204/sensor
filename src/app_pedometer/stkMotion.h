#ifndef __STKMOTION_H_
#define __STKMOTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#define STKMOTION_VERSION "S.0.3.7.2"

/*******************************************************************************
Macro definitions - Algorithm Build Options
********************************************************************************/

#define STK_PEDOMETER_ENABLE    1
#define STK_FALL_ENABLE         1
#define STK_SLEEP_ENABLE        1
#define STK_FLIP_ENABLE         0   //not implement
#define STK_RAISE_ENABLE        1
#define STK_SHAKE_ENABLE        1

#if STK_PEDOMETER_ENABLE
  #define STK_CALORIE_ENABLE      1
  #define STK_DISTANCE_ENABLE     1
  #define STK_ACTION_ENABLE       1
  #define STK_SEDENTARY_ENABLE    1
#endif

#define STK2x2_FIFO_WML_RANGE ((unsigned char)32)

/*******************************************************************************
Typedef definitions
********************************************************************************/
typedef enum {
  STK_EVENT_NONE,
  STK_EVENT_STEP_NOTIFY,
  STK_EVENT_FALL_NOTIFY,
  STK_EVENT_SLEEP_NOTIFY,
  STK_EVENT_ACTION_NOTIFY,
  STK_EVENT_SEDENTARY_NOTIFY,
  STK_EVENT_CALORIE_NOTIFY,
  STK_EVENT_DISTANCE_NOTIFY,
  STK_EVENT_FLIP_NOTIFY,
  STK_EVENT_RAISE_NOTIFY,
  STK_EVENT_SHAKE_NOTIFY
}STKMOTION_EVENT;

typedef enum {
  STK_NONE_X,
  STK_PEDOMETER_X,
  STK_FALL_X,
  STK_SLEEP_X,
  STK_ACTION_X,
  STK_SEDENTARY_X,
  STK_CALORIE_X, 
  STK_DISTANCE_X, 
  STK_FLIP_X,
  STK_RAISE_X,
  STK_SHAKE_X
}STK_Algorithm;

typedef enum {
  STK_DISABLE_X,
  STK_ENABLE_X
}STK_SwitchCmd;


typedef enum {
  STK_SLEEP_AWAKE,
  STK_SLEEP_INTO_SLEEP,
  STK_SLEEP_LIGHT,
  STK_SLEEP_DEEP
}STK_Tsleep;	//sleep state

typedef enum {
  STK_MOTIONLESS,
  STK_SLOW_WALK,
  STK_FAST_WALK,
  STK_RUN
}STK_Taction;	//action state

typedef enum {
  STK_RAISE_NONE = 0,
  STK_RAISE_ON   = 1,
  STK_RAISE_OFF  = -1
}STK_Traise;	//raise state

typedef enum {
  STK_SHAKE_NONE  = 0x00,
  STK_SHAKE_ON    = 1,
  STK_SHAKE_OFF   = -1,
}STK_Tshake;	//shake state


typedef enum{
  STK_DEBUG_ERR = 1,	
  STK_DEBUG_MSG = 1<<1,
  STK_DEBUG_DATA= 1<<2,
  STK_DEBUG_RAW = 1<<3,
}STK_TdebugMarker;

typedef enum{
  STK_INFO,
  STK_WARN,
  STK_DEBUG,
  STK_ERROR,    
}STK_TtraceLevel;



struct stkMotion_op_s {
  signed char 	(*event_send)(STKMOTION_EVENT event, signed int data_m);
  signed char 	(*stk2x2_register_read)(unsigned char dev_addr, unsigned char reg_addr, unsigned char *reg_data, unsigned char len);
  signed char 	(*stk2x2_register_write)(unsigned char dev_addr, unsigned char reg_addr, unsigned char *reg_data, unsigned char len);
  void		      (*stk2x2_mdelay)(unsigned int msec);
  signed char 	(*stk2x2_save_cali_file)(signed short x, signed short y,signed short z);
  signed char 	(*stk2x2_get_cali_file)(signed short *x, signed short *y,signed short *z);
  signed char 	(*stk2x2_printf)(char *fmt, ...); 
};  

struct stk2x2_time_t {
    unsigned char t_RTC_Year;
    unsigned char t_RTC_Month;
    unsigned char t_RTC_Day;
    unsigned char t_RTC_Hour;
    unsigned char t_RTC_Minute;
    unsigned char t_RTC_Second;
    unsigned int t_sys_milliSecond;
};

/*******************************************************************************
Global variables and functions
********************************************************************************/


/*******************************************************************************
* @brief
* This function initializes the stkMotion.
*
*	@param  *ops - the point to the stkMotion_op_s structure
*
*	@return results of stkMotion initialization
*	@retval 0 -> Success
*	@retval -1 -> Type Error
*	@retval -2 -> OP Error
*	@retval -3 -> Chip Error
********************************************************************************/
signed char stkMotion_Init(struct stkMotion_op_s *ops);

#if STK_PEDOMETER_ENABLE
/*******************************************************************************
* @brief
* This function clears pedometer value.
*
*	@param  None
*
*	@return None
********************************************************************************/
void stkMotion_Clear_Pedometer_Value(void);

/*******************************************************************************
* @brief
* This function set pedometer parameters.

*   @param  level - the input level of pedometer, default 10, range 1~30, level 30 is most sensitive
*	@param  window - the time window of pedometer, default 2, range 0~4, level 0 is most sensitive
*
*	@return None
********************************************************************************/
void stkMotion_Set_Pedometer_Params(unsigned char level, unsigned char window);
#endif

#if STK_SEDENTARY_ENABLE
/*******************************************************************************
* @brief
* This function sets sedentary parmas.
*
*	@param  time - the time to monitor sedentary, unit: minute. default is 30 mins(1~59)
* @param  steps - the target steps in the monitoring time interval, unit: steps. default is 30 steps
*
*	@return None
********************************************************************************/
void stkMotion_Set_Sedentary_Parma(unsigned char time, unsigned char steps);
#endif

#if STK_CALORIE_ENABLE
/*******************************************************************************
* @brief
* This function sets calorie parmas.
*
*	@param  height  - the user's height, default is 180 cm
* @param  weight  - the user's weight, default is 70 kg
*	@param  age     - the user's age, default is 30 years old
* @param  gender  - the user's gender, default is male.(female:0, male:1)
*
*	@return None
********************************************************************************/
void stkMotion_Set_Calorie_Parma(unsigned char height, unsigned char weight, unsigned char age, unsigned char gender);


/*******************************************************************************
* @brief
* This function clears calorie value.
*
*	@param  None
*
*	@return None
********************************************************************************/
void stkMotion_Clear_Step_Calorie_Distance_Value(void);
#endif

#if STK_RAISE_ENABLE
/*******************************************************************************
* @brief
* This function sets raise parmas.
*
*	@param   dir - direction of coordinate axes, default is 0 (0~7)
*	@param   sensitivity - the sensitivity to trigger raise motion, default is 3 (1~5)
*
*
*	@return None
********************************************************************************/
void stkMotion_Set_Raise_Parma(unsigned char dir, unsigned char sensitivity);
#endif

#if STK_SHAKE_ENABLE
/*******************************************************************************
* @brief
* This function sets shake parmas.
*
*	@param   level - the slope threshold of shake, unit: mg. default is 80mg
*
*	@return None
********************************************************************************/
void stkMotion_Set_Shake_Parma(unsigned char level);
#endif

#if STK_FALL_ENABLE
/*******************************************************************************
* @brief
* This function sets fall parmas.
*
*	@param   level - the sensitivity(1~10), default is 5, level 1 is most sensitive
*
*	@return None
********************************************************************************/
void stkMotion_Set_Fall_Parma(unsigned char level);
#endif



#if STK_SLEEP_ENABLE
/*******************************************************************************
* @brief
* This function sets sleep parmas.
*
*	@param   process_time - current time, include year, month, day, hour, minute, second, sys_millisecond
*	@param   sensitivity - the sensitive setting of sleep, range 1~10,  level 1 is most sensitive
*
*	@return None
********************************************************************************/
void stkMotion_Set_Sleep_Parma(struct stk2x2_time_t process_time, unsigned int sensitivity);
void stkMotion_Set_Sleep_Latency(unsigned int start, unsigned int end);
#endif


/*******************************************************************************
* Function Name: stkMotion_Control
* Description  : This function initializes the stkMotion.
* Arguments    : name - select which algorithm to control
                 cmd - enable/disable
* Return Value : None
********************************************************************************/
void stkMotion_Control(STK_Algorithm name, STK_SwitchCmd cmd);


#if ANDROID
/*******************************************************************************
* Function Name: stkMotion_Process_Data
* Description  : This function runs the stkMotion at 32Hz.
* Arguments    : pData: acceleration data
* Return Value : time interval of samples (sensor ODR)
********************************************************************************/
unsigned short stkMotion_Process_Data(AccData* pData);
#else
/*******************************************************************************
* Function Name: stkMotion_Process_Data
* Description  : This function runs the stkMotion at 32Hz.
* Arguments    : Time Stamp(struct stk2x2_time_t)
* Return Value : time interval of samples (sensor ODR)
********************************************************************************/
unsigned short stkMotion_Process_Data(struct stk2x2_time_t t_time);
#endif 
/*******************************************************************************
* Function Name: stkMotion_Get_Version
* Description  : This function gets stkMotion version.
* Arguments    : *ver - the point to char string of the version Num
* Return Value : None
********************************************************************************/
void stkMotion_Get_Version(char *ver);

/*******************************************************************************
* Function Name: stkMotion_QueryControl
* Description  : This function gets algorithm state.
* Arguments    : name - select which algorithm
* Return Value : SwitchCmd - enable/disable
********************************************************************************/
STK_SwitchCmd stkMotion_QueryControl(STK_Algorithm name);

/*******************************************************************************
* Function Name: stkMotion_QueryEvent
* Description  : This function gets algorithm value.
* Arguments    : event - select which event
* Return Value : Correspond event value
********************************************************************************/
signed int stkMotion_QueryEvent(STKMOTION_EVENT event);

/*******************************************************************************
* Function Name: stkMotion_Set_Debug_level
* Description  : This function sets the debug log level
* Arguments    : Log marker and level
* Return Value : None
********************************************************************************/
void stkMotion_Set_Debug_Marker_Level(unsigned int marker, unsigned int level);

/*******************************************************************************
* Function Name: stkMotion_chip_check_id
* Description  : This function checks the chip id.
* Arguments    : None
* Return Value : 0: OK; -1: Error
********************************************************************************/
signed char stkMotion_chip_check_id(void);

/*******************************************************************************
* Function Name: stkMotion_chip_read_xyz
* Description  : This function reads the chip xyz.
* Arguments    : x, y, z - acc data
* Return Value : 0: OK; -1: Error
********************************************************************************/
signed char stkMotion_chip_read_xyz(signed short *x,signed short *y,signed short *z);

/*******************************************************************************
* Function Name: stkMotion_chip_read_FIFO_xyz
* Description  : This function reads the chip FIFO xyz.
* Arguments    : x, y, z - acc data, fifoFrameSize - fifo frame size for reading out
* Return Value : None
********************************************************************************/
void stkMotion_chip_read_FIFO_xyz(signed short *x,signed short *y,signed short *z,unsigned char fifoFrameSize);


/*******************************************************************************
* Function Name: stkMotion_INT_Process_Data
* Description  : This function is used in conjunction with low power interrupt.
* Arguments    : None
* Return Value : None
********************************************************************************/
unsigned short stkMotion_INT_Process_Data(void);

/*******************************************************************************
* Function Name: stkMotion_selftest
* Description  : This function test the hardware
* Arguments    : allRegValue[127] - all register value of accelerometer,
*                average[3] - mean of 3-axis data output, 
*                noise[3] - stdev of 3-axis data output
* Return Value : 0: OK; -1: Error
********************************************************************************/
signed char stkMotion_selftest(unsigned char *allRegValue, float *average, float *noise);





#ifdef __cplusplus
}
#endif

#endif /* __STK832X_SENSOR_ADAPTOR_H_ */
