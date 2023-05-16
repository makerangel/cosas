#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <DHT.h>
#include <SD.h>

#define DHTPIN 2          // Pin para el sensor DHT22
#define DHTTYPE DHT22     // Tipo de sensor DHT22
DHT dht(DHTPIN, DHTTYPE); // Crear objeto del sensor DHT22

#define MQ7_PIN A0        // Pin para el sensor MQ7
float MQ7_Ro = 9.83;     // Resistencia en aire limpio
float MQ7_A = 1.67;      // Pendiente de la curva
float MQ7_B = -2.23;     // Intercepción de la curva

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345); // Crear objeto del acelerómetro ADXL345

File dataFile; // Declarar objeto de archivo para el registro de datos
const int chipSelect = 10; // Pin para la selección de la tarjeta microSD

void setup() {
  Serial.begin(9600);
  dht.begin();
  accel.begin();
  accel.setRange(ADXL345_RANGE_2_G); // Establecer el rango del acelerómetro en 2G

  // Inicializar la tarjeta microSD
  if (!SD.begin(chipSelect)) {
    Serial.println("Error al inicializar la tarjeta microSD");
    while (1);
  }

  // Abrir el archivo de registro de datos para escritura
  dataFile = SD.open("datalog.txt", FILE_WRITE);

  // Escribir una línea de encabezado en el archivo de registro de datos
  dataFile.println("Timestamp, Temperatura (C), Humedad (%), MQ7 PPM, Aceleración (X, Y, Z)");
}

void loop() {
  float temperature = dht.readTemperature(); // Leer temperatura del sensor DHT22
  float humidity = dht.readHumidity();       // Leer humedad del sensor DHT22
  float mq7_voltage = analogRead(MQ7_PIN) * 5.0 / 1023.0; // Leer voltaje del sensor MQ7
  float mq7_rs = (5.0 - mq7_voltage) / mq7_voltage * 10.0; // Calcular la resistencia del sensor
  float mq7_ppm = MQ7_A * pow((mq7_rs / MQ7_Ro), MQ7_B);   // Calcular la concentración de gas en ppm
  
  sensors_event_t event;
  accel.getEvent(&event);    // Leer datos del acelerómetro
  float accel_x = event.acceleration.x;
  float accel_y = event.acceleration.y;
  float accel_z = event.acceleration.z;

  // Imprimir las lecturas de los sensores en el monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print("C, Humedad: ");
  Serial.print(humidity);
  Serial.print("%, MQ7 PPM: ");
  Serial.print(mq7_ppm);
  Serial.print(", Aceleración: (");
  Serial.print(accel_x);
  Serial.print(", ");
  Serial.print(accel_y);
  Serial.print(", ");
  Serial.print(accel_z);
  Serial.println(")");

  // Escribir las lecturas de


 delay(2000); // Esperamos 2 segundos para volver a realziar las lecturas 
}

 
