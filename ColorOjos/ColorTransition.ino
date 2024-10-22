int ledRojo = 11;
int ledVerde = 10;
int ledAzul = 9;

void setup() {
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
}

void loop() {
  // Transiciones suaves entre colores
  smoothTransition(255, 0, 0, 0, 255, 0, 50);  // Rojo a verde
  smoothTransition(0, 255, 0, 0, 0, 255, 50);  // Verde a azul
  smoothTransition(0, 0, 255, 255, 255, 255, 50);  // Azul a blanco
  smoothTransition(255, 255, 255, 255, 255, 0, 50);  // Blanco a amarillo
  smoothTransition(255, 255, 0, 255, 0, 255, 50);  // Amarillo a magenta
  smoothTransition(255, 0, 255, 0, 255, 255, 50);  // Magenta a cian
  smoothTransition(0, 255, 255, 255, 0, 128, 50);  // Cian a rosa
}

void smoothTransition(int r1, int g1, int b1, int r2, int g2, int b2, int steps) {
  for (int i = 0; i <= steps; i++) {
    int r = map(i, 0, steps, r1, r2);
    int g = map(i, 0, steps, g1, g2);
    int b = map(i, 0, steps, b1, b2);

    analogWrite(ledRojo, r);
    analogWrite(ledVerde, g);
    analogWrite(ledAzul, b);

    delay(30); // Controla la velocidad de la transición
  }
}


