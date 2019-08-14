/*
 * timer.h
 *
 *  Created on: Aug 13, 2019
 *      Author: linux
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xstatus.h"

#define TIMER_1_INTR		29
#define TIMER_2_INTR		30

#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID



static void Timer1IntrHandler(void *CallBackRef);
static void Timer2IntrHandler(void *CallBackRef);

static int TimerSetupIntrSystem(XScuGic *IntcInstancePtr,XScuTimer *TimerInstancePtr, u16 TimerIntrId,Xil_InterruptHandler Handler);
static void TimerDisableIntrSystem(XScuGic *IntcInstancePtr, u16 TimerIntrId);
void TimerInit();
#endif /* SRC_TIMER_H_ */
