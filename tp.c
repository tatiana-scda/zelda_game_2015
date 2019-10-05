#include <stdio.h>
#include <allegro5/base.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
//#include <allegro5/allegro_audio.h>
//#include <allegro5/allegro_acodec.h>

const float FPS = 10; //frames per second
const int SCREEN_W = 700; //largura da tela
const int SCREEN_H = 515; //altura da tela

/*
void IniciaVariaveis (float *xlink, float *ylink, float *xvaati, float *yvaati, float *xganondorf, float *yganondorf, float *xshadow, float *yshadow, int *salas, int llink, int alink){
    FILE *save;
    save = fopen("save.txt", "r");
    if(save){
        char string[1000];
        fgets (string, 1000, save);
        *xlink = atof(strtok(string, ","));
        *ylink = atof(strtok(NULL, ","));
        *xvaati = atof(strtok(NULL, ","));
        *yvaati = atof(strtok(NULL, ","));
        *xganondorf = atof(strtok(NULL, ","));
        *yganondorf = atof(strtok(NULL, ","));
        *xshadow = atof(strtok(NULL, ","));
        *yshadow = atof(strtok(NULL, ","));
        *salas = atoi(strtok(NULL, ","));
    }
    else {
      *xlink = (SCREEN_W/2)-llink+15;
      *ylink = SCREEN_H-alink;
      *xvaati = (4*SCREEN_W/5)-llink;
      *yvaati = SCREEN_H/2;
      *xganondorf = SCREEN_W/2;
      *yganondorf = SCREEN_H/2;
      *xshadow = (4*SCREEN_W/5)-llink;
      *yshadow = SCREEN_H/2;
      *salas = 1;

  }
*/

int main(int argc, char **argv){

/*
//Files para salvas as posicoes
    FILE *savelink;
    FILE *saveganondorf;
    FILE *saveshadow;
 */

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
//ALLEGRO_AUDIO_STREAM *musica = NULL;
    ALLEGRO_BITMAP *link = NULL;
    ALLEGRO_BITMAP *vaati = NULL;
    ALLEGRO_BITMAP *ganondorf = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *shadow = NULL;
    bool redraw = true;

//INICIALIZACAO DO ALLEGRO

    if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
    }


    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
    }

    //    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    //    al_set_audio_stream_playing(musica, true);
    al_init_image_addon();
    al_install_keyboard();

//BITMAPS

