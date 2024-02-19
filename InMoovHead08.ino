/*
Código para mover servomotores de la cabeza InMoov

Notas de la versión:
4/7/2019
Se le añade un tercer modo de operación:
modo 1: se mueve con el joystick
modo 2: se mueve en una secuencia determinada
modo 3: se mueve a una posición que recibe por el puerto serie (para ser usado con programa processing Seguir_color)
*/
#include <Servo.h>

//LED del Arduino para pruebas
const int LEDArduino = 13; 

//Pines de entrada analógica de los joystick
const int pinJoystickOjosX = A1;  
const int pinJoystickOjosY = A0;
const int pinJoystickCuelloX = A3;  
const int pinJoystickCuelloY = A2; //*********
//Pines de entrada digital para botones
const int pinBotonBocaAbre = 8;
const int pinBotonBocaCierra = 6;
const int pinBotonControlRemoto = 3;
const int pinBotonEsperaActiva = 4;

//Pines de salida, señal de control de los servos
const int pinServoOjosX= 10;
const int pinServoOjosY = 11;
const int pinServoCuelloX= 12;
const int pinServoCuelloY = 13;
const int pinServoBoca = 9;

//Cambio en la posición del servo, en grados, en cada ciclo
const int cambioServo = 1;

//Posiciones máximas y mínimas de los servos
const int posOjosXmax= 76;
const int posOjosXmin= 32;
const int posOjosYmax= 126;
const int posOjosYmin= 50;
const int posCuelloXmax= 135;
const int posCuelloXmin= 40;
const int posCuelloYmax= 120;
const int posCuelloYmin= 50;
const int posBocamax= 116;
const int posBocamin= 64;

//Valores para ajustar el punto neutro global de los joystick
const int neutralMin=430;
const int neutralMax=510;

//Declaración de los servos para la librería
Servo servoOjosX;
Servo servoOjosY;
Servo servoCuelloX;
Servo servoCuelloY;
Servo servoBoca;

//Variables de posición a escribir en los servos, inicializadas en la posición central
int posOjosX = 58;        
int posOjosY = 90;
int posCuelloX = 90;        
int posCuelloY = 78;
int posBoca = 66;

//Variables de lectura de los joystick
int jsOjosX = 0;
int jsOjosY = 0;
int jsCuelloX = 0;
int jsCuelloY = 0;
bool btBocaAbre = 0;
bool btBocaCierra = 0;

//Variables de mapeado de los joystick
int mapOjosX = 0;
int mapOjosY = 0;
int mapCuelloX = 0;
int mapCuelloY = 0;

enum PosiblesEstados {CONTROL_REMOTO=0, ESPERA_ACTIVA, VISTA, NADA};
char *PosEstados[] = {"CONTROL_REMOTO", "ESPERA_ACTIVA", "VISTA", "NADA"};
PosiblesEstados estado = NADA;

//Variables para puerto serie
byte numCuadro=72;

//Matriz de posiciones para la secuencia de Espera Activa
int contadorPosEA=0;
const int posicionesEA[][5]={{58,90,90,78,66},//*Posicion 0
                             {58,90,85,83,66},
                             {58,90,80,88,66},
                             {58,90,75,93,66},
                             {58,90,70,98,66},
                             {58,90,68,103,66},
                             {58,90,65,107,66},
                             {58,90,63,111,66},//*Posicion 1
                             {58,90,63,111,66},
                             {58,90,63,111,66},
                             {58,90,63,111,66},
                             {58,90,63,111,66},
                             {58,90,68,112,66},
                             {58,90,73,113,66},
                             {58,90,78,114,66},
                             {58,90,83,116,66},
                             {58,90,88,118,66},
                             {58,90,94,120,66},//*Posicion 2
                             {58,90,94,120,66},
                             {58,90,94,120,66},
                             {58,90,94,120,66},
                             {58,90,94,120,66},
                             {58,90,93,110,66},
                             {58,90,92,100,66},
                             {58,90,90,95,66},
                             {58,90,89,90,66},
                             {58,90,88,85,66},
                             {58,90,86,80,66},
                             {58,90,84,75,66},
                             {58,90,83,70,66},
                             {58,90,82,65,66},
                             {58,90,80,60,66},
                             {58,90,79,57,66},
                             {58,90,77,52,66},//*Posicion 3
                             {58,90,77,52,66},
                             {58,90,77,52,66},
                             {58,90,77,52,66},
                             {58,90,77,52,66},
                             {58,90,82,56,66},
                             {58,90,87,60,66},
                             {58,90,92,64,66},
                             {58,90,97,65,66},
                             {58,90,102,66,66},
                             {58,90,107,68,66},
                             {58,90,112,70,66},
                             {58,90,117,72,66},
                             {58,90,124,74,66},//*Posicion 4
                             {58,90,124,74,66},
                             {58,90,124,74,66},
                             {58,90,124,74,66},
                             {58,90,124,74,66},
                             {58,90,119,79,66},
                             {58,90,114,84,66},
                             {58,90,109,89,66},
                             {58,90,104,92,66},
                             {58,90,99,94,66},
                             {58,90,96,96,66},//*Posicion 5
                             {58,90,96,96,66},
                             {58,90,96,96,66},
                             {58,90,96,96,66},
                             {58,90,96,96,66},
                             {58,90,91,91,66},
                             {58,90,86,86,66},
                             {58,90,81,81,66},
                             {58,90,76,76,66},
                             {58,90,71,71,66},
                             {58,90,66,66,66},
                             {58,90,64,61,66},//*Posicion 6 
                             {58,90,64,61,66},
                             {58,90,64,61,66},
                             {58,90,64,61,66},
                             {58,90,64,61,66},
                             {58,90,69,66,66},
                             {58,90,74,71,66},
                             {58,90,79,76,66},
                             {58,90,84,81,66},
                             {58,90,88,87,66},//*Posicion 7
                             {58,90,88,87,66},
                             {58,90,88,87,66},
                             {58,90,88,87,66},
                             {58,90,88,87,66},
                             {68,80,89,82,85},
                             {68,80,89,82,100},
                             {68,80,89,82,100},
                             {68,80,89,82,100},
                             {68,80,89,82,85},
                             {58,90,90,78,66}};//*Posicion 0

