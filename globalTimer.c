/*
 * globalTimer.c
 *
 *  Created on: Aug 14, 2019
 *      Author: linux
 */

#include "globalTimer.h"

static void TimerIntrHandler()
{
	int status=Xil_In32(Global_Timer_Interrupt_Status_Register);
	if(status)
	{
		Xil_Out32(Global_Timer_Interrupt_Status_Register, status);
//		TimerExpired=~TimerExpired;
//		XGpio_DiscreteWrite(&Gpio_LEDS, SEG_CHANNEL, TimerExpired);
		xil_printf("Gobal Timer Intr Handler \n");
	}
}

void Stop_Global_Timer()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status&(~Timer_Enable_Bit));
}

void Start_Global_Timer()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status|Timer_Enable_Bit);
}

void AUTO_Increment_Enable()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status|Auto_Increment_Bit);
}

void AUTO_Increment_Disable()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status&(~Auto_Increment_Bit));
}

void GT_Interrupt_Enable()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status|IRQ_Enable_Bit);
}

void GT_Interrupt_Disable()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status|IRQ_Enable_Bit);
}
void GT_Comp_Enable()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status|Comp_Enable_Bit);
}
void GT_Comp_Disable()
{
	int status=Xil_In32(Global_Timer_Control_Register);
	Xil_Out32(Global_Timer_Control_Register, status|Comp_Enable_Bit);
}
void GT_Write_Reg(u32 Reg_Addr,u32 Reg_Val)
{
	Xil_Out32(Reg_Addr, Reg_Val);
}
u32 GT_Read_Reg(u32 Reg_Addr)
{
	return Xil_In32(Reg_Addr);
}

int SetupIntrSystem(INTC *IntcInstancePtr)
{
	int Status;
	XScuGic_Config *IntcConfig;
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}
	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Initialize the  exception table
	 */
	Xil_ExceptionInit();
	/*
	 * Register the interrupt controller handler with the exception table
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 (Xil_ExceptionHandler)INTC_HANDLER,
			 IntcInstancePtr);

	GT_Write_Reg(Global_Timer_Control_Register,0);//停止全局定时器
	GT_Write_Reg(Global_Timer_Counter_Register0,0);//清空计数器低32位
	GT_Write_Reg(Global_Timer_Counter_Register1,0);//清空计数器高32位
	GT_Write_Reg(Global_Timer_Interrupt_Status_Register,1);//清除中断标志位
	GT_Write_Reg(Comparator_Value_Register0,TIMER_LOAD_VALUE);//加载比较器低32位
	GT_Write_Reg(Comparator_Value_Register1,0);//加载比较器高32位
	GT_Write_Reg(Auto_increment_Register,TIMER_LOAD_VALUE);//加载递增寄存器数值

	Status = XScuGic_Connect(IntcInstancePtr, Global_Timer_INTR,
					(Xil_ExceptionHandler)TimerIntrHandler,
					0);//绑定全局定时器中断服务函数
	if (Status != XST_SUCCESS)
	{
		return Status;
	}
	XScuGic_InterruptMaptoCpu(IntcInstancePtr,0,Global_Timer_INTR);//将27号全局定时器中断映射到CPU1
	XScuGic_Enable(IntcInstancePtr, Global_Timer_INTR);//打开全局定时器中断（27号）
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}
