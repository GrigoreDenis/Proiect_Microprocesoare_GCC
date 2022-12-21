#include "gpio.h"
#include "Adc.h"
#include "Uart.h"

int main()
{
	RGBLed_Init();
	UART0_Init(115200);
	int switch_senzor =0; // Ne spune de pe ce senzor citim, 0 = Lumina | 1 = Sunet
	int aux_recv=1; //Daca primim "enter" pe UART atunci schimbam senzorul de pe care citim ( 1 implicit pentru a avea un senzor deja)
	int start=0; // daca incepem nu este nevoie de completarea registrului COCO (Conversion Complete) Daca schimbam senzorul atunci este nevoie de completarea registrului COCO
	int i;
	for(;;) {
		if(aux_recv==1)
				{
						if(switch_senzor ==0)
						{
							if(start==1)
								ADC0->SC1[0]=ADC_SC1_COCO_MASK; //completam registrul COCO pentru a putea schimba senzorii de pe care ascultam
							ADC_LUMINA_0_Init(); //initializam senzorul de lumina
							aux_recv=0; //setam aux_recv = 0 -> am schimbat senzorul
							switch_senzor=1; //urmatorul senzor, daca se va schimba, va fi cel de SUNET
							start=1; //Verificare inceput de program
						}
						else
						{
							if(start==1)
								ADC0->SC1[0]=ADC_SC1_COCO_MASK;
							ADC_SUNET_0_Init();
							aux_recv=0;
							switch_senzor=0;
							start=1;
						}
					}
		if (read != write)	
		{
			if (buffer[read] == 0x0D) //Daca primim "enter" pe UART schimbam senzorul
			{
				aux_recv=1; // =1 => schimbam
			}
			if (buffer[read] == 0x61) //Daca primim 'a' pe UART atunci transmitem date
			{
				ADC_SEND(); //Functie custom in adc.c
			}
			read=write; //resetare recv uart
			print_buffer=0;

		}
					
	}	
}
