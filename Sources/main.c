/********************************************************************************************************************************/
/**************************************** ANGEL HABID NAVARRO MENDEZ ************************************************************/
/****************************************      20110320 7K IDESI     ************************************************************/
/****************************************           Servo            ************************************************************/
/********************************************************************************************************************************/

//Connections:
//Canal 1 para PWM (TPM CH 1, PTB5).

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


void MCU_init (void);
void output_control(void);
void PWM(unsigned int periodo, unsigned int ancho);

unsigned long grados = 0;
unsigned int ADCresult = 0;
unsigned int ADCresult1 = 0;
unsigned long conv_cuentas = 0;


void main(void) {
	
	MCU_init(); 
	


  
  for(;;) {
	grados=180;
	output_control();

  }
}

void MCU_init(void){
	/*Kill the Dog*/
	  SOPT1 = 0x12;
	/*Basic initialisations */  
	  DisableInterrupts;
}

void output_control(void){//esta funcion contiene el main del codigo para PWM y ADC
	
	grados = grados * 6944; // 0.694;
	conv_cuentas = grados / 10000 ;//conversion por punto fijo.
	ADCresult = 125;
	ADCresult = ADCresult + conv_cuentas;
	
	if(ADCresult >= 251){
		ADCresult = 251;
	}
	if(ADCresult <= 124){
			ADCresult = 124;
		}
	
	if(ADCresult1<ADCresult-3 ||ADCresult1>ADCresult+3){
		ADCresult1=ADCresult;
		TPMCNT = 0X0000;
		TPMSC=0X00;
	}
	PWM(2500,ADCresult1);//2500 CUENTAS = 20ms = 50hz	
}

void PWM(unsigned int periodo, unsigned int ancho)
{
TPMMOD=periodo;//time of the cycle (valued in quantity of counts). 
TPMC1V=ancho + 20;//High time + offset.
TPMSC = 0x0D; // Sin IRQ, BusClk=4Mhz, Preescaler=32
TPMC1SC = 0x28; //ModoPWM, Aligned rising edge.
while(!TPMC1SC_CH1F);//Wait for the flag (End of High time).
TPMC1SC_CH1F=0; // Clear the flag to CH1 (TPM enabled as PWM).

}


