// Jose Javier Gonzalez Ortiz
// Libreria para la definicion del juego del snake3D en el cubo LED
// snake.c


#include "snake.h"

// ------------------------------------ GLOBALES ------------------------------------------

static point head,food;
static point snake[MAX_LENGTH];

static uint8_t game_over = false;
static point cmd2dir[NUM_COMMAND];
static uint8_t snake_length = MIN_LENGTH;


// ----------------------------------- PROTOTIPOS -----------------------------------------

void    init_snake      (void);
void    game_snake      (uint8_t reset);
void    effect_snake    (uint8_t reset);
void    set_food        (void);

// ----------------------------------- FUNCIONES ------------------------------------------

/* Nombre: init_snake
 * Descripción: Funcion de incializacion del diccionario para pasar de comandos a vectores de direccion
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno */

void init_snake(void)
{
    cmd2dir[FWD_COMMAND  ] = Point( 0,  1,  0);
    cmd2dir[REV_COMMAND  ] = Point( 0, -1,  0);
    cmd2dir[LEFT_COMMAND ] = Point(-1,  0,  0);
    cmd2dir[RIGHT_COMMAND] = Point( 1,  0,  0);
    cmd2dir[UP_COMMAND   ] = Point( 0,  0,  1);
    cmd2dir[DOWN_COMMAND ] = Point( 0,  0, -1);

}

/* Nombre: game_snake
 * Descripción: Juego del snake, formando por subefectos que se lanzan de forma secundaria
 * Argumentos: reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */

void game_snake(uint8_t reset)
{
    static uint8_t estado = 0;
    char str[2];

    if(reset)
    {
        estado = 0;

        return;
    }

    estado++;
    switch(estado)
    {
        case 1:
            setMessage("3 2 1");
            effect_launch_second(&font_effect_standard_push_message,6*6);
            break;
        case 2: 
            effect_launch_second(&effect_snake,-1);
            break;
        case 3:
            setMessage("GAME OVER");
            effect_launch_second(&font_effect_slide_message,(10)*N);
            break;
        case 4:
            itoa(str,snake_length-MIN_LENGTH,10);
            setMessage(str);
            effect_launch_second(&font_effect_broadway_message,-1);
            break;
        default:
            effect_quit();
            break;

    }   
}


/* Nombre: effect_snake
 * Descripción: efecto del snake como tal, llamado por game_snake
 * Argumentos: reset   - variable de reinicializacion del efecto
 * Valor devuelto: Ninguno */

void effect_snake(uint8_t reset)
{
    
    
    static point direction;
    uint8_t i;
    
    uint8_t command;

    if(reset)
    {
        clearCube();
        game_over = false;      // 
        snake_length = MIN_LENGTH;


        for(i = 0; i < snake_length; i--)
        {
            snake[i] = Point(X_0,Y_0,Z_0+snake_length-1-i);
            setPoint(snake[i]);
            // TODO averiguar porque solo se enciende la cabeza
        }
        direction = cmd2dir[UP_COMMAND];            // direccion +Z
        set_food();
        
        setFactor(8);                               // Para que sea manejable se ralentiza
        
        echo = false;

        return;

    }

    
        
    command = getCommand();
    if(command != NO_COMMAND)     
    {
        direction = cmd2dir[command];
    }
    // Obtiene la nueva posicion de la cabeza 
    head = sumPoints(snake[0],direction);
    //Comprueba que no se haya salido por las paredes
    if(!inrange(head.x) || !inrange(head.y) || !inrange(head.z))
    {
        game_over = true;
        
    }

    if(getPoint(head))              
    {
        if(pointEquals(head,food))                  //Si alcanza la comida
        {
            snake_length++;
            if(snake_length % 10 == 0)              // Cada 10 aumenta la velocidad
            {
                setFactor(8-snake_length/10);
            }

            set_food();                             // Coloca de forma aleatoria la comida

        }else{                                      // Si choca consigo misma
             game_over = true;
        }
       
    }else{                                          // Else, Hay que desplazar la cola
        setPoint(head);     
        clearPoint(snake[snake_length-1]);
    }

    for(i = snake_length-1; i > 0; i--)
    {
        snake[i] = snake[i-1];
    }
    snake[0] = head;

    setPoint(food);         // TODO Se debe poner debido a un error que hacia que en algunos casos la food no se iluminase
    
    // Si se ha acabado el juego
    if(game_over)
    {
        echo = true;
        cleanBuffer();
        setPeriodo(1000);
        effect_second_quit();
    }
}

/* Nombre: set_food
 * Descripción: Funcion auxiliar para obtener la posicion de una nueva comida
 * Argumentos:  Ninguno
 * Valor devuelto: Ninguno */ 

void set_food(void)
{
    do{
        food = getRandomPoint();
        if(!getPoint(food))   //Si esta apagado es posible
        {   
            if(count_neighboors(food.x,food.y,food.z)==0)
            {
                setPoint(food);
                return; 
            }  
        }
    }while(!getPoint(food));  //Mientras este apagado

}
