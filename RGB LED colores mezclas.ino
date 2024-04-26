  int Rojo=3;
  int Azul=5;
  int Verde=6;
  int IntensidadR=0;
  int IntensidadA=0;
  int IntensidadV=0;
  int Rojo2=9;
  int Azul2=10;
  int Verde2=11;
  


void setup() {
  pinMode(Rojo, OUTPUT);
  pinMode(Azul, OUTPUT);
  pinMode(Verde, OUTPUT);
  pinMode(Rojo2, OUTPUT);
  pinMode(Azul2, OUTPUT);
  pinMode(Verde2, OUTPUT);
  
  
}

void loop() {
  
  while( IntensidadR < 175){
   analogWrite(Rojo , IntensidadR );
   analogWrite(Rojo2 , IntensidadR );
   analogWrite(Azul,IntensidadA);
   analogWrite(Azul2,IntensidadA);
   IntensidadA++;
   IntensidadR++;

   
   delay(50);

  }
  while(IntensidadR>0){
   analogWrite(Rojo,IntensidadR);
   analogWrite(Rojo2,IntensidadR);
   analogWrite(Azul,IntensidadA);
   analogWrite(Azul2,IntensidadA);
   IntensidadA--;
   IntensidadR--;
   delay(50);

  }
  while(IntensidadA<175){
   analogWrite(Azul,IntensidadA);
   analogWrite(Azul2,IntensidadA);
   analogWrite(Verde,IntensidadV);
   analogWrite(Verde2,IntensidadV);
   IntensidadV++;
   IntensidadA++;
   delay(50);

  }
  while(IntensidadA>0){
   analogWrite(Azul,IntensidadA);
   analogWrite(Azul2,IntensidadA);
   analogWrite(Verde,IntensidadV);
   analogWrite(Verde2,IntensidadV);
   IntensidadV--;
   IntensidadA--;
   delay(50);

  }
  while(IntensidadV<175){
   analogWrite(Verde,IntensidadV);
   analogWrite(Verde2,IntensidadV);
   analogWrite(Rojo,IntensidadR);
   analogWrite(Rojo2,IntensidadR);
   IntensidadR++;
   IntensidadV++;
   delay(50);

  }
  while(IntensidadV>0){
   analogWrite(Verde,IntensidadV);
   analogWrite(Verde2,IntensidadV);
   analogWrite(Rojo,IntensidadR);
   analogWrite(Rojo2,IntensidadR);
   IntensidadR--;
   IntensidadV--;
   delay(50);

  }
    
 

  

}
