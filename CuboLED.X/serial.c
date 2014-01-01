// Jose Javier Gonzalez Ortiz
// Libreria para el control de ejecucion de efectos en el cubo mediante el modulo UART
// serial.c

#include "serial.h"

static peffect effects[NUM_EFFECTS];

// ------------------------------------ GLOBALES ------------------------------------------

static uint8_t echo = true;
static uint8_t current_command = 0;

// ----------------------------------- PROTOTIPOS -----------------------------------------
void    initEffects     (void);
char    watch_uart      (void);
void    parse_message   (char* code);
void    send_periodo    (void);
void    parse_effect    (char* code);
void    parse_command   (uint8_t message);
uint8_t getCommand      (void);

// ----------------------------------- FUNCIONES ------------------------------------------

void initEffects(void)
{   
    uint8_t i;

    for(i=0; i<NUM_EFFECTS; i++)
    {
        effects[i] = &effect_empty;
    }
    effects[10] = &effect_animate_cube;
    effects[11] = &effect_expand_cube;
    effects[12] = &effect_rain;
    effects[13] = &effect_crossing_piramids;
    effects[14] = &effect_sweep_plane;
    effects[15] = &effect_random_move_axis;

    effects[20] = &font_effect_standard_push_message; 
    effects[21] = &font_effect_broadway_message;      
    effects[22] = &font_effect_slide_message;         

}


/* Nombre: watch_uart 
 * Descripción: Función de atencion al modulo UART
 * Argumentos: Ninguno
 * Valor devuelto: mensaje */ 

char watch_uart(void)
{
    static int mensaje;
    static int i = 0;
    static char buffer[BUFFER_SIZE];
     
    if (HayAlgoRecibido()) {
        mensaje = SacarDeColaRecepcionUART();
            //Operar Mensaje

        if(echo)
        {
            PonerEnColaTransmisionUART(mensaje);        //Realiza el eco de lo recibido
            Transmite();
        }

        if(mensaje == 0x08) // Backspace
        {
            if(i > 0)
            {
                i--;
            }
        }
        else if(mensaje == '\r') //Nueva linea
        {
            PonerEnColaTransmisionUART('\n');
            Transmite();
            buffer[i] = '\0';
            i = 0;
            parse_message(buffer);
        }else{
            buffer[i++] = mensaje;
            if(i>BUFFER_SIZE)
                i = 0;
        }
        parse_command(mensaje);
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
        case EFFECT_ID:
            parse_effect(code);
            break;
        case FONT_ID:
            break;
        case GAMES_ID:
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
        case 'o':
        case 'O':
            current_command = UP_COMMAND;
            break;
        case 'l':
        case 'L':
            current_command = DOWN_COMMAND;
            break;
        default:
            current_command = NO_COMMAND;
            break;
    }
}

uint8_t getCommand(void)
{   
    uint8_t cmd = current_command;
    current_command = NO_COMMAND;
    return cmd; 
}


