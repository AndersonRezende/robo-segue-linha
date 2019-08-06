/*
* robo_segue_linha
* Anderson Freire Rezende
* 
* Robô seguidor de linha, via sensores infravermelhos,
* com simples recurso de memória de percurso que tem
* o objetivo de gravar o último ponto pelo qual passou
* no intuito de retornar a trajetória original quando o
* mesmo se perde.
* 16/08/2017
* 
* Recursos necessários: 
* 1- 2x pares de sensores infravermelhos
* 2- 1x driver ponte H para controle dos motores
* 3- 2x motores DC
* 4- 1x ou 2x baterias (no caso de duas, uma pro arduino e outra para o driver)
* 5- 1x arduino
*/

#include <SoftwareSerial.h>   //Inlcui a biblioteca SoftwareSerial.h

#define motor11 3   //Roda esquerda
#define motor12 5   //Roda esquerda
#define motor21 6   //Roda direita
#define motor22 9   //Roda direita

#define sensor1 7   //sensor do lado esquerdo - azul | Controla a roda de direita (6,9)
#define sensor2 8   //sensor do lado direito - verde | Controla a roda da esquerda (3,5)

int s1,s2;          //Sensor == HIGH <=> Faixa preta //Sensor == LOW <=> Fora da faixa
int velocidade=70;

boolean faixa_direita_atual = false;  //Estas variáveis ativam quando o carrinho sai da faixa
boolean faixa_esquerda_atual = false;

boolean faixa_direita_antiga = false; //Estas variáveis gravam os valores antigos
boolean faixa_esquerda_antiga = false;

char saida;

void setup()
{
  Serial.begin(9600);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  pinMode(motor11, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(motor21, OUTPUT);
  pinMode(motor22, OUTPUT);

  digitalWrite(motor12, LOW);
  digitalWrite(motor21, LOW);
}


void loop()
{
  s1=digitalRead(sensor1);
  s2=digitalRead(sensor2);
  andar(s1,s2);
  //delay(20);
}


void andar(int s1,int s2)
{ 
  if(s1 == LOW)                         //Ta fora da faixa preta
    faixa_esquerda_atual = true;
  else                                  //Ta na faixa preta
    faixa_esquerda_atual = false;
  if(s2 == LOW)                         //Ta fora da faixa preta  
    faixa_direita_atual = true;
  else                                  //Ta na faixa preta
    faixa_direita_atual = false;


      

     
  if(faixa_esquerda_atual && faixa_direita_atual)           //Se ambos estiverem fora da faixa atualmente, então significa que o carrinho se perdeu da faixa...
  {
    if(faixa_esquerda_antiga && !faixa_direita_antiga)      //Se o primeiro sensor a ter saido da faixar foi o da esquerda, então deixe a roda direita desligada e ligue a esquerda
    {
      analogWrite(motor22,0);
      analogWrite(motor11,velocidade);
    }
    if(!faixa_esquerda_antiga && faixa_direita_antiga)      //Se o primeiro sensor a ter saido da faixa foi o da direita, então deixe a roda esquerda desligada e ligue a direita
    {
      analogWrite(motor22,velocidade);
      analogWrite(motor11,0);
    } 
  }
  else                                                      //Se pelo menos um estiver na faixa, então faça o ajuste para tentar se recuperar
  {
     if(faixa_esquerda_atual)                               //Se o sensor esquerdo estiver fora da faixa, então desligue a roda direita
        analogWrite(motor22,0);
     if(faixa_esquerda_atual == false)                      //Se o sensor esquerdo estiver sobre a faixa, então ligue a roda direita
        analogWrite(motor22,velocidade);
     if(faixa_direita_atual)                                //Se o sensor direito estiver fora da faixa, então desligue a roda esquerda
        analogWrite(motor11,0);
     if(faixa_direita_atual == false)                       //Se o sensor direito estiver sobre a faixa, então ligue a roda esquerda
        analogWrite(motor11,velocidade);
  }

  if (!(faixa_esquerda_atual && faixa_direita_atual))       //Se ambos não estiverem fora da faixa, entãoa tualize os valores antigos das variáveis
  {                                                       //Se estiverem fora da faixa não atualiza os dados para não perder o sentido de recuperação!!!
    faixa_direita_antiga = faixa_direita_atual;
    faixa_esquerda_antiga = faixa_esquerda_atual;
  }
}
