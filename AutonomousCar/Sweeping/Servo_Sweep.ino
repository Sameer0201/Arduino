#include <NewPing.h>

#include <Servo.h>

Servo myservo1; // create servo object to control a servo
Servo myservo2;
// twelve servo objects can be created on most boards

double pos = 0;    // variable to store the servo position
int counter =0;
void setup() {
  Serial.begin(9600);
  myservo1.attach(2);
  myservo2.attach(4);// attaches the servo on pin 9 to the servo object
  delay(1000);


  
    for(int i=0; i< 1; i++){
    myservo1.write(26);  //left side stop (down)
    myservo2.write(175);  // right side stop
    delay(1000);
    Serial.println(i);
  }
   myservo1.write(96);  //left side stop (down)
    myservo2.write(95);  // right side stop
delay(10000);
    for(int i=0; i< 2; i++){
    myservo1.write(107);  //left side stop (down)
    myservo2.write(84);  // right side stop
    delay(1000);
    Serial.println(i);
  }
    myservo1.write(96);  //left side stop (down)
    myservo2.write(95);  // right side stop
}

void loop(){
//  pos = 180;
//  int pos2 = 0;
//  while(pos > 90 && pos2 < 90){
//  myservo1.write(pos2++);
//  myservo2.write(pos--);
//  }
//
//  delay(500000);
//  
//delay(1000);

//  if(counter < 5){
//  myservo1.write(107);  //left side stop (down)
//  myservo2.write(84);  // right side stop
//  delay(1000);
//  Serial.println("bob");
//  counter++;
//}else{
//  
//}

//myservo1.write(26);  //left side stop (up)
//myservo2.write(175);  // right side stop
//
//Serial.println("hi");
//delay(1000);
//for (pos = pos; pos < 5; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    Serial.println("here i am");
//    if(pos<2){
//      myservo1.write(107);  //left side
//      myservo2.write(84);
//      Serial.println("one");
//      delay(1000);
//    }else {
//      myservo1.write(26);  //left side
//      myservo2.write(175);
//      Serial.println("two");
//      delay(1000);
//    }
//                          // tell servo to go to position in variable 'pos'
//   //delay(1000);                       // waits 15ms for the servo to reach the position
//  Serial.println(pos);
//  }
  //myservo1.write(96);  //left side stop
  //myservo2.write(95);  // right side stop
  //delay(1000);
  /*for (pos = 2; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos);// tell servo to go to position in variabl
    myservo2.write(2-pos);
    delay(10000);                       // waits 15ms for the servo to reach the position
  }*/

}

