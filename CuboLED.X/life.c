// Jose Javier Gonzalez Ortiz
// Libreria para la ejecucion del juego de la vida de Conway en el Cubo LED
// life.c


#include "life.h"

// #define MIN_SURVIVE     4//3
// #define MAX_SURVIVE     5//4

// #define MIN_BORN        5//4
// #define MAX_BORN        5//4

#define WARP

// ------------------------------------ GLOBALES ------------------------------------------

static uint8_t life[N][N];

static uint8_t MIN_SURVIVE  = 3;
static uint8_t MAX_SURVIVE  = 4;
static uint8_t MIN_BORN     = 4;
static uint8_t MAX_BORN     = 4;

// ----------------------------------- PROTOTIPOS -----------------------------------------

void    game_of_life            (uint8_t reset);
void    gol_init                (void);
void    gol_next_generation     (void);
uint8_t gol_count_neighboors    (uint8_t x, uint8_t y, uint8_t z);
void    gol_glider_45_55        (void);
void    gol_glider_57_66        (void);

// ----------------------------------- FUNCIONES ------------------------------------------

void game_of_life(uint8_t reset)
{
    if(reset)
    {   
        //gol_init();
        gol_glider_45_55();
        //gol_glider_57_66();
        return;
    }

    gol_next_generation();

}

void gol_glider_45_55(void)
{
    MIN_SURVIVE  = 4;
    MAX_SURVIVE  = 5;
    MIN_BORN     = 5;
    MAX_BORN     = 5;
    clearCube();
    setVoxel(5,3,0);
    setVoxel(5,4,0);
    setVoxel(5,3,1);
    setVoxel(5,4,1);
    setVoxel(6,2,0);
    setVoxel(6,5,0);
    setVoxel(6,2,1);
    setVoxel(6,5,1);
    setVoxel(7,3,1);
    setVoxel(7,4,1);

}

void gol_glider_57_66(void)
{
    MIN_SURVIVE  = 5;
    MAX_SURVIVE  = 7;
    MIN_BORN     = 6;
    MAX_BORN     = 6;
    clearCube();
    setVoxel(7,3,2);
    setVoxel(6,3,2);
    setVoxel(5,3,2);
    setVoxel(5,3,1);
    setVoxel(6,3,0);

    setVoxel(7,4,2);
    setVoxel(6,4,2);
    setVoxel(5,4,2);
    setVoxel(5,4,1);
    setVoxel(6,4,0);

}

void gol_init(void)
{
    MIN_SURVIVE  = 3;
    MAX_SURVIVE  = 4;
    MIN_BORN     = 4;
    MAX_BORN     = 4;
    clearCube();
    uint8_t x,y,z;
    for(z = 0 ; z < N ; z++)
    {
        for(y = 0 ; y < N ; y++)
        {
            voxel[z][y] = 0x00;
            for(x = 0; x < N;x++)
            {
                if(!(rand()&0x01))
                {
                    setVoxel(x,y,z);
                }
            }
        }
    }
}

void gol_next_generation(void)
{
    uint8_t x,y,z,tmp,neighboors;

    for(z = 0 ; z < N ; z++)
    {
        for(y = 0 ; y < N ; y++)
        {
            life[z][y] = 0x00;
            for(x = 0 ; x < N ; x++)
            {
                neighboors = gol_count_neighboors(x,y,z);
                // clear(life[z][y],x);
                //tmp = life[z][y];
                if(getVoxel(x,y,z))
                {
                    
                    if(MIN_SURVIVE <= neighboors && neighboors <= MAX_SURVIVE)
                    {
                        set(life[z][y],x);
                    }

                }else{
                    if(MIN_BORN <= neighboors && neighboors <= MAX_BORN)
                    {
                        set(life[z][y],x);
                    }
                }
                //life[z][y]= tmp;

            }   
        }
    }

    for(z = 0; z < N; z++)
    {
        for(y = 0; y < N ; y++)
        {
            voxel[z][y] = life[z][y];
        }
    }
}
uint8_t gol_count_neighboors(uint8_t x, uint8_t y, uint8_t z)
{
    uint8_t neighboors=0;
    int8_t dx, dy, dz;

#ifdef WARP

    for(dx = -1; dx < 2 ; dx++)
    {
        for(dy = -1; dy < 2 ; dy++)
        {
            for(dz = -1; dz < 2 ; dz++)
            {
                if(dx != 0 || dy != 0 || dz!= 0)
                {
                    neighboors += getVoxel((x+dx)&0x07,(y+dy)&0x07,(z+dz)&0x07);
                }
            }   
        }    
    }

#else

    for(dx = -1; dx < 2 ; dx++)
    {
        if(inrange(x+dx))
        {
            for(dy = -1; dy < 2 ; dy++)
            {
                if(inrange(y+dy))
                {
                    for(dz = -1; dz < 2 ; dz++)
                    {
                        if(inrange(z+dz))
                        {
                            if(dx != 0 || dy != 0 || dz!= 0)
                            {
                                neighboors += getVoxel(x+dx,y+dy,z+dz);
                            }
                            
                        }
                    }   
                }
            }    
        }
    }
#endif
        return neighboors;
}