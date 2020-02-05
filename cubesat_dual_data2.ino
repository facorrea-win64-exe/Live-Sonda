#include <SFE_BMP180.h>
#include <Wire.h>
SFE_BMP180 bmp180;

SFE_BMP180 pressure;
double baseline; // baseline pressure
double Po; //presion del punto inicial para h=0;
char status;
double T, P, A;

#include <TroykaDHT.h>  //Librería DHT por Troyka
DHT dht(4, DHT11);      //Dht pin, tipo de dht.

#include <SoftwareSerial.h>
SoftwareSerial HC12(2, 3); //tx,rx

#define buzzer 9
#define azul 10
#define rojo 7
#define verde 8

//Blink without delay multitarea
unsigned long intervalo_buzzer = 5000;
unsigned long intervalo_led = 300;
unsigned long intervalo_data = 1000;
unsigned long millis_previos_buzzer;
unsigned long millis_previos_led;
unsigned long millis_previos_data;

void setup() {
  bmp180.begin();
  dht.begin();            //Iniciar el DHT
  HC12.begin(9600);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  millis_previos_buzzer = millis();
  millis_previos_led = millis();
  millis_previos_data = millis();
  pressure.begin();
  HC12.print("configurando punto de referencia 0...");
  Po = P; //Asignamos el valor de presión actual como punto de referencia.
  HC12.println("Punto de referncia establecido: h=0");
  baseline = getPressure();
}

void loop() {
  unsigned long Millis_actuales = millis();

  // Gestionar tareas
  if ((unsigned long)(Millis_actuales - millis_previos_buzzer) >= intervalo_buzzer)
  {
    buzzerTask();
    millis_previos_buzzer = millis();
  }
  if ((unsigned long)(Millis_actuales - millis_previos_led) >= intervalo_led)
  {
    ledTask();
    millis_previos_led = millis();
  }

  if ((unsigned long)(Millis_actuales - millis_previos_data) >= intervalo_data)
  {
    dataTask();
    millis_previos_data = millis();
  }
}

void buzzerTask()
{
  int bu = 500;
  while ( bu <= 900) {
    bu++;
    tone(buzzer, bu * 1.5, 2500);
    delay(0.9);
  }
}

void ledTask()
{
  digitalWrite(rojo, HIGH);
  delay(150);
  digitalWrite(rojo, LOW);
  digitalWrite(azul, HIGH);
  delay(150);
  digitalWrite(azul, LOW);
  //delay(10);
}


void dataTask() {
  dht.read();
  delay(100);
  HC12.print("T= ");
  HC12.print(dht.getTemperatureC());
  HC12.print(" H= ");
  HC12.print(dht.getTemperatureC());

  double a,P;
  P = getPressure();
  a = pressure.altitude(P,baseline);


  HC12.print(" RA:");
//  if (a >= 0.0) HC12.print(" "); // add a space for positive numbers
  HC12.print(a, 1);
  HC12.print(" m ");
  HC12.print("P= ");
  HC12.print(P);
  HC12.println(" mb");
}

double getPressure()
{
  char status;
  double T, P, p0, a;

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
        else HC12.println("error retrieving pressure measurement\n");
      }
      else HC12.println("error starting pressure measurement\n");
    }
    else HC12.println("error retrieving temperature measurement\n");
  }
  else HC12.println("error starting temperature measurement\n");
}
