/*
Exemplo 1:
** Uso do mecanismo de comunicação síncrona
*/

// Leds ligados nas portas do arduino
#define LED_AZUL        2
#define LED_VERDE       3
#define LED_VERMELHO    4
#define LED_AMARELO     5

#include "ChRt.h"

thread_t *cliente, *servidor;

// Implementacao das tarefas
static THD_WORKING_AREA(waThread_Cliente, 64);
static THD_FUNCTION(Thread_Cliente, arg) 
{
  msg_t msgs[3] = {0, 1, 2};
  int i = 0;

  while (true) {
    // Envia mensagem para o servidor
    if (chMsgSend(servidor, msgs[i]) == MSG_OK) {
      i = (i + 1) % 3;    
      digitalWrite(LED_VERMELHO, HIGH);
    }
    chThdSleepMilliseconds(200);
    digitalWrite(LED_VERMELHO, LOW);    
  }
}

static THD_WORKING_AREA(waThread_Servidor, 64);
static THD_FUNCTION(Thread_Servidor, arg) 
{
  msg_t msg = 0;

  while (true) {        
    // Aguarda comunicação de um cliente
    chMsgWait();
    msg = chMsgGet(cliente);
    digitalWrite(LED_AMARELO, HIGH);    

    if (msg == 1) {
      digitalWrite(LED_VERDE, HIGH);
    }
    else if (msg == 2) {
      digitalWrite(LED_AZUL, HIGH);
    }
    else if (msg == 0) {
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, LOW); 
    }
    chMsgRelease(cliente, MSG_OK);    
    // Aguarda por 200 milisegundos
    chThdSleepMilliseconds(200);
    digitalWrite(LED_AMARELO, LOW);
  }
}

void chSetup()
{
  // Criação das tarefas
  cliente = chThdCreateStatic(waThread_Cliente, sizeof(waThread_Cliente), NORMALPRIO, Thread_Cliente, NULL);
  servidor = chThdCreateStatic(waThread_Servidor, sizeof(waThread_Servidor), NORMALPRIO, Thread_Servidor, NULL);
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
