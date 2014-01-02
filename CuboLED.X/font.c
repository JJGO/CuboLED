// Jose Javier Gonzalez Ortiz
// Libreria para el manejo de caracteres alfanumericos en el cubo LED
// font.c


#include "font.h"

static char message[MESSAGE_SIZE] = {"HELLO WORLD   "};

// ----------------------------------- PROTOTIPOS -----------------------------------------

void    putFont                             (uint8_t dim,   uint8_t coord,  uint8_t* character);
void    putAscii                            (uint8_t dim,   uint8_t coord,  char c);
void    setMessage                          (char* str);

void    font_effect_push_message            (uint8_t dim,    uint8_t space, uint8_t reset);
void    font_effect_standard_push_message   (uint8_t reset);
void    font_effect_broadway_message        (uint8_t reset);
void    font_effect_slide_message           (uint8_t reset);


uint8_t getColumnFont                       (uint8_t* c,    uint8_t column);


// ----------------------------------- FUNCIONES ------------------------------------------


/* Nombre: putFont
 * Descripción: Funcion de activacion de un plano perpendicular al eje (X,Y,Z) del cubo con el caracter correspondiente
 * Argumentos:  dim - dimension del plano 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - coordenada del plano  
                    X -> coordenada en X
                    Y -> coordenada en Y
                    Z -> Coordenanda en Z
                character - mapa bidimensional de constantes que respresentar el caracter
                    
 * Valor devuelto: Ninguno */ 


void putFont(uint8_t dim,  uint8_t coord, uint8_t* character)
{
    uint8_t y,z;
    if(dim == Y || dim == Z){   
        putPlane(dim,coord,character);
    }
    else if(dim == X)
    {
        for(z = 0; z < N; z++)
        {
            for(y = 0; y < N; y++)
            {
                putVoxel(coord,y,z,test(character[z],(N-1-y)));
            }
        }
    }
}



/* Nombre: putAscii
 * Descripción: Funcion de activacion de un plano perpendicular al eje (X,Y,Z) del cubo con el caracter ascii correspondiente
 * Argumentos:  dim - dimension del plano 
                    X -> X
                    Y -> Y
                    Z -> Z
                coord1 - coordenada del plano  
                    X -> coordenada en X
                    Y -> coordenada en Y
                    Z -> Coordenanda en Z
                c - caracter ascii a representar
                    
 * Valor devuelto: Ninguno */ 


void putAscii(uint8_t dim, uint8_t coord, char c)
{
    if(c >= ASCII_OFFSET)
        putFont(dim,coord,ascii[c-ASCII_OFFSET]);
}

/* Nombre: font_effect_push_message
 * Descripción: Efecto de empujar un mensaje a traves del cubo
 * Argumentos:  dim     - dimension en la que enviar el mensaje (X,Y,Z)
                space   - separacion entre las letras
                reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */ 

void font_effect_push_message(uint8_t dim, uint8_t space,uint8_t reset)
{
    static uint8_t i = 0,c=0;

    if(reset)
    {
        i = c = 0;
        clearCube();
        return;
        //putAscii(dim,N-1,message[c]);
    }
    
    if(dim != Z){
        shiftCube(dim,true,false);
        fillPlane(dim,7,0x00);
    }else{
        shiftCube(dim,false,false);
        fillPlane(dim,0,0x00);
    }
    
    i++;
    if(i > space){
        i = 0;
        if(message[c] == '\0'){
            c = 0;
        }
        putAscii(dim,dim != Z ? N-1 : 0,message[c++]);
    }
    

}

/* Nombre: setMessage
 * Descripción: Funcion auxiliar para modificar el mensaje que sacan los efectos de cadena font_effect_
 * Argumentos:  str - nueva cadena a copiar en el mensaje
 * Valor devuelto: Ninguno */ 


void setMessage (char* str)
{
    uint8_t l = strlen(str);
    strcpy(message,str);
    message[l] = ' ';       // termianting character se pasa a espacio
    message[l+1] = '\0';    // Se añade un termianting character 
}

// ------------------------------------ EFECTOS -------------------------------------------

/* Nombre: font_effect_push_standard_message
 * Descripción: Efecto de empujar un mensaje a traves del cubo
 * Argumentos:  reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */ 

void font_effect_standard_push_message(uint8_t reset)
{
    font_effect_push_message(Y,5,reset);
}

/* Nombre: font_effect_broadway_message
 * Descripción: Efecto de retransimitir por las caras X=0, Y=0 y X= N-1 un mensaje
 * Argumentos: reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */ 

void font_effect_broadway_message(uint8_t reset)
{
    const uint8_t bufferSize = 22;
    static uint8_t buffer[22];
    static uint8_t count = 0,letter = 0;
    uint8_t i;

    if(reset)
    {   
        count = letter = 0;
        for(i = 0; i < bufferSize; i++)
            buffer[i] = 0x00;
        clearCube();
        return;
    }

    // Desplaza el buffer
    for(i = bufferSize-2; i < bufferSize; i--){
        buffer[i+1] = buffer[i];
    }

    //Comprueba el cambio de letra y reinicializacion del mensaje
    if(count >= N){
        count = 0;
        letter++;
        if(message[letter] == '\0')
            letter = 0;
    }

    buffer[0] = getColumnFont(ascii[(int)message[letter]-ASCII_OFFSET],count++);

   

    //COPY TO CUBE
    // Cara X=0
    for(i = 0; i < N ; i++)
    {
        putAxis(Z,N-1-i,0,buffer[i]);
    }
    // Cara Y=0
    for(i = 1; i < N; i++)
    {
        putAxis(Z,0,i,buffer[i+7]);
    }
    // Cara X = N-1
    for(i = 1; i < N; i++)
    {
        putAxis(Z,i,N-1,buffer[i+14]);
    }
    // Cara Y = N-1
    // for(i = 1; i < N-1; i++)
    // {
    //     putAxis(Z,N-1-i,0,buffer[i+21]);
    // }
}

/* Nombre: font_effect_slide_message
 * Descripción: Efecto de mostrar de forma lateral un mensaje en los planos Y = 3 e Y = 4
 * Argumentos:  reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */ 

void font_effect_slide_message(uint8_t reset)
{
    static uint8_t buffer[8];
    static uint8_t count = 0,letter = 0;
    uint8_t i;

    if(reset)
    {   
        count = letter = 0;
        for(i = 0; i < N; i++)
            buffer[i] = 0x00;
        clearCube();
        return;
    }

    for(i = N-2; i < N; i--){
        buffer[i+1] = buffer[i];
    }

    if(count >= N){
        count = 0;
        letter++;
        if(message[letter] == '\0')
            letter = 0;
    }

    buffer[0] = getColumnFont(ascii[(int)message[letter]-ASCII_OFFSET],count++);

    for(i = 0; i < N ; i++)
    {
        putAxis(Z,N-1-i,3,buffer[i]);
        putAxis(Z,N-1-i,4,buffer[i]);
    }

}




// ---------------- FUNCIONES AUXILIARES PARA LA CREACION DE EFECTOS ----------------------

/* Nombre: getColumnFont
 * Descripción: Funcion para obtener las columnas de las fuentes (necesario para effect_broadway_message)
 * Argumentos:  c       - Fuente del tipo char[8]
                column  - columna a obtener
 * Valor devuelto: config - configuracion de la columna pedida */ 

uint8_t getColumnFont(uint8_t* c, uint8_t column)
{
    uint8_t config = 0,i;
    for(i = 0; i < N; i++)
    {
        if(test(c[i],column)){
            set(config,i);
        }
    }
    return config;
}


