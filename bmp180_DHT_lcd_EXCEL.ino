
/*
***********************************************************************
***********************************************************************
 *           
 *           **********      **********     **********
 *           **********      **********     **********
 *           **                  **         **
 *           **                  **         **
 *           **   *****          **         **********
 *           **   *****          **         **********
 *           **      **          **         ** 
 *           **      **          **         **
 *           **********  **  ********** **  **********
 *           **********  **  ********** **  **********
 *               GRUPO DE INNOVACIÓN EDUCATIVA
************************************************************************ 
************************************************************************
                    ESTACIÓN METEOROLÓGICA
     ESTE PROYECTO HA SIDO REALIZADO PARA EL PROYECTO GLOBE - CLIMA
                FEBRERO - MARZO y ABRIL DE 2016
*/


#include <SFE_BMP180.h> // Incluímos las librerías del sensor de presión / altura
#include <Wire.h> // Librería que permite el uso de la tecnología I2C
#include <LiquidCrystal_I2C.h> // Con esta librería usamos los comandos adecuados 
// para usar la pantalla LCD   
//#include <DHT11.h> // Incorporamos la librería DHT11 encargada de leer humedad y 
// temperatura
//#include <DHT22.h>//Incorporamos la librería DHT22 encargada de leer humedad y 
// temperatura
#include <DHT.h>


LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);//Direccion de LCD Dependiendo de 
//la pantalla que se use, esta dirección puede cambiar. Otro valor común es el 0x27
#define DHTPIN 2
#define DHTTYPE DHT22
//#define DHTTYPE DHT11   // Si el sensor que estamos usando es el DHT 11
DHT dht(DHTPIN, DHTTYPE);

SFE_BMP180 pressure;
//Se declaran las variables. Es necesario tomar en cuenta una presión inicial
//esta será la presión que se tome en cuenta en el cálculo de la diferencia de altura
double PresionBase;
//Leeremos presión y temperatura. Calcularemos la diferencia de altura
double Presion = 0;
double Altura = 0;
double Temperatura = 0;
char status;
void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16,2);// Indicamos medidas de LCD   
//Se inicia el sensor y se hace una lectura inicial
 Serial.println("LABEL, Tiempo,Temperatura (C), Humedad (%), Altura rel (m), Presion (mb)");
 //Se imprime la tabla de valores en excel
SensorStart();
}
void loop() {
int err;
       float temp=dht.readTemperature();
       float hum=dht.readHumidity();
       
       if (isnan(hum)||isnan(temp)){
         Serial.print("error de lectura");
         return; }
          lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("Temp.: ");
             lcd.print(temp);
             lcd.print(" C");
             lcd.setCursor(0,1);
             lcd.print("Hdad: ");
             lcd.print(hum);
             lcd.print(" %");
             Serial.print("DATA,TIME,"); 
             Serial.print(temp);
             Serial.print(",");
             Serial.print(hum);
             Serial.print(",");
             Serial.print(Altura);
             Serial.print(",");
             Serial.println(Presion);

       delay(3000);            //Recordad que solo lee una vez por segundo
ReadSensor();
lcd.clear();//Elimina todos los simbolos del LCD
lcd.setCursor(0,0);//Posiciona la primera letra en fila 0 columna 0     
//Se imprimen las variables
lcd.print("Hrel.: ");
lcd.print(Altura);
lcd.print(" m");
lcd.setCursor(0,1);
lcd.print("P: ");
lcd.print(Presion);
lcd.print(" mbar");
 
delay(3000);
//Cada 3 segundos hará una nueva lectura
}
void SensorStart() {
//Secuencia de inicio del sensor
if (pressure.begin())
Serial.println("BMP180 iniciado adecuadamente");
else
{
Serial.println("La inicialización del sensor BMP180 falló");
while (1);
}
//Se inicia la lectura de temperatura
status = pressure.startTemperature();
if (status != 0)  {
delay(status);
//Se lee una temperatura inicial
status = pressure.getTemperature(Temperatura);
if (status != 0)    {
//Se inicia la lectura de presiones
status = pressure.startPressure(3);
if (status != 0)     
{
delay(status);
//Se lee la presión inicial incidente sobre el sensor en la primera ejecución
status = pressure.getPressure(PresionBase, Temperatura);
}
}
}
}
void ReadSensor() {
//En este método se hacen las lecturas de presión y temperatura y se calcula la altura
//Se inicia la lectura de temperatura
status = pressure.startTemperature();
if (status != 0)
{
delay(status);
//Se realiza la lectura de temperatura
status = pressure.getTemperature(Temperatura);
if (status != 0)
{
//Se inicia la lectura de presión
status = pressure.startPressure(3);
if (status != 0)
{
delay(status);
//Se lleva a cabo la lectura de presión,</span>
//considerando la temperatura que afecta el desempeño del sensor</span>
status = pressure.getPressure(Presion, Temperatura);
if (status != 0)
{
//Cálculo de la altura en base a la presión leída en el Setup
Altura = pressure.altitude(Presion, PresionBase);
}
else Serial.println("Error en la lectura de presion\n");
}
else Serial.println("Error iniciando la lectura de presion\n");
}
else Serial.println("Error en la lectura de temperatura\n");
}
else Serial.println("Error iniciando la lectura de temperatura\n");
}
