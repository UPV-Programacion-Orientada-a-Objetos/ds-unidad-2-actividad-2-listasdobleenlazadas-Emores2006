/**
 * @file arduino_prt7.ino
 * @brief Código Arduino para transmitir tramas del protocolo PRT-7
 * @author Eliezer Mores Oyervides
 * @date 2024
 * 
 * Este programa envía tramas de prueba a través del puerto serial
 * para ser decodificadas por el programa en C++ en Linux
 */

// Mensaje de prueba que será enviado
const char* tramas[] = {
  "L,H",
  "L,O",
  "L,L",
  "M,2",
  "L,A",
  "L,Space",
  "L,W",
  "M,-2",
  "L,O",
  "L,R",
  "L,L",
  "L,D"
};

const int numTramas = 12;
int indiceActual = 0;
bool transmisionCompletada = false;

void setup() {
  // Inicializar comunicación serial a 9600 baud
  Serial.begin(9600);
  
  // Esperar a que se establezca la conexión
  delay(2000);
  
  Serial.println("Arduino PRT-7 - Iniciando transmision...");
  delay(1000);
}

void loop() {
  if (!transmisionCompletada) {
    if (indiceActual < numTramas) {
      // Enviar la trama actual
      Serial.println(tramas[indiceActual]);
      
      // Avanzar al siguiente índice
      indiceActual++;
      
      // Esperar 1 segundo antes de enviar la siguiente trama
      delay(1000);
    } else {
      // Todas las tramas enviadas
      Serial.println("END");
      transmisionCompletada = true;
      Serial.println("Transmision completada.");
    }
  }
  // Una vez completada, no hacer nada más
}

/**
 * Función opcional para enviar tramas personalizadas
 * Se puede llamar desde el loop si se desea interactividad
 */
void enviarTramaPersonalizada(char tipo, char dato) {
  Serial.print(tipo);
  Serial.print(',');
  Serial.println(dato);
}

void enviarTramaMapeo(char tipo, int rotacion) {
  Serial.print(tipo);
  Serial.print(',');
  Serial.println(rotacion);
}