//posOjosXmax= 76; 32;
//posOjosYmax= 126; 50;
//posCuelloXmax= 135; 40;
//posCuelloYmax= 120; 50;
//posBocamax= 116; 64;


void setup() {
  Serial.begin(9600);

  servoOjosX.attach(pinServoOjosX);
  servoOjosY.attach(pinServoOjosY);
  servoCuelloX.attach(pinServoCuelloX);
  servoCuelloY.attach(pinServoCuelloY);
  servoBoca.attach(pinServoBoca);
}


void loop() {

  //lectura del estado de los botones que cambian el estado
  //son activos en bajo
  int btnEsperaActiva= digitalRead(pinBotonEsperaActiva);
  int btnControlRemoto= digitalRead(pinBotonControlRemoto);

  if (btnControlRemoto==LOW && btnEsperaActiva==LOW) {
    estado=VISTA;
  }else if (btnControlRemoto==LOW) {
    estado=CONTROL_REMOTO;
  }else if (btnEsperaActiva==LOW) {
    estado=ESPERA_ACTIVA;
  } 

 //Máquina de estados que cambia el modo de operación
   switch (estado) {
      case CONTROL_REMOTO: { 
        MovimientoControlRemoto();  
        break; 
      }
      case ESPERA_ACTIVA:  { 
        MovimientoEsperaActiva();  
        break; 
      }
      case VISTA:  { 
        MovimientoVista();  
        break; 
      }
      case NADA: { 
        break; 
      }
  }

  //Enviar nueva posición a los servos
  servoOjosX.write(posOjosX);
  servoOjosY.write(posOjosY);
  servoCuelloX.write(posCuelloX);
  servoCuelloY.write(posCuelloY);
  servoBoca.write(posBoca);

////Escribir posición de los servos en la consola
  Serial.print("OjosX= ");
  Serial.print(posOjosX);
  Serial.print("  OjosY= ");
  Serial.print(posOjosY);
  Serial.print("  CuelloX= ");
  Serial.print(posCuelloX);
  Serial.print("  CuelloY= ");
  Serial.print(posCuelloY);
  Serial.print("  Boca= ");
  Serial.println(posBoca);

//  Serial.print("estado= ");
//  Serial.print(PosEstados[estado]);
//  Serial.print("   btonD= "); 
//  Serial.print(btnEsperaActiva);
//  Serial.print("  btonE= ");
//  Serial.println(btnControlRemoto);


  delay(80); //Retardo entre cada cambio al valor de posición de los servos
}



