// Jose Javier Gonzalez Ortiz
// Libreria para el control de ejecucion de efectos en el cubo mediante el modulo UART
// parser.c

#include "parser.h"

static peffect effects[EFFECTS_NUM];


// ----------------------------------- PROTOTIPOS -----------------------------------------
void initEffects(void);
void parse_message(char* code);
void send_periodo(void);
void parse_effect(char* code);

// ----------------------------------- FUNCIONES ------------------------------------------

void initEffects(void)
{
    effects[10] = &effect_animate_cube;
    effects[11] = &effect_expand_cube;
    effects[12] = &effect_rain;
    effects[13] = &effect_crossing_piramids;

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
            //send_periodo();
        case 'E':
            parse_effect(code);
            break;
    }
}
void send_periodo(void)
{
    char periodoStr[64],*periodoPtr;
    itoa(periodoStr,(int)getPeriodo(),10);              // !!!!!!!!!
    periodoPtr = periodoStr;
    while(*periodoPtr)
    {
        PonerEnColaTransmisionUART(*periodoPtr++);
        Transmite();
    }
}

void parse_effect(char* code)
{
    uint8_t effect_id = atoi((const char*)code[1]);             // !!!!!!!!!
    effect_launch(effects[effect_id]);
}

