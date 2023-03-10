#include "gpio.h"
#include "Uart.h"

#define SENZOR_LUMINA (8) //PORT B
#define RED_LED_PIN (18) // PORT B
#define GREEN_LED_PIN (19) // PORT B
#define BLUE_LED_PIN (1) // PORT D


uint8_t state;
uint8_t value;

void Switch_Init(void) {
	
	// Activarea semnalului de ceas pentru a putea folosi GPIO cu ajutorul pinului 1 de pe portul C
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	// Utilizarea GPIO impune selectarea variantei de multiplexare cu valorea 1
	PORTB->PCR[SENZOR_LUMINA] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[SENZOR_LUMINA] |= PORT_PCR_MUX(1);
	
	// Activare Óntreruperi pe rising edge
	PORTB->PCR[SENZOR_LUMINA] |= PORT_PCR_IRQC(0x09) | PORT_PCR_PE_MASK;
	
	state = 0;
	value=3;
	// Activare Óntrerupere pentru a folosi switch-u
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_SetPriority(PORTA_IRQn, 128);
	NVIC_EnableIRQ(PORTA_IRQn);
	

}


void PORTA_IRQHandler() {
	
	
	// Prin utilizarea variabilei state, putem realiza un FSM
	// si sa configuram fiecare tranzitie in parte prin 
	// stingerea ledului anterior si aprinderea ledului curent
	
	//CULORILE NU SUNT CHIAR BUNE
	
	if(state == 0) {
		GPIOB_PTOR |= (1<<RED_LED_PIN);
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		state = 1;
		value=5;
	} else if (state == 1) {
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		GPIOD_PTOR |= (1<<BLUE_LED_PIN);
		state = 2;
		value=6;
	} else if (state == 2) {
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		GPIOB_PTOR |= (1<<RED_LED_PIN);
		state = 3;
		value=0;
	} else if (state == 3) {
		GPIOB_PTOR |= (1<<RED_LED_PIN);
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		GPIOD_PTOR |= (1<<BLUE_LED_PIN);
		state = 4;
		value=7;
	}
	else if (state == 4) {
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		GPIOD_PTOR |= (1<<BLUE_LED_PIN);
		state = 5;
		value=4;
	}
	else if (state == 5) {
		GPIOB_PTOR |= (1<<RED_LED_PIN);
		GPIOD_PTOR |= (1<<BLUE_LED_PIN);
		state = 6;
		value=1;
	}
	else if (state == 6) {
		GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		GPIOD_PTOR |= (1<<BLUE_LED_PIN);
		state = 7;
		value=2;
	}
	else if (state == 7) {
				GPIOB_PTOR |= (1<<RED_LED_PIN);
				GPIOB_PTOR |= (1<<GREEN_LED_PIN);
		state = 0;
		value=3;
	}
	
	
	
	//PORTA_ISFR = (1<<SWITCH_PIN);	
}
void RGBLed_Init(void){
	
	// Activarea semnalului de ceas pentru pinii folositi Ón cadrul led-ului RGB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	// --- RED LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PCOR |= (1<<RED_LED_PIN);
	
	// --- GREEN LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<GREEN_LED_PIN);
	
	// --- BLUE LED ---
	
		// Utilizare GPIO ca varianta de multiplexare
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOD_PDDR |= (1<<BLUE_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOD_PSOR |= (1<<BLUE_LED_PIN);
}