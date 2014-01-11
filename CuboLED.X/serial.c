// Jose Javier Gonzalez Ortiz
// Copyright ©2013-2014, Jose Javier Gonzalez Ortiz. All rights reserved.

// Libreria para el control de ejecucion de efectos en el cubo mediante el modulo UART
// serial.c

#include "serial.h"



// ------------------------------------ GLOBALES ------------------------------------------

uint8_t echo = true;
static uint8_t current_command = 0;

static char buffer[BUFFER_SIZE];
static char* pbuffer = buffer;

// ----------------------------------- PROTOTIPOS -----------------------------------------
char    watch_uart              (void);
void    parse_message           (char* code);
void    parse_effect            (char* code);
void    parse_font_effect       (char* code);
void    parse_command           (uint8_t message);
void    parse_escape_command    (uint8_t message);
uint8_t getCommand              (void);
void    cleanBuffer             (void);
    
void    send_str                (char* str);
void    send_int                (uint16_t i);
void    send_hex                (uint16_t i);
void    send_periodo            (void);

// ----------------------------------- FUNCIONES ------------------------------------------




/* Nombre: watch_uart 
 * Descripción: Función de atencion al modulo UART
 * Argumentos: Ninguno
 * Valor devuelto: mensaje */ 

char watch_uart(void)
{
    static int mensaje;
    //static int i = 0;
    
     
    if (HayAlgoRecibido()) {                            // Mira si hay algo recibido
        mensaje = SacarDeColaRecepcionUART();
            //Operar Mensaje

        if(echo)                                        // Si el echo esta activo repite lo recibido para la terminal de usuario
        {
            PonerEnColaTransmisionUART(mensaje);        //Realiza el eco de lo recibido
            Transmite();
        }

        switch(mensaje)
        {
            case 0x08:                                  // Backspace
                if(pbuffer-buffer > 0)
                {
                    pbuffer--;                          // En caso de backspace retrocede el puntero
                }
                break;

            case '\r':                                  // Si recibe el retorno de carro, envia nueva linea e interpreta el mensaje
                PonerEnColaTransmisionUART('\n');
                Transmite();
                *pbuffer = '\0';
                pbuffer = buffer; 
                parse_message(buffer);
                break;

            case 'A':
            case 'B':
            case 'C':
            case 'D':
                if(pbuffer[-1] == '[' && pbuffer[-2] == ESC)
                {
                    parse_escape_command(mensaje);
                    break;
                }

            default:
                *pbuffer++ = mensaje;                       // Si es un caracter normal, se copia en el buffer, y si rebosa lo resetea
                if(pbuffer-buffer >BUFFER_SIZE){
                    pbuffer = buffer; 
                }
                break;
        }
            parse_command(mensaje);                         // Comprueba si lo recibido es un comando 
                                                        // TODO Condicionarlo a la ejecucion de un juego
        return mensaje;                         
    }
    return '\0';
}

/* Nombre: parse_message
 * Descripción: Función de interpretado de los codigos de operacion
 * Argumentos: code - cadena que apunta al buffer de recepcion con el codigo de operacion
 * Valor devuelto: Ninguno */ 

void parse_message(char* code)
{   

    //Comprueba que sea un efecto
    switch(code[0])
    {
        case 'Q':
            effect_quit();
            break;
        case 'R':
            effect_reset();
            break;
        case 'N':
            effect_next();
            break;
        case 'P':
            if(code[1] == 0){
                send_periodo();                         // P : envia el perido actual
            }else if(code[1] == 'A'){ 
                analog_period = true;                   // PA : Reactiva el periodo analogico
            }else{
                setPeriodo(atoi(code+1));               // PX : Pone el periodo a valor X 
            }
            break;
        case EFFECT_ID:                                 // EXX : Activa el efecto XX de forma indefinida
            parse_effect(code);
            break;
        case FONT_ID:
            parse_font_effect(code);                    // FXX MMM.. : Activa el efecto XX de cadena con el mensaje MMM..
            break;
        case GAMES_ID:
            break;
    }
}

/* Nombre: parse_effect
 * Descripción: Funcion para lanzar e efecto asociado al codigo de operacion
 * Argumentos: code - cadena que apunta al buffer de recepcion con el codigo de operacion
 * Valor devuelto: Ninguno */ 

