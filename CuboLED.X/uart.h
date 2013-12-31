// ----------- MÓDULO UART (Universal Asynchronous Receiver Transmitter) 
// Módulo encargado de gestionar las comunicaciones por el puerto serie. 
#ifndef _UART_H
#define _UART_H

#include "config.h"

// Velocidad de transmisión en baudios (bps)
#define BAUDRATE 38400
// Tamaño de los vectores y colas
#define TAM_TR_UART 200				// Cola de Transimision				
#define TAM_REC_UART 200			// Cola de Recepcion
#define TAM_MENSAJE_REC 100			// Mensaje recibido
// Prioridad de las interrupciones (máx. 7 - mín. 1)
#define PR_INT_TX_UART 4
#define PR_INT_RX_UART 4
// --------------- DECLARAACIÓN DE LAS FUNCIONES PÚBLICAS --------- 
void            InicializarUART             (void);
int             HayAlgoRecibido             (void);
void            Transmite                   (void);
void            PonerEnColaTransmisionUART  (unsigned char uc_caracter); 
unsigned char   SacarDeColaRecepcionUART    (void);
void            Transmite                   (void);

#endif