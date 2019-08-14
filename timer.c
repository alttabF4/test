/*
 * timer.c
 *
 *  Created on: Aug 13, 2019
 *      Author: linux
 */
#include "timer.h"
#include "xil_printf.h"

XScuTimer Timer1Instance;	/* Cortex A9 Scu Private Timer Instance */
XScuTimer Timer2Instance;	/* Cortex A9 Scu Private Timer Instance */
XScuGic IntcInstance;		/* Interrupt Controller Instance */

static void Timer1IntrHandler(void *CallBackRef)
{
	printf("timer 1 handler\n");
}

static void Timer2IntrHandler(void *CallBackRef)
{
	printf("timer 2 handler\n");
}

static int TimerSetupIntrSystem(XScuGic *IntcInstancePtr,XScuTimer *TimerInstancePtr, u16 TimerIntrId,Xil_InterruptHandler Handler)
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


	Xil_ExceptionInit();



	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				IntcInstancePtr);

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XScuGic_Connect(IntcInstancePtr, TimerIntrId,
				(Xil_ExceptionHandler)Handler,
				(void *)TimerInstancePtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	/*
	 * Enable the interrupt for the device.
	 */
	XScuGic_Enable(IntcInstancePtr, TimerIntrId);

	/*
	 * Enable the timer interrupts for timer mode.
	 */
	XScuTimer_EnableInterrupt(TimerInstancePtr);

	/*
	 * Enable interrupts in the Processor.
	 */
	Xil_ExceptionEnable();


	return XST_SUCCESS;
}

static void TimerDisableIntrSystem(XScuGic *IntcInstancePtr, u16 TimerIntrId)
{
	XScuGic_Disconnect(IntcInstancePtr, TimerIntrId);
}

void TimerInit()
{
	int Status;

	XScuTimer_Config *ConfigPtr;

	/*
	 * Initialize the Scu Private Timer driver.
	 */
	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);

	/*
	 * This is where the virtual address would be used, this example
	 * uses physical address.
	 */
	Status = XScuTimer_CfgInitialize(&Timer1Instance, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XScuTimer_CfgInitialize(&Timer2Instance, ConfigPtr,
						ConfigPtr->BaseAddr);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XScuTimer_SelfTest(&Timer1Instance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XScuTimer_SelfTest(&Timer2Instance);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

	/*
	 * Connect the device to interrupt subsystem so that interrupts
	 * can occur.
	 */
	Status = TimerSetupIntrSystem(&IntcInstance,&Timer1Instance, TIMER_1_INTR,Timer1IntrHandler);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = TimerSetupIntrSystem(&IntcInstance,&Timer2Instance, TIMER_2_INTR,Timer2IntrHandler);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

}