//Bitmap Link; jogador
    link = al_load_bitmap("link.png");
    if(!link) {
      fprintf(stderr, "failed to create Link bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }
    float llink =  al_get_bitmap_width(link);
    float alink =  al_get_bitmap_height(link);
    float xlink = (SCREEN_W / 2) - llink + 15;
    float ylink = SCREEN_H - alink;

    float dxlink = 15.0, dylink = 15.0;

//Bitmap Vaati; inimigo 1
    vaati = al_load_bitmap("vaati.png");
    if(!vaati) {
      fprintf(stderr, "failed to create vaati bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }
    float xvaati = (4*SCREEN_W / 5) - llink;
    float yvaati = SCREEN_H/2;


//Bitmap Ganondorf; inimigo 2
    ganondorf = al_load_bitmap("ganondorf.png");
    if(!ganondorf) {
      fprintf(stderr, "failed to create Ganondorf bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }
    float xganondorf = SCREEN_W/2;
    float yganondorf = SCREEN_H/2;

    //Bitmap ShadowLink; inimigo 3
    shadow = al_load_bitmap("shadowlink.png");
    if(!shadow) {
      fprintf(stderr, "failed to create ShadowLink bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }
    float xshadow = (4*SCREEN_W / 5) - llink;
    float yshadow = SCREEN_H/2;


//Bitmap fundo; cenario 1
    fundo = al_load_bitmap("fundo.png");
    if(!fundo) {
      fprintf(stderr, "failed to create fundo bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }


//Evento
    event_queue = al_create_event_queue();
    if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_bitmap(link);
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();


    al_start_timer(timer);

    int salas = 1, dir = 0, esq = 0;
    int playing = 1, win=0;
    while(playing) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE  ) {
            break;
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    playing = 0;
                    break;

                case ALLEGRO_KEY_W:
                    ylink -= dylink;
                    break;

                case ALLEGRO_KEY_S:
                    ylink += dylink;
                    break;

                case ALLEGRO_KEY_A:
                    xlink -= dxlink;
                    break;

                case ALLEGRO_KEY_D:
                    xlink += dxlink;
                    break;


                    /*
                           case ALLEGRO_KEY_Q:{

                          FILE *save;
                                      save = fopen("save.txt", "w");
                                        fprintf(save, "%f,", xlink);
                                        fprintf(save, "%f,", ylink);
                                        fprintf(save, "%f,", xvaati);
                                        fprintf(save, "%f,", yvaati);
                                        fprintf(save, "%f,", xganondorf);
                                        fprintf(save, "%f,", yganondorf);
                                        fprintf(save, "%f,", xshadow);
                                        fprintf(save, "%f,", yshadow);
                                        fprintf(save, "%d", salas);

                                        fclose(save);

                                        playing = 0;
                                        break;
                                        }
                                }
                                }

                                if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                                    switch(ev.keyboard.keycode) {
                                      case ALLEGRO_KEY_W:
                                          move_cima = 1;
                                          break;

                                      case ALLEGRO_KEY_S:
                                          move_baixo = 1;
                                          break;

                                      case ALLEGRO_KEY_A:
                                          move_esquerda = 1;
                                          break;

                                      case ALLEGRO_KEY_D:
                                          move_direita = 1;
                                          break;
                                  }
                                }
                    */


        if(ev.type == ALLEGRO_EVENT_TIMER) {
          //Define a movimentacao de Vaati
            if (xvaati >= ((4* SCREEN_W / 5) - llink)) {
              dir = 1;esq = 0; } //Vaati vai para direita enquanto a possicao for esta
            else if (xvaati <= (SCREEN_W / 5)) {
              dir = 0;esq = 1; } //Vaat vai para esquerda enquanto a possicao for esta

            if (dir) xvaati -= 20;   //Velocidade de Vaati para a direita
            if (esq) xvaati += 20;   //Velocidade de Vaati para a esquerda

            //Define a movimentacao de Ganondorf
            if(xlink>xganondorf) xganondorf+=3;
            else if(xlink<xganondorf) xganondorf-=3;
            if(ylink>yganondorf) yganondorf+=3;
            else if(ylink<yganondorf) yganondorf-=3;

            //Define a movimentacao de Shadow Link
            if(xlink>xshadow) xshadow+=5;
            else if(xlink<xshadow) xshadow-=5;
            if(ylink>yshadow) yshadow+=3;
            else if(ylink<yshadow) yshadow-=3;
        }

        if (((xlink + llink) > xvaati && xlink < (xvaati + llink)) && ((ylink + alink) > yvaati && ylink < (yvaati + alink)))
            playing = 0; //Se Link e Vaati colidirem, o jogo acaba
        else if (((xlink + llink) > xganondorf && xlink < (xganondorf + llink)) && ((ylink + alink) > yganondorf && ylink < (yganondorf + alink)) && salas==2)
            playing = 0; //Se Link e Ganondorf colidirem, o jogo acaba
        else if (((xlink + llink) > xshadow && xlink < (xshadow + llink)) && ((ylink + alink) > yshadow && ylink < (yshadow + alink)) && salas==3)
            playing = 0; //Se Link e ShadowLink colidirem, o jogo acaba

        float ldir = SCREEN_W - 30 - llink;
        if (ylink < 0) {
            salas++;
            if (salas==3) {
                playing = 0;
                win=1;

//                fopen ("link.txt", "a");

            }
            else {
                ylink = SCREEN_H - alink;
                xvaati = (4*SCREEN_W / 5) - llink;
                xganondorf=50; yganondorf=50;
            }
        }
        else if (ylink > (SCREEN_H - alink)) {
            if(salas==2) {
                salas = 1;
                ylink=0;
            }
            else ylink = SCREEN_H - alink; //recua embaixo
        }
        else if (xlink < 30) xlink = 30; //Nao ultrapassa muro esquerda
        else if (xlink > ldir) xlink = ldir; //Nao ultrapassa muro direita
        else if (ylink < 30) { //acima
            if (xlink < (272 - dxlink) || xlink > (372 - llink + dxlink)) ylink = 30;
            else if (xlink < 272) xlink = 272; //Nao ultrapassa muro porta esquerda
            else if (xlink > 370 - llink) xlink = 370 - llink; //Nao ultrapassa muro porta direita
        }
        else if (ylink > SCREEN_H - 30 - alink) { //nao ultrapassa abaixo
            if (xlink < (272 - dxlink) || xlink > (372 - llink + dxlink)) ylink = SCREEN_H - 30 - alink;
            else if (xlink < 272) xlink = 272; //Nao ultrapassa muro saida esquerda
            else if (xlink > 370 - llink) xlink = 370 - llink; //Nao ultrapassa muro saida direita
        }

        if(redraw && al_is_event_queue_empty(event_queue))  {
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(fundo, 0, 0, 0);
            al_draw_bitmap(link, xlink, ylink, 0);
            al_draw_bitmap(vaati, xvaati, yvaati, 0);
            if(salas==2) {
                al_draw_bitmap(ganondorf, xganondorf, yganondorf, 0);
                al_flip_display();
            }
            if(salas==3){
              al_draw_bitmap(shadow, xshadow, yshadow, 0);
              al_draw_bitmap(ganondorf, xganondorf, yganondorf, 0);
            }
            al_flip_display();
    } //fim while

    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *size_40 = al_load_font("Triforce.ttf", 20, 1);
    al_draw_bitmap(fundo, 0, 0, 0);

    char jogo[15], ganhou[15], perdeu[15];

    sprintf(ganhou, "Fim de jogo!");
    al_draw_text(size_40, al_map_rgb(0, 0, 0), SCREEN_W/3, SCREEN_H/3, 0, ganhou);
// SCREEN_W/3 e SCREEN_H/3 ajustam a posição do escrito na tela

    if (win) {
        sprintf(ganhou, "Voce ganhou!");
        al_draw_text(size_40, al_map_rgb(0, 0, 0), SCREEN_W/3, (SCREEN_W/3)+30, 0, ganhou);
// (SCREEN_W/3)+30 faz com que o escrito um pouco abaixo do aviso de fim de jogo
    }
    else {
        sprintf(perdeu, "Voce perdeu.");
        al_draw_text(size_40, al_map_rgb(0, 0, 0), SCREEN_W/3, (SCREEN_W/3)+30, 0, perdeu);
    }
}
    al_flip_display();
    al_rest(3);

//Fecha as rotinas do jogo
    al_destroy_bitmap(link);
    al_destroy_bitmap(vaati);
    al_destroy_bitmap(ganondorf);
    al_destroy_bitmap(shadow);
    al_destroy_bitmap(fundo);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
