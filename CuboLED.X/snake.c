// Jose Javier Gonzalez Ortiz
// Libreria para la definicion del juego del snake3D en el cubo LED
// snake.c


#include "snake.h"

#define GOM_LENGTH  13

// ------------------------------------ GLOBALES ------------------------------------------

static point head,food;
static point snake[MAX_LENGTH];

static uint8_t game_over = false;
static point cmd2dir[NUM_COMMAND];


// ----------------------------------- PROTOTIPOS -----------------------------------------

void    init_snake  (void);
void    game_snake  (uint8_t reset);
void    set_food    (void);
//void    getNewTail  (void);

// ----------------------------------- FUNCIONES ------------------------------------------
void init_snake(void)
{
    cmd2dir[FWD_COMMAND  ] = Point( 0,  1,  0);
    cmd2dir[REV_COMMAND  ] = Point( 0, -1,  0);
    cmd2dir[LEFT_COMMAND ] = Point(-1,  0,  0);
    cmd2dir[RIGHT_COMMAND] = Point( 1,  0,  0);
    cmd2dir[UP_COMMAND   ] = Point( 0,  0,  1);
    cmd2dir[DOWN_COMMAND ] = Point( 0,  0, -1);

}

void game_snake(uint8_t reset)
{
    
    static uint8_t snake_length;
    static point direction;
    static char game_over_msg[GOM_LENGTH] = {"GAME OVER "};
    uint8_t i;
    
    uint8_t command;

    if(reset)
    {
        clearCube();
        game_over = false;      // 
        snake_length = MIN_LENGTH;


        for(i = 0; i<snake_length; i--)
        {
            snake[i] = Point(X_0,Y_0,Z_0+snake_length-1-i);
            setPoint(snake[i]);
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

    setPoint(food);
    
    // Si se ha acabado el juego
    if(game_over)
    {
        echo = true;
        cleanBuffer();
        send_int(snake_length - MIN_LENGTH);

        game_over_msg[GOM_LENGTH-1]= ' ';
        game_over_msg[GOM_LENGTH-2]= (snake_length - MIN_LENGTH)%10+'0';
        game_over_msg[GOM_LENGTH-3]= (snake_length - MIN_LENGTH)/10+'0';
        setMessage(game_over_msg);
        setPeriodo(1000);
        effect_repeat(&font_effect_slide_message,(GOM_LENGTH)*N);
    }
}

void set_food(void)
{
    do{
        food = getRandomPoint();
        if(!getPoint(food))   //Si esta apagado es posible
        {   
            if(count_neighboors(food.x,food.y,food.z)==0)
            {
                setPoint(food);
                // send_int(food.x);
                // send_int(food.y);
                // send_int(food.z);
                return; 
            }  
        }
    }while(!getPoint(food));  //Mientras este apagado

}
