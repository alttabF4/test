/*
 * globalTimer.h
 *
 *  Created on: Aug 14, 2019
 *      Author: linux
 */

#ifndef SRC_GLOBALTIMER_H_
#define SRC_GLOBALTIMER_H_

#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xgpio.h"
#include "xil_io.h"

#define TIMER_LOAD_VALUE	    166666667UL                                  //0.5s
#define LEDS_DEVICE_ID		    XPAR_AXI_GPIO_0_DEVICE_ID
#define INTC		            XScuGic
#define INTC_DEVICE_ID	        XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INTC_HANDLER	        XScuGic_InterruptHandler
#define Global_Timer_INTR                       XPAR_GLOBAL_TMR_INTR
#define Global_Timer_Counter_Register0          XPAR_GLOBAL_TMR_BASEADDR+0x0U
#define Global_Timer_Counter_Register1          XPAR_GLOBAL_TMR_BASEADDR+0x4U
#define Global_Timer_Control_Register           XPAR_GLOBAL_TMR_BASEADDR+0x8U
#define Global_Timer_Interrupt_Status_Register  XPAR_GLOBAL_TMR_BASEADDR+0xCU
#define Comparator_Value_Register0              XPAR_GLOBAL_TMR_BASEADDR+0x10U
#define Comparator_Value_Register1              XPAR_GLOBAL_TMR_BASEADDR+0x14U
#define Auto_increment_Register                 XPAR_GLOBAL_TMR_BASEADDR+0x18U
#define Auto_Increment_Bit                      0x08U
#define IRQ_Enable_Bit                          0x04U
#define Comp_Enable_Bit                         0x02U
#define Timer_Enable_Bit                        0x01U

int  SetupIntrSystem(INTC *IntcInstancePtr);
void Stop_Global_Timer();
void Start_Global_Timer();
void AUTO_Increment_Enable();
void AUTO_Increment_Disable();
void GT_Interrupt_Enable();
void GT_Interrupt_Disable();
void GT_Comp_Enable();
void GT_Comp_Disable();
void GT_Write_Reg(u32 Reg_Addr,u32 Reg_Val);
u32 GT_Read_Reg(u32 Reg_Addr);
static void TimerIntrHandler();

#endif /* SRC_GLOBALTIMER_H_ */
