#include <allegro.h>
#include <string.h>
#include <conio.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winalleg.h>
#include <fstream>

using namespace std;

#define ancho 1024
#define alto 740



int inicializo();
void inicializo_pantalla();
void inicializo_sonidos();
void armoPantalla();
void jugar();
void inicializo_nivel();
void muestro_ladrillos();
void muevo_bola();
void chequeo_teclasSonido();
void chequeo_base();
void retoma_juego ();
void configura_level();
int cuentaLadrillos();
void destruyo_componentes();
void dibujaMuerte();
void grabo_archivo();
void cargo_archivo();


MIDI *musicaInicio;
MIDI *musicaJuego;
SAMPLE *sonido_InicioJuego;
SAMPLE *sonido_InicioNivel;
SAMPLE *sonido_LadrilloRoto;
SAMPLE *sonido_RebotePelota;
SAMPLE *sonido_Revivir;
SAMPLE *sonido_VidaExtra;
SAMPLE *sonido_RebotaBase;
SAMPLE *sonido_vidaPerdida;
SAMPLE *sonido_Gameover;
SAMPLE *sonido_rebotaParedes;

BITMAP *buffer;
BITMAP *logo;
BITMAP *panel;
BITMAP *recuadro;
BITMAP *fondo1;
BITMAP *fondo2;
BITMAP *fondo3;
BITMAP *fondo4;
BITMAP *fondo5;
BITMAP *gameOver;
BITMAP *lad1;
BITMAP *lad2;
BITMAP *lad3;
BITMAP *lad4;
BITMAP *lad5;
BITMAP *lad6;
BITMAP *lad7;
BITMAP *ladd;
BITMAP *base;
BITMAP *base2;
BITMAP *base3;
BITMAP *base4;


int retardo=100;
int vidas=3;
int level=1;
int score=0;
bool juegoIniciado=false;
bool fin=false;
bool nuevoNivel=false;
bool enJuego=false;
int dirY=-1;
int dirX=1;
int velocidad=3;
int velocidadIncial=3;
int fondoN=1;
bool muerte=false;
int secuenciaMuerte=1;
bool musica=true;
bool efectos=true;
bool finJuego=false;
bool existeArchivo=false;
int highScore=0;

int baseX=255;
int bolaX=285;
int bolaY=650;


int pant1[63]=
{
    1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7
};

int pant2[63]=
{
    1,2,3,2,1,2,3,2,1,
    2,1,4,2,4,2,4,2,4,
    3,7,2,6,7,6,7,6,3,
    4,4,5,4,5,4,5,4,4,
    5,7,5,6,5,6,5,7,5,
    6,6,6,6,6,6,6,6,6,
    7,1,7,7,7,7,7,1,7
};



int mapa[63];

int puntaIzq;
int puntaDer;

int colBola;
int filaBola;
int elemento;

int fila[]= {20,50,80,110,140,170,200};

DATAFILE *datfile;
FONT *arialB;
FONT *arial20;

int main()
{
    if(inicializo()==1)
        return 1;

    while(!fin)
    {
        armoPantalla();

        if(key[KEY_ESC]) fin=true;
        if(key[KEY_ENTER] && juegoIniciado==FALSE)
        {
            jugar();
            midi_pause();
            if (efectos) play_sample( sonido_Gameover, 200,150,1000,0);
            if(score>highScore)
            {
                highScore=score;
                grabo_archivo();
            }

            while (!key[KEY_ESC] && !key[KEY_ENTER])
            {

            }
            vidas=3;
            level=1;
            velocidad=velocidadIncial;
            score=0;
        }






    }
    destruyo_componentes();
return 0;

}
END_OF_MAIN();

int inicializo()
{
    allegro_init();
    install_keyboard();

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT,NULL) !=0)
    {
        allegro_message("Error ! Inicializando sistema de sonido \n\n\n", allegro_error);
        return 1;
    }

    datfile=load_datafile("resources.dat");
    arialB=(FONT*)datfile[1].dat;
    arial20=(FONT*)datfile[0].dat;


    inicializo_pantalla();
    inicializo_sonidos();

    cargo_archivo();
    play_midi(musicaInicio, 0);

    return 0;

}


