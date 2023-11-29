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

void setup() {
  // Configuração do fluxo de dados das portas
  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_vermelho, OUTPUT);

  pinMode(sens_n_agua, INPUT);
  pinMode(sens_n_po, INPUT);
  pinMode(sens_iniciar, INPUT);
  pinMode(sens_temp_agua, INPUT);
  
}

void loop() {
  int iniciar = digitalRead(sens_iniciar);
  int n_agua = digitalRead(sens_n_agua);
  int n_po = digitalRead(sens_n_po);
  int t_agua = analogRead(sens_temp_agua);
  if (t_agua > 127) {
    if (iniciar == 0) digitalWrite(led_amarelo, 1); else digitalWrite(led_amarelo, 0);
    if (n_agua == 0) digitalWrite(led_vermelho, 1); else digitalWrite(led_vermelho, 0);
    if (n_po == 0) digitalWrite(led_verde, 1); else digitalWrite(led_verde, 0);
  } else {
    digitalWrite(led_verde, 0);
    digitalWrite(led_amarelo, 0);
    digitalWrite(led_vermelho, 0);
  }
  

}
