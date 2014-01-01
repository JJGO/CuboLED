// Jose Javier Gonzalez Ortiz
// Libreria para la definicion del juego del snake3D en el cubo LED
// snake.c


#include "snake.h"

// ------------------------------------ GLOBALES ------------------------------------------

static point head, tail, food;
static point dirPoint[6];
static uint8_t game_over = false;
static uint8_t cmd2dir[NUM_COMMAND];
// ----------------------------------- PROTOTIPOS -----------------------------------------

void    init_snake  (void);
void    game_snake  (uint8_t reset);
void    set_food    (void);
void    getNewTail  (void);

// ----------------------------------- FUNCIONES ------------------------------------------
void init_snake(void)
{
    cmd2dir[FWD_COMMAND  ] = PY;
    cmd2dir[REV_COMMAND  ] = NY;
    cmd2dir[LEFT_COMMAND ] = NX;
    cmd2dir[RIGHT_COMMAND] = PX;
    cmd2dir[UP_COMMAND   ] = PZ;
    cmd2dir[DOWN_COMMAND ] = NZ;

    dirPoint[PX] = Point( 1,  0,  0);
    dirPoint[NX] = Point(-1,  0,  0);
    dirPoint[PY] = Point( 0,  1,  0);
    dirPoint[NY] = Point( 0, -1,  0);
    dirPoint[PZ] = Point( 0,  0,  1);
    dirPoint[NZ] = Point( 0,  0, -1);

}

void game_snake(uint8_t reset)
{
    
    static uint8_t snake_length;
    static uint8_t direction;
    
    volatile uint8_t command;

    if(reset)
    {
        clearCube();
        game_over = false;      // 
        snake_length = 3;
        head.x = 3; tail.x = 3;
        head.y = 3; tail.y = 3;
        head.z = 3; tail.z = 1;
        direction = PZ;          // direccion +Z
        setVoxel(3,3,3);         // head
        setVoxel(3,3,2);
        setVoxel(3,3,1);         //tail
        set_food();
        
        setFactor(10);          // Para que sea manejable se ralentiza
        
        echo = false;

        return;

    }

    command = getCommand();
    if(command)
    {
        direction = cmd2dir[command];
    }

    // Obtiene la nueva posicion de la cabeza 
    head = sumPoints(head,dirPoint[direction]);
    //Comprueba que no se haya salido por las paredes
    if(!inrange(head.x))
    {
        game_over = true;
        //if(head.x == N)
        //{
        //    fillPlane(X,N-1,0xff);
        //}else
        //{
        //    fillPlane(X,0,0xff);
        //}
    }
    else if(!inrange(head.y))
    {
        game_over = true;
        // if(head.y == N)
        //{   
        //    fillPlane(Y,N-1,0xff);
        //}else
        //{
        //    fillPlane(Y,0,0xff);
        //}

    }
    else if(!inrange(head.z))
    {
        game_over = true;
        //if(head.z == N)
        //{
        //    fillPlane(Z,N-1,0xff);
        //}else
        //{
        //    fillPlane(Z,0,0xff);
        //}
    }

    // 
    if(getPoint(head))              
    {
        if(pointEquals(head,food))  //Si alcanza la comida
        {
            snake_length++;

            set_food();        // Coloca de forma aleatoria la comida
            // tail no se modifica
        }else{                      //Si choca consigo misma
             game_over = true;
        }
       
    }
    else{               // Else, Hay que desplazar la cola
        setPoint(head);
        clearPoint(tail);       
        getNewTail();
    }
    
    // Si se ha acabado el juego
    if(game_over)
    {
        echo = true;
        send_int(head.x);
        send_int(head.y);
        send_int(head.z);
        setMessage("GAME OVER");
        effect_repeat(&font_effect_slide_message,9*N);
    }
}

void set_food(void)
{
    do{
        food = getRandomPoint();
        if(!getPoint(food))   //Si esta apagado es posible
        {   
            //if(count_neighboors(food.x,food.y,food.z)==0)
            //{
                setPoint(food);
                return; 
            //}  
        }
    }while(!getPoint(food));  //Mientras este encendido

}

void getNewTail(void)
{
    point new_tail;
    uint8_t i;
    for(i = 1; i < 7; i++)
    {
        new_tail = sumPoints(tail,dirPoint[i]);
        if(getPoint(new_tail))
        {
            if(!pointEquals(new_tail,food)){
                tail = new_tail;
                return;
            }
        }
    }
    //game_over = true;
}

//switch(direction)
    // {
    //     case +X:
    //         if(inrange(head.x+1,head.y,head.z))
    //         {
    //             head.x++;
    //         }else{
    //             game_over = true;
    //             fillPlane(X,N-1,0xff);
    //         }
    //         break;

    //     case -X:
    //         if(inrange(head.x-1,head.y,head.z))
    //         {
    //             head.x--;
    //         }else{
    //             game_over = true;
    //             fillPlane(X,0,0xff);
    //         }
    //         break;

    //     case +Y:
    //         if(inrange(head.x,head.y+1,head.z))
    //         {
    //             head.y++;
    //         }else{
    //             game_over = true;
    //             fillPlane(Y,N-1,0xff);
    //         }
    //         break;

    //     case -Y:
    //         if(inrange(head.x,head.y-1,head.z))
    //         {
    //             head.y--;
    //         }else{
    //             game_over = true;
    //             fillPlane(Y,0,0xff);
    //         }
    //         break;

    //     case +Z:
    //         if(inrange(head.x,head.y,head.z+1))
    //         {
    //             head.z++;
    //         }else{
    //             game_over = true;
    //             fillPlane(Z,N-1,0xff);
    //         }
    //         break;

    //     case -Z:
    //         if(inrange(head.x,head.y,head.z-1))
    //         {
    //             head.z--;
    //         }else{
    //             game_over = true;
    //             fillPlane(Z,0,0xff);
    //         }
    //         break;
    // }