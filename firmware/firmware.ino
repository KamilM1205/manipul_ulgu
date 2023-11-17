#include <Servo.h>
 
Servo myservoA;
Servo myservoB;
Servo myservoC;
Servo myservoD;
Servo myservoE;
Servo myservoF;
int i, pos, myspeed;
int sea, seb, sec, sed, see, sef;
static int v = 0;
int V_neg = 1;
char x[100] = { 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24 };
 
String mycommand = "";  //Захват команд порта  #auto：Автоматический режим   #com：Управление последовательным портом     #stop：Останов
static int mycomflag = 2; // #auto：2 запускать автоматически , #com： 1  управление с компьютера   #stop：0 останов   
 
void myprint()
{
  sea = myservoA.read();
  seb = myservoB.read();
  sec = myservoC.read();
  sed = myservoD.read();
  see = myservoE.read();
  sef = myservoF.read();
 
  Serial.print("A=");
  Serial.print(sea);
  Serial.print(" B=");
  Serial.print(seb);
  Serial.print(" C=");
  Serial.print(sec);
  Serial.print(" D=");
  Serial.print(sed);
  Serial.print(" E=");
  Serial.print(see);
  Serial.print(" F=");
  Serial.println(sef);
}
 
void myservosetup() //перевести сервоприводы в режим ожидания
{
  sea = myservoA.read();
  seb = myservoB.read();
  sec = myservoC.read();
  sed = myservoD.read();
  see = myservoE.read();
  sef = myservoF.read();
 
  myspeed = 500;
  for (pos = 0; pos <= myspeed; pos += 1)
  {
    myservoA.write(int(map(pos, 1, myspeed, sea, 66)));
    myservoB.write(int(map(pos, 1, myspeed, seb, 90)));
    myservoC.write(int(map(pos, 1, myspeed, sec, 50)));
    myservoD.write(int(map(pos, 1, myspeed, sed, 90)));
    myservoE.write(int(map(pos, 1, myspeed, see, 120)));
    myservoF.write(int(map(pos, 1, myspeed, sef, 90)));
    delay(1);
  }
}
 
void setup()
{
  Serial.begin(9600, SERIAL_8N1);
 
  mycomflag = 2;  // По умолчанию режим работы манипулятора 2: автоматический режим
  myservoA.attach(3); // Контроль поворота（A）Порт 3
  myservoB.attach(5); // Правый поперечный сервопривод（B）Порт 5
  myservoC.attach(6); // Левый поперечный сервопривод（C）Порт 6
  myservoD.attach(9); // Продольный сервопривод（D）Порт 9
  myservoE.attach(10);  // Сервопривод наклона лапки（E）Порт 10
  myservoF.attach(11);  // Сервопривод поворота лапки（F）Порт 11
 
  //myservoA.write(66);
  // myservoB.write(90);
  // myservoC.write(55);
  // myservoD.write(90);
  //  myservoE.write(120);
  //  myservoF.write(90);
 
  myservoA.write(60);
  myservoB.write(30);
  myservoC.write(90);
  myservoD.write(90);
  myservoE.write(170);
  myservoF.write(90);
 
}
 
void write_servo(Servo servo, int _min, int _max, int &val)
{
  char buff[50];
  if (val < _min){
    val = _min;
    sprintf(buff, "%s %d", "ERROR: servo position less then ", _min);
    Serial.println(buff);
  }
 
  if (val > _max){
    val = _max;
    sprintf(buff, "%s %d", "ERROR: servo position greater then ", _max);
    Serial.println(buff);
  }
 
  servo.write(val);
  myprint();
 
  val = 0;
}
 
