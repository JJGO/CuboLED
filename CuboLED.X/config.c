
#include "config.h"



// ----------------------------------------------------------------------------------
// ----------------------------------- FUNCIONES ------------------------------------
// ----------------------------------------------------------------------------------

/* Nombre: InicializarReloj
 * Descripción: Inicializa el reloj interno FRC para que funcione con PLL.
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno
 */
void InicializarReloj(void)
 {
	// Configurar la frecuencia del oscilador FRC (FOSC), cuya frecuencia nominal
	// (Fin) son 7.37 MHz, para que el microprocesador opere a 40 MIPS (FCY)
	// FOSC = Fin * M/(N1*N2) 	FCY = FOSC/2
	// FOSC = 79.2275 MHz 		FCY = 39.61375 MHz
	CLKDIVbits.PLLPRE  =  0;	// Preescalado del PLL:   N1 = 2
	PLLFBD             = 41; 	// Multiplicador del PLL: M = PLLFBD + 2 = 43
	CLKDIVbits.PLLPOST =  0; 	// Postescalado del PLL:  N2 = 2
	
	// Si hiciese falta llegar hasta las 40 MIPS de forma exacta, se podría hacer un
	// ajuste fino de la frecuencia del reloj usando OSCTUN para incrementar o
	// decrementar la frecuencia a razón de OSCTUN * 30 kHz.
	// IMPORTANTE: Esta característica ha sido implementada para corregir derivas
	// producidas a causa de la temperatura y no está garantizado que los incrementos
	// sean de 30 kHz
	/*
		OSCTUN 			   = 21; 	// Fin = 7.37 MHz + OSCTUN * 30 kHz = 8 MHz
		CLKDIVbits.PLLPRE  =  0; 	// Preescalado del PLL:   N1 = 2
		PLLFBD 			   = 40;	// Multiplicador del PLL: M = PLLFBD + 2 = 43
		CLKDIVbits.PLLPOST =  0; 	// Postescalado del PLL:  N2 = 2
	*/
	
	// Funciones para desbloquear la escritura del registro OSCCON
	__builtin_write_OSCCONH(0x01);			// Nuevo reloj: FRC w/ PLL
	__builtin_write_OSCCONL(OSCCON | 0x01);	// Iniciar el cambio de reloj

	while (OSCCONbits.COSC != 1);	// Esperar a que se produzca el cambio de reloj	
	while (OSCCONbits.LOCK != 1);	// Esperar a que se sincronice el PLL
}

/* Nombre: RemapeaPerifericos
 * Descripci—n: Situa en los pines adecuados los perifŽricos del micro
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno
 */
void RemapeaPerifericos(void)
 {

	// Funciones para desbloquear la escritura del registro OSCCON
	__builtin_write_OSCCONL(OSCCON & 0xBF);	// Desbloquea el PPS

        RPINR18bits.U1RXR = 5; // Asigna U1RX al pin 14 que es RP5
        RPOR2bits.RP4R = 3; // Asignar U1TX al pin 11 que es RP4

        __builtin_write_OSCCONL(OSCCON | 0x040); // Bloquea los PPS

}