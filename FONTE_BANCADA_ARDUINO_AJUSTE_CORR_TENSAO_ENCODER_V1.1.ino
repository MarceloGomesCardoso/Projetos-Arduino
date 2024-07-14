#include <RotaryEncoder.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

#define OUT 5 // pwm
#define SAIDA_FONTE A1// tensão lida na saída da fonte

// --- variaveis do controle de tensao e corrente

 int potenciometro;  //variável para armazenar o valor lido no potenciometro tensão constante
 int potcorrent;     //variável para armazenar o valor lido no potenciometro corrente ma´xima
 int leitura_fonte;  //variável para armazenar o valor lido na saída fonte
 int sensor_corrent; //variável para armazenar o valor lido de tensão shunt amplificado
 float corrente; //variável para armazenar o valor de corrente calculado pelo sinal shunt amplificado
 float tensao; //tensão
 float setpoint;
 float setcorrent;
 float divisor_tensao = 5.11;
 float ganho_opamp = 3.78;
 int PWM;




 // --- variaveis de controle de botoes ---

int PARA1 = 0;
int CONTA1 = 0;

int PARA2 = 0;
int CONTA2 = 0;

int PARA3 = 0;
int CONTA3 = 0;

 // --- mapeamento de botoes, buzzer ---

#define botaorele1 8
#define botaorele2 9
#define botaorele3 10
#define rele1 11
#define rele2 12
#define rele3 13
#define buzzer

 // --- encoder ---
RotaryEncoder EncoderOne(2, 3);
#define ButtonEncoder  encoder 4
int ValorAtual;
int ValorAntigo;


void setup()
 { 
  Serial.begin(250000);
    lcd.init();
    lcd.backlight();
    pinMode(rele1, OUTPUT);
    pinMode(OUT, OUTPUT); // configura pino como saída
    pinMode(A0, INPUT); 
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT); 
    PWM = potenciometro/4;
 }
 
 void loop()
 {

 // --- Encoder ---
EncoderOne.tick();// Começa a ler o valor do encoder
ValorAtual = EncoderOne.getPosition();//Captura o valor do encoder

 
    if (ValorAntigo != ValorAtual) {// Se houve mudança do valor do encoder
      Serial.println(ValorAtual);
      lcd.clear();// Limpa o display
      lcd.setCursor(5, 0);
      lcd.print(ValorAtual);
      ValorAntigo =  ValorAtual;//Carrega o valor antigo
    }//end if

 // ---------------
 
    //potenciometro = analogRead(A0);  // le o valor analógico
    potenciometro = ValorAtual; // atribui o valor do encoder ao setpoint
    leitura_fonte = analogRead(A1); // le valor da saída da fonte na entrada analogica ou calculado a media movel
    potcorrent = analogRead(A3);
    tensao = leitura_fonte*0.0047*divisor_tensao;
    setpoint = potenciometro*0.00488*divisor_tensao;
    //setpoint = 3.3; //valor para testar uma tensão fixa
    sensor_corrent = analogRead(A2); //le o valor de tensão amplificado do shunt
    corrente = sensor_corrent*0.00488/ganho_opamp;
    setcorrent = potcorrent*0.004; //potenciometro que ajusta corrente máxima


 // --- ajuste tensão constante e corrente máxima---
    if (setcorrent < corrente) {
      PWM = PWM -1;
      if (PWM < 0) PWM = 0;
    }
    else {
    if (setpoint > tensao) PWM = PWM + 1;
    if (setpoint < tensao) PWM = PWM - 1;
    if (PWM < 0) PWM = 0;
    if (PWM > 255) PWM = 255;
    }
   analogWrite(OUT, PWM); //envia pulso pwm para controle da fonte 

 // --- controle lcd ---
   lcd.setCursor(0,0); //SETA A POSIÇÃO DO CURSOR
   //lcd.print("Tensao: "); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(10,0); //SETA A POSIÇÃO DO CURSOR
   lcd.print(tensao, 1); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(15,0); //SETA A POSIÇÃO DO CURSOR
   lcd.print(" V"); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(0,1); //SETA A POSIÇÃO DO CURSOR
   lcd.print("SetTensao: "); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(12,1); //SETA A POSIÇÃO DO CURSOR
   lcd.print(setpoint, 1); //SEQUÊNCIA DE ESPAÇOS
   lcd.setCursor(17,1); //SETA A POSIÇÃO DO CURSOR
   lcd.print(" V"); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(0,2); //SETA A POSIÇÃO DO CURSOR
   lcd.print("Corrent: "); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(9,2); //SETA A POSIÇÃO DO CURSOR
   lcd.print(corrente, 1); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(15,2); //SETA A POSIÇÃO DO CURSOR
   lcd.print("A"); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(0,3); //SETA A POSIÇÃO DO CURSOR
   lcd.print("SetCorrent: "); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(13,3); //SETA A POSIÇÃO DO CURSOR
   lcd.print(setcorrent, 1); //IMPRIME O TEXTO NO DISPLAY LCD
   lcd.setCursor(19,3); //SETA A POSIÇÃO DO CURSOR
   lcd.print("A"); //IMPRIME O TEXTO NO DISPLAY LCD
   //lcd.clear();
   
// -------------------- controle de botoes --------------------------
// --- botaorele1 ---             
  if (digitalRead(botaorele1) == HIGH && PARA1 == 0) PARA1 = 1;
  if (digitalRead(botaorele1) == LOW && PARA1 == 1 ) { CONTA1++; PARA1 = 0;}
  if (CONTA1 > 0) digitalWrite(rele1, HIGH); else {digitalWrite(rele1, LOW);}
  if (CONTA1 > 1) CONTA1 = 0;

// --- botaorele2 ---             
  if (digitalRead(botaorele2) == HIGH && PARA2 == 0) PARA2 = 1;
  if (digitalRead(botaorele2) == LOW && PARA2 == 1 ) { CONTA2++; PARA2 = 0;}
  if (CONTA2 > 0) digitalWrite(rele2, HIGH); else {digitalWrite(rele2, LOW);}
  if (CONTA2 > 1) CONTA2 = 0;

// --- botaorele3 ---             
  if (digitalRead(botaorele3) == HIGH && PARA3 == 0) PARA3 = 1;
  if (digitalRead(botaorele3) == LOW && PARA3 == 1 ) { CONTA3++; PARA3 = 0;}
  if (CONTA1 > 0) digitalWrite(rele3, HIGH); else {digitalWrite(rele3, LOW);}
  if (CONTA1 > 1) CONTA3 = 0;

 }
 
