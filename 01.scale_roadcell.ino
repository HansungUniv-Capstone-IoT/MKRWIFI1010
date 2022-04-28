/*
* 엘레파츠 전자저울 예제 프로그램 - loadcell + HX711
* 사용 라이브러리 : https://github.com/bogde/HX711
* 
* 아두이노 GPIO 연결
* loadcell / HX711 board
* Dout PIN : D2
* SCK PIN  : D3
*/

#include "HX711.h"
#include <Servo.h>
int motor1 = 4;
int motor2 = 5;
//int pos = 0;

int distance1 = 0; // 거리값을 저장할 변수
int distance2 = 0; // 거리값을 저장할 변수
int distance3 = 0; // 거리값을 저장할 변수

Servo servo1, servo2;

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN         2
#define LOADCELL_SCK_PIN          3




// scale - 10Kg loadcell : 226 / 5kg loadcell : 372
// ADC 모듈에서 측정된 결과값을 (loadcellValue)값 당 1g으로 변환해 줌
float loadcellValue = 226.0;

HX711 scale;

void setup() {
	// put your setup code here, to run once:

 servo1.attach(motor1);
  servo2.attach(motor2);

  

  
	Serial.begin(115200);

	// 로드셀 HX711 보드 pin 설정
	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

	// 부팅 후 잠시 대기 (2초)
	delay(2000);

	// 측정값 1회 읽어오기
	Serial.print("read: \t\t\t");
	Serial.println(scale.read());

	delay(1000);

	// 스케일 설정
	scale.set_scale(loadcellValue);
	
	// 오프셋 설정(10회 측정 후 평균값 적용) - 저울 위에 아무것도 없는 상태를 0g으로 정하는 기준점 설정(저울 위에 아무것도 올려두지 않은 상태여야 합니다.)   
	scale.tare(10);    

	// 설정된 오프셋 및 스케일 값 확인
	Serial.print("Offset value :\t\t");
	Serial.println(scale.get_offset());
	Serial.print("Scale value :\t\t");
	Serial.println(scale.get_scale());

	// (read - offset) 값 확인 (scale 미적용)
	Serial.print("(read - offset) value: \t");  
	Serial.println(scale.get_value());
	delay(2000);

  
}



/*
long microsecondsToInches(long microseconds)
{
// According to Parallax's datasheet for the PING))), there are
// 73.746 microseconds per inch 
return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}
*/


void loop() {
	// put your main code here, to run repeatedly:

	// 오프셋 및 스케일이 적용된 측정값 출력 (5회 측정 평균값) 
	Serial.print("value :\t");
  float scaleResult = scale.get_units(5);
	Serial.print(scaleResult, 2);    // 5회 측정 평균값, 소수점 아래 2자리 출력
	Serial.println(" g");
	delay(1000);  // 1초 대기

  int volt1 = map(analogRead(A0), 0, 1023, 0, 5000); // 0~1023 사이 값을 갖는 아날로그 신호값을  //  0~5000 (5V) 사이 값으로 변환
  int volt2 = map(analogRead(A1), 0, 1023, 0, 5000);
  int volt3 = map(analogRead(A2), 0, 1023, 0, 5000);
  
  distance1 = (27.61 / (volt1 - 0.1696)) * 1000; // 읽어들인 Voltage 값을 거리값(단위: cm)로 변환하는 공
  distance2 = (27.61 / (volt2 - 0.1696)) * 1000;
  distance3 = (27.61 / (volt3 - 0.1696)) * 1000;
  
  Serial.print(distance1);  //거리값을 시리얼모니터로 출력해줍니다.
  Serial.print(" cm \n");
  Serial.print(distance2);  //거리값을 시리얼모니터로 출력해줍니다.
  Serial.print(" cm \n");
  Serial.print(distance3);  //거리값을 시리얼모니터로 출력해줍니다.
  Serial.print(" cm \n \n");
  delay(1000);

  
    if(scaleResult < 40 && scaleResult > 20)
    {
      servo1.write(90);
      delay(1500);
      servo1.write(0);
      Serial.println("can");
      delay(100);
    }
    
    
    else if( scaleResult > 40){
      servo2.write(90);
      delay(1500);
      servo2.write(0);
      Serial.println("plastic");
      delay(100);
    }

    else if( scaleResult < 20){
      servo1.write(0);
      servo2.write(0);
      delay(100);
    }

   	
}