void inicializo_pantalla()
{
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT, ancho, alto, 0, 0);
    buffer=create_bitmap(ancho, alto);

    logo=load_bitmap("img/logo.bmp",NULL);
    panel=load_bitmap("img/panel.bmp",NULL);
    recuadro=load_bitmap("img/recuadro.bmp",NULL);
    fondo1=load_bitmap("img/fondo1.bmp",NULL);
    fondo2=load_bitmap("img/fondo2.bmp",NULL);
    fondo3=load_bitmap("img/fondo3.bmp",NULL);
    fondo4=load_bitmap("img/fondo4.bmp",NULL);
    fondo5=load_bitmap("img/fondo5.bmp",NULL);
    lad1=load_bitmap("img/ladrillo1.bmp",NULL);
    lad2=load_bitmap("img/ladrillo2.bmp",NULL);
    lad3=load_bitmap("img/ladrillo3.bmp",NULL);
    lad4=load_bitmap("img/ladrillo4.bmp",NULL);
    lad5=load_bitmap("img/ladrillo5.bmp",NULL);
    lad6=load_bitmap("img/ladrillo6.bmp",NULL);
    lad7=load_bitmap("img/ladrillo7.bmp",NULL);
    ladd=load_bitmap("img/ladrilloduro.bmp",NULL);
    gameOver=load_bitmap("img/gameover.bmp",NULL);
    base=load_bitmap("img/base.bmp",NULL);
    base2=load_bitmap("img/base2.bmp",NULL);
    base3=load_bitmap("img/base3.bmp",NULL);
    base4=load_bitmap("img/base4.bmp",NULL);


}

void inicializo_sonidos()
{
    set_volume(230, 209);
    musicaInicio=load_midi("sonidos/ark.mid");
    musicaJuego=load_midi("sonidos/Arkanoid.mid");

    sonido_InicioJuego=load_wav("sonidos/inicioJuego.wav");
    sonido_InicioNivel=load_wav("sonidos/InicioNivel.wav");
    sonido_LadrilloRoto=load_wav("sonidos/ladrilloRoto.wav");
    sonido_RebotePelota=load_wav("sonidos/rebotePelota.wav");
    sonido_VidaExtra=load_wav("sonidos/vidaExtra.wav");
    sonido_rebotaParedes=load_wav("sonidos/rebotaParedes.wav");
    sonido_RebotaBase=load_wav("sonidos/reboteBase.wav");
    sonido_vidaPerdida=load_wav("sonidos/fallo.wav");
    sonido_Gameover=load_wav("sonidos/game-over.wav");
}


void armoPantalla()
{
    clear_to_color(buffer, makecol(0,0,0));
    draw_sprite(buffer, logo, 610,5);
    draw_sprite(buffer, panel, 620,140);
    textprintf_ex(panel, arialB, 130, 3, makecol(0,0,0), makecol(0,0,0),"           " );
    textprintf_ex(panel, arialB, 130, 3, makecol(255,0,0), makecol(0,0,0) , "%d", level);

    textprintf_ex(panel, arialB, 160, 65, makecol(0,0,0), makecol(0,0,0),"           " );
    textprintf_ex(panel, arialB, 160, 65, makecol(255,0,0), makecol(0,0,0) , "%d", score);

    textprintf_ex(panel, arialB, 130, 130, makecol(0,0,0), makecol(0,0,0),"           " );
    textprintf_ex(panel, arialB, 130, 130, makecol(255,0,0), makecol(0,0,0) , "%d", vidas);


    textprintf_ex(buffer, arial20, 700, 100, makecol(255,255,255), makecol(0,0,0) , "Highscore: %d", highScore);

    draw_sprite(buffer, recuadro, 5, 10);

    switch (fondoN)
    {
    case 1:
        draw_sprite(buffer, fondo1, 11,16);
        break;
    case 2:
        draw_sprite(buffer, fondo2, 11,16);
        break;
    case 3:
        draw_sprite(buffer, fondo3, 11,16);
        break;
    case 4:
        draw_sprite(buffer, fondo4, 11,16);
        break;
    case 5:
        draw_sprite(buffer, fondo5, 11,16);
        break;
    }


    if (!muerte)
    {
        draw_sprite(buffer, base, baseX, 660);

    }
    else
    {

        switch (secuenciaMuerte)
        {
        case 1:
            draw_sprite(buffer,base2, baseX, 655);
            break;
        case 2:
            draw_sprite(buffer,base3, baseX, 650);
            break;
        case 3:
            draw_sprite(buffer,base4, baseX, 640);
            break;



        }

    }
    /*if (!enJuego) bolaX=baseX+50;
    circlefill(buffer, bolaX, bolaY,10, makecol(124,250,16));
    */
    if(enJuego)
    {
        circlefill(buffer, bolaX,bolaY, 10, makecol(124,250,16));
    }
    else
    {
        bolaX=baseX+50;
        circlefill(buffer, bolaX, bolaY, 10, makecol(124,250,16));
    }

    muestro_ladrillos();
    if(vidas==0) draw_sprite(buffer, gameOver, 150, 300);

    blit(buffer, screen, 0,0,0,0, ancho, alto);
}

