#include <stm32f4xx.h>

volatile uint32_t tmp;
volatile int * temp;

void EXTI0_IRQHandler(void){
	NVIC_SetPendingIRQ(7);	//generate the interrupt IRQn=2
}

void EXTI1_IRQHandler(void){
	tmp++;
	NVIC_SetPendingIRQ(7);	//generate the interrupt IRQn=2
	temp = NVIC_GetPendingIRQ;
}

void EXTI3_IRQHandler(void){
	tmp++;
	temp = NVIC_GetPendingIRQ;
}

int main(){
	NVIC_SetPriorityGrouping(5);	//[5:0]: subpriority
	
	NVIC_SetPriority(6, 0x60>>4);	//priority of IRQ=1 => 0x60
	NVIC_SetPriority(7, 0x40>>4);	//priority of IRQ=2 => 0x40
	NVIC_SetPriority(8, 0x00>>4);	//priority of IRQ=3 => 0x00
	NVIC_SetPriority(9, 0x20>>4);	//priority of IRQ=4 => 0x20

	NVIC_EnableIRQ(6);		//IRQn=1 Interrupt Enable
	NVIC_EnableIRQ(7);		//IRQn=2 Interrupt Enable
	NVIC_EnableIRQ(8);		//IRQn=3 Interrupt Enable
	NVIC_EnableIRQ(9);		//IRQn=4 Interrupt Enable
	
	NVIC_SetPendingIRQ(6);	//generate the interrupt IRQn=1
	NVIC_SetPendingIRQ(7);	//generate the interrupt IRQn=2
	NVIC_SetPendingIRQ(8);	//generate the interrupt IRQn=3
	NVIC_SetPendingIRQ(9);	//generate the interrupt IRQn=4

}