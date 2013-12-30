// Jose Javier Gonzalez Ortiz
// Libreria de driver para el micro dsPIC33FJ32MC202
// driver.c 

#include "driver.h"
// ----------------------------------- PROTOTIPOS -----------------------------------------
void mainInit(void);
void setDigitalPorts(void);
void initTimer1(void);
void delay(int ms);
void delay_us(int us);
void vTimer1(void);
void vTimer2(void);
void PWM1(void);
void EdgeDetect(int signal);
char watchUART(void);
void init_ad(int canales );
int get_ad(int canal);

// ----------------------------------- FUNCIONES -----------------------------------------

//Variables de virtual timers
static unsigned int ticksVT1 = 0;
static unsigned int periodoVT1 = 2000;

static unsigned int ticksVT2 = 0;
static unsigned int periodoVT2 = 7919;

static int effect = 2;
static int Neffect = 7;

/* Nombre: initTimer1
 * Descripción: Función de inicialización de los distintos perifericos del micro.
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
void mainInit(void){

    InicializarReloj();
    RemapeaPerifericos();

    TRISA = TRISA_Def;
    TRISB = TRISB_Def;
    AD1PCFGL = 0xFFFF;

    PORTB = 0xF000;
    PORTA = 0x0000;

    initTimer1();
    InicializarUART();

    init_ad(0x20);


}


/* Nombre: setDigitalPorts
 * Descripción: Función de configuración de los puertos analógicos como digitales (Para leer los pulsadores)
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void setDigitalPorts(void){
    AD1PCFGL = 0xFFFF;  // Configura los puertos analogicos como digitales
}


/* Nombre: initTimer1
 * Descripción: Función de inicialización del timer1
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void initTimer1(void){
    TMR1 = 0;
    PR1 = DMS;                                  // Configura a decimas de milisegundos
    IFS0bits.T1IF = 0;  //clear(IFS0,3)             // Borra el flag de interrupción
    IEC0bits.T1IE = 1;  //clear(IEC0,3)             // Habilita la interrupcion
    T1CON = 0x8000;                                 // arranca con pres = 1
}

/* Nombre: delay
 * Descripción: Función de espera en modo bloqueado
 * Argumentos: int ms -> la cantidad de milisegundos a esperar
 * Valor devuelto: Ninguno */ 

void delay(int ms){
    int i;
    TMR1=0; 
    PR1= MS; 
    T1CON=0x8000; 
    IFS0bits.T1IF=0;    //clear(IFS0,3)

    for(i = 0 ; i < ms ; i++)
    {
        while(IFS0bits.T1IF == 0); 
        IFS0bits.T1IF = 0;
    }
}

/* Nombre: delay
 * Descripción: Función de espera en modo bloqueado
 * Argumentos: int ms -> la cantidad de milisegundos a esperar
 * Valor devuelto: Ninguno */ 

void delay_us(int us){
    int i;
    TMR1=0; 
    PR1= US; 
    T1CON=0x8000; 
    IFS0bits.T1IF=0;    //clear(IFS0,3)

    for(i = 0 ; i < us ; i++)
    {
        while(IFS0bits.T1IF == 0); 
        IFS0bits.T1IF = 0;
    }
}



