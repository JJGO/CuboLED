

void drawCube(uint8_t edge,uint8_t x,uint8_t y,uint8_t z)
{
    clearCube();
    uint8_t config;
    // Aristas en eje x
    uint8_t pos = edge-1;
    config = (0xff>>(N-edge))<<x;
    putAxis(X,  y,          z,      config);
    putAxis(X,  y,          z+pos,  config);
    putAxis(X,  y+pos,      z,      config);
    putAxis(X,  y+pos,      z+pos,  config);
    // Aristas en eje y
    config = (0xff>>(N-edge))<<y;
    putAxis(Y,  x,          z,      config);
    putAxis(Y,  x,          z+pos,  config);
    putAxis(Y,  x+pos,      z,      config);
    putAxis(Y,  x+pos,      z+pos,  config);
    // Aristas en eje z
    config = (0xff>>(N-edge))<<z;
    putAxis(Z,  x,          y,      config);
    putAxis(Z,  x,          y+pos,  config);
    putAxis(Z,  x+pos,      y,      config);
    putAxis(Z,  x+pos,      y+pos,  config);
}


//5 Efecto lluvia
void rain(void)
{
    i--;
    if(i<=0)
        i=7;

    if(i%2=1){
        for(j=0;j<=N; j++)
        {
            srand (time (NULL));
            leds=rand()%8;
            putAxis(X, j, i, leds);
            
        }
        putPlane(Z,i-1,getPlane(Z,i));
    }
}

//6 Cubo que cambia de tamaño y posicion
        //Cubo tamaño n
        // for(n=8;n<=3;n--){
        //     for(i=0;i<=N;i++){
        //         if(i==n || i==0)
        //         {
        //             for(j=0;j<=N;j++)
        //             {
        //                 if(j==0||j==n)
        //                  putAxis(X,j,i,0xFF>>(n-1));
        //                 else
        //                     putAxis(X,j,i,(0x01||0x01<<(n-1)));
        //             }
        //         }else{
        //             for(j==0||j==n)
        //                 putAxis(X,j,i,(0x01||0x01<<(n-1)));
        //         }
        //     }      
        // }

//7  Efecto anillos que se juntan
void ringMovement(void)
{
    for(j=-4;j<=11;j++)
    {
        z=j;
        for(i=1;i<=4;i++)
        {
            z = j;
            if(j<=0)
            {
                z=0;
            }else if(j>=7){
                z=7;
            }
            ring(i,z);
            ring(i,7-z);
        }
    }
}

//8 Ir rellenando el cubo desde una esquina
        //Variable d varía con cada ciclo de reloj
void setOblique(int d)
{
    clearCube();
    for(i=0;i<=8;i++)
    {
        for(j=0;j<=8;j++)
        {
            for(k=0;k<=8;k++)

                if(x+y+k==d)
                    setVoxel(i,j,k);
        }
    }
}

//9 Pasar los LEDs de un lado al contrario de forma aleatoria
void RandomMoveVertical(void)
{
    for(i=0; i<=7; i++)
    {
        for(j=0; j<=7; j++)
        {
            axis=getAxis(Z,i,j);
            if(test(axis,0)){
                srand (time (NULL));
                d=rand()%4;
                if(d!=0)
                   putAxis(Z,i,j, axis<<1);
            }else if(test(axis,7));
            else{
                putAxis(Z,i,j, axis<<1);
            }


        }

    }
}

/*Nombre: Ring
 * Descripcion: Crea un anillo de lado l
 * Argumentos: l (lado) y z (plano en el que se pone el anillo)
 * Valor devuelto: Ninguno*/
 void ring(int l, int z)
 {
    int a=4-l;
    putAxis(X, a, z, (0xFF>>2*a)<<a);
    putAxis(X, 7-2*a, z, (0xFF>>2*a)<<a);
    putAxis(Y, a, z, (0xFF>>2*a)<<a);
    putAxis(Y, 7-2*a, z, (0xFF>>2*a)<<a);
 }