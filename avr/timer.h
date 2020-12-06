#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

/*
  MÒDUL TIMER:: temporalització. */

#define TIMER_MS(ms)(ms/10) //macro que converteix unitats de temps. de milisegons a ticks.
#define TIMER_ERR(timer_handler_t) -1 //contant de tipus handlert que indica un error. 

typedef void (*timer_callback_t)(void); //Apuntador a una funció sense paràmetres.

typedef int8_t timer_handler_t; //Identifica una funció que s'ha planificat per ser executada en un futur sota el control d'aquest mòdul.

void timer_init(void); //Inicialitza el mòdul. 

timer_handler_t timer_after(uint8_t ticks, timer_callback_t f); /*Planifica una funció per ser executada al cap de tick ticks.
								  Retorna un handler que identifica l'acció planificada. 
								  Sinó es pot planifica retorna TIMER_ERR. */

timer_handler_t timer_every(uint8_t ticks, timer_callback_t f); //Planifica una funció per ser executada cada tick ticks indefinidament. 

timer_handler_t timer_ntimes(uint8_t n, uint8_t ticks, timer_callback_t f); //Igual que l'anterior però cada n vegades. 

void timer_cancel(timer_handler_t h); //Cancela una acció planificada.
 
void timer_cancel_all(void); //Cancela totes les accions planificades. 

//Crono

typedef uint8_t timer_chrono_t; //Handler que fa referència al cronòmetre.

timer_chrono_t chrono(void); //Crea un nou cronòmetre i retorna el handler. 

void chrono_start(timer_chrono_t c); //Posa a zero el cronòmetre i fa que comenci a comptar. 

uint8_t chrono_get(timer_chrono_t c); //Retorna el temps transcorregut des del darrer star del cronòmetre.

void chrono_stop(timer_chrono_t c); //Atura el cronòmetre però no el destrueix.

void chrono_cancel(timer_chrono_t c); //Destrueix el cronòmetre. El handler és invàlid. 

#endif
