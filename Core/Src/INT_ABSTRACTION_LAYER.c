/*
 * INT_ABSTRACTION_LAYER.c
 *
 *  Created on: Sep 4, 2022
 *      Author: fabio
 */

#include "IGN_MGMT.h"
#include "GENERAL_DEF.h"
#include "MATH_LIB.h"
#include "IO_CONTROL.h"

//All this functions are related to IGN_MGMT module

void Rising_Edge_Event(void)
{
    scenario.Measured_Period = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);
    scenario.nOverflow_RE = scenario.nOverflow;
    __HAL_TIM_SET_COUNTER(&htim2,0u);
    scenario.nOverflow = 0u;
    Pulse_Program.engSpeed = request.engSpeed;

    if((Pulse_Program.engSpeed == HIGH)&&
    (scenario.Cutoff_IGN == OFF))
    {
        Pulse_Generator_Scheduler();
    }

    scenario.Rising_Edge_Counter++;
	Toggle_LED_Red();
}

void Falling_Edge_Event(void)
{
    scenario.TDuty_Input_Signal = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);
    scenario.nOverflow_FE = scenario.nOverflow;

    if((Pulse_Program.engSpeed == LOW)&&
    (scenario.Cutoff_IGN == OFF))
    {
        Pulse_Generator_Scheduler();
    }

    if (scenario.Rising_Edge_Counter>=2u)
    {
        scenario.Update_calc = TRUE;        //set zero after Engine Stop was detected
    }
}

void Pulse_Generator_Scheduler(void)
{
    uint8_t i;

    for(i=0;i<4;i++)
    {
       Pulse_Program.timerCtrl[i].counter = request.timerCtrl[i].counter;
    }

    Set_Ouput_Trigger(OFF);
    Set_Ouput_Inversor(OFF);

    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,Pulse_Program.timerCtrl[0].counter);
    Pulse_Program.timerCtrl[0].timer_program = PROGRAMMED;

    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,Pulse_Program.timerCtrl[1].counter);
    Pulse_Program.timerCtrl[1].timer_program = PROGRAMMED;

    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,Pulse_Program.timerCtrl[2].counter);
    Pulse_Program.timerCtrl[2].timer_program = PROGRAMMED;

    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,Pulse_Program.timerCtrl[3].counter);
    Pulse_Program.timerCtrl[3].timer_program = PROGRAMMED;

    __HAL_TIM_SET_COUNTER(&htim4,0u);
    HAL_TIM_OC_Start_IT(&htim4,TIM_CHANNEL_1);
    HAL_TIM_OC_Start_IT(&htim4,TIM_CHANNEL_2);
    HAL_TIM_OC_Start_IT(&htim4,TIM_CHANNEL_3);
    HAL_TIM_OC_Start_IT(&htim4,TIM_CHANNEL_4);
}

void TurnOffAllPulseInt(void)
{
    HAL_TIM_OC_Stop_IT(&htim4,TIM_CHANNEL_1);
    HAL_TIM_OC_Stop_IT(&htim4,TIM_CHANNEL_2);
    HAL_TIM_OC_Stop_IT(&htim4,TIM_CHANNEL_3);
    HAL_TIM_OC_Stop_IT(&htim4,TIM_CHANNEL_4);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
    {
        scenario.nOverflow++;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if((htim->Instance == TIM2)&&
       (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1))
    {
        Rising_Edge_Event();
    }

    if((htim->Instance == TIM2)&&
       (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2))
    {
        Falling_Edge_Event();
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if((htim->Instance == TIM4)&&
       (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1))
    {
        Treat_Int(INT_FROM_CH1);
    }

    if((htim->Instance == TIM4)&&
       (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2))
    {
        Treat_Int(INT_FROM_CH2);
    }

    if((htim->Instance == TIM4)&&
       (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3))
    {
        Treat_Int(INT_FROM_CH3);
    }

    if((htim->Instance == TIM4)&&
       (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4))
    {
        Treat_Int(INT_FROM_CH4);
    }
}
