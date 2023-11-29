#include <ChRt.h>

#define ESPERANDO       0
#define REABASTECER     1
#define PREPARANDO      2
#define ESQUENTANDO     3

#define sens_n_agua     PIND0
#define sens_n_po       PIND1
#define sens_iniciar    PIND2
#define sens_temp_agua  PINC0

#define led_verde       PIND3
#define led_vermelho    PIND4
#define led_amarelo     PIND5

MUTEX_DECL(mutex);
const uint8_t buffer_size = 4;
msg_t buffer[buffer_size];
mailbox_t fila_mensagem;
//MAILBOX_DECL(fila_mensagem, buffer, buffer_size);

static THD_WORKING_AREA(sensores, 64);
static THD_FUNCTION(Controle_Central, arg) {
  uint8_t status = ESPERANDO;
  uint8_t t_agua;

  while (true) {
    uint8_t n_agua = digitalRead(sens_n_agua);
    uint8_t n_po = digitalRead(sens_n_po);
    uint8_t iniciar = digitalRead(sens_iniciar);
    uint8_t t_agua = analogRead(sens_temp_agua);
    
    if(n_agua == 1 || n_po == 1){
      status = REABASTECER;
      digitalWrite(led_amarelo, 1);
    }
    else if(n_agua == 1 && n_po == 1 && iniciar == 1){
      status = ESPERANDO;
      digitalWrite(led_amarelo, 0);
    }
    else if(n_agua == 0 && n_po == 0 && iniciar == 0 && t_agua < 127){
      status = ESQUENTANDO;
      digitalWrite(led_amarelo, 0);
      
      chMBPostTimeout(&fila_mensagem, status, TIME_INFINITE);
      
      chMtxUnlock(&mutex);
    }
    else if(n_agua == 0 && n_po == 0 && iniciar == 0 && t_agua >= 127){
      status = PREPARANDO;
      digitalWrite(led_amarelo, 0);
      
      chMBPostTimeout(&fila_mensagem, status, TIME_INFINITE);
      
      chMtxUnlock(&mutex);
    }
  }
}

void aquecer()
{
  digitalWrite(led_vermelho, 1);
    
  return;
}

void dispenser()
{
  digitalWrite(led_verde, 1);
  
  return;
}

static THD_WORKING_AREA(atuadores, 64);
static THD_FUNCTION(Controle_Atuadores, arg) {
  msg_t status_fila;
    
  while(1){
    chMBFetchTimeout(&fila_mensagem, &status_fila, TIME_INFINITE);
    
    chMtxLock(&mutex);
    
    if(status_fila == ESQUENTANDO){
      aquecer();
      chThdSleep(TIME_MS2I(50));
      digitalWrite(led_vermelho, 0);
      
      chMtxUnlock(&mutex);
    }
    else if(status_fila == PREPARANDO){
      dispenser();
      chThdSleep(TIME_MS2I(50));
      digitalWrite(led_verde, 0);
      
      chMtxUnlock(&mutex);
    }
  }
}

void chSetup() {
  // criacao das tarefas
  chThdCreateStatic(sensores, sizeof(sensores), NORMALPRIO, Controle_Central, NULL);
  chThdCreateStatic(atuadores, sizeof(atuadores), NORMALPRIO, Controle_Atuadores, NULL);
}

void setup() {
  // Configuração do fluxo de dados das portas
  pinMode(sens_n_agua, INPUT);
  pinMode(sens_n_po, INPUT);
  pinMode(sens_iniciar, INPUT);
  pinMode(sens_temp_agua, INPUT);

  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_vermelho, OUTPUT);

  chMtxObjectInit(&mutex);
  chMBObjectInit(&fila_mensagem, buffer, buffer_size);

  // Inicialização do kernel do SO
  chBegin(chSetup);

  while (true);
  
}

void loop() {

}