void jugar()
{
    level=1;
    finJuego=false;

    while(!key[KEY_ESC] && !finJuego)
    {
        midi_pause();
        inicializo_nivel();

        while(!nuevoNivel && !key[KEY_ESC] && vidas>0 )
        {
            if (key[KEY_SPACE] && enJuego==false)
            {
                if(efectos) stop_sample(sonido_InicioNivel);
                if(musica)play_midi(musicaJuego, 1);
                enJuego=true;

            }
            chequeo_base();

            if (enJuego)
            {
                muestro_ladrillos();
                muevo_bola();
            }
            if(key[KEY_0])   //borra mapa para chequeo
            {
                for (int i=0; i<63; i++)
                    mapa[i]=0;
            }
            if (cuentaLadrillos()==0)
            {
                level++;
                nuevoNivel=true;
                fondoN++;
                if (fondoN==6) fondoN=1;
                inicializo_nivel();
            }

            chequeo_teclasSonido();
            armoPantalla();
        }
        if(vidas==0) finJuego=true;
    }
}

void inicializo_nivel()
{
    configura_level();
    retoma_juego();
    if(efectos) play_sample(sonido_InicioNivel,200,150,1000,0);

}


int cuentaLadrillos()
{
    for (int i=0; i<63; i++)
    {
        if (mapa[i]!= 8 && mapa[i]>0) return 1;
    }
    return 0;
}

void muestro_ladrillos()
{
    int x, y, col;
    int ladn=0;
    int lad;
    int fila[7]= {20,50,80,110,140,170,200};

    for (int i=0; i<63; i++)
    {
        if(mapa[i]>0)
        {
            lad=mapa[i];
            y=fila[int(i/9)];
            col= i-(((int)(i/9))*9)+1;
            x=13+((col-1)*65);

            switch (lad)
            {
            case 1:
                draw_sprite(buffer, lad1, x, y);
                break;
            case 2:
                draw_sprite(buffer, lad2, x, y);
                break;
            case 3:
                draw_sprite(buffer, lad3, x, y);
                break;
            case 4:
                draw_sprite(buffer, lad4, x, y);
                break;
            case 5:
                draw_sprite(buffer, lad5, x, y);
                break;
            case 6:
                draw_sprite(buffer, lad6, x, y);
                break;
            case 7:
                draw_sprite(buffer, lad7, x, y);
                break;
            case 8:
                draw_sprite(buffer, ladd, x, y);
                break;
            }
        }

    }
}