/* Nombre: VTimer1 
 * Descripción: Función de atencion al timer virtual 1 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void vTimer1(void){
    
    static int i = 0, j = 0;
    const static uint8_t sin[32] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 0, -1, -1, -2, -2, -2, -3, -3, -3, -3, -3, -2, -2, -2, -1, -1};
    //const static uint8_t sin[32] = {4, 5, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 5, 4, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 3};

    static  char mensaje[13] = {"THE GAME "};

    ticksVT1++;
    periodoVT1 = 2*get_ad(5)+100;
    if(ticksVT1 >= periodoVT1){
        ticksVT1 = 0;
        //Accion del vTimer1
        switch(effect)
        {  
//1         
            case 1:
                effect_crossing_piramids(false);
                
                break;  
//2             
            case 2:
                clearCube();
                i++;
                if(i>=5800)
                {
                    i = 0;
                }
                //putPlane(X,7-i,ASCII[mensaje[j]-'A']);
                // if(mensaje[j]==' ' || mensaje[j] == 0)
                    // clearCube();
                //
                //effect_broadway_message(mensaje,true);
                //putPlane(X,3,greek[i/100]);
                //putAscii(X,0,'J');
                // putAscii(Y,0,'J');
                // putAscii(Z,7,'J');
                putFont(Z,0,hiragana[2]);
                break;
                
//3
            case 3:
                // j++;
                // if(j >= 32)
                //     j=0;
                // clearCube();
                // for(i=0;i<N;i++){
                //     //setVoxel(sin[(j+i)&0x1F]+3,i,0);
                //     setVoxel(sin[(j+i+8)&0x1F]+3,sin[(j+i)&0x1F]+3,1);
                // }

                //effect_push_message(mensaje,Y,4,false);
                effect_slide_message(mensaje,false);
                //effect_broadway_message(mensaje,false);
                break;

            case 4: 
                
                effect_animate_cube(false);
                break;

            case 5:
                i++;
                if(i >= N)
                    i = 0;
                clearCube();
                setLayer(i);
                break;

            case 6:
                //putPlane(Z,0,font8x8[0x41+i/100]);
                //clearCube();
                // i++;
                // if(i >= N)
                //     i = 0;
                effect_rain(false);
                break;
            case 7:
                i++;
                if(i >= 22){
                    i = 0;
                    clearCube();
                }
                set_oblique(i);
                break;

            default:
                break;

//4     
        // j = getAxis(X,0,0);
        // putAxis(X,0,0,getAxis(Y,0,0));
        // putAxis(Y,0,0,j);
        }
                

    }

}

/* Nombre: VTimer2 
 * Descripción: Función de atencion al timer virtual 2 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void vTimer2(void){
    ticksVT2++;
    if(ticksVT2 >= periodoVT2){
        ticksVT2 = 0;
        //Accion del vTimer2
    }

}

/* Nombre: PWM1 
 * Descripción: Función de atencion al Pulse Width Modulator 1 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void PWM1(void){
    //Variables de PWM1
    static unsigned int ticksPWM1 = 0;
    static unsigned int dutyPWM1 = 0;
    static unsigned int periodoPWM1 = 20;

    ticksPWM1++;
    if(ticksPWM1 >= periodoPWM1)
        ticksPWM1 = 0;
    if(ticksPWM1 < dutyPWM1){
        //Accion a nivel alto
        setLayer(0);
    }else{
        //Accion a nivel bajo
        clearLayer(0);
    }
    
}

/* Nombre: EdgeDetect 
 * Descripción: Función de atencion al detector de flanco de signal
 * Argumentos: int signal -> La variable sobre la que actua el detector de flanco.
 * Valor devuelto: Ninguno */ 

void EdgeDetect(int signal){
    //Variables de Edge detector
    static unsigned int ant = 0;
    static unsigned int act = 0;
    static unsigned int _REdge = 1;     // Habilita las acciones en flanco de subida
    static unsigned int _FEdge = 0;     // Habilita las acciones en flanco de bajada

    ant = act;
    act = signal;
    if(ant != act){
        if(_REdge && act){
            //Accion en flanco de subida
            effect++;
            clearCube();
            if(effect >= Neffect+1)
                effect = 1;
        }

        if(_FEdge && !act){
            //Accion en flanco de bajada
        }
    }
}

/* Nombre: watchUART 
 * Descripción: Función de atencion al modulo UART
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

char watchUART(void){
    static int mensaje;
    if (HayAlgoRecibido()) {
        mensaje = SacarDeColaRecepcionUART();
            //Operar Mensaje
        PonerEnColaTransmisionUART(mensaje);        //Realiza el eco de lo recibido
        Transmite();
    }
    return mensaje;
}

void init_ad(int canales ){

    TRISB |= canales & 0x3F >> 2 ;
    TRISA |= canales & 3 ;
    AD1PCFGL = ~ canales;
    AD1CON3 = 0x105;
    AD1CON2 = 0;
    AD1CON1 = 0x80E0;
}

int get_ad(int canal){

    AD1CHS0 = canal;
    AD1CON1bits.SAMP = 1;
    IFS0bits.AD1IF = 0;
    while (!IFS0bits.AD1IF);
    return ADC1BUF0;
}


// ----------------------------------- INTERRUPCIONES---------------------------------------
/* Nombre: _T1Interrupt
* Descripción: Rutina de atención a interrupción del timer1.
* Valor devuelto: Ninguno */

void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF=0; //limpieza del flag
    
    Refresh();
    vTimer1();
    
    vTimer2();

    //PWM1();

    EdgeDetect(SWITCH);

    // watchUART();


 }