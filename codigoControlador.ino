
#include <NewPing.h>
unsigned long int Tm=10.0, Tinicio=0.0;
const float  offset=127.5;
 float   Referencia=0.0,
Control=0.0, Kp=4.0, Ti=0.0, Td=0.07, 
Salida=0.0, Error=0.0, Error_ant=0.0, Vmax=0.0;


// Definir pines para el sensor de ultrasonido
const int trigPin = 9;
const int echoPin = 10;

// Variables para almacenar la distancia
float distance;
float total=27;
float altura=0;
//


NewPing sonar(trigPin, echoPin);  // Inicializar el sensor de ultrasonido

const int buffer_size = 4;  // Tamaño del búfer para enviar datos (4 bytes para float)
byte buf[buffer_size];
union BtoF {
  byte b[4];
  float fval; 
} u;



void setup() {
Serial.begin(115200);

}

void loop() {

Tinicio= millis();
Referencia= leerReferencia();
Salida= leerSensor();
Control= Controlador( Referencia, Salida);
 Control+= offset;

Tm= Tiempo_Muestreo(Tinicio);


}

float leerSensor() {
    distance = sonar.ping_cm();
  altura = total-distance;
  return altura;
}

float leerReferencia(){
    float Reference;
if(Serial.available()>0){
       Reference = readFromMatlab();
    }
    return Reference;
    
  }
  
  float readFromMatlab() {
  byte buf[4];
  Serial.readBytes(buf, 4);
  float output;
  memcpy(&output, buf, sizeof(output));
  return output;
}

float Controlador(float Referencia, float salida){
  
  float E_derivativo, E_integral, Control;
 E_integral= E_integral + ((Error*((Tm/1000.0))+ ((Tm/1000.0)*Error-Error_ant))/2.0);
  if(E_integral>100.0){E_integral=100.0;}
  if(E_integral<-100.0){E_integral=-100.0;}
E_derivativo= (Error - Error_ant)/(Tm/1000.0);
Control =Kp*(Error + Ti*E_integral + Td*E_derivativo);
Control = Control + offset;
if(Control>255){
  Control=255;
  }
  if(Control<0){
    Control=0;
    }
    //colocar aqui lo del actuador


 Error_ant= Error;
 Error= Referencia- Salida;
return Control;
  }

  unsigned long int Tiempo_Muestreo(unsigned long int Tinicio){
    unsigned long int T= millis()-Tinicio;
    return T;
    }