void muevo_bola()
{

    puntaIzq=baseX+20;
    puntaDer=baseX+100;

    if(bolaY<225)
    {
        filaBola= ((int)(((bolaY-20)/30))+1);
        colBola=((int)((bolaX-13)/64)+1);
        elemento=(((filaBola-1)*9)+colBola)-1;

        if (mapa[elemento]!=0)
        {
            if(dirY==-1)
            {
                dirY=1;
            }
            else
            {
                dirY=-1;
            }

            if(mapa[elemento]!=8  )
            {
                if (efectos) play_sample (sonido_LadrilloRoto, 200, 150 ,1000,0);
                mapa[elemento]=0;
                score=score+10;
                muestro_ladrillos();

            }
            else
            {
                if(efectos) play_sample (sonido_RebotePelota, 200, 150 ,1000,0);
            }
        }
    }
    else
    {
        if (bolaY>650 && dirY==1)
        {
            if (bolaX>=baseX && bolaX<=baseX+120)
            {
                if(efectos) play_sample (sonido_RebotaBase, 200, 150 ,1000,0);

                if(bolaX<=puntaIzq) dirX=-1;
                if(bolaX>=puntaDer)dirX=1;

                dirY=-1;

            }
            else
            {
                if (efectos) play_sample (sonido_vidaPerdida, 200, 150 ,1000,0);
                vidas--;
                dibujaMuerte();
                if(vidas>0) retoma_juego();

            }
            return;
        }
    }

    if (bolaX>580) dirX=-1;
    if(bolaX<15) dirX=1;
    if (bolaY<15) dirY=1;
    if (bolaX>580 || bolaX<15 || bolaY<15)
    {
        if (efectos) play_sample (sonido_RebotePelota, 200, 150 ,1000,0);
    }
    if (dirX==1) bolaX=bolaX+ velocidad;
    if (dirX==-1) bolaX=bolaX- velocidad;
    if (dirY==1) bolaY=bolaY+ velocidad;
    if (dirY==-1) bolaY=bolaY- velocidad;
    armoPantalla();
}

void chequeo_teclasSonido()
{
    if(key[KEY_DEL])
    {
        if (musica==true)
        {
            musica=false;
            midi_pause();
        }
        else if (musica==false)
        {
            musica=true;
            midi_resume();
        }
    }
    if (key[KEY_TAB])
    {
        if (efectos)
        {
            efectos=false;
        }
        else
        {
            efectos=true;
        }
    }
}
void chequeo_base()
{
    if(key[KEY_RIGHT])
    {
        if (baseX<476)
        {
            baseX=baseX+velocidad;
        }
    }

    if (key[KEY_LEFT])
    {
        if(baseX>11)
            baseX=baseX-velocidad;
    }
}

void retoma_juego ()
{
    baseX=255;
    bolaX=285;
    bolaY=650;
    enJuego=false;
    nuevoNivel=false;
    armoPantalla();
    velocidad=3+((int)(level/5));
}

void configura_level()
{
    for (int i=0; i<63; i++)
    {
        if (level==1) mapa[i]= pant1[i];
        if (level==2) mapa[i]= pant2[i];
        if (level>2) mapa[i]= rand()%9;
    }
}


void dibujaMuerte()
{
    muerte=true;
    for (secuenciaMuerte =1; secuenciaMuerte < 5; secuenciaMuerte++)
    {
        armoPantalla();
        Sleep(200);

    }
    muerte =false;
    armoPantalla();
}


void destruyo_componentes()
{
    destroy_midi(musicaJuego);
    destroy_sample(sonido_InicioJuego);
    destroy_sample(sonido_InicioNivel);
    destroy_sample(sonido_LadrilloRoto);
    destroy_sample(sonido_Gameover);
    destroy_sample(sonido_RebotaBase);
    destroy_sample(sonido_rebotaParedes);
    destroy_sample(sonido_RebotePelota);
    destroy_sample(sonido_Revivir);
    destroy_sample(sonido_VidaExtra);
    destroy_sample(sonido_vidaPerdida);
    destroy_bitmap(buffer);

}



void cargo_archivo()
{
    ifstream puntaje;
    char textoPuntaje[100];
    puntaje.open("high.dat");
    if (puntaje.fail())
    {

        existeArchivo=false;
        return;
    }

    if (!puntaje.eof())
    {
        puntaje.getline(textoPuntaje, sizeof(puntaje));
        string s= string(textoPuntaje);
        highScore= atoi(s.c_str());
    }

    puntaje.close();



}

void grabo_archivo()
{
    ofstream puntaje;
    puntaje.open("high.dat");
    puntaje << highScore << endl;
    puntaje.close();
}

