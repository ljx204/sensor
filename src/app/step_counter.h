

#ifndef _STEP_COUNTER_H_
#define _STEP_COUNTER_H_

#include <stdint.h>
#include <nrf_assert.h>
#include "app_timer.h"

void sensor_timer_handle(void * val);

void sensor_timer_create(void);

void sensor_timers_start(void);

void sensor_timers_stop(void);


#define FILTER_CNT			            4
#define SAMPLE_SIZE                 50
#define DYNAMIC_PRECISION     			30     	 /*��̬����*/
#define MOST_ACTIVE_NULL      			0      	 /*δ�ҵ����Ծ��*/

#define MOST_ACTIVE_X					      1		     /*���Ծ��X*/	
#define MOST_ACTIVE_Y					      2        /*���Ծ��Y*/	
#define MOST_ACTIVE_Z					      3        /*���Ծ��Z*/	
 
#define ACTIVE_PRECISION      			60       /*��Ծ����С�仯ֵ*/

#define ABS(a) (0 - (a)) > 0 ? (-(a)) : (a)

typedef struct {

 	int16_t x;

  int16_t y;

  int16_t z;

}axis_info_t;


typedef struct filter_avg {

	axis_info_t info[FILTER_CNT];

	unsigned char count;
	
}filter_avg_t;

typedef struct {

  	axis_info_t newmax;
	
	  axis_info_t newmin;
	
	  axis_info_t oldmax;
	
	  axis_info_t oldmin;

}peak_value_t;

/*һ��������λ�Ĵ��������ڹ��˸�Ƶ����*/
typedef struct slid_reg {
	
	  axis_info_t new_sample;
 
  	axis_info_t old_sample;

}slid_reg_t;

#endif
