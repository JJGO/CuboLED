// Jose Javier Gonzalez Ortiz
// Libreria para el control de ejecucion de efectos en el cubo mediante el modulo UART
// serial.c

#include "serial.h"

static peffect effects[EFFECTS_NUM];


// ----------------------------------- PROTOTIPOS -----------------------------------------
void initEffects(void);
char watch_uart(void);
void parse_message(char* code);
void send_periodo(void);
void parse_effect(char* code);

// ----------------------------------- FUNCIONES ------------------------------------------

void initEffects(void)
{   
    uint8_t i;
    for(i=0; i<EFFECTS_NUM; i++)
    {
        effects[i] = &effect_empty;
    }
    effects[10] = &effect_animate_cube;
    effects[11] = &effect_expand_cube;
    effects[12] = &effect_rain;
    effects[13] = &effect_crossing_piramids;

}


/* Nombre: watch_uart 
 * Descripción: Función de atencion al modulo UART
 * Argumentos: Ninguno
 * Valor devuelto: mensaje */ 

char watch_uart(void)
{
    static int mensaje;
    static int i = 0;
    static char buffer[64];
    if (HayAlgoRecibido()) {
        mensaje = SacarDeColaRecepcionUART();
            //Operar Mensaje
        PonerEnColaTransmisionUART(mensaje);        //Realiza el eco de lo recibido
        send_int(mensaje);
        Transmite();
        if(mensaje == 0x08) // Backspace
        {
            if(i > 0)
            {
                i--;
            }
        }
        else if(mensaje == '\r')
        {
            PonerEnColaTransmisionUART('\n');
            Transmite();
            buffer[i] = '\0';
            i = 0;
            parse_message(buffer);
        }else{
            buffer[i++] = mensaje;
            if(i>64)
                i = 0;
        }
    }
    return mensaje;
}

void parse_message(char* code)
{   

    //Comprueba que sea un efecto
    switch(code[0])
    {
        case 'Q':
            effect_quit();
            break;
        case 'N':
            //effect_next();
            break;
        case 'P':
            if(code[1] == 0){
                send_periodo();
            }else{
                setPeriodo(atoi(code+1));
            }
            break;
        case 'E':
            parse_effect(code);
            break;
    }
}

void send_int(uint8_t i)
{

    static const char mensaje[11] = {"ASCII = "};
    char periodoStr[64],*periodoPtr;
    
    PonerEnColaTransmisionUART('\r');
    PonerEnColaTransmisionUART('\n');

    periodoPtr = mensaje;
    while(*periodoPtr)
    {
        PonerEnColaTransmisionUART(*periodoPtr++);
    }
    itoa(periodoStr,i,10);
    periodoPtr = periodoStr;
    while(*periodoPtr)
    {
        PonerEnColaTransmisionUART(*periodoPtr++);
    }
    PonerEnColaTransmisionUART('\r');
    PonerEnColaTransmisionUART('\n');
    Transmite();
}

void send_periodo(void)
{
    static const char mensaje[11] = {"Periodo = "};
    char periodoStr[64],*periodoPtr;
  
    periodoPtr = mensaje;
    while(*periodoPtr)
    {
        PonerEnColaTransmisionUART(*periodoPtr++);
    }
    itoa(periodoStr,(int)getPeriodo(),10);
    periodoPtr = periodoStr;
    while(*periodoPtr)
    {
        PonerEnColaTransmisionUART(*periodoPtr++);
    }
    PonerEnColaTransmisionUART('\r');
    PonerEnColaTransmisionUART('\n');
    Transmite();
}

void parse_effect(char* code)
{
    uint8_t effect_id = atoi(code+1);            
    effect_launch(effects[effect_id]);
}



