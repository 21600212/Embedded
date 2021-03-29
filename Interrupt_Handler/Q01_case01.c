#include <stm32f4xx.h>

volatile uint32_t tmp;

void EXTI0_IRQHandler(void){
	NVIC_DisableIRQ(6);
}

void EXTI1_IRQHandler(void){
	NVIC_DisableIRQ(7);
}

int main(){
	tmp = SystemCoreClock/2;
	SysTick->LOAD = tmp;
	SysTick->CTRL = 0x07;

	NVIC_SetPriorityGrouping(5);	//[5:0]: subpriority

	NVIC_SetPriority(6, 0x40>>4);	//priority of IRQ=6 => 0x40
	NVIC_SetPriority(7, 0x80>>4);	//priority of IRQ=7 => 0x80

	NVIC_EnableIRQ(6);		//IRQn=6 Interrupt Enable
	NVIC_EnableIRQ(7);		//IRQn=7 Interrupt Enable

	NVIC_SetPendingIRQ(6);	//generate the first interrupt IRQn=6
	NVIC_SetPendingIRQ(7);	//generate the second interrupt IRQn=7
}
