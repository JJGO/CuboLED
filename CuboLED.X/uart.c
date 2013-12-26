#include "uart.h"
static char uc_cola_transmision[TAM_TR_UART];
static char uc_cola_recepcion[TAM_REC_UART];
static unsigned int ui_icabeza_tr = 0;
static unsigned int ui_icola_tr = 0;
static unsigned int ui_icabeza_rec = 0;
static unsigned int ui_icola_rec = 0;

// ----------------------------------- FUNCIONES ----------------------- 
/* Nombre: InicializarUART
 * Descripción: Función de inicialización de la UART
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
void InicializarUART(void){
	TRISB |= 0x20; 						// Configura RX como entrada
	TRISB &= 0xFFEF;					// Configura TX como salida 
	U1BRG = (FCY/BAUDRATE)/16 - 1;		// Velocidad de transmisión
	U1MODEbits.STSEL = 0;				// Bits de parada: 1
	U1MODEbits.PDSEL = 0;				// Bits de datos: 8, Paridad: Ninguna
	U1MODEbits.ABAUD = 0;				// Deshabilitar detección de velocidad automática
	U1MODEbits.UARTEN = 1;				// Habilitar el módulo UART
	U1STAbits.UTXISEL1 = 1; 			// Modo interrupcion Tx:Cada vez que vacia U1TXREG 
	U1STAbits.URXISEL = 0;				// Modo interrupcion Rx: Al recibir un caracter
	U1STAbits.UTXEN = 1;				// Habilitar la transmisión (Sólo si UARTEN = 1) 
	IPC3bits.U1TXIP = PR_INT_TX_UART;	// Prioridad de la interrupción de transmisión
	IPC2bits.U1RXIP = PR_INT_RX_UART;	// Prioridad de la interrupción de recepción
	IFS0bits.U1TXIF = 0;				// Borrar flag de la interrupción de transmisión
	IFS0bits.U1RXIF = 0;				// Borrar la bandera de la interrupción de recepción
	IEC0bits.U1TXIE = 1;				// Habilitar las interrupciones de transmisión
	IEC0bits.U1RXIE = 1;				// Habilitar las interrupciones de recepción
 }


 // ---------------------------------------------------------------------
/* Nombre: PonerEnColaTransmisionUART
* Descripción: Función que coloca un dato en la cola de transmisión y
* gestiona el rebose del índice.
* Argumentos:
* 	unsigned char uc_caracter: Caracter que se quiere poner en cola 
* Valor devuelto: Ninguno */

void PonerEnColaTransmisionUART(unsigned char uc_caracter) {
	uc_cola_transmision[ui_icabeza_tr] = uc_caracter; 
	ui_icabeza_tr++;
	if (ui_icabeza_tr == TAM_TR_UART)
		ui_icabeza_tr = 0;
}


// --------------------------------------------------------------------- 
/* Nombre: SacarDeColaTransmision
* Descripción: Función que obtiene un dato de la cola de transmisión y
* gestiona el rebose del índice.
* Argumentos: Ninguno * Valor devuelto:
* unsigned char: Caracter recuperado de la cola */ 

unsigned char SacarDeColaTransmisionUART(void){
	unsigned char uc_caracter;
	uc_caracter = uc_cola_transmision[ui_icola_tr]; 
	ui_icola_tr++;
	if (ui_icola_tr == TAM_TR_UART)
		ui_icola_tr = 0; 
	return uc_caracter;
}

// --------------------------------------------------------------------- 
/* Nombre: PonerEnColaRecepcionUART
* Descripción: Función que coloca un dato en la cola de recepción y
* gestiona el rebose del índice.
* Argumentos:
* 	unsigned char uc_caracter: Caracter que se quiere poner en cola 
* Valor devuelto: Ninguno */

void PonerEnColaRecepcionUART(unsigned char uc_caracter) {
	uc_cola_recepcion[ui_icabeza_rec] = uc_caracter; 
	ui_icabeza_rec++;
	if (ui_icabeza_rec == TAM_REC_UART)
		ui_icabeza_rec = 0;
}

// --------------------------------------------------------------------- 
/* Nombre: SacarDeColaRecepcionUART
* Descripción: Función que obtiene un dato de la cola de recepción y
* gestiona el rebose del índice.
* Argumentos: Ninguno 
* Valor devuelto:
* 	unsigned char: Caracter recuperado de la cola */ 

unsigned char SacarDeColaRecepcionUART(){
	unsigned char uc_caracter;
	uc_caracter = uc_cola_recepcion[ui_icola_rec];
	ui_icola_rec++;
	if (ui_icola_rec == TAM_REC_UART)
		ui_icola_rec = 0; 
	return uc_caracter;
}

// ---------------------------------------------------------------------
/* Nombre: Transmite
* Descripción: Comienza a enviar la cola de transmisión 
* Argumentos: Ninguno
* Valor devuelto: Ninguno*/

void Transmite(void) {
	IFS0bits.U1TXIF = 1; // Borrar la bandera de la interrupción
}

/* Nombre: _U1TXInterrupt
* Descripción: Rutina de atención a interrupción de transmisión.
* Valor devuelto: Ninguno */

void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void) {
	IFS0bits.U1TXIF = 0; // Borrar la bandera de la interrupción 
	if (ui_icola_tr != ui_icabeza_tr)
		U1TXREG = SacarDeColaTransmisionUART();
}
// ---------------------------------------------------------------------
/* Nombre: _U1RXInterrupt
* Descripción: Rutina de atención a interrupción de recepción.
* Valor devuelto: Ninguno */

void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void){
	IFS0bits.U1RXIF = 0; // Borrar la bandera de la interrupción 
	PonerEnColaRecepcionUART(U1RXREG);
}


// --------------------------------------------------------------------- 
/* Nombre: HayAlgoRecibido
* Descripción: Indica si hay algo disponible en la cola de recepción 
* Valor devuelto:
* int: distinto de 0 si hay algo, 0 si no hay nada */
int HayAlgoRecibido(void) {
	return (ui_icola_rec != ui_icabeza_rec);
}


