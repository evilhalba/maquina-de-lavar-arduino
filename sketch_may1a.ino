// C++ code
//
// Definindo os pinos dos botões
const int LIGAR_BTN_PIN = 8;
const int BAIXO_BTN_PIN = 9;
const int CIMA_BTN_PIN = 10;
const int ALARME_BTN_PIN = 11;

// Definindo os pinos dos LEDs
const int LED_MOTOR = 3;
const int ALARME_LED_PIN = 4;
const int LED_PORTA = 5;
const int LED_PAINEL = 2;
const int RELE_1_PIN = 6;
const int RELE_2_PIN = 7;

// Variáveis para controle de tempo
unsigned long tempo_ligado_rele_1_inicial = 0;
unsigned long tempo_ligado_rele_1 = 0;
unsigned long tempo_ligado_rele_2_inicial = 0;
unsigned long tempo_ligado_rele_2 = 0;
unsigned long tempo_botao_ligado = 0;
unsigned long tempo_botao_ligado_inicial = 0;
unsigned long TEMPO_GIRO = 6000; // 10 minutos em milissegundos
unsigned long TEMPO_ROLAGEM = 1000; // 1 minutos em milissegundos

// Variáveis para controle dos botões
bool ligar_btn_estado_anterior = LOW;
bool baixo_btn_estado_anterior = LOW;
bool cima_btn_estado_anterior = LOW;
bool alarme_btn_estado_anterior = LOW;
bool rele_1_estado = HIGH;
bool rele_2_estado = HIGH;
bool LED_MOTOR_ESTADO = LOW;





void setup()
{
  Serial.begin(9600);
  pinMode(LIGAR_BTN_PIN, INPUT_PULLUP);
  pinMode(BAIXO_BTN_PIN, INPUT_PULLUP);
  pinMode(CIMA_BTN_PIN, INPUT_PULLUP);
  pinMode(ALARME_BTN_PIN, INPUT_PULLUP);

  pinMode(LED_MOTOR, OUTPUT);
  pinMode(ALARME_LED_PIN, OUTPUT);
  pinMode(LED_PAINEL, OUTPUT);
  pinMode(RELE_1_PIN, OUTPUT);
  pinMode(RELE_2_PIN, OUTPUT);

  digitalWrite(RELE_1_PIN, HIGH); // desliga o relé 1
  digitalWrite(RELE_2_PIN, HIGH); // desliga o relé 2
  digitalWrite(LED_PAINEL, HIGH); // liga o led painel 
}



/*
tem 2 reles, ambos nao podem estar ativos ao mesmo tempo,
quando o botao ligar estiver ativo ele roda uma rotina que deixa o rele 1 ligado por 1 minuto e depois desliga o rele e também liga o rele 2 por um minuto e desliga
esse ciclo se reinicia até completar 10 min do botao ligar ativo, se caso o botao for desligado a rotina para
caso aperte o botao de emergencia todos os outros comandos sao ignorados até que seja desativado, e os reles desligam
os botoes para cima e para baixo nao podem se sobre por, ou seja so da pra acionar um por vez e o ligar nao pode estar acionado, nem o botao de emergencia
a logica da tampa aberta nao precisa fazer pq a gnt nao tem a fechadura eletronica ainda, quando tiver a gnt faz



*/


