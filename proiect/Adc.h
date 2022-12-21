#include "MKL25Z4.h"

void ADC_LUMINA_0_Init(void);
void ADC_SUNET_0_Init(void);
int ADC0_Calibrate(void);
void ADC0_IRQHandler(void);
void ADC0_SUNET_IRQHandler(void);
uint16_t ADC_Get_Input(void);
void ADC_SEND(void);