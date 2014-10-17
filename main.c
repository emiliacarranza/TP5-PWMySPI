#include <stdint.h>
#include "bsp/bsp.h"
#include "string.h"
#define LONG_BUFFER 7
#define LONG_ENCABEZADO 3

/**
 * @brief Se encarga de prender un led y apagarlo luego de un tiempo
 *
 * @param led    Numero de led a pulsar
 * @param tiempo Numero de ciclos del delay entre prendido y apagado
 */
struct st_trama{
	uint8_t encab[LONG_ENCABEZADO];
	uint8_t n_led;
	uint8_t dos_p;
	uint8_t estado;
	uint8_t fin_trama;
};

union rx_trama{
	struct st_trama trama;
	uint8_t buffer[7];
} rx;

void ledPulso(uint8_t led, uint32_t tiempo);

int analizar_trama(rx_trama rx);

/**
 * @brief Aplicacion principal
 */
int main(void) {
	bsp_init();
	int brillo = 0;
	int flag = 0;
	uint8_t i;

	while (1) {
		uint16_t valor_pot, porcentaje;


		valor_pot = bsp_convertir();

	    porcentaje = (uint16_t) valor_pot*100.0/4095;

	    for( i=0;i<8;i++)
	    	led_off(i);

	    if (porcentaje > 10)
	    				led_on(0);
	    if (porcentaje > 20)
	    				led_on(1);
	    if (porcentaje > 30)
	    				led_on(2);
	    if (porcentaje > 40)
	    				led_on(3);
	    if (porcentaje > 50)
	    				led_on(4);
	    if (porcentaje > 60)
	    				led_on(5);
	    if (porcentaje > 70)
	    				led_on(6);
	    if (porcentaje > 90)
				led_on(7);




	/*	TP 5
	    bsp_delayMs(100);

		led_setBright(0,brillo);
		led_setBright(1,brillo);
		led_setBright(2,brillo);
		led_setBright(3,brillo);

		if(brillo >= 100)
			flag = 0;
		if(brillo <=0)
			flag = 1;

		if(flag)
			brillo++;
		else
			brillo--; */


	}
}

/**
 * @brief Se preciono el pulsador
 *
 */
void APP_ISR_sw(void){

}

/**
 * @brief Interrupcion cada 1ms
 *
 */
void APP_ISR_1ms(void){
	static uint16_t count_1s = 1000;
	count_1s--;
	if (!count_1s) {
		led_toggle(0);
		count_1s = 1000;
	}
}

void APP_ISR_RX(char data){
int i,resultado;
for (i=0; i<LONG_BUFFER-1 ; i++)
	rx.buffer[i] = rx.buffer[i+1];
rx.buffer[LONG_BUFFER-1] = data;
if (rx.trama.fin_trama == 0xD){
	resultado = analizar_trama(rx);
	if (resultado)
		led_on(rx.buffer);//seguir
}
}

int analizar_trama(rx_trama rx){
int i,encontro=0;
string cab[3];
for (i=0;i<3;i++)
	cab[i]=rx.trama.encab[i];
cab[3]=null;
if (!strcmp("LED",cab)){
	        nro_led = rx.buffer[4];
			estado= rx.buffer[6];
			return 1;
}
return 0;
}

void ledPulso(uint8_t led, uint32_t tiempo){
	led_on(led);
	Delay(tiempo);
	led_off(led);
}


