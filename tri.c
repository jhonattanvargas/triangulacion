#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

/**************************************************************************************************************/
struct linea{ //estructura de la linea
    int x0;
    int x1;
    int y0;
    int y1;
};

typedef struct linea _linea;

int min(int a, int b){ //función mínimo entre dos números.
	if(a>=b)
		return b;
	else
		return a;
}

int max(int a, int b){ //función máximo entre dos números.
	if(a>=b)
		return a;
	else
		return b;

}

int det(int x0, int y0, int x1, int y1){ //determinante entre dos puntos.
	return x0*y1 - x1*y0;
}

int inter(_linea a, _linea b){ //intersección entre dos líneas.
	int r = 0;
	if ( (a.x0 != b.x0 || a.y0 != b.y0) && (a.x0 != b.x1 || a.y0 != b.y1) && (a.x1 != b.x0 || a.y1 != b.y0) && (a.x1 != b.x1 || a.y1 != b.y1) )
	{
		if(	min(max(a.x0,a.x1),max(b.x0,b.x1)) >= max(min(a.x0,a.x1),min(b.x0,b.x1))  &&  
			min(max(a.y0,a.y1),max(b.y0,b.y1)) >= max(min(a.y0,a.y1),min(b.y0,b.y1))   ){
			if(det(b.x0 - a.x0,b.y0 - a.y0,a.x1 - a.x0,a.y1 - a.y0)*det(b.x1 - a.x0,b.y1 - a.y0,a.x1 - a.x0,a.y1 - a.y0)<=0){
				if(det(a.x0 - b.x0,a.y0 - b.y0,b.x1 - b.x0,b.y1 - b.y0)*det(a.x1 - b.x0,a.y1 - b.y0,b.x1 - b.x0,b.y1 - b.y0)<=0){
				r=1;
				}
			}
		}
	}
	return r;
}

double pitagoras(_linea l){//longitud de la línea.
	return sqrt(pow((l.x0 -l.x1),2)+pow((l.y0-l.y1),2));
}

void quicksort(_linea * lineas,int lim_izq,int lim_der){ //método de ordenamiento para las líneas.
	int izq,der,pivote;
	_linea aux;
	izq=lim_izq;
	der=lim_der;
	pivote = pitagoras(lineas[(izq+der)/2]);
	do{
		while(pitagoras(lineas[izq])<pivote && izq<lim_der)
			izq++;
		while(pivote<pitagoras(lineas[der]) && der>lim_izq)
			der--;
		if(izq <=der){
			aux= lineas[izq];
			lineas[izq]=lineas[der];
			lineas[der]=aux;
			izq++;
			der--;
		}
	}while(izq<=der);
		if(lim_izq<der){
			quicksort(lineas,lim_izq,der);
		}
		if(lim_der>izq){
			quicksort(lineas,izq,lim_der);
		}
}
/**************************************************************************************************************/

int main(){

	int SCREEN_W = 400;
	int SCREEN_H = 400;
	ALLEGRO_DISPLAY *display = NULL;

	if(!al_init()) {
      	fprintf(stderr, "failed to initialize allegro!\n");
      	return 0;
   	}
	
 	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		return 0;
	}
	if(!al_install_keyboard()) {
	fprintf(stderr, "failed to initialize the keyboard!\n");
	return 0;
	}

	ALLEGRO_COLOR  negro = al_map_rgb(0,0,0);
	ALLEGRO_COLOR  rojo = al_map_rgb(255,0,0);
	ALLEGRO_COLOR  azul = al_map_rgb(0,0,255);
   	
   	al_clear_to_color(negro);
	al_flip_display();

	int i,j,n,nl;
	scanf("%d",&n);//cantidad de puntos.
	nl = n*(n-1)/2;//cantidad de lineas.
	_linea * l =  (_linea *)malloc(sizeof(_linea)*nl);
	int puntos[n][2];//matriz para almacenar los puntos.
	for(i=0;i<n;i++){
		scanf("%d",&puntos[i][0]);
		scanf("%d",&puntos[i][1]);
		al_draw_circle(puntos[i][0],SCREEN_H - puntos[i][1], 1, rojo,2);//dibujo de los puntos en la pantalla.
		al_flip_display();
    }

    ALLEGRO_EVENT_QUEUE *queue = NULL;
	queue = al_create_event_queue();
	if(!queue) {
	return 0;
	}
	ALLEGRO_EVENT event;
	al_register_event_source(queue, al_get_keyboard_event_source());
	do{
	al_wait_for_event(queue, &event);
	}
	while (event.type != ALLEGRO_EVENT_KEY_DOWN); //espera para que el usuario aprete una tecla para que el programa siga.

    int a=0;
    for(i=0;i<n;i++){ //creación de todas las lineas.
		for(j=i+1;j<n;j++){
				l[a].x0=puntos[i][0];
				l[a].y0=puntos[i][1];
				l[a].x1=puntos[j][0];
				l[a].y1=puntos[j][1];
				a++;
		}
	}
	quicksort(l,0,nl-1); // ordenamiento de las lineas para que queden primero las con menor longitud.


	_linea *sol = (_linea *)malloc(sizeof(_linea)*nl); // conjunto de líneas que forman la solución.

	int sol_size = 0;
	for (i=0;i<nl;i++){ //comprobar si cada linea existente se intersecta con las lineas que pertenecen a la solución
		int flag=1;
		for(j=0;j<sol_size;j++){
			if(inter(l[i],sol[j])==1){ 
				flag=0;
				j=sol_size;
			}
		}
		if(flag==1){ // si no se intersecta con ninguna linea que sea parte de la solución se guarda como parte de la solución
				sol[sol_size]=l[i];
				al_draw_line(sol[sol_size].x0,SCREEN_H-sol[sol_size].y0, sol[sol_size].x1,SCREEN_H-sol[sol_size].y1, azul, 1);
				al_flip_display();
				sol_size++;
			}
	}

	al_rest(3);
	al_destroy_display(display);
	return 0;
}