void MovimientoControlRemoto(){
//Método para indicar vlaores a los servomotores en base a los valores de los joysticks y botones

//Leer valores de los joystick
  jsOjosX = analogRead(pinJoystickOjosX);
  jsOjosY = analogRead(pinJoystickOjosY);
  jsCuelloX = analogRead(pinJoystickCuelloX);
  jsCuelloY = analogRead(pinJoystickCuelloY);
  btBocaAbre = digitalRead(pinBotonBocaAbre);
  btBocaCierra = digitalRead(pinBotonBocaCierra);
    
  //Debido a que el shield esta al reves, se le da vuelta al dato
  //jsOjosX = map(jsOjosX, 0, 1023, 1023, 0);
  //jsOjosY = map(jsOjosY, 0, 1023, 1023, 0);

//Mapear los valores de posición del joystick para que sean equivalentes al rango de cada servo
  mapOjosX = map(jsOjosX, 0, 1023, posOjosXmin, posOjosXmax);
  mapOjosY = map(jsOjosY, 0, 1023, posOjosYmin, posOjosYmax);
  mapCuelloX = map(jsCuelloX, 0, 880, posCuelloXmin, posCuelloXmax);
  mapCuelloY = map(jsCuelloY, 0, 880, posCuelloYmin, posCuelloYmax);

//Si el joystick se movió fuera del punto neutro comenzar a cambiar la posición del servo hasta lo que señale el joystick
  if(jsOjosX<neutralMin || jsOjosX>neutralMax){
    if(posOjosX < mapOjosX){posOjosX = posOjosX+1;}
    else if(posOjosX > mapOjosX){posOjosX = posOjosX-1;}
  }

  if(jsOjosY<neutralMin || jsOjosY>neutralMax){
    if(posOjosY < mapOjosY){posOjosY = posOjosY+1;}
    else if(posOjosY > mapOjosY){posOjosY = posOjosY-1;}
  }

  if(jsCuelloX<434 || jsCuelloX>438){
    if(posCuelloX < mapCuelloX){posCuelloX = posCuelloX+1;}
    else if(posCuelloX > mapCuelloX){posCuelloX = posCuelloX-1;}
  }

  if(jsCuelloY<443 || jsCuelloY>447){
    if(posCuelloY < mapCuelloY){posCuelloY = posCuelloY+1;}
    else if(posCuelloY > mapCuelloY){posCuelloY = posCuelloY-1;}
  }

  /*if (jsOjosX > 800 & posOjosX < posOjosXmax) { //Los números identtifican la deflección mínima del joystick para comenzar el movimiento
    posOjosX = posOjosX + cambioServo;
  }
  if (jsOjosX < 200 & posOjosX > posOjosXmin) { 
    posOjosX = posOjosX - cambioServo;
  }
  
  if (jsOjosY > 800 & posOjosY < posOjosYmax) { 
    posOjosY = posOjosY + cambioServo;
  }
  if (jsOjosY < 200 & posOjosY > posOjosYmin) { 
    posOjosY = posOjosY - cambioServo;
  }

  if (jsCuelloX > 800 & posCuelloX < posCuelloXmax) { 
    posCuelloX = posCuelloX + cambioServo;
  }
  if (jsCuelloX < 200 & posCuelloX > posCuelloXmin) { 
    posCuelloX = posCuelloX - cambioServo;
  }

  if (jsCuelloY > 800 & posCuelloY < posCuelloYmax) { 
    posCuelloY = posCuelloY + cambioServo;
  }
  if (jsCuelloY < 200 & posCuelloY > posCuelloYmin) { 
    posCuelloY = posCuelloY - cambioServo;
  }*/

  if (btBocaAbre & posBoca < posBocamax) { 
    posBoca = posBoca + cambioServo;
  }
  if (btBocaCierra & posBoca > posBocamin) { 
    posBoca = posBoca - cambioServo;
  }

}

void MovimientoEsperaActiva(){
  
  posOjosX= posicionesEA[contadorPosEA][0];
  posOjosY= posicionesEA[contadorPosEA][1];
  posCuelloX= posicionesEA[contadorPosEA][2];
  posCuelloY= posicionesEA[contadorPosEA][3];
  posBoca= posicionesEA[contadorPosEA][4];
  
  if (contadorPosEA == (sizeof(posicionesEA)/sizeof(posicionesEA[0])-1) ){ //reinicia el contador cuando está en la última posición
    contadorPosEA=0;
  }else{
    contadorPosEA++;
  }

  //Verificación de seguridad por si se escribe algún ángulo incorrecto
  if (posOjosX < posOjosXmin){ posOjosX = posOjosXmin;
  }else if (posOjosX > posOjosXmax){ posOjosX = posOjosXmax; } 
  if (posOjosY < posOjosYmin){ posOjosY = posOjosYmin;
  }else if (posOjosY > posOjosYmax){ posOjosY = posOjosYmax; } 

  if (posCuelloX < posCuelloXmin){ posCuelloX = posCuelloXmin;
  }else if (posCuelloX > posCuelloXmax){ posCuelloX = posCuelloXmax; } 
  if (posCuelloY < posCuelloYmin){ posCuelloY = posCuelloYmin;
  }else if (posCuelloY > posCuelloYmax){ posCuelloY = posCuelloYmax; } 
  
  if (posBoca < posBocamin){ posBoca = posBocamin;
  }else if (posBoca > posBocamax){ posBoca = posBocamax;  }

  delay(80); //Retardo para que el cambio de posiciones sea un poco más lento
}


void MovimientoVista(){

  if (Serial.available()>0)  {
    
    digitalWrite(LEDArduino, 1);
    numCuadro = Serial.read();

    //revisa que el segundo mensaje sea un punto y coma
    if (numCuadro<128){
      posCuelloX= map(numCuadro%16, 0, 16, posCuelloXmin, posCuelloXmax);
      posCuelloY= map(numCuadro/16, 0, 8, posCuelloYmin, posCuelloYmax);;
    }
    
    //Verificación de seguridad por si se escribe algún ángulo incorrecto
    if (posCuelloX < posCuelloXmin){ posCuelloX = posCuelloXmin;
    }else if (posCuelloX > posCuelloXmax){ posCuelloX = posCuelloXmax; } 
    if (posCuelloY < posCuelloYmin){ posCuelloY = posCuelloYmin;
    }else if (posCuelloY > posCuelloYmax){ posCuelloY = posCuelloYmax; } 

    delay(30); //Retardo para que el cambio de posiciones sea un poco más lento

    digitalWrite(LEDArduino, 0);
  }
}
