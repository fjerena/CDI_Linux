/*
 * INT_ABSTRACTION_LAYER.h
 *
 *  Created on: Sep 4, 2022
 *      Author: fabio
 */

#ifndef INC_INT_ABSTRACTION_LAYER_H_
#define INC_INT_ABSTRACTION_LAYER_H_

void Rising_Edge_Event(void);
void Falling_Edge_Event(void);
void Pulse_Generator_Scheduler(void);
void TurnOffAllPulseInt(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* INC_INT_ABSTRACTION_LAYER_H_ */
