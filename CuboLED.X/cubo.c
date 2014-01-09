// Jose Javier Gonzalez Ortiz
// Libreria para el manejo del cubo LED
// cubo.c 

#include "cubo.h"

// Variables globales
uint8_t     voxel[N][N];

// ----------------------------------- PROTOTIPOS -----------------------------------------

void        cubeInit        (void);
void        init_ports      (void);
void        init_timer1     (void);

void        SPI_PinRemap    (void);
void        SPIinit         (void);
void        Refresh         (void);
void        loadLayer       (uint8_t layer);

void        EdgeDetect      (int signal);
void        init_ad         (int canales);
int         get_ad          (int canal);
void        PWMCube         (void);

// ----------------------------------- FUNCIONES -----------------------------------------

/* Nombre: cubeInit
 * Descripción: Funcion de incializacion completa del cubo LED
 * Argumentos: Nnguno
 * Valor devuelto: Ninguno */ 

void cubeInit(void)
{
    InicializarReloj();     // Inicializacion del Reloj del micro a 40 MHz
    RemapeaPerifericos();

    init_ports();           // Incializacion de los puertos

    init_timer1();           // Inicializacion del timer1 con interrupciones
    init_ad(0x20);          // Habilita el canal 5 del potenciometro
    InicializarUART();      // Inicializacion del modulo UART

    SPIinit();              // Inicializacion del SPI

    effect_init();          // Incializacion del diccionario de efectos

    init_snake();

    effect_quit();
}

/* Nombre: portInit
 * Descripción: Función de inicialización de los puertos del micro
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
void init_ports(void)
{
    
    TRISA = TRISA_Def;
    TRISB = TRISB_Def;
    AD1PCFGL = 0xFFFF;  // Configura los puertos analogicos como digitales

    PORTB = 0xF000;
    PORTA = 0x0000;
}

/* Nombre: init_timer1
 * Descripción: Función de inicialización del timer1
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void init_timer1(void)
{
    TMR1 = 0;
    PR1 = DMS;                                  // Configura a decimas de milisegundos
    IFS0bits.T1IF = 0;  //clear(IFS0,3)             // Borra el flag de interrupción
    IEC0bits.T1IE = 1;  //clear(IEC0,3)             // Habilita la interrupcion
    T1CON = 0x8000;                                 // arranca con pres = 1
}

/* Nombre: SPI_PinRemap
 * Descripción: Funcion especifica para el remapeado de puertos del SPI para la placa trainer de ICAI
 * Argumentos: Nnguno
 * Valor devuelto: Ninguno */ 

void SPI_PinRemap(void)
{
    // Unlock Registers
    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); 

    // Configure Input Functions
    // (See Table 30-1)

    // Assign SPI1 Data SDI to pin RP11 (RB11)
    RPINR20bits.SDI1R = 11;

    // Configure Output Functions
    // (See Table 30-2)
    
    // Assign SSDO1 (Serial Data Out) to pin RP10 (RB10)
    RPOR5bits.RP10R = 0x07; // 0b00111;
    //Asign SCK1OUT (Serial clock out) to pin RP7 (RB7)
    RPOR3bits.RP7R  = 0x08; //0b01000;
    // Assign SS1OUT (SPI1 Slave Select) to pin RP6 (RB6)
    // No se usa el SS como tal 
    RPOR3bits.RP6R  = 0x09; // 0b01001;

    // Lock Registers
    __builtin_write_OSCCONL(OSCCON | (1<<6));
}

/* Nombre: SPIInit
 * Descripción: Funcion de incializacion del SPI para la comunicacion entre el cubo y el microsegundos
 * Argumentos: Nnguno
 * Valor devuelto: Ninguno */ 

void SPIinit(void)
{

    //PORT Remap And Tristate configuration
    SPI_PinRemap();
    set(TRISB,11);      // SDI1
    clear(TRISB,10);    // SD01
    clear(TRISB,7);     // SCK
    clear(TRISB,6);     // SS
    _SS = 1;            // /Clear a 1

    
    //SPI1CON1
    IFS0bits.SPI1IF = 0;    // Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0;    // Disable the Interrupt

                            // SPI1CON1 Register Settings 
    SPI1CON1bits.DISSCK=0;  // InternalSerialClockisEnabled 
    SPI1CON1bits.DISSDO=0;  // SDO1 pin is controlled by the module
    SPI1CON1bits.MODE16=0;  // Communicationisword-wide(8bits)
    SPI1CON1bits.SMP = 0;   // Serial output data changes on transition
    SPI1CON1bits.CKE = 1; // 0;   // from Idle clock state to active clock state
    SPI1CON1bits.CKP = 0;   // Idle state for clock is a low level;
                            // active state is a high level

    SPI1CON1bits.SPRE = 000;// Secondary prescaler 8:1
    SPI1CON1bits.PPRE = 11; // Primary prescaler 1:1
                            // FCY = 40 MHz -> SPI 5MHz

    SPI1CON1bits.SSEN = 0;  // Se utiza para clear por lo que se deshabilita

    SPI1CON1bits.MSTEN = 1; // Master mode Enabled
    SPI1STATbits.SPIEN = 1; // Enable SPI module

                            // Interrupt Controller Settings
    IFS0bits.SPI1IF = 0;    // Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0;    // Enable the Interrupt
    
    
}

