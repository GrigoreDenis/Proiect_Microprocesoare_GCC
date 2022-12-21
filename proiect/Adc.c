#include "Adc.h"
#include "Uart.h"

#define ADC_CHANNEL_LUMINA (8) // PORT B PIN 0
#define ADC_CHANNEL_SUNET (9) // PORT B PIN 0
#define RED_LED_PIN (18) // PORT B
#define GREEN_LED_PIN (19) // PORT B
#define BLUE_LED_PIN (1) // PORT D
uint16_t test; //tine minte ultimul input de senzor
uint16_t max = 0; //tine minte maximul senzorului
uint16_t prag_mic_mediu; //praguri pentru LED-uri
uint16_t prag_mediu_mare;

int send=0; //Daca send = 1 trimitem datele de la senzor prin UART,


void ADC_LUMINA_0_Init() {
	
	// Activarea semnalului de ceas pentru modulul periferic ADC
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	// Functia de calibrare
	ADC0_Calibrate();
	
	ADC0->CFG1 = 0x00;

	// Selectarea modului de conversie pe 16 biti single-ended --> MODE
	// Selectarea sursei de ceas pentru generarea ceasului intern --> ADICLK
	// Selectarea ratei de divizare folosit de periferic pentru generarea ceasului intern --> ADIV
	// Set ADC clock frequency fADCK less than or equal to 4 MHz (PG. 494)
	ADC0->CFG1 |= ADC_CFG1_MODE(3) |
							 ADC_CFG1_ADICLK(0) |
							 ADC_CFG1_ADIV(2);
	
	// DIFF = 0 --> Conversii single-ended (PG. 464)
	ADC0->SC1[0] = 0x00;
	ADC0->SC3 = 0x00;

	// Selectarea modului de conversii continue, 
	// pentru a-l putea folosi in tandem cu mecanismul de intreruperi
	ADC0->SC3 |= ADC_SC3_ADCO_MASK;
	
	// Activarea subsistemului de conversie prin aproximari succesive pe un anumit canal (PG.464)
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL_LUMINA);
	// Enables conversion complete interrupts
	ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
	
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);	
}


void ADC_SUNET_0_Init() {
	
	// Activarea semnalului de ceas pentru modulul periferic ADC
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	// Functia de calibrare
	ADC0_Calibrate();
	
	ADC0->CFG1 = 0x00;

	// Selectarea modului de conversie pe 16 biti single-ended --> MODE
	// Selectarea sursei de ceas pentru generarea ceasului intern --> ADICLK
	// Selectarea ratei de divizare folosit de periferic pentru generarea ceasului intern --> ADIV
	// Set ADC clock frequency fADCK less than or equal to 4 MHz (PG. 494)
	ADC0->CFG1 |= ADC_CFG1_MODE(3) |
							 ADC_CFG1_ADICLK(0) |
							 ADC_CFG1_ADIV(2);
	
	// DIFF = 0 --> Conversii single-ended (PG. 464)
	ADC0->SC1[0] = 0x00;
	ADC0->SC3 = 0x00;
	// Selectarea modului de conversii continue, 
	// pentru a-l putea folosi in tandem cu mecanismul de intreruperi
	ADC0->SC3 |= ADC_SC3_ADCO_MASK;
	
	// Activarea subsistemului de conversie prin aproximari succesive pe un anumit canal (PG.464)
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_CHANNEL_SUNET);
	// Enables conversion complete interrupts
	ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
	
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);	
}


