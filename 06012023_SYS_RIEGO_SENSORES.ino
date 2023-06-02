#include <LiquidCrystal.h>
#include <DFRobot_DHT11.h>

#define DHT11_PIN 9
DFRobot_DHT11 _dht11;


  // Configuración de pines de LCD con pines de Arduino UNO
  const int RS = 12;
  const int E = 11;
  const int D4 = 5;
  const int D5 = 4;
  const int D6 = 3;
  const int D7 = 2;

  int serial_baudios = 9600;

  int valor_h_sensor = 0;             // Humedad
  int valor_t_sensor = 0;             // Temperatura
  
  String msg = "";

  const int PIN9_DIGITAL = 9;

// Constructor de la clase para controlar pantalla LCD
LiquidCrystal lcd(RS,E,D4,D5,D6,D7);

void setup() {

  //Configuración de filas y columnas del LCD
  lcd.begin(16,2);
  lcd.clear();
  lcd.noBlink();
  
  //Velocidad de transferencia de datos del puerto serial de 9600b baudios
  Serial.begin(serial_baudios);
  msg = "[SERIAL] VELOCIDAD DE TRANSFERENCIA DEL PUERTO SERIAL EN BAUDIOS: " + serial_baudios;
  Serial.println(msg);
  
  //Setear pin 8 de energia para el motor
  pinMode(8, OUTPUT);
  pinMode(PIN9_DIGITAL,INPUT);
}

void regado(){
  
  Serial.println("[INFO] INICIANDO PROCESO DE REGADO.");
  
  bool motor_estado = true;
  digitalWrite(8,HIGH);
  
  do{
    _dht11.read(DHT11_PIN);
    valor_h_sensor = _dht11.humidity;
    
    msg = "[DATA:SENSOR H] ";
    msg += String(valor_h_sensor);
    Serial.println(msg);
    
    if(valor_h_sensor > 70){
      digitalWrite(8,LOW);
      motor_estado = false;
      msg = "[DATA:MOTOR] LOW ";
      Serial.println(msg);
    }

    delay(2000);
    
  }while(motor_estado != false);
  
  Serial.println("[INFO] PROCESO DE REGADO FINALIZADO");
}

void loop(){

  _dht11.read(DHT11_PIN);                                       //Lectura digital de sensor de humedad y temperatura
  
  valor_h_sensor = _dht11.humidity;
  valor_t_sensor = _dht11.temperature;
  
  //Impreción serial de datos de humedad y temperatura
  Serial.print("[DATA:SENSOR] TEMP: ");
  Serial.println(valor_t_sensor);
  Serial.print("[DATA:SENSOR] HUMI: ");
  Serial.println(valor_h_sensor);
  delay(1000);
  
  if(valor_h_sensor >= 70){
    
    //Impresión de estado de tierra y valor de sensor de humedad
    lcd.setCursor(0,1);
    lcd.print("TIERRA HUMEDA");
    lcd.setCursor(0,0);
    msg = "H: ";
    msg += String(valor_h_sensor);
    lcd.print(msg);
    delay(5000);
    
    //Impresión de estado de motor
    lcd.setCursor(0,1);
    lcd.print("MOTOR OFF    ");
    delay(5000);
    
  }else if(valor_h_sensor < 600){
    
    //Impresión de estado de tierra y valor de sensor de humadad
    lcd.setCursor(0,1);
    lcd.print("TIERRA SECA  ");
    lcd.setCursor(0,0);
    msg = "H: ";
    msg += String(valor_h_sensor);
    lcd.print(msg);
    delay(5000);
    
    //Impresión de estado de motor
    lcd.setCursor(0,1);
    lcd.print("MOTOR ON     ");
    regado();
    lcd.setCursor(0,1);
    lcd.print("MOTOR OFF    ");
    delay(2000);
  }
  lcd.clear();
}