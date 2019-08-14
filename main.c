#include "timer.h"
#include "globalTimer.h"
#define TIMER_LOAD_VALUE	0xFFFF
volatile int TimerExpired;

extern XScuTimer Timer1Instance;	/* Cortex A9 Scu Private Timer Instance */
extern XScuTimer Timer2Instance;	/* Cortex A9 Scu Private Timer Instance */
extern XScuGic IntcInstance;		/* Interrupt Controller Instance */

INTC   IntcInstancePtr;
int main()
{
//	int LastTimerExpired = 0;
//	TimerInit();
//	/*
//	* Enable Auto reload mode.
//	*/
//	XScuTimer_EnableAutoReload(&Timer1Instance);
//	XScuTimer_EnableAutoReload(&Timer2Instance);
//	/*
//	* Load the timer counter register.
//	*/
//	XScuTimer_LoadTimer(&Timer1Instance, TIMER_LOAD_VALUE);
//	XScuTimer_LoadTimer(&Timer2Instance, TIMER_LOAD_VALUE/2);
//	/*
//	* Start the timer counter and then wait for it
//	* to timeout a number of times.
//	*/
//	XScuTimer_Start(&Timer1Instance);
//	XScuTimer_Start(&Timer2Instance);
//	while (1) {
////		/*
////		* Wait for the first timer counter to expire as indicated by
////		* the shared variable which the handler will increment.
////		*/
////		while (TimerExpired == LastTimerExpired) {
////		}
////
////		LastTimerExpired = TimerExpired;
////		/*
////		* If it has expired a number of times, then stop the timer
////		* counter and stop this example.
////		*/
////		if (TimerExpired == 3) {
////			XScuTimer_Stop(&Timer1Instance);
////			break;
////		}
//	}

			SetupIntrSystem(&IntcInstancePtr);
			GT_Write_Reg(Global_Timer_Control_Register,//启动全局定时器
			Auto_Increment_Bit|IRQ_Enable_Bit|Comp_Enable_Bit|Timer_Enable_Bit);
			while(1);

	return 0;
}
