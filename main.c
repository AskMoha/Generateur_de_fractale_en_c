#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <time.h>
int width=150;                                                          //Longueur de la fenetre SDL
int height=100;                                                         //Largeur de la fenetre SDL
SDL_Window *window;                                                     //On initialise la fenetre window
SDL_Renderer *renderer;                                                 //on initialise là ou on va dessiner la fractale, renderer
int couleurc=1;
void UpdateEvents(int *mousex,int *mousey,char boutons[8])
{
    SDL_Event evenement;
    while(SDL_PollEvent(&evenement))
    {
        switch (evenement.type)
        {
        case SDL_MOUSEMOTION:
            *mousex=evenement.motion.x;
            *mousey=evenement.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            boutons[evenement.button.button]=1;
            break;
        case SDL_MOUSEBUTTONUP:
            boutons[evenement.button.button]=0;
            break;
        }
    }
}
void COULEUR(int g)
{
    switch (g)
    {
    case 0 :
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        break;
    case 1:
        SDL_SetRenderDrawColor(renderer,64,224,208,255);
    case 2 :
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case 3 :
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        break;
    case 4 :
        SDL_SetRenderDrawColor(renderer,rand()%256,rand()%256,rand()%256,255);
        break;
    }
}
void Mandelbrot(SDL_Renderer *renderer,int X,int Y,double range[])
{
    int i,j;
    int k;
    double xmin = range[0];
    double ymin = range[1];
    double xstep = (range[2]-range[0])/X;
    double ystep = (range[3]-range[1])/Y;
    double x,y,z0x,z0y,temp;
    for(i=0; i<X; i++)
    {
        for(j=0; j<Y; j++)
        {
            z0x = 1.6*xmin + xstep*i;
            z0y = ymin + ystep*j;


            x = 0;
            y = 0.0;
            k = 0;
            while((x*x+y*y)<4.0 && k<255)
            {
                temp=x;
                x=x*x-y*y+z0x;
                y=2*temp*y+z0y;
                k++;
            }
            if (k>10 && k<250)

                //SDL_PutPixel32(renderer,i,j,k);
                SDL_RenderDrawPoint(renderer,i,j);
            SDL_RenderPresent(renderer);
        }
    }
}
void menu ()
{
    double zoom=0.3;
    int mousex=0;
    int mousey=0;
    char boutons[4] = {0};
    int X=width;
    int Y=height;
    COULEUR(0);
    SDL_RenderClear(renderer);
    double echelle[4] = {-1.0,-1.0,1.0,1.0};
    COULEUR(couleurc);
    int x=width;
    int y=height;
    SDL_ShowCursor(1);
    int choix =0;
    printf("Bienvenue sur notre generateur de fractale\nEntrez 1 pour lancer le programme\nAppuyez sur 2 pour afficher l'explication\n Appuyez sur 3 pour changer les parametres \n");
    scanf("%d",&choix);
    Mandelbrot(renderer,x,y,echelle);
    switch(choix)
    {
    case 1:


        while(!boutons[SDL_BUTTON_RIGHT])
        {
            UpdateEvents(&mousex,&mousey,boutons);
            if (boutons[SDL_BUTTON_LEFT])
            {
                COULEUR(0);
                SDL_RenderClear(renderer);
                double xdrange = echelle[2]-echelle[0];
                double ydrange = echelle[3]-echelle[1];
                double dx = echelle[0] + mousex*xdrange/X;
                double dy = echelle[1] + mousey*ydrange/Y;
                xdrange*=zoom;
                ydrange*=zoom;
                echelle[0] = dx - xdrange/2.0;
                echelle[1] = dy - ydrange/2.0;
                echelle[2] = dx + xdrange/2.0;
                echelle[3] = dy + ydrange/2.0;
                boutons[SDL_BUTTON_LEFT] = 0;
                COULEUR(couleurc);
                Mandelbrot(renderer,X,Y,echelle);
            }
        }
        break;
    case 2:
        printf("Blabla");
        menu();
        break;
    case 3:
        printf("choisissez une couleur pour la fractale: 1 pour turquoise, 2 pour rouge, 3 pour vert, 4 pour une couleur aléatoire qui change a chaque fois\n ");
        scanf("%d",&couleurc);
        menu();
        break;
    }
}
int main(int argc, char *argv[])
{
    srand(time(NULL));
    SDL_CreateWindowAndRenderer(width,height,0,&window, &renderer);
    COULEUR(0);
    SDL_RenderClear(renderer);
    menu();


    SDL_Quit();
    return EXIT_SUCCESS;
}
