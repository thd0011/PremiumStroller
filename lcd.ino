#include <SoftwareSerial.h>
#include <Servo.h>
#include<Wire.h> //가속도 센서를 제어하기 위한 라이브러리

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; //가속도 센서 변수 지정
const int MPU=0x68;  //MPU 6050 의 I2C 기본 주소

int flag1 = 0;
int flag2 = 0;
SoftwareSerial BTSerial(2,3);

Servo servo1;

int pos = 0;    // variable to store the servo position
const int servoPin1=11;
void setup() {

  Wire.begin();      //Wire 라이브러리 초기화
  Wire.beginTransmission(MPU); //MPU로 데이터 전송 시작
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //MPU-6050 시작 모드로
  Wire.endTransmission(true); 
  
  BTSerial.begin(9600);
  servo1.attach(servoPin1); 
  Serial.begin(9600);
  servo1.write(80);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {

  int rawvoltage= analogRead(A1);
  float volts= rawvoltage/205.0;
  float celsiustemp= 100.0 * volts - 50;

  int brightness = analogRead(A2);

  Serial.print(celsiustemp);
  Serial.println(" Celsius");
  Serial.print(brightness);
  Serial.println("brightness");

  Wire.beginTransmission(MPU);    //데이터 전송시작
  Wire.write(0x3B);               // register 0x3B (ACCEL_XOUT_H), 큐에 데이터 기록
  Wire.endTransmission(false);    //연결유지
  Wire.requestFrom(MPU,14,true);  //MPU에 데이터 요청
  //데이터 한 바이트 씩 읽어서 반환
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  // put your main code here, to run repeatedly:
  Serial.print("AcY = "); 
  Serial.println(AcY); //y축 가속도 출력

  if(celsiustemp>25 && flag1==0){
    BTSerial.write("5");
  }
  if (brightness>700 && flag2==0){
    BTSerial.write("6");
  }
  if(AcY<14000){
    BTSerial.write("7");
  }
  delay(1500);
  
  if (BTSerial.available()){
    int command =  BTSerial.read();
    if(command == 0){ //현재 상태값 불러오기
      /*if 선풍기가 작동되있다
           BTSerial.write("1");
          else 작동안된다
            BTSerial.write("2");
          if 캐노피 온이다
          BTSerial.write("3");
          else 오프다
          BTSerial.write("4");
          */
    }
    if(command == 49){
      //여기에 선풍기작동
      //  digitalWrite(7, HIGH);
      //  digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(4, HIGH);
        flag1=1;
        delay(2000);
        
    }
    else if(command == 50){
      //여기에 선풍기끔
      //  digitalWrite(7, HIGH);
       // digitalWrite(6, LOW);
         digitalWrite(7, LOW);
        digitalWrite(6, LOW);
        digitalWrite(4, LOW);
        flag1=0;
        delay(2000);
        
    }
    else if(command == 51){
      //캐노피 on
      servo1.write(0);
      flag2=1;
      delay(2000);
    }
    else if(command == 52){
      //캐노피 off
      servo1.write(80);
      flag2=0;
      delay(2000);
    }
  }
}