void loop()
{
  char data = 2;
  while (Serial.available() > 0)
  {
    // int amout=Serial.readBytesUntil(';',data,30);
    // data[amout]
    data = char(Serial.read());
 
    if (data != '\r' && data != '\n')
    {
      mycommand += data;
      delay(2);
    }
  }
 
  if (mycommand.length() > 0)
  {
    Serial.println(mycommand);
    if (mycommand == "#auto")
    {
      mycomflag = 2;
      Serial.println("auto station");
      mycommand = "";
    }
 
    if (mycommand == "#com")
    {
      mycomflag = 1;
      Serial.println("computer control station");
      mycommand = "";
      myservosetup();
    }
 
    if (mycommand == "#stop")
    {
      mycomflag = 0;
      Serial.println("stop station");
      mycommand = "";
    }
 
    if (mycommand == "#dec")
    {
      mycomflag = 3;
      Serial.println("start dec");
      mycommand = "";
    }
  }
 
  if (mycomflag == 1) //Если включено управление с компьютера[1]
  {
      if (mycommand.length() == 0) {
        return;
      }
      
      char cmd;
      char buff[50];

      for (int i = 0; i < mycommand.length(); i++) {
        if (i == 0) {
          if (mycommand[0] >= 'a' && mycommand[0] <= 'z') {
            cmd = mycommand[0];
          }
        } else {
          v = v*10 + (mycommand[i] - '0');
        }
      }
        
      switch (cmd)
      {
        case 'a': //Если после идет "а", это указывает на 1ый сервопривод
          write_servo(myservoA, 0, 120, v);
          break;
 
        case 'b': //Если после идет "b", это указывает на 2ый сервопривод
          write_servo(myservoB, 30, 125, v);
          break;
 
        case 'c': //Если после идет "c", это указывает на 3ый сервопривод
          write_servo(myservoC, 20, 90, v);
          break;
 
        case 'd': //Если после идет "d", это указывает на 4ый сервопривод
          write_servo(myservoD, 0, 120, v);
          break;
 
        case 'e': //Если после идет "e", это указывает на 5ый сервопривод
         write_servo(myservoE, 10, 170, v);
          break;
 
        case 'f': //Если после идет "f", это указывает на 6ый сервопривод
          write_servo(myservoF, 0, 155, v);
          break;
 
         default:
          Serial.println("ERROR: wrong command \"" + mycommand + "\"");
          break;
      }

      sprintf(buff, "%s %d", cmd, v);
      Serial.println(buff);
      
      mycommand = "";
  } // end if(mycomflag=2)
 
  if (mycomflag == 2) //Если автоматический режим работы[2]
  {
    delay(3000);
    //Serial.println("auto station"); 
    myservosetup();
    myspeed = 500;
    for (pos = 0; pos <= myspeed; pos += 1)
    {
      myservoA.write(int(map(pos, 1, myspeed, 66, 90)));  // Вращение от 66 градусов до 90 градусов (угол можно менять)
      myservoB.write(int(map(pos, 1, myspeed, 90, 70)));  // Вращение от 90 градусов до 40 градусов (угол может быть изменен)
      delay(1);
    }
 
    delay(1000);
    myspeed = 500;
    for (pos = 0; pos <= myspeed; pos += 1)
    {
      myservoC.write(int(map(pos, 1, myspeed, 50, 65)));  // 
      myservoD.write(int(map(pos, 1, myspeed, 90, 170))); //
      myservoE.write(int(map(pos, 1, myspeed, 90, 5)));
      delay(1);
    }
 
    myspeed = 1000;
    for (pos = 0; pos <= myspeed; pos += 1)
    {
      myservoB.write(int(map(pos, 1, myspeed, 70, 90)));  // 
      myservoC.write(int(map(pos, 1, myspeed, 65, 50)));  //
      delay(1);
    }
 
    myspeed = 500;
    for (pos = 0; pos <= myspeed; pos += 1)
    {
      myservoC.write(int(map(pos, 1, myspeed, 50, 45)));  // 
      myservoD.write(int(map(pos, 1, myspeed, 170, 90))); //
      myservoE.write(int(map(pos, 1, myspeed, 5, 50)));
      myservoF.write(int(map(pos, 1, myspeed, 90, 40)));
      delay(1);
    }
 
    myspeed = 1000;
    for (pos = 0; pos <= myspeed; pos += 1)
    {
      myservoA.write(int(map(pos, 1, myspeed, 90, 140))); // 
      myservoF.write(int(map(pos, 1, myspeed, 40, 130)));
      delay(1);
    }
 
    myspeed = 500;
    for (pos = 0; pos <= myspeed; pos += 1)
    {
      myservoA.write(int(map(pos, 1, myspeed, 140, 90))); // 
      myservoC.write(int(map(pos, 1, myspeed, 45, 50)));  // 
      myservoB.write(int(map(pos, 1, myspeed, 90, 70)));  //
      myservoE.write(int(map(pos, 1, myspeed, 50, 120))); //
      delay(1);
    }
  }
 
  if (mycomflag == 0) //Состояние останова[0]
  {
    myservosetup();
  }
 
  if (mycomflag == 3) //Состояние останова[0]
  {
    for (int m = 0; m < mycommand.length(); m++)  // Чтение данных
    {
      char ch = mycommand[m];
 
      switch (ch)
      {
        case '0'...'9':
          v = v *10 + ch - '0'; //字符转换成十进制   
          Serial.println(v);
          break;
 
        case 'x':
          Serial.println(V_neg* int(x[v]) + 60);
          if ((v >= 0) && (v <= 120))
          {
            myservoA.write(V_neg* int(x[v]) + 60);
            v = 0;
            V_neg = 1;
            myprint();
          }
          else if (0 > v)
          {
            v = 0;
            myservoA.write(V_neg* int(x[v]) + 60);
            Serial.println("The minimum value of 0 has been reached");
            V_neg = 1;
            myprint();
          }
          else if (120 < v)
          {
            v = 120;
            myservoA.write(V_neg* int(x[v]) + 60);
            V_neg = 1;
            Serial.println("The maximum value of 120 has been reached");
            v = 0;
            myprint();
          }
 
          break;
 
        case '-':
          V_neg = -1; //字符转换成十进制
          break;
      }
    }
 
    mycommand = "";
  }
}
