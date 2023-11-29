/*
Exemplo 1:
** Criação de tarefas
** Teste do algoritmo de escalonamento
*/

// Leds ligados nas portas do arduino
#define LED_AZUL        2
#define LED_VERDE       3
#define LED_VERMELHO    4
#define LED_AMARELO     5


#include "ChRt.h"

//SEMAPHORE_DECL(sem, 0);


// Implementacao das tarefas
static THD_WORKING_AREA(waThread_Azul, 64);
static THD_FUNCTION(Thread_Azul, arg) 
{
  while (true) {
    digitalWrite(LED_AZUL, !digitalRead(LED_AZUL));
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
    //chThdSleepMilliseconds(200);
  }
}

static THD_WORKING_AREA(waThread_Vermelho, 64);
static THD_FUNCTION(Thread_Vermelho, arg) 
{
  while (true) {
    digitalWrite(LED_VERMELHO, !digitalRead(LED_VERMELHO));
    // Aguarda por 200 milisegundos
   //chThdSleepMilliseconds(200);
  }
}

static THD_WORKING_AREA(waThread_Amarelo, 64);
static THD_FUNCTION(Thread_Amarelo, arg) 
{
  while (true) {
    digitalWrite(LED_AMARELO, !digitalRead(LED_AMARELO));
    // Aguarda por 200 milisegundos
    //chThdSleepMilliseconds(200);
  }
}

void chSetup()
{
  // Criação das tarefas
  chThdCreateStatic(waThread_Azul, sizeof(waThread_Azul), NORMALPRIO+2, Thread_Azul, NULL);
  chThdCreateStatic(waThread_Verde, sizeof(waThread_Verde), NORMALPRIO+2, Thread_Verde, NULL);
  chThdCreateStatic(waThread_Vermelho, sizeof(waThread_Vermelho), NORMALPRIO, Thread_Vermelho, NULL);
  chThdCreateStatic(waThread_Amarelo, sizeof(waThread_Amarelo), NORMALPRIO, Thread_Amarelo, NULL);
}

void setup()
{
  // Configuração do fluxo de dados das portas
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);


  // Inicialização do kernel do SO
  chBegin(chSetup);

  while (true);

}

void loop()
{

}
