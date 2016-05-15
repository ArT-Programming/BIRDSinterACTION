#include <MIDI.h> 

const int amount = 6;
const int mean = 5;
const int knockSensor[amount] = {
  A0, A1, A2, A3, A4, A5}; // the piezo is connected to analog pin 0
const int threshold = 700;  // threshold value to decide when the detected sound is a knock or not

// these variables will change:
int sensorReading[amount];      // variable to store the value read from the sensor pin
int bufMean[amount][mean];      // variable to store the value read from the sensor pin
int meanFilt;
int count;
int diff;
unsigned long timer[amount];
int cooldown; 
boolean state[amount];
const int meanTAmount = 25;
int meanTBuf[amount][meanTAmount];
unsigned long meanThreshold;
int meanCount; 
void setup() {
  for(int i = 0; i < amount; i++){
    sensorReading[i] = analogRead(knockSensor[i]);  
    state[i] = false;
    timer[i] = 0;
    for(int j = 0; j < mean; j++){
      bufMean[i][j] = sensorReading[i];
    }
    for(int g = 0; g < meanTAmount; g++){
    meanTBuf[i][g] = 0;
    }
  }
  
  //bufMean = 0;      // variable to store the value read from the sensor pin
  meanThreshold = 0;
  meanFilt = 0;
  cooldown = 100; 
  count = 0; 
  meanCount = 0; 
  Serial.begin(9600);
  //MIDI.begin();
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  for(int i = 0; i < amount; i++){
    sensorReading[i] = analogRead(knockSensor[i]);
    // Mean Filter
    
    bufMean[i][count]  = sensorReading[i];
    
    for(int j = 0; j < mean; j++){
      meanFilt += bufMean[i][j];
    }
    meanFilt /= mean; 
    
    meanTBuf[i][meanCount] = meanFilt; 
    for(int j = 0; j < meanTAmount; j++){
      meanThreshold += meanTBuf[i][j];
    }
    meanThreshold /= meanTAmount; 
    
   /* if(sensorReading[i] >= meanFilt) diff = sensorReading[i] - meanFilt;
    else if(meanFilt > sensorReading[i]) diff = meanFilt - sensorReading[i];*/ 
    if(meanThreshold < 200){
      meanThreshold = 200;
    }
    // if the sensor reading is greater than the threshold:    
    if(sensorReading[i] > meanThreshold + 50 && millis() > timer[i] + cooldown) {
      //MIDI.sendNoteOn(i, 127, 1); 
      timer[i] = millis();
      
       Serial.print("What sensor: "); 
       Serial.print(i);
       Serial.print(" Sensor reading: "); 
       Serial.print(sensorReading[i]);      
       Serial.print(" threshold: ");
       Serial.print(meanThreshold); 
       Serial.print(" difference: ");
       Serial.println(diff); 
      
    } //else state[i] = false;   
    
    if(i == amount - 1){
      count++; 
      meanCount++; 
    }
    if(count > mean) count = 0; 
    if(meanCount > meanTAmount) meanCount = 0; 
    
    meanFilt = 0; 
    meanThreshold = 0; 
    
  }
}






