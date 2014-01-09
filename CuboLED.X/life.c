// Jose Javier Gonzalez Ortiz
// Libreria para la ejecucion del juego de la vida de Conway en el Cubo LED
// life.c


#include "life.h"

// ------------------------------------ GLOBALES ------------------------------------------

static uint8_t life[N][N];

static uint8_t MIN_SURVIVE  = 3;
static uint8_t MAX_SURVIVE  = 4;
static uint8_t MIN_BORN     = 4;
static uint8_t MAX_BORN     = 4;
static uint8_t WARP         = 0;

// ----------------------------------- PROTOTIPOS -----------------------------------------

void    effect_game_of_life_34_44   (uint8_t reset);
void    effect_game_of_life_45_55   (uint8_t reset);
void    effect_game_of_life_57_66   (uint8_t reset);
void    effect_gol_glider_45_55     (uint8_t reset);
void    effect_gol_glider_56_55     (uint8_t reset);
void    effect_gol_glider_57_66     (uint8_t reset);
void    effect_gol_blicker_57_66  (uint8_t reset);

void    gol_random_init             (void);
void    gol_next_generation         (void);
uint8_t gol_count_neighboors        (uint8_t x, uint8_t y, uint8_t z);
void    gol_set_rules               (uint8_t MINS, uint8_t MAXS, uint8_t MAXB, uint8_t MINB);

// ----------------------------------- FUNCIONES ------------------------------------------

void effect_game_of_life_34_44(uint8_t reset)
{
    if(reset)
    {   
        WARP = 0;
        gol_set_rules(3,4,4,4);
        gol_random_init();
        return;
    }

    gol_next_generation();
}

void effect_game_of_life_45_55(uint8_t reset)
{
    if(reset)
    {   
        WARP = 1;
        gol_set_rules(4,5,5,5);
        gol_random_init();
        return;
    }

    gol_next_generation();
}

void effect_game_of_life_57_66(uint8_t reset)
{
    if(reset)
    {   
        WARP = 1;
        gol_set_rules(5,7,6,6);
        gol_random_init();
        return;
    }

    gol_next_generation();
}


void gol_set_rules(uint8_t MINS, uint8_t MAXS, uint8_t MAXB, uint8_t MINB)
{
    MIN_SURVIVE  = MINS;
    MAX_SURVIVE  = MAXS;
    MIN_BORN     = MINB;
    MAX_BORN     = MAXB;
}

void effect_gol_glider_45_55(uint8_t reset)
{
    if(reset)
    {
        WARP = 1;
        gol_set_rules(4,5,5,5);
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
    gol_next_generation();
}

void effect_gol_glider_56_55(uint8_t reset)
{
    if(reset)
    {
        WARP = 1;
        gol_set_rules(5,6,5,5);
        clearCube();
        
        setVoxel(7,3,0+3);
        setVoxel(6,3,0+3);
        setVoxel(7,3,1+3);
        setVoxel(6,3,1+3);
        setVoxel(6,3,2+3);
        setVoxel(6,4,1+3);
        setVoxel(6,4,2+3);

        setVoxel(3,7,0+3);
        setVoxel(3,6,0+3);
        setVoxel(3,7,1+3);
        setVoxel(3,6,1+3);
        setVoxel(3,6,2+3);
        setVoxel(4,6,1+3);
        setVoxel(4,6,2+3);
    }
    gol_next_generation();
}

void effect_gol_glider_57_66(uint8_t reset)
{
    if(reset)
    {
        WARP = 1;
        gol_set_rules(5,7,6,6);
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
    gol_next_generation();
}

void effect_gol_blicker_57_66(uint8_t reset)
{
    if(reset)
    {
        WARP = 1;
        gol_set_rules(5,7,6,6);
        clearCube();
        
        setVoxel(1,0,1);
        setVoxel(0,1,1);
        setVoxel(2,1,1);
        setVoxel(1,1,2);
        setVoxel(0,2,1);
        setVoxel(1,2,1);
        setVoxel(2,2,1);
        setVoxel(1,2,2);

    }
    gol_next_generation();
}

void gol_random_init(void)
{
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
    uint8_t x,y,z,neighboors;

    for(z = 0 ; z < N ; z++)
    {
        for(y = 0 ; y < N ; y++)
        {
            life[z][y] = 0x00;
            for(x = 0 ; x < N ; x++)
            {
                neighboors = gol_count_neighboors(x,y,z);
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

    for(dx = -1; dx < 2 ; dx++)
    {
        if(WARP || inrange(x+dx))
        {
            for(dy = -1; dy < 2 ; dy++)
            {
                if(WARP || inrange(y+dy))
                {
                    for(dz = -1; dz < 2 ; dz++)
                    {
                        if(WARP || inrange(z+dz))
                        {
                            if(dx != 0 || dy != 0 || dz!= 0)
                            {
                                neighboors += getVoxel((x+dx)&0x07,(y+dy)&0x07,(z+dz)&0x07);
                            }
                            
                        }
                    }   
                }
            }    
        }
    }
    return neighboors;
}