void parse_effect(char* code)
{
    uint8_t effect_id = atoi(code+1);    
    num_effect = effect_id;
    if(effect_id < NUM_EFFECTS)
    {
        effect_launch(effects[effect_id]);
    }        
    
}

/* Nombre: parse_font_effect
 * Descripción: Funcion para lanzar e efecto asociado al codigo de operacion
 * Argumentos: code - cadena que apunta al buffer de recepcion con el codigo de operacion
 * Valor devuelto: Ninguno */ 

void parse_font_effect(char* code)
{
    code[3] = '\0';                         // Se coloca un caracter terminador para el correcto funcionamiento de atoi

    uint8_t effect_id = atoi(code+1);    
    if(effect_id < NUM_EFFECTS)
    {
        effect_launch(effects[effect_id]);
    } 

    setMessage(code+4);                     // Pone el mensaje 
}

/* Nombre: parse_command
 * Descripción: Funcion de interpretado de mensajes a comandos
 * Argumentos: message - el caracter del comando
 * Valor devuelto: mensaje */ 

void parse_command(uint8_t message)
{
    switch(message){
        case 'w':
        case 'W':
            current_command = FWD_COMMAND;
            break;
        case 's':
        case 'S':
            current_command = REV_COMMAND;
            break;
        case 'a':
        case 'A':
            current_command = LEFT_COMMAND;
            break;
        case 'd':
        case 'D':
            current_command = RIGHT_COMMAND;
            break;
        case 'i':
        case 'I':
            current_command = UP_COMMAND;
            break;
        case 'k':
        case 'K':
            current_command = DOWN_COMMAND;
            break;
        default:
            current_command = NO_COMMAND;
            break;
    }
}

/* Nombre: parse_escape_command
 * Descripción: Funcion de interpretado de comandos de escape
 * Argumentos: message - el caracter del comando
 * Valor devuelto: mensaje */ 

void parse_escape_command(uint8_t message)
{
    switch(message){
        case 'A':
            current_command = UP_KEY_COMMAND;
            break;
        case 'B':
            current_command = DOWN_KEY_COMMAND;
            break;
        case 'C':
            current_command = RIGHT_KET_COMMAND;
            break;
        case 'D':
            current_command = LEFT_KEY_COMMAND;
            break;
        default:
            current_command = NO_COMMAND;
            break;
    }
}

/* Nombre: getCommand
 * Descripción: Devuelve el ultimo comando recibido y resetea el comando actual 
                (para no tomar 2 veces el mismo comando)
 * Argumentos: Ninguno
 * Valor devuelto: El Codigo numerico del ultimo comando recibido */ 

uint8_t getCommand(void)
{   
    uint8_t cmd = current_command;
    current_command = NO_COMMAND;
    return cmd; 
}

/* Nombre: cleanBuffer
 * Descripción: Resetea el buffer de recepcion 
                (utilizado por juegos que reciben comandos y en vez de codigos de operacion)
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void cleanBuffer (void)
{
    pbuffer = buffer;
}

// ------------------------------ FUNCIONES AUXILIARES ------------------------------------

/* Nombre: send_str
 * Descripción: Funcion para el envio de una cadena 
 * Argumentos: str - cadena que desea enviar
 * Valor devuelto: Ninguno */ 
void send_str(char* str)
{

    while(*str)
    {
        PonerEnColaTransmisionUART(*str++);
    }
    Transmite();
}

/* Nombre: send_int
 * Descripción: Funcion para el envio de un numero entero decimal 
 * Argumentos: i - numero a enviar
 * Valor devuelto: Ninguno */
void send_int(uint16_t i)
{
    static char str[6];
    itoa(str,i,10);
    send_str(str);
}

/* Nombre: send_hex
 * Descripción: Funcion para el envio de un numero entero hexadecimal 
 * Argumentos: i - numero a enviar
 * Valor devuelto: Ninguno */
void send_hex(uint16_t i)
{  
    static char str[6];
    send_str("0x");
    itoa(str,i,16);
    send_str(str);
}

/* Nombre: send_periodo
 * Descripción: Funcion para el envio del actual periodo entre efectos
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */
void send_periodo(void)
{
    send_str("Periodo = ");
    send_int(getPeriodo());
    PonerEnColaTransmisionUART('\r');
    PonerEnColaTransmisionUART('\n');
    Transmite();
}




