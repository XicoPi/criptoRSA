#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include "timer.h"
#include "serial_device.h"

#define N 20
typedef enum {CHRONO, TIMER, EMPTY}entry_type;

typedef enum {ON, OFF}chrono_state;

typedef struct {
  chrono_state c_state;
  uint8_t mesure;
}chrono_entry_t;

typedef struct {
  uint8_t remaining, every, ntimes;
  timer_callback_t callback;
}timer_entry_t;

typedef union {
  timer_entry_t t_timer;
  chrono_entry_t c_timer;
}u_entry;

typedef struct {
  u_entry unio;
  entry_type state;
}entry;

static struct {
  entry t[N];
  uint8_t n;
}table;


static void timer_enable(void) {

  TCNT1H = 0;
  TCNT1L = 0;
  
  TCCR1B &= ~(_BV(CS10));
  TCCR1B &= ~(_BV(CS11));
  TCCR1B |= _BV(CS12);
}

static void timer_disable(void) {

  TCCR1B &= ~(_BV(CS10));
  TCCR1B &= ~(_BV(CS11));
  TCCR1B &= ~(_BV(CS12));
}

ISR(TIMER1_COMPA_vect) {
  uint8_t i,n;
  //índex de la taula
  i = 0;
  n = 20;
  //esquema de recorregut per a recorrer tota la taula
  while (i < n) {
    
    if (table.t[i].state != EMPTY) {
      if (table.t[i].state == TIMER) {
	table.t[i].unio.t_timer.remaining--;
	
	if (table.t[i].unio.t_timer.remaining == 0) {
	  (*(table.t[i].unio.t_timer.callback))();
	  
	  //el cas si s'ha d'executar de manera indefinida o infinita
	  if (table.t[i].unio.t_timer.ntimes == 0) {
	    table.t[i].unio.t_timer.remaining = table.t[i].unio.t_timer.every;
	  }
	  
	  //si és l'últim cicle, s'ha de borrar de la taula
	  else if (table.t[i].unio.t_timer.ntimes == 1) {
	    timer_cancel(i);
	  }
	  
	  else {
	    table.t[i].unio.t_timer.remaining = table.t[i].unio.t_timer.every;
	    table.t[i].unio.t_timer.ntimes--;
	  }
	}
      }
      else if ((table.t[i].state == CHRONO) && (table.t[i].unio.c_timer.c_state == ON)) {
	table.t[i].unio.c_timer.mesure++;
      }
    }
    i++;
  }
  
  //si no queden elements en la taula desactivem les interrupcions del timer (No fa falta, ho fa la funció cancel)
  /*  if (table.n == 0) {
    
    timer_disable();
    }*/
}



void timer_init(void) {

  //variable estàtica d'inicialització del timer
  static bool t_init = false;

  //incialització de la taula de funcions i desactivació de les interrupcions del TIMER
  if (!t_init) {
    timer_cancel_all();
  
    //Bits 4:3 a 01 -> Mode CTC (Compare Match)
    TCCR1A &= ~(_BV(WGM10));
    TCCR1A &= ~(_BV(WGM11));
    TCCR1B |= _BV(WGM12);
    TCCR1B &= ~(_BV(WGM13));
    OCR1A = 624;//és el nivell de comparació que serveix per trobar la freqüència del timer (10ms)
    TIMSK1 |= _BV(OCIE1A);
    t_init = true;
  }
}

timer_handler_t timer_after(uint8_t ticks, timer_callback_t f) {
  
  timer_handler_t h;
  
  if (table.n == N) {
    h = -1;
  }
  else {
    timer_entry_t new;
    h = 0;
    while (!(table.t[h].state == EMPTY)) {
      h++;
    }
    if (table.t[h].state == EMPTY) {
      new.remaining = ticks;
      new.every = ticks;
      new.ntimes = 1;
      new.callback = f;
      table.t[h].unio.t_timer = new;
      table.t[h].state = TIMER;
      table.n++;
    }
  }
  if (table.n == 1) {
    timer_enable();
  }
  return h;
}

timer_handler_t timer_every(uint8_t ticks, timer_callback_t f) {
  
  timer_handler_t h;

  if (table.n == N) {
    h = -1;
  }
  else {
   timer_entry_t new;
    h = 0;
    while (!(table.t[h].state == EMPTY)) {
      h++;
    }
    if (table.t[h].state == EMPTY) {
      new.remaining = ticks;
      new.every = ticks;
      new.ntimes = 0;
      new.callback = f;
      table.t[h].unio.t_timer = new;
      table.t[h].state = TIMER;
      table.n++;
    }
    
  }
   if (table.n == 1) {
    timer_enable();
   }
  return h;
}

timer_handler_t timer_ntimes(uint8_t n, uint8_t ticks, timer_callback_t f) {
  
  timer_handler_t h;

  if (table.n == N) {
    h = -1;
  }
  else {
    timer_entry_t new;
    h = 0;
    while (!(table.t[h].state == EMPTY)) {
      h++;
    }
    if (table.t[h].state == EMPTY) {
      new.remaining = ticks;
      new.every = ticks;
      new.ntimes = n;
      new.callback = f;
      table.t[h].unio.t_timer = new;
      table.t[h].state = TIMER;
      table.n++;
    }
  }
  if (table.n == 1) {
    timer_enable();
  }
  return h;
}

void timer_cancel(timer_handler_t h) {

  if (table.t[h].state == TIMER) {
    table.t[h].state = EMPTY;
    table.n--;
  }
  if (table.n == 0) {
    timer_disable(); 
  }
}

void timer_cancel_all(void) {

  uint8_t i;
  timer_disable();
  i = 0;
  table.n = 0;
  while (i < 20) {
    table.t[i].state = EMPTY;
    i++;
  }
}

//CHRONO

timer_chrono_t chrono(void) {

  timer_chrono_t h;
  h = 0;
  if (table.n == N) {
    h = -1;
  }
  else {
    chrono_entry_t new;
    while (table.t[h].state != EMPTY) {
      h++;
    }
    new.mesure = 0;
    new.c_state = OFF;
    table.t[h].state = CHRONO;
    table.t[h].unio.c_timer=new;
    table.n++;
    
  }
  return h;
}

void chrono_start(timer_chrono_t c) {
  
  if (c < 20 && table.t[c].state == CHRONO) {
    table.t[c].unio.c_timer.c_state = ON;
    table.t[c].unio.c_timer.mesure = 0;
  }
  
  timer_enable();

}

uint8_t chrono_get(timer_chrono_t c) {
  uint8_t result;
  result = -1;
  if (c < 20 && table.t[c].state == CHRONO) {
    result = table.t[c].unio.c_timer.mesure;   
  }
  return result;
}

void chrono_stop(timer_chrono_t c) {

  if (c < 20 && table.t[c].state == CHRONO) {
    table.t[c].unio.c_timer.c_state = OFF;
  }
}

void chrono_cancel(timer_chrono_t c) {

  if (c < 20 && table.t[c].state == CHRONO) {
    table.t[c].state = EMPTY;
    table.n--;
  }
  if (table.n == 0) {
    timer_disable();
  }
}

