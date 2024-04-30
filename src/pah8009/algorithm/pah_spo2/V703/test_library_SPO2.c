// algorithm
#include "pxialg_spo2.h"
// HR algorithm
#include "pxialg.h"
// testpattern
#include "pah8009_testpattern_SPO2.h"
// platform
#include <stdlib.h>

#include "pah800x_main.h"
//#include "system_clock.h"
//#include "i2c.h"
//#include "gpio_ctrl.h"
//#include "pah_comm.h"
#include <math.h>  
#include "nrf_delay.h"
#define array_szie(arr) (sizeof(arr) / sizeof(arr[0]))

#define SAMPLES_PER_CHANNEL 25
#define TOTAL_CHANNEL 3

//#define Open_Rest_Flow

static SpO2_mems_t  mems_data_raw[150];        //add item

typedef struct
{
    void *pBuffer;
    void *hr_pBuffer;
} pxialg_spo2_state;

//#define parameter_printf(...) NRF_LOG_RAW_INFO(__VA_ARGS__)

extern void spo2_log_printf(char s, float f);
//static void spo2_log_printf(char s, float f)
//{
//    parameter_printf("%s= "LOG_FLOAT_MARKER"   \n",s, LOG_FLOAT(f));

//}




static void bubblesort(float *input,int count) 
{
    int i,j;
    float sort_temp;
    for(i=0;i<count - 1;i++)
    {
        for(j=0;j<count-i - 1;j++) 
        {
            if(input[j+1]<input[j])
            {
                sort_temp=input[j+1];
                input[j+1]=input[j];
                input[j]=sort_temp;
            }
        }
    }
}
    
    
    
    
    

    
void test_library_SPO2(void)
{
    int i               = 0;
    int j               = 0;
    int32_t version     = 0;
    uint8_t ReadyFlag   = 0;
    int     buffer_size = 0;
    void *pBuffer       = NULL;
    
    
    
    
    
    // add for new flow
    float            spo2_array[31]={0.0f};   
    int              count         =0;

    //int     time_s,time_e,time_c;
    int8_t alg_ch[2]    = {0, 1};
    static pxialg_spo2_state    _spo2_state;
    pah8series_data_t   pxialg_data;
        
    version     = SpO2_Version();
    LOG_PRINT("SPO2_version =%d \n",version);
    buffer_size = SpO2_OpenSize();
    LOG_PRINT("SPO2_size = %d  \n", buffer_size);
    pBuffer = malloc(buffer_size);

    
    nrf_delay_ms(50);
    if (!pBuffer)
    {
        LOG_PRINT("Out of memory. \n");
        return;
    }
    nrf_delay_ms(50);
    if (SpO2_Open(pBuffer) != SPO2_MSG_SUCCESS)
    {
        LOG_PRINT("SpO2_Open() failed. \n");
        free(pBuffer);
        return;
    }
    
       //log_printf("pBuffer =%x \n",pBuffer);
    SpO2_SetChMapping(alg_ch, 2);
    SpO2_SetInputRatioHighBound(94.0f);
    SpO2_SetInputRatioLowBound(87.0f);
    SpO2_SetInputModelScale(0.50f);
    
    SpO2_SetInputCoefHigh0(-36877.85546875f);
    SpO2_SetInputCoefHigh1(1135.91394042969f);
    SpO2_SetInputCoefHigh2(-11.6429319381714f);
    SpO2_SetInputCoefHigh3(0.0398150011897087f);
    
    SpO2_SetInputCoefMid0(-1430.70520019531f);
    SpO2_SetInputCoefMid1(50.1599006652832f);
    SpO2_SetInputCoefMid2(-0.567422986030579f);
    SpO2_SetInputCoefMid3(0.00219199992716312f);
    
    SpO2_SetInputCoefLow0(-8.19796657562256f);
    SpO2_SetInputCoefLow1(1.74200701713562f);
    SpO2_SetInputCoefLow2(-0.0173850003629923f);
    SpO2_SetInputCoefLow3(0.000105999999505002f);
    
    SpO2_SetIIRFilterRatio(0.5f);
    SpO2_SetInputCorrelationTh(0.850000078678131f);
    SpO2_SetInputRatioLow(0.05f);        
    
    SpO2_SetInputFlagCheckHr(1);
    SpO2_SetMotionDetectionLevel(2.0f);
    SpO2_Set_Update_Sec(1);
    
    SpO2_Set_Init_Low_SpO2_Constrain(0.0);
    SpO2_SetOutputHighBound(100.0f);
    SpO2_SetOutputLowBound(60.0f);
    SpO2_Set_Keep_Init_SpO2_Bound(95.0f);    
    SpO2_Set_Keep_Init_SpO2_Count_Th(18.0f);
    SpO2_Set_SpO2_Bias_Value(0.5f);
    SpO2_Set_SpO2_Outlier_Smooth_Coef(5.5f);
    SpO2_Set_SpO2_Decrease_Coef(0.5f);
    SpO2_SetSkipDataLen(25);
             
    SpO2_Set_Flag_Hold_Breathe_Mode(0);
    SpO2_Set_Input_Correlation_Th2(0.85000f);
    SpO2_Set_Correlation_Th2_Count(30.0f);
    SpO2_Set_Init_Low_SpO2_Th(94.5f);
    SpO2_Set_Motion_Rms_Th(3000.0f);
    SpO2_Set_Motion_Rms_Th2(1500.0f);
    SpO2_Set_Motion_MEMS_Th(0.0f);     
    SpO2_Set_T1(0.85f);
    SpO2_Set_Spo2_Force_Output_Second(30.0f);
    
    Spo2_Set_Force_Output_Value(97.50f);  // Good Signal Force SpO2
    Spo2_Set_Force_Output_Value2(95.00f); // Bad  Signal Force SpO2
    
    SpO2_Set_Init_Low_SpO2_Check_Sec(20);
    Spo2_Set_SpO2_First_Smooth_Th(95.0f);
    Spo2_Set_SpO2_First_Smooth_Sec(15);
    Spo2_Set_SpO2_Correlation_Smooth_Th(0.85f);
    SpO2_Set_Signal_Periodicity_Th(0.50f);
             
    
   /// static  Hr
    version = PxiAlg_Version();
    LOG_PRINT("HR_Version = %d \r\n", version);        
    
    uint32_t Query_Mem_Size = PxiAlg_Query_Mem_Size();
    LOG_PRINT("HR_Mem_Size = %d \r\n", Query_Mem_Size);
    
    _spo2_state.hr_pBuffer = malloc(Query_Mem_Size);
    
    PxiAlg_Open_Mem(_spo2_state.hr_pBuffer);
    
////                
    PxiAlg_SetQualityThreshold(0.35f); 
    PxiAlg_SetHRSmoothWeight(0.9f); 
    PxiAlg_SetProcWindow(5.0f, 5.0f); 
    PxiAlg_SetTimeoutLength(30.0f);
    PxiAlg_SetMemsScale(0);              //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetSkipSampleNum(10); 
    PxiAlg_SetMemsScaleBase(1, 0);    //MEMS 2/4/8/16G : 0/1/2/3
    PxiAlg_SetForceOutputLink(15, 90, 60, 1, 0.9f); 
    PxiAlg_SetNormalOutput(200, 40, 1); 
    PxiAlg_SetForceRipple(3, 1); 

    PxiAlg_SetPackage(0, 1);
         

//-----------------------------------------------//

       int32_t  alg_count=0;

       LOG_PRINT("ARRAY_SIZE = %d\n",array_szie(PAH8009_rawdata_SPO2));
//******************************************************************         
    
      
    float   mySpO2          = 0      ;
    float   MEMS_RMS        = 0.0f   ;
    float   myHR            = 0.0f   ;
    int     ret_value  = 0   ;
    float   hr_grader       = 0.0f   ;
    uint8_t ready_flag = 0   ;
    float   correlation     = 0.0f   ;
    float   trust_flag      = 0.0f   ;
    uint8_t no_value_reason = 0      ;
    int     time_accu       = 0      ;
    float   SignalPeriodicity=0.0f   ;
//*******************************************************************         
         
    nrf_delay_ms(10);
         

    for( i = 0 ; i <array_szie(PAH8009_rawdata_SPO2) ; i ++)
    {
    
        uint32_t  touch_flag         = PAH8009_rawdata_SPO2[i][0];               
        uint32_t  time               = duration_SPO2[i];              
        int       ppg_count_per_ch   = PAH8009_rawdata_SPO2[i][1];    
        int       frame_number       = frame_SPO2[i]; 
        int       mems_number        =    PAH8009_mems_SPO2[i][0];         
        int32_t   RAW_DATA [75]       = {0}; 
              
        
        //log_printf("RAW_DATA =\n");
        for (j=0;j<ppg_count_per_ch; j++)
        {
            RAW_DATA[j] = PAH8009_rawdata_SPO2[i][j+2] ;
            //log_printf("%d , ", RAW_DATA[j]);
            
        }
         
        
         for( j = 0 ; j <(PAH8009_mems_SPO2[i][0]) ; j ++)
        {            
            mems_data_raw[j].data[0]=PAH8009_mems_SPO2[i][3*j+1];
            mems_data_raw[j].data[1]=PAH8009_mems_SPO2[i][3*j+2];
            mems_data_raw[j].data[2]=PAH8009_mems_SPO2[i][3*j+3];   
        
        }
         
                

        nrf_delay_ms(10);
        pxialg_data.touch_flag         = touch_flag;
        pxialg_data.time               = time;
        pxialg_data.ppg_data           = (int32_t*)RAW_DATA;
        pxialg_data.nf_ppg_channel     = NF_PPG_CHANNEL;
        pxialg_data.nf_ppg_per_channel = ppg_count_per_ch/NF_PPG_CHANNEL;
        pxialg_data.frame_count        = frame_number;
        pxialg_data.nf_mems            = mems_number;
        pxialg_data.mems_data          = (int16_t*)mems_data_raw;
                 
                 
        
        ret_value = PxiAlg_Entrance(&pxialg_data); 
        
        
        alg_count  = alg_count + PAH8009_rawdata_SPO2[i][1]/NF_PPG_CHANNEL ;
        nrf_delay_ms(10);
        
        //if( ret_value == FLAG_DATA_READY) 
        if(PxiAlg_GetHrUpdateFlag())
        {
            PxiAlg_HrGet(&myHR);
            ready_flag = PxiAlg_GetReadyFlag();
            PxiAlg_GetSigGrade(&hr_grader);
        
        }
        //ready_flag = PxiAlg_GetReadyFlag();
        //PxiAlg_GetSigGrade(&hr_grader);
                 
                 
        nrf_delay_ms(50);
        SpO2_SetHrValue(myHR);
        SpO2_SetMems(mems_data_raw, pxialg_data.nf_mems);
//        uint32_t time_s = get_tick_count();
        ret_value = SpO2_Process(1, NF_PPG_CHANNEL, pxialg_data.ppg_data, pxialg_data.nf_ppg_per_channel,pxialg_data.time );
        
      //  uint32_t time_e = pah_get_tick_count();
//        uint32_t time_diff=time_e-time_s;
//        log_printf("process-time = %d \n",time_diff);
        
        
        SpO2_GetSpO2(&mySpO2);
        SpO2_GetReadyFlag(&ReadyFlag);
        SpO2_info_t    _spo2_detect_item;
        SpO2_GetInfo(&_spo2_detect_item);
        correlation=_spo2_detect_item.Noise;
        trust_flag =_spo2_detect_item.Output[3];
        no_value_reason=_spo2_detect_item.Output[4];
        SpO2_GetPeriodicity(&SignalPeriodicity);
        MEMS_RMS  =_spo2_detect_item.RMS;
         
                                  
                 
        #if defined(Optimize_SPO2_flow)     
            time_accu=time_accu+time;
        #endif
        nrf_delay_ms(10);
        if( ret_value == 0  && (alg_count>=25) )
        {        
            alg_count = alg_count -25;
            if (ReadyFlag > 0)
            {
                LOG_PRINT("myHR = "LOG_FLOAT_MARKER" ",LOG_FLOAT(myHR));
                //nrf_delay_ms(10);
                LOG_PRINT(",grade = "LOG_FLOAT_MARKER" ",LOG_FLOAT(hr_grader));
                //nrf_delay_ms(10);
                LOG_PRINT(",spo2 = "LOG_FLOAT_MARKER" ",LOG_FLOAT(mySpO2));
                //nrf_delay_ms(10);
                LOG_PRINT(",correlation = "LOG_FLOAT_MARKER" ",LOG_FLOAT(correlation));
                //nrf_delay_ms(10);
                LOG_PRINT(",trust_flag = "LOG_FLOAT_MARKER" ",LOG_FLOAT(trust_flag));
                //nrf_delay_ms(10);
                LOG_PRINT(", SignalPeriodicity = "LOG_FLOAT_MARKER" ",LOG_FLOAT(SignalPeriodicity));
                //nrf_delay_ms(10);
                LOG_PRINT(",MEMS_RMS    = "LOG_FLOAT_MARKER"  ",LOG_FLOAT(MEMS_RMS));
                //nrf_delay_ms(10);
                LOG_PRINT(",reason = %d \n",no_value_reason);
            }
                else
            {
                LOG_PRINT("myHR = "LOG_FLOAT_MARKER" ",LOG_FLOAT(myHR));
                //nrf_delay_ms(10);
                LOG_PRINT(",grade = "LOG_FLOAT_MARKER" ",LOG_FLOAT(hr_grader));
                //nrf_delay_ms(10);
                LOG_PRINT(",spo2 = "LOG_FLOAT_MARKER" ",LOG_FLOAT(mySpO2));
                //nrf_delay_ms(10);
                LOG_PRINT(",correlation = "LOG_FLOAT_MARKER" ",LOG_FLOAT(correlation));
                //nrf_delay_ms(10);
                LOG_PRINT(",trust_flag = "LOG_FLOAT_MARKER" ",LOG_FLOAT(trust_flag));
                //nrf_delay_ms(10);
                LOG_PRINT(", SignalPeriodicity = "LOG_FLOAT_MARKER" ",LOG_FLOAT(SignalPeriodicity));
                //nrf_delay_ms(10);
                LOG_PRINT(",MEMS_RMS    = "LOG_FLOAT_MARKER"  ",LOG_FLOAT(MEMS_RMS));
                 //nrf_delay_ms(10);
                LOG_PRINT(",reason = %d \n",no_value_reason);

            }
                     
//                float  new_spo2=mySpO2;
            
                
       #if defined(Optimize_SPO2_flow)     
            //time_accu=time_accu+time;
            LOG_PRINT("time_accu = %d\n",time_accu);
            if(time_accu>=31000)
            {
                spo2_array[count]=mySpO2;
                count=count+1;
                
                LOG_PRINT("Over_30_sec_Stop_measurement\n");
                LOG_PRINT("This_meausrement_suggest_spo2_value is");
                
                bubblesort(spo2_array,count);
                
                if(count % 2 == 0)
                {
                    //LOG_PRINT(" "LOG_FLOAT_MARKER" \n", LOG_FLOAT(spo2_array[count/2]));
                    LOG_PRINT(" "LOG_FLOAT_MARKER" \n",LOG_FLOAT((spo2_array[(int)(count/2)] + spo2_array[(int)(count/2) - 1]) / 2.0f));  
                }
                else
                {
                    LOG_PRINT(" "LOG_FLOAT_MARKER"\n", LOG_FLOAT(spo2_array[(int)(count/2)]*1.0f));
                }
                count=0;
                nrf_delay_ms(50);
              //add sensor stop here              
                SpO2_Close();
                PxiAlg_Close();
                free(pBuffer);
                free(_spo2_state.hr_pBuffer);
                //NRF_LOG_FLUSH();
                return;                

            }
            else
            {
                if(mySpO2>0)
                {
                    spo2_array[count]=mySpO2;
                    count=count+1;                    
                }
                                    
            }
            #endif  
        }
        else
        {
        
            switch (ret_value)
            {
                case SPO2_MSG_ALG_NOT_OPEN:
                     LOG_PRINT("Algorithm is not initialized. \n");
                break;
                                
                case SPO2_MSG_ALG_REOPEN:
                     LOG_PRINT("Algorithm is re initialized. \n");
                break;
                                
                case SPO2_MSG_ALG_NOT_SUPPORT:
                     LOG_PRINT("Algorithm is not support. \n");
                break;
                case SPO2_MSG_SUCCESS:
                     //LOG_PRINT("Algorithm is SUCCESS. \n");
                break;                
                default:
                     LOG_PRINT("Algorithm unhandle error = %d \n", ret_value);
                break;
            }
        }

                
        
        nrf_delay_ms(40); 
    }
    
    LOG_PRINT("Out of memory. \n");
    nrf_delay_ms(10);
    SpO2_Close();
    PxiAlg_Close();
    free(pBuffer);
    free(_spo2_state.hr_pBuffer);
    return;
}