int ADC0_Calibrate() {
	
	// ===== For best calibration results =====
	
	ADC0_CFG1 |= ADC_CFG1_MODE(3)  |  				 // 16 bits mode
                ADC_CFG1_ADICLK(1)|  // Input Bus Clock divided by 2
                ADC_CFG1_ADIV(3);   // Clock divide by 8
	
	// The calibration will automatically begin if the SC2[ADTRG] is 0. (PG. 495)
	ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
	
	// Set hardware averaging to maximum, that is, SC3[AVGE]=1 and SC3[AVGS]=0x11 for an average of 32 (PG. 494)
	ADC0->SC3 |= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3));
	
	// To initiate calibration, the user sets SC3[CAL] (PG. 495)
	ADC0->SC3 |= ADC_SC3_CAL_MASK;
	
	// At the end of a calibration sequence, SC1n[COCO] will be set (PG. 495)
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
	
	// At the end of the calibration routine, if SC3[CALF] is not
	// set, the automatic calibration routine is completed successfully. (PG. 495)
	if(ADC0->SC3 & ADC_SC3_CALF_MASK){
		return (1);
	}
	
	// ====== CALIBRATION FUNCTION (PG.495) =====
	
	// 1. Initialize or clear a 16-bit variable in RAM.
	uint16_t calibration_var = 0x0000;
	
	// 2. Add the plus-side calibration results CLP0, CLP1, CLP2, CLP3, CLP4, and CLPS to the variable.
	calibration_var += ADC0->CLP0;
	calibration_var += ADC0->CLP1;
	calibration_var += ADC0->CLP2;
	calibration_var += ADC0->CLP3;
	calibration_var += ADC0->CLP4;
	calibration_var += ADC0->CLPS;
	
	// 3. Divide the variable by two.
	calibration_var /= 2;
	
	// 4. Set the MSB of the variable. 
	calibration_var |= 0x8000;
	
	// 5. Store the value in the plus-side gain calibration register PG.
	ADC0->PG = ADC_PG_PG(calibration_var);
	
	// 6. Repeat the procedure for the minus-side gain calibration value.
	calibration_var = 0x0000;
	
	calibration_var += ADC0->CLM0;
	calibration_var += ADC0->CLM1;
	calibration_var += ADC0->CLM2;
	calibration_var += ADC0->CLM3;
	calibration_var += ADC0->CLM4;
	calibration_var += ADC0->CLMS;
	
	calibration_var /= 2;
	
	calibration_var |= 0x8000;
	
	ADC0->MG = ADC_MG_MG(calibration_var);
	
	// Incheierea calibrarii
	ADC0->SC3 &= ~ADC_SC3_CAL_MASK;
	
	return (0);
}



void ADC_SEND() //Setam send pe 1 => vom intra pe if(send==1) in ADC0_IRQHandler
{
	send=1;
}

void ADC0_IRQHandler(){
	uint16_t analog_input = (uint16_t) ADC0->R[0];

	if(send==1) // daca send a fost setat pe 1 atunci transmitem prin UART inputul de la senzor, facem apoi send=0
	{
		float measured_voltage = (analog_input * 3.3f) / 65535;		
					
			uint8_t parte_zecimala= (uint8_t) measured_voltage;
					
			uint8_t parte_fractionara1 = ((uint8_t)(measured_voltage * 10)) % 10;
			uint8_t parte_fractionara2 = ((uint8_t)(measured_voltage * 100)) % 10;
		
			UART0_Transmit(parte_zecimala + 0x30);
			UART0_Transmit('.');
			UART0_Transmit(parte_fractionara1 + 0x30);
			UART0_Transmit(parte_fractionara2 + 0x30);
			UART0_Transmit('V');
			UART0_Transmit(0x0A);
			UART0_Transmit(0x0D);
		send=0;
	}

		if(max < analog_input) //prag pentru led..
			{
				max = analog_input;
				prag_mic_mediu = max/3; 
				prag_mediu_mare =(max *2)/3; 
			}
			
		if(test != analog_input)
		{
				if(analog_input < prag_mic_mediu) {
					if(test>= prag_mic_mediu && test <= prag_mediu_mare){
						GPIOB_PTOR |= (1<<GREEN_LED_PIN);
						GPIOB_PTOR |= (1<<RED_LED_PIN);
					}
					if(test>prag_mediu_mare){
						GPIOD_PTOR |= (1<<BLUE_LED_PIN);
						GPIOB_PTOR |= (1<<RED_LED_PIN);
					}
				
			} else if (analog_input >= prag_mic_mediu && analog_input <= prag_mediu_mare) {
				if(test<prag_mic_mediu){
						GPIOB_PTOR |= (1<<RED_LED_PIN);
						GPIOB_PTOR |= (1<<GREEN_LED_PIN);
					}
					if(test>prag_mediu_mare){
						GPIOD_PTOR |= (1<<BLUE_LED_PIN);
						GPIOB_PTOR |= (1<<GREEN_LED_PIN);
					}
				

			} else if (analog_input > prag_mediu_mare) {
				if(test>= prag_mic_mediu && test <= prag_mediu_mare){
						GPIOB_PTOR |= (1<<GREEN_LED_PIN);
						GPIOD_PTOR |= (1<<BLUE_LED_PIN);
					}
					if(test<prag_mic_mediu){
						GPIOB_PTOR |= (1<<RED_LED_PIN);
						GPIOD_PTOR |= (1<<BLUE_LED_PIN);
					}


			}
			
			 
		}

			test=analog_input; //tinem minte ultimul input luat de la senzor
	
	
	}



