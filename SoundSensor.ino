int soundSensor= A0;

int red1=5;
int blue1=6;
int green1=7;

int red2=8;
int blue2=9;
int green2=10;

int red3=11;
int blue3=12;
int green3=13;

int waterpump3=4;
int waterpump2=3;
int waterpump1=2;

int flag=1;

int doubleFlag=1;

int sensorStatus1;

void setup() {
  Serial.begin(9600);
  pinMode(soundSensor,INPUT);
  pinMode(red1,OUTPUT);
  pinMode(red2,OUTPUT);
  pinMode(red3,OUTPUT);

  pinMode(blue1,OUTPUT);
  pinMode(blue2,OUTPUT);
  pinMode(blue3,OUTPUT);
  
  pinMode(green1,OUTPUT);
  pinMode(green2,OUTPUT);
  pinMode(green3,OUTPUT);

  sensorStatus1=analogRead(soundSensor);
 }

void loop() {
  int sensorStatus2=analogRead(soundSensor);
  int diff= abs(sensorStatus1-sensorStatus2);

  Serial.println(diff);
 // Serial.println(millis());

 if(sensorStatus1<=50 || sensorStatus2<=50){
     analogWrite(red1,0);
     analogWrite(blue1,0);
     analogWrite(green1,0);
     analogWrite(red2,0);
     analogWrite(blue2,0);
     analogWrite(green2,0);
     analogWrite(red3,0);
     analogWrite(blue3,0);
     analogWrite(green3,0);
     digitalWrite(waterpump3,LOW);
     digitalWrite(waterpump2,LOW);
     digitalWrite(waterpump1,LOW);
     
 }
//if(sensorStatus>30 && sensorStatus<=800){
 else if(diff>=0 && diff<10){   
   
    if (flag==1){
     analogWrite(red1,255);
     analogWrite(blue1,0);
     analogWrite(green1,0);
     analogWrite(red2,0);
     analogWrite(blue2,0);
     analogWrite(green2,0);
     analogWrite(red3,0);
     analogWrite(blue3,0);
     analogWrite(green3,0);
     analogWrite(waterpump1,191);
     analogWrite(waterpump2,0);
     analogWrite(waterpump3,0);
     delay(100);
     flag=2;
    // delay(75);   
      }
   else if(flag==2){
    analogWrite(red1,0);
     analogWrite(blue1,0);
     analogWrite(green1,0);
     analogWrite(red2,0);
     analogWrite(blue2,255);
     analogWrite(green2,0);
     analogWrite(red3,0);
     analogWrite(blue3,0);
     analogWrite(green3,0);
     analogWrite(waterpump1,0);
     analogWrite(waterpump2,191);
     analogWrite(waterpump3,0);
     delay(100);
     flag=3;
   //  delay(75); 
   }
   else if(flag==3){
     analogWrite(red3,50);
     analogWrite(blue3,50);
     analogWrite(green3,50);
     analogWrite(red1,0);
     analogWrite(blue1,0);
     analogWrite(green1,0);
     analogWrite(red2,0);
     analogWrite(blue2,0);
     analogWrite(green2,0);
     analogWrite(waterpump1,0);
     analogWrite(waterpump2,0);
     analogWrite(waterpump3,191);
     delay(100);
     flag=1;
         
   }
   }
   //else if (sensorStatus>801 && sensorStatus<890){
    else if(diff>=10 && diff<=967){
    if(doubleFlag==1){
     analogWrite(red1,255);
     analogWrite(blue1,255);
     analogWrite(green1,255);
     analogWrite(red2,255);
     analogWrite(blue2,255);
     analogWrite(green2,255);
     analogWrite(red3,0);
     analogWrite(blue3,0);
     analogWrite(green3,0);
     analogWrite(waterpump1,191);
     analogWrite(waterpump2,191);
     analogWrite(waterpump3,0);
     delay(100);
     doubleFlag=2;
    
    }
    else if(doubleFlag==2){
     analogWrite(red3,255);
     analogWrite(blue3,255);
     analogWrite(green3,255);
     analogWrite(red2,255);
     analogWrite(blue2,255);
     analogWrite(green2,255);
     analogWrite(red1,0);
     analogWrite(blue1,0);
     analogWrite(green1,0);
     analogWrite(waterpump1,0);
     analogWrite(waterpump2,191);
     analogWrite(waterpump3,191);
     delay(100);
     doubleFlag=3;
    
   }
   else if(doubleFlag==3){
     analogWrite(red1,255);
     analogWrite(blue1,255);
     analogWrite(green1,255);
     analogWrite(red3,255);
     analogWrite(blue3,255);
     analogWrite(green3,255);
     analogWrite(red2,0);
     analogWrite(blue2,0);
     analogWrite(green2,0);
     analogWrite(waterpump1,191);
     analogWrite(waterpump2,0);
     analogWrite(waterpump3,191);
     delay(100);
     doubleFlag=1;
  
   }
        
   }
   //else if(sensorStatus>890){
   else if(diff>967){
   
     analogWrite(red1,50);
     analogWrite(blue1,50);
     analogWrite(green1,50);
     analogWrite(red3,50);
     analogWrite(blue3,50);
     analogWrite(green3,50);
     analogWrite(red2,50);
     analogWrite(blue2,50);
     analogWrite(green2,50);
     analogWrite(waterpump1,191);
     analogWrite(waterpump2,191);
     analogWrite(waterpump3,191);
     delay(100);
     
   }
   sensorStatus1=sensorStatus2;
   delay(1);
}