/* Nombre: Refresh
 * Descripción: Funcion que es llamada por la interrupcion cada TREFRESH decimas de ms
                Responsable de la multiplexacion , carga las capas una a una 
                mediante loadLayer 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
void Refresh(void)
{   
    static int ticks = 0;
    static uint8_t layer = 0;
    ticks++;
    if(ticks >= TREFRESH)
    {
        ticks = 0;
        // Se incrementa esta capa
        layer &= 0x07;
        loadLayer(layer);
        layer++;
        if(layer >= N)
            layer =0;
    }
    

}

/* Nombre: loadLayer
 * Descripción: Funcion de envio de datos por SPI
 * Argumentos: el identificador de la capa a enviar al cubo
 * Valor devuelto: Ninguno */ 

void loadLayer(uint8_t layer)
{
    static uint8_t i;

    // Clear de los shift registers

    _SS = 0;
    //delay_us(TCLEAR);
    for(i = 0 ; i < TCLEAR; i++);
    _SS = 1;
    
    // Envio del identificador de capa 
    
    while(SPI1STATbits.SPITBF);
    SPI1BUF = 0x01 << layer;

    for(i = 7 ; i <= 7 ; i--)
    {
        // Envio de la configuracion de capa
        while(SPI1STATbits.SPITBF);
        SPI1BUF = voxel[layer][i];
    } 
}

/* Nombre: EdgeDetect 
 * Descripción: Función de atencion al detector de flanco de signal
 * Argumentos: int signal -> La variable sobre la que actua el detector de flanco.
 * Valor devuelto: Ninguno */ 

void EdgeDetect(int signal)
{
    //Variables de Edge detector
    static uint8_t ant = 0;
    static uint8_t act = 0;
    static uint8_t _REdge = 1;     // Habilita las acciones en flanco de subida
    static uint8_t _FEdge = 0;     // Habilita las acciones en flanco de bajada

    ant = act;
    act = signal;
    if(ant != act){
        if(_REdge && act){
            //Accion en flanco de subida 
            effect_next();  
        }

        if(_FEdge && !act){
            //Accion en flanco de bajada
        }
    }
}

/* Nombre: init_ad 
 * Descripción: Función de inicializacion de los puertos analogicos para el conversor A/D
 * Argumentos: Canales a inicializar
 * Valor devuelto: mensaje */

void init_ad(int canales)
{

    TRISB |= canales & 0x3F >> 2 ;
    TRISA |= canales & 3 ;
    AD1PCFGL = ~ canales;
    AD1CON3 = 0x105;
    AD1CON2 = 0;
    AD1CON1 = 0x80E0;
}

/* Nombre: get_ad 
 * Descripción: Función de obtencion de lecturas analogicas del conversor A/D
 * Argumentos: Canal a obtener la lectura
 * Valor devuelto: Lectura del canal analogico */

int get_ad(int canal)
{

    AD1CHS0 = canal;
    AD1CON1bits.SAMP = 1;
    IFS0bits.AD1IF = 0;
    while (!IFS0bits.AD1IF);
    return ADC1BUF0;
}

/* Nombre: PWMCube 
 * Descripción: Función de atencion al Pulse Width Modulator del Cubo 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void PWMCube(void)
{
    //Variables de PWM1
    static unsigned int ticksPWMCube = 0;
    static unsigned int dutyPWMCube = 0;
    static unsigned int periodoPWMCube = 20;

    ticksPWMCube++;
    if(ticksPWMCube >= periodoPWMCube)
        ticksPWMCube = 0;
    if(ticksPWMCube < dutyPWMCube){
        //Accion a nivel alto

    }else{
        //Accion a nivel bajo
    }
    
}

// ----------------------------------- INTERRUPCIONES---------------------------------------
/* Nombre: _T1Interrupt
* Descripción: Rutina de atención a interrupción del timer1.
* Valor devuelto: Ninguno */

void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void) {

    // Monitor monolitico
    // TODO - Poner flags virtuales y desplazar el procesamiento fuerte al main

    IFS0bits.T1IF=0; //limpieza del flag

    EdgeDetect(SWITCH);
    watch_uart();

    effect_launcher();
    Refresh();
    
    
    
 }



