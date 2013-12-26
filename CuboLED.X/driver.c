// Jose Javier Gonzalez Ortiz
// Libreria de driver para el micro dsPIC33FJ32MC202
// driver.c 

#include "driver.h"

// ----------------------------------- FUNCIONES -----------------------------------------

static int effect = 1;
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
    
    static int i = 0, j = 0, d = 0, z=0;
    const static unsigned char sin[32] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 0, -1, -1, -2, -2, -2, -3, -3, -3, -3, -3, -2, -2, -2, -1, -1};
    //const static unsigned char sin[32] = {4, 5, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 5, 4, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 3};
    const static unsigned char J[8] = {0x00, 0x1E, 0x33, 0x33, 0x30, 0x30,0x30 ,  0x78};
    const static unsigned char ASCII[26][8]={
    {0x00, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x78, 0x30},  // U+0041 (A)
    {0x00, 0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC},  // U+0042 (B)
    {0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C},  // U+0043 (C)
    {0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8},  // U+0044 (D)
    {0x00, 0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE},  // U+0045 (E)
    {0x00, 0xF0, 0x60, 0x68, 0x78, 0x68, 0x62, 0xFE},  // U+0046 (F)
    {0x00, 0x3E, 0x66, 0xCE, 0xC0, 0xC0, 0x66, 0x3C},  // U+0047 (G)
    {0x00, 0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC},  // U+0048 (H)
    {0x00, 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78},  // U+0049 (I)
    {0x00, 0x78, 0xCC, 0xCC, 0x0C, 0x0C, 0x0C, 0x1E},  // U+004A (J)
    {0x00, 0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6},  // U+004B (K)
    {0x00, 0xFE, 0x66, 0x62, 0x60, 0x60, 0x60, 0xF0},  // U+004C (L)
    {0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0xFE, 0xEE, 0xC6},  // U+004D (M)
    {0x00, 0xC6, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6},  // U+004E (N)
    {0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38},  // U+004F (O)
    {0x00, 0xF0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xFC},  // U+0050 (P)
    {0x00, 0x1C, 0x78, 0xDC, 0xCC, 0xCC, 0xCC, 0x78},  // U+0051 (Q)
    {0x00, 0xE6, 0x66, 0x6C, 0x7C, 0x66, 0x66, 0xFC},  // U+0052 (R)
    {0x00, 0x78, 0xCC, 0x1C, 0x70, 0xE0, 0xCC, 0x78},  // U+0053 (S)
    {0x00, 0x78, 0x30, 0x30, 0x30, 0x30, 0xB4, 0xFC},  // U+0054 (T)
    {0x00, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC},  // U+0055 (U)
    {0x00, 0x30, 0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC},  // U+0056 (V)
    {0x00, 0xC6, 0xEE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6},  // U+0057 (W)
    {0x00, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0xC6},  // U+0058 (X)
    {0x00, 0x78, 0x30, 0x30, 0x78, 0xCC, 0xCC, 0xCC},  // U+0059 (Y)
    {0x00, 0xFE, 0x66, 0x32, 0x18, 0x8C, 0xC6, 0xFE},  // U+005A (Z)
    
    };

    const static unsigned char mensaje[12] = {"JOSE JAVIER"};
    static unsigned int lenMensaje = 12;


    ticksVT1++;
    periodoVT1 = 2*get_ad(5)+100;
    if(ticksVT1 >= periodoVT1){
        ticksVT1 = 0;
        //Accion del vTimer1
        switch(effect)
        {  
//1         
            case 1:
                // clearCube();

                // j++;
                // if(j >= 12)
                //     j=0;
                // z=j;
                // for(i=1;i<=4;i++)
                // {
                //     z = j-(i-1);
                //     if(z<0)
                //     {
                //         z=0;
                //     }else if(z>7){
                //         z=7;
                //     }
                //     ring(i,z);
                //     ring(i,7-z);
                // }
                
                // break;  
//2             
            case 2:
                clearCube();
                i++;
                if(i>=8)
                {
                    i = 0;
                    j++;
                    if(j >= lenMensaje)
                        j=0;

                }
                putPlane(X,7-i,ASCII[mensaje[j]-'A']);
                if(mensaje[j]==' ' || mensaje[j] == 0)
                    clearCube();
                break;
                
//3
            case 3:
                j++;
                if(j >= 32)
                    j=0;
                //putAxis(Y,0,0,0x00);
                //setVoxel(0,sin[j]+4,0);
                //putAxis(X,0,0,0x00);
                //setVoxel(sin[(j+8)&0x1F]+4,0,0);
                //clearLayer(0);
                clearCube();
                // setVoxel(sin[(j+8)&0x1F]+3,sin[j]+3,0);
                // setVoxel(sin[j]+3,sin[(j+8)&0x1F]+3,1);
                for(i=0;i<N;i++){
                    //setVoxel(sin[(j+i)&0x1F]+3,i,0);
                    setVoxel(sin[(j+i+8)&0x1F]+3,sin[(j+i)&0x1F]+3,1);
                }
                break;

            case 4: 
                
                // animateCube(ticksVT2);
                // break;

            case 5:
                i++;
                if(i >= N)
                    i = 0;
                clearCube();
                setLayer(i);
                break;

            case 6:
                //putPlane(Z,0,font8x8[0x41+i/100]);
                // clearCube();
                // i++;
                // if(i >= N)
                //     i = 0;
                clearCube();
                putPlane(Z,4,J);
                setVoxel(7,0,3);
                break;
            case 7:
                // i++;
                // if(i >= 22)
                //     i = 0;
                // clearCube();
                // setOblique(i);
                // break;

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

/* Nombre: VTimer3
 * Descripción: Función de atencion al timer virtual 3 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void vTimer3(void){
    ticksVT3++;
    if(ticksVT3 >= periodoVT3){
        ticksVT3 = 0;
        //Accion del vTimer3
    }

}

/* Nombre: PWM1 
 * Descripción: Función de atencion al Pulse Width Modulator 1 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 

void PWM1(void){

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

/* Nombre: PWM2 
 * Descripción: Función de atencion al Pulse Width Modulator 2 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
 
void PWM2(void){
    ticksPWM2++;
    if(ticksPWM2 >= periodoPWM2)
        ticksPWM2 = 0;
    if(ticksPWM2 < dutyPWM2){
        //Accion a nivel alto
    }else{
        //Accion a nivel bajo
    }
    
}

/* Nombre: PWMPer1 
 * Descripción: Función de atencion al Pulse Width Percentage Modulator 1 
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
void PWMPer1(int percentage){
    ticksPWMPer1++;
    if(ticksPWMPer1 >= periodoPWMPer1)
        ticksPWMPer1 = 0;

    // Se usa el cast a long para evitar la perdida de precision en la operacion
    dutyPWMPer1 = ((long)periodoPWMPer1*percentage)/100; 
    if(ticksPWMPer1 < dutyPWMPer1){
        //Accion a nivel alto
    }else{
        //Accion a nivel bajo
    }
    
}

/* Nombre: PWMPer2
 * Descripción: Función de atencion al Pulse Width Percentage Modulator 2
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */ 
void PWMPer2(int percentage){
    ticksPWMPer2++;
    if(ticksPWMPer2 >= periodoPWMPer2)
        ticksPWMPer2 = 0;

    // Se usa el cast a long para evitar la perdida de precision en la operacion
    dutyPWMPer2 = ((long)periodoPWMPer2*percentage)/100; 
    if(ticksPWMPer2 < dutyPWMPer2){
        //Accion a nivel alto
    }else{
        //Accion a nivel bajo
    }
    
}


/* Nombre: EdgeDetect 
 * Descripción: Función de atencion al detector de flanco de signal
 * Argumentos: int signal -> La variable sobre la que actua el detector de flanco.
 * Valor devuelto: Ninguno */ 

void EdgeDetect(int signal){
    static unsigned int ant = 0;
    static unsigned int act = 0;
    ant = act;
    act = signal;
    if(ant != act){
        if(_REdge && act){
            //Accion en flanco de subida
            effect++;
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
    
    //vTimer2();
    // vTimer3();

    //PWM1();
    
    // PWM2();

    // PWMPer1(50);
    // PWMPer2(50);

    //EdgeDetect(SWITCH);

    // watchUART();


 }


 // ----------------------------------FUNCION SETUP -------------------------------



// // ----------------------------------------------------------------------------------
// // ----------------------------- BITS DE CONFIGURACI?N ------------------------------
// // ----------------------------------------------------------------------------------

// // 1. Eliminar el segmento de arranque flash
// // 2. Permitir la escritura del segmento de arranque flash
// _FBS(BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF);

// // 1. No proteger la memoria de programa contra escritura
// // 2. No proteger el cÛdigo
// _FGS(GWRP_OFF & GCP_OFF);

// // 1. Utilizar el oscilador interno (FRC) en el arranque
// // 2. Arrancar directamente con el oscilador seleccionado
// _FOSCSEL(FNOSC_FRC & IESO_OFF);

// // 1. Permitir la conmutaciÛn del reloj y deshabilitar la monitorizaciÛn de fallos
// // 2. Desactivar el oscilador primario
// // 3. Utilizar el oscilador secundario como entrada y salida digital
// // 4. Permitir m?ltiples remapeos de los pines
// _FOSC(FCKSM_CSECMD & POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF);

// // 1. Permitir la deshabilitaciÛn del watchdog timer
// //    poniendo a 0 el bit SWDTEN del registro RCON
// _FWDT(FWDTEN_OFF);

// // 1. Esperar 128 ms y resetear el microcontrolador al enchufar la alimentaciÛn
// // 2. Controlar los pines de PWM desde el registro PORT al arrancar
// // 3. Los pines PWM high est·n activos a nivel alto
// // 4. Los pines PWM low est·n activos a nivel alto
// // 5. Utilizar los pines est·ndar (SDA1 y SCL1) para el I2C
// _FPOR(FPWRT_PWR128 & PWMPIN_ON & HPOL_ON & LPOL_ON & ALTI2C_OFF);

// // 1. Programar y depurar a travÈs de los pines PGC1 y PGD1
// // 2. Desactivar el interfaz para JTAG
// _FICD(ICS_PGD1 & JTAGEN_OFF);
