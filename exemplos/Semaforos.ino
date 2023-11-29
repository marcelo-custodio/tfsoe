/*
Exemplo 1:
** Uso de semáforos
*/

// Leds ligados nas portas do arduino
#define LED_AZUL        2
#define LED_VERDE       3

#include "ChRt.h"


SEMAPHORE_DECL(sem, 0);


// Implementacao das tarefas
static THD_WORKING_AREA(waThread_Azul, 64);
static THD_FUNCTION(Thread_Azul, arg) 
{
  while (true) {
    digitalWrite(LED_AZUL, !digitalRead(LED_AZUL));
    chSemWait(&sem);
    // Aguarda por 200 milisegundos
    chThdSleepMilliseconds(200);    
  }
}

static THD_WORKING_AREA(waThread_Verde, 64);
static THD_FUNCTION(Thread_Verde, arg) 
{
  while (true) {
    digitalWrite(LED_VERDE, !digitalRead(LED_VERDE));
    // Aguarda por 200 milisegundos
    chThdSleepMilliseconds(300);
    chSemSignal(&sem);
  }
}

void chSetup()
{
  // Criação das tarefas
  chThdCreateStatic(waThread_Azul, sizeof(waThread_Azul), NORMALPRIO+2, Thread_Azul, NULL);
  chThdCreateStatic(waThread_Verde, sizeof(waThread_Verde), NORMALPRIO+2, Thread_Verde, NULL);
}

void setup()
{
  // Configuração do fluxo de dados das portas
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);


  // Inicialização do kernel do SO
  chBegin(chSetup);

  while (true);

}

void loop()
{

}