void loop()
{
   	// Lê o estado dos botões
  	bool ligar_btn_estado = digitalRead(LIGAR_BTN_PIN);
  	bool baixo_btn_estado = digitalRead(BAIXO_BTN_PIN);
  	bool cima_btn_estado = digitalRead(CIMA_BTN_PIN);
  	bool alarme_btn_estado = digitalRead(ALARME_BTN_PIN);
  	

  

  
  	//VERIFICA BOTOES
  	if(ligar_btn_estado == HIGH && ligar_btn_estado_anterior == LOW){
      Serial.println("BATEU EM BTN HIGH ANTERIOR LOW");
     // delay(10000);
      tempo_botao_ligado_inicial = millis();
      tempo_ligado_rele_1_inicial = millis();
      digitalWrite(RELE_1_PIN,LOW);
      digitalWrite(LED_MOTOR,HIGH);
      ligar_btn_estado_anterior = HIGH;
      rele_1_estado = LOW;
      rele_2_estado = HIGH;
      LED_MOTOR_ESTADO = HIGH;
           
    }
  	
    if(ligar_btn_estado == HIGH && ligar_btn_estado_anterior == HIGH){
      
      
  	  tempo_botao_ligado = millis() - tempo_botao_ligado_inicial;
      tempo_ligado_rele_1 = millis() - tempo_ligado_rele_1_inicial;
      tempo_ligado_rele_2 = millis() - tempo_ligado_rele_2_inicial;
      
    Serial.println("TEMPO BTN LIGADO "+ (String)tempo_botao_ligado);
    //delay(10000);
      if(tempo_botao_ligado > TEMPO_GIRO){
        Serial.println("BATEU EM BTN LIGADO >= TEMPO_GIRO");
        //delay(10000);
      	digitalWrite(RELE_1_PIN,HIGH);//DESLIGA O RELE 1
        digitalWrite(RELE_2_PIN,HIGH);//DESLIGA O RELE 2
        digitalWrite(LED_MOTOR,LOW);
        //ZERAR QUANDO O BOTAO FOR DESLIGADO
        
        
      }else{


        if(tempo_ligado_rele_1 > TEMPO_ROLAGEM && rele_1_estado == LOW){
          Serial.println("BATEU EM tempo_ligado_rele_1 > TEMPO_ROLAGEM");
          //delay(10000);
        
        
          if( rele_1_estado == LOW ){

            Serial.println("BATEU EM rele_1_estado == LOW");
            //delay(10000);
        	  digitalWrite(RELE_1_PIN,HIGH);//DESLIGA RELE 1 
            digitalWrite(RELE_2_PIN,HIGH);//GARANTE RELE 2 DESLIGADO
            digitalWrite(LED_MOTOR,LOW);
            rele_1_estado = HIGH;
            rele_2_estado = HIGH;
            LED_MOTOR_ESTADO = LOW;
          }
        
          if(rele_2_estado == HIGH){
            Serial.println("BATEU EM rele_2_estado == HIGH");
            delay(500);
            digitalWrite(RELE_2_PIN,LOW);//LIGO RELE2
            digitalWrite(LED_MOTOR,HIGH);
            rele_2_estado = LOW;
            LED_MOTOR_ESTADO = HIGH;
            tempo_ligado_rele_2_inicial = 0;
          }
        }
      
        if(tempo_ligado_rele_2 >= TEMPO_ROLAGEM && tempo_ligado_rele_2_inicial != 0 ){
          Serial.println("BATEU EM tempo_ligado_rele_2 >= TEMPO_ROLAGEM");
          //delay(10000);
          //tempo_botao_ligado_inicial = millis();
          tempo_ligado_rele_1_inicial = millis();
          digitalWrite(RELE_2_PIN,HIGH);
          delay(500);
          digitalWrite(RELE_1_PIN,LOW);
          digitalWrite(LED_MOTOR,HIGH);
          ligar_btn_estado_anterior = HIGH;
          rele_1_estado = LOW;
          rele_2_estado = HIGH;
          LED_MOTOR_ESTADO = HIGH;
          tempo_ligado_rele_2_inicial = 0;
          tempo_ligado_rele_2 = 0;
          tempo_ligado_rele_1 = 0;
        } 
        if(tempo_ligado_rele_2_inicial == 0 && rele_2_estado == LOW){  
          tempo_ligado_rele_2_inicial = millis();
        }
      
    
      
      
      }
    }

    //ATUALIZA ESTADOS DOS BOTOES SEMPRE
    if(ligar_btn_estado == LOW){
        digitalWrite(RELE_2_PIN, HIGH);
        digitalWrite(RELE_1_PIN, HIGH);
        digitalWrite(LED_MOTOR, LOW);
        ligar_btn_estado_anterior = LOW;
    }else{
      ligar_btn_estado_anterior = HIGH;
    }


    if(baixo_btn_estado == LOW){
      if(baixo_btn_estado_anterior == HIGH){
        
        digitalWrite(RELE_2_PIN, HIGH);
        digitalWrite(RELE_1_PIN, LOW);
        digitalWrite(LED_MOTOR, HIGH);
      }
      baixo_btn_estado_anterior = LOW;

    }else{
      //logica para ligar btn para baixo devo ter q por uma flag pra validar no if
      if(ligar_btn_estado == HIGH || alarme_btn_estado == HIGH || cima_btn_estado == HIGH){
        baixo_btn_estado_anterior = LOW;
      }else{
        baixo_btn_estado_anterior = HIGH;
        digitalWrite(RELE_2_PIN, HIGH);
        digitalWrite(RELE_1_PIN, LOW);
        digitalWrite(RELE_1_PIN, HIGH);
      }
    }

    if(cima_btn_estado == LOW){
      cima_btn_estado_anterior = LOW;
    }else{
      //logica para ligar btn para baixo devo ter q por uma flag
      /*if(){

      }*/
      cima_btn_estado_anterior = HIGH;
    }

    //esse é especial tem que travar o motor e ligar o led, so pode destravar tudo que travou quando desativar o botao  
    if(alarme_btn_estado == LOW){
      alarme_btn_estado_anterior = LOW;
    }else{
      alarme_btn_estado_anterior = HIGH;
    }

}

