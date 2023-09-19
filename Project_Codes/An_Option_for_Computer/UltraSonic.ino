const int sensorPin_1 = A4;
float sensorValue_1;
float voltageOut_1;
float temperatureK_1;
float temperatureC_1;

const int sensorPin_2 = A5;
float sensorValue_2;
float voltageOut_2;
float temperatureK_2;
float temperatureC_2;

char message;
int threshold = 60;
int relay_pin_1 = 10; 
int relay_pin_2 = 11;
int relay_pin_3 = 12; 

void setup(){
  pinMode(sensorPin_1, INPUT);
  pinMode(sensorPin_2, INPUT);
  pinMode(relay_pin_1, OUTPUT);
  pinMode(relay_pin_2, OUTPUT);
  pinMode(relay_pin_3, OUTPUT);
  Serial.begin(9600);
}

void temperature(){
  while(1){
      sensorValue_1 = analogRead(sensorPin_1);
      voltageOut_1 = (sensorValue_1 * 5000) / 1023;
      temperatureK_1 = voltageOut_1 / 10;
      temperatureC_1 = temperatureK_1 - 273;
      //Serial.print("temperatureC_1 =");
      Serial.print(temperatureC_1);
      //Serial.print("      ");
      if(temperatureC_1 >= threshold){
        digitalWrite(relay_pin_1, HIGH);
      }
      if(temperatureC_1 <= (threshold-5)){
        digitalWrite(relay_pin_1, LOW);
      }
      Serial.print(",") ;
      sensorValue_2 = analogRead(sensorPin_2);
      voltageOut_2 = (sensorValue_2 * 5000) / 1023;
      temperatureK_2 = voltageOut_2 / 10;
      temperatureC_2 = temperatureK_2 - 273;
      //Serial.print("temperatureC_2 =");
      Serial.print(temperatureC_2);
      Serial.print("\n");
      if(temperatureC_2 >= threshold){
        digitalWrite(relay_pin_2, HIGH);
      }
      if(temperatureC_2 <= (threshold-5)){
        digitalWrite(relay_pin_2, LOW);
      }
      delay(500);
      
      if(Serial.available() > 0){
        message = Serial.read();
        if (message == 's')
        {
          digitalWrite(relay_pin_3, HIGH);
          break;
        }
      }
      
    }
}

void loop(){
  digitalWrite(relay_pin_1, HIGH);
  digitalWrite(relay_pin_2, HIGH);
  digitalWrite(relay_pin_3, HIGH);
  
  if(Serial.available() > 0){
    message = Serial.read();
    if (message == 'o'){
      digitalWrite(relay_pin_1, LOW);
      digitalWrite(relay_pin_2, LOW);
      digitalWrite(relay_pin_3, LOW);
      temperature();}
    if (message == '1'){
      threshold = 50;
    }
    if (message == '2'){
      threshold = 60;
    }
    if (message == '3'){
      threshold = 70;
    }
    if (message == '4'){
      threshold = 80;
    }
    if (message == '5'){
      threshold = 90;
    }
    if (message == '6'){
      threshold = 100;
    }
  }  
} 
