#include "Wire.h"
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
#include "RTClib.h"
#include <DS3231.h>

DS3231  rtc(SDA, SCL);
Time now;
LiquidCrystal_I2C lcd (0x27,2,1,0,4,5,6,7); //I2C Configuration
const int BUTTON1 = 7; // Naming switch button pin for changing time zones
int BUTTON1state =0; //when the button is not pressed means it is at default
const int P1 = 2; //for menu
const int P2 = 5; //for incrementing the value+

//************Variables Used Including New Debounce Variable**************//
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dateupg;
int dayofweekupg;
int menu=0;
const int debounce = 500;
int count=0;

char week [7] [12] ={"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
void setup() {
  Serial.begin(9600);
    // LCD Configuration
    lcd.begin(20,4);
    lcd.setBacklightPin(3,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.clear();
    //RTC Configuration
     rtc.begin();
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(P1,INPUT_PULLUP);
    pinMode(P2,INPUT_PULLUP);

    delay(1000);
    lcd.setCursor(7, 0);
    lcd.print("Hello");
    lcd.setCursor(4, 1);
    lcd.print("Welcome to");
    lcd.setCursor(4, 2);
    lcd.print("World Clock");
    delay(5000);
    lcd.clear();
}

void loop() {
  // in which subroutine should we go?
  if (menu==0)
  {
     DisplayDateTime(); // void DisplayDateTime
  }
  if (menu==1)
  {
    // lcd.clear();
    DisplaySetHour();
    delay(100);
  }
  if (menu==2)
  {
    DisplaySetMinute();
     delay(100);
  }
  if (menu==3)
  {
    DisplaySetYear();
     delay(100);
  }
  if (menu==4)
  {
    DisplaySetMonth();
     delay(100);
  }
  if (menu==5)
  {
    DisplaySetDate();
     delay(100);
  }
  if (menu==6)
  {
    StoreAgg(); 
     delay(100);
    //delay(500);
    menu=0;
  }
    delay(50);

}

//displaying the world clock
void DisplayDateTime()
{
    if(digitalRead(P1) == LOW){
      menu=1;
      delay(debounce);
    }
   
   now = rtc.getTime();
   if(digitalRead(BUTTON1)==LOW)
   {
    count++;
    lcd.clear();
   }
    // Reading button status / input

  //India Time Zone Code
  if(count%6==0)
  {
    //lcd.clear();
    
    // DateTime now = rtc.now();

    //Time
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    if(now.hour!=0 && (now.hour < 10 || (12 < now.hour && now.hour < 22)))
    {
      lcd.print(0);
    }
    if(now.hour == 0)
    {
      lcd.print(12);
    }
    else if(now.hour > 12) //12 hour clock
    {
      lcd.print(now.hour - 12, DEC);
    }
    else
    {
      lcd.print(now.hour,DEC);
    }
    hourupg = now.hour;
    lcd.print(':');
    if(now.min < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.min,DEC);
    minupg = now.min;
    lcd.print(':');
    if(now.sec < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.sec,DEC);

    //Date
    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(now.year,DEC);
    yearupg = now.year;
    lcd.print('/');
    if(now.mon <10)
    {
      lcd.print(0);
    }
    lcd.print(now.mon,DEC);
    monthupg = now.mon;
    lcd.print('/');
    if(now.date < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.date,DEC);
    dateupg = now.date;

    //Day of Week
    lcd.setCursor(0,2);
    lcd.print("Day: ");
    if(now.dow == 7)
    {
      lcd.print(week[0]);
    }
    else
    {
      lcd.print(week[now.dow]);
    }    
    dayofweekupg = now.dow;    
    lcd.print(" ");

    //Time Zone
    lcd.setCursor(0,3);
    lcd.print("TimeZone:");
    lcd.print("INDIA(IST)");

    // AM and PM
    if(now.hour <12)
    {
      lcd.setCursor(17, 1);
      lcd.print("A.M");
    }
    else
    {
      lcd.setCursor(17, 1);
      lcd.print("P.M");
    }
    Serial.println(now.sec);
    int i = now.sec;

    delay(500);
    if(i==59){
      lcd.clear(); //Clear the display every minute
      Serial.println("CLEAR");
    }
  }
  
  // New York Time Zone Code
  if(count%6==1)
  {
    // lcd.clear();
    // now = rtc.getTime();
    int hour = now.hour - 9;
    int dayofweek = now.dow;
    int date = now.date;
    int month = now.mon;
    int year = now.year;
    int minute = now.min - 30;
    if(minute < 0)//when minute becomes negative
    {
      minute = 60 + minute;
      hour = hour - 1;
    }
    if(hour<0)//when hour becomes negative
    {
      hour = 24 + hour;
      dayofweek = dayofweek - 1;
      date = date - 1;
    }
    //for day of week
    if(dayofweek < 0)
    {
      dayofweek = 7 + dayofweek;
    }

    //for date
    if(date == 0)
    {
      month = month - 1;
      if(month == 0)
      {
        month = 12;
        year = year -1;
      }
      date = 30;
      if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
      {
        date = 31;
      }
      if(month == 2)
      {
        date = 28;
        //leap year
        if(year % 100 == 0)
        {
          if(year % 400 == 0)
          {
            date = 29;
          }
        }
        else if(year % 4 == 0)
        {
          date = 29;
        }
      }
    }
   
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    if(hour!=0 && (hour < 10 || (12 < hour && hour < 22)))
    {
      lcd.print(0);
    }
    if(hour == 0)
    {
      lcd.print(12);
    }
    if(hour > 12)
    {
      lcd.print(hour - 12, DEC);
    }
    else
    {
      lcd.print(hour,DEC);
    }
    
    lcd.print(':');
    if(minute < 10)
    {
      lcd.print(0);
    }
    lcd.print(minute,DEC);
    lcd.print(':');
    if(now.sec < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.sec,DEC);

    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(year,DEC);
    lcd.print('/');
    if(month <10)
    {
      lcd.print(0);
    }
    lcd.print(month,DEC);
    lcd.print('/');
    if(date <10)
    {
      lcd.print(0);
    }
    lcd.print(date,DEC);

    lcd.setCursor(0,2);
    lcd.print("Day: ");
    if(dayofweek == 7)
    {
      lcd.print(week[0]);
    }
    else
    {
      lcd.print(week[dayofweek]);
    } 
    lcd.print(" ");

    lcd.setCursor(0,3);
    lcd.print("TimeZone:");
    lcd.print("N.Y(EDT)");

    // AM and PM
    if(hour <12)
    {
      lcd.setCursor(17, 1);
      lcd.print("A.M");
    }
    else
    {
      lcd.setCursor(17, 1);
      lcd.print("P.M");
    }
    Serial.println(now.sec);
    
    int i = now.sec;

    delay(500);
    if(i==59){
      lcd.clear(); //Clear the display every minute
      Serial.println("CLEAR");
    }

  }

  //London Time Zone Code
  if(count%6==2)
  {
    // lcd.clear();
    // now = rtc.getTime();
    int hour = now.hour - 5;
    int dayofweek = now.dow;
    int date = now.date;
    int month = now.mon;
    int year = now.year;
    int minute = now.min - 30;
    if(minute < 0)
    {
      minute = 60 + minute;
      hour = hour - 1;
    }
    if(hour<0)
    {
      hour = 24 + hour;
      dayofweek = dayofweek - 1;
      date = date - 1;
    }
    //for day of week
    if(dayofweek < 0)
    {
      dayofweek = 7 + dayofweek;
    }
    if(date == 0)
    {
      month = month - 1;
      if(month == 0)
      {
        month = 12;
        year = year -1;
      }
      date = 30;
      if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
      {
        date = 31;
      }
      if(month == 2)
      {
        date = 28;
        //leap year
        if(year % 100 == 0)
        {
          if(year % 400 == 0)
          {
            date = 29;
          }
        }
        else if(year % 4 == 0)
        {
          date = 29;
        }
      }
    }
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    if(hour!=0 && (hour < 10 || (12 < hour && hour < 22)))
    {
      lcd.print(0);
    }
    if(hour == 0)
    {
      lcd.print(12);
    }
    if(hour > 12)
    {
      lcd.print(hour - 12, DEC);
    }
    else
    {
      lcd.print(hour,DEC);
    }
    
    lcd.print(':');
    if(minute < 10)
    {
      lcd.print(0);
    }
    lcd.print(minute,DEC);
    lcd.print(':');
    if(now.sec < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.sec,DEC);

    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(year,DEC);
    lcd.print('/');
    if(month <10)
    {
      lcd.print(0);
    }
    lcd.print(month,DEC);
    lcd.print('/');
    if(date <10)
    {
      lcd.print(0);
    }
    lcd.print(date,DEC);

    lcd.setCursor(0,2);
    lcd.print("Day: ");
    if(dayofweek == 7)
    {
      lcd.print(week[0]);
    }
    else
    {
      lcd.print(week[dayofweek]);
    } 
    lcd.print(" ");

    lcd.setCursor(0,3);
    lcd.print("TimeZone:");
    lcd.print("LONDON(GMT)");

    //AM to PM
    if(hour <12)
    {
      lcd.setCursor(17, 1);
      lcd.print("A.M");
    }
    else
    {
      lcd.setCursor(17, 1);
      lcd.print("P.M");
    }

    Serial.println(now.sec);
    
    int i = now.sec;

    delay(500);
    if(i==59){
      lcd.clear(); //Clear the display every second
      Serial.println("CLEAR");
    }
  }

  //Tokyo Time Zone Code
  if(count%6 == 3)
  {
    // lcd.clear();
    // now = rtc.getTime();
    int hour = now.hour + 3;
    int dayofweek = now.dow;
    int date = now.date;
    int month = now.mon;
    int year = now.year;
    int minute = now.min + 30;
    if(minute >= 60)//when minutes exceed 60
    {
      minute = minute - 60;
      hour = hour + 1;
    }
    if(hour >= 24)//when hours exceed 24
    {
      hour = hour - 24;
      dayofweek = dayofweek + 1;
      date = date + 1;
    }
    // for week day
    if(dayofweek > 6)
    {
      dayofweek = dayofweek - 7;
    }
    //for date
    if(date > 30)
    {
      if(month == 4 || month == 6 || month == 9 || month == 11)
      {
        date = date - 30;
      }
      month = month + 1;
      if(month > 12)
      {
        month = month -12;
        year = year + 1;
      }
    }
    //when the standard month is february
    if(now.mon == 2)
    {
      if(date > 28)
      {
        if(date >29)
        {
          date = date - 29;
        }
        if(year % 100 == 0)
        {
          if(year % 400 == 0)
          {
            date = date;
          }
          else
          {
            date = date - 28;
          }
        }
        else if(year % 4 == 0)
        {
          date = date;
        }
        else
        {
          date = date - 28;
        }
      }
    }
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    if(hour!=0 && (hour < 10 || (12 < hour && hour < 22)))
    {
      lcd.print(0);
    }
    if(hour == 0)
    {
      lcd.print(12);
    }
    if(hour > 12)
    {
      lcd.print(hour - 12, DEC);
    }
    else
    {
      lcd.print(hour,DEC);
    }
    
    lcd.print(':');
    if(minute < 10)
    {
      lcd.print(0);
    }
    lcd.print(minute,DEC);
    lcd.print(':');
    if(now.sec < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.sec,DEC);

    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(year,DEC);
    lcd.print('/');
    if(month <10)
    {
      lcd.print(0);
    }
    lcd.print(month,DEC);
    lcd.print('/');
    if(date <10)
    {
      lcd.print(0);
    }
    lcd.print(date,DEC);

    lcd.setCursor(0,2);
    lcd.print("Day: ");
    if(dayofweek == 7)
    {
      lcd.print(week[0]);
    }
    else
    {
      lcd.print(week[dayofweek]);
    } 
    lcd.print(" ");

    lcd.setCursor(0,3);
    lcd.print("TimeZone:");
    lcd.print("TOKYO(JST)");

    //AM to PM
    if(hour <12)
    {
      lcd.setCursor(17, 1);
      lcd.print("A.M");
    }
    else
    {
      lcd.setCursor(17, 1);
      lcd.print("P.M");
    }

    Serial.println(now.sec);
    
    int i = now.sec;

    delay(500);
    if(i==59){
      lcd.clear(); //Clear the display every second
      Serial.println("CLEAR");
    }

  }
  //Istanbull Time Zone
  if(count%6==4)
  {
    // lcd.clear();
    // now = rtc.getTime();
    int hour = now.hour - 2;
    int dayofweek = now.dow;
    int date = now.date;
    int month = now.mon;
    int year = now.year;
    int minute = now.min - 30;
    if(minute < 0)
    {
      minute = 60 + minute;
      hour = hour - 1;
    }
    if(hour<0)
    {
      hour = 24 + hour;
      dayofweek = dayofweek - 1;
      date = date - 1;
    }
    //for day of week
    if(dayofweek < 0)
    {
      dayofweek = 7 + dayofweek;
    }

    //for date
    if(date == 0)
    {
      month = month - 1;
      if(month == 0)
      {
        month = 12;
        year = year -1;
      }
      date = 30;
      if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
      {
        date = 31;
      }
      if(month == 2)
      {
        date = 28;
        //leap year
        if(year % 100 == 0)
        {
          if(year % 400 == 0)
          {
            date = 29;
          }
        }
        else if(year % 4 == 0)
        {
          date = 29;
        }
      }
    }
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    if(hour!=0 && (hour < 10 || (12 < hour && hour < 22)))
    {
      lcd.print(0);
    }
    if(hour == 0)
    {
      lcd.print(12);
    }
    if(hour > 12)
    {
      lcd.print(hour - 12, DEC);
    }
    else
    {
      lcd.print(hour,DEC);
    }
    
    lcd.print(':');
    if(minute < 10)
    {
      lcd.print(0);
    }
    lcd.print(minute,DEC);
    lcd.print(':');
    if(now.sec < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.sec,DEC);

    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(year,DEC);
    lcd.print('/');
    if(month <10)
    {
      lcd.print(0);
    }
    lcd.print(month,DEC);
    lcd.print('/');
    if(date <10)
    {
      lcd.print(0);
    }
    lcd.print(date,DEC);

    lcd.setCursor(0,2);
    lcd.print("Day: ");
    if(dayofweek == 7)
    {
      lcd.print(week[0]);
    }
    else
    {
      lcd.print(week[dayofweek]);
    } 
    lcd.print(" ");

    lcd.setCursor(0,3);
    lcd.print("TimeZone:");
    lcd.print("TURKEY(TRT)");

    //AM to PM
    if(hour <12)
    {
      lcd.setCursor(17, 1);
      lcd.print("A.M");
    }
    else
    {
      lcd.setCursor(17, 1);
      lcd.print("P.M");
    }

    Serial.println(now.sec);
    
    int i = now.sec;

    delay(500);
    if(i==59){
      lcd.clear(); //Clear the display every second
      Serial.println("CLEAR");
    }

  }

  //RIO de janerio Time Zone
  if(count%6==5)
  {
    // lcd.clear();
    // now = rtc.getTime();
    int hour = now.hour - 8;
    int dayofweek = now.dow;
    int date = now.date;
    int month = now.mon;
    int year = now.year;
    int minute = now.min - 30;
    if(minute < 0)
    {
      minute = 60 + minute;
      hour = hour - 1;
    }
    if(hour<0)
    {
      hour = 24 + hour;
      dayofweek = dayofweek - 1;
      date = date - 1;
    }
    //for day of week
    if(dayofweek < 0)
    {
      dayofweek = 7 + dayofweek;
    }

    //for date
    if(date == 0)
    {
      month = month - 1;
      if(month == 0)
      {
        month = 12;
        year = year -1;
      }
      date = 30;
      if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
      {
        date = 31;
      }
      if(month == 2)
      {
        date = 28;
        //leap year
        if(year % 100 == 0)
        {
          if(year % 400 == 0)
          {
            date = 29;
          }
        }
        else if(year % 4 == 0)
        {
          date = 29;
        }
      }
    }
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    if(hour!=0 && (hour < 10 || (12 < hour && hour < 22)))
    {
      lcd.print(0);
    }
    if(hour == 0)
    {
      lcd.print(12);
    }
    if(hour > 12)
    {
      lcd.print(hour - 12, DEC);
    }
    else
    {
      lcd.print(hour,DEC);
    }
    
    lcd.print(':');
    if(minute < 10)
    {
      lcd.print(0);
    }
    lcd.print(minute,DEC);
    lcd.print(':');
    if(now.sec < 10)
    {
      lcd.print(0);
    }
    lcd.print(now.sec,DEC);

    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(year,DEC);
    lcd.print('/');
    if(month <10)
    {
      lcd.print(0);
    }
    lcd.print(month,DEC);
    lcd.print('/');
    if(date <10)
    {
      lcd.print(0);
    }
    lcd.print(date,DEC);

    lcd.setCursor(0,2);
    lcd.print("Day: ");
    if(dayofweek == 7)
    {
      lcd.print(week[0]);
    }
    else
    {
      lcd.print(week[dayofweek]);
    } 
    lcd.print(" ");

    lcd.setCursor(0,3);
    lcd.print("TimeZone:");
    lcd.print("RDJ(BRT)");

    //AM to PM
    if(hour <12)
    {
      lcd.setCursor(17, 1);
      lcd.print("A.M");
    }
    else
    {
      lcd.setCursor(17, 1);
      lcd.print("P.M");
    }

    Serial.println(now.sec);
    
    int i = now.sec;

    delay(1000);
    if(i==59){
      lcd.clear(); //Clear the display every second
      Serial.println("CLEAR");
    }
  }
}

void DisplaySetHour()
{
//setting hour
  // P2state = digitalRead(P2);
  lcd.clear();
  // DateTime now = rtc.now();
  
  lcd.setCursor(0,0);
  lcd.print("Set Hour:");
  lcd.setCursor(0,1);
  if(hourupg < 10)
  {
    lcd.print(0);
  }
  // lcd.blink();
  lcd.print(hourupg,DEC);
  
  while(digitalRead(P1) == HIGH){
    // lcd.setCursor(1,1);
    // lcd.blink();
  if(digitalRead(P2) == LOW)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
    delay(500);
  
  }
  // DateTime now = rtc.now();
  lcd.setCursor(0,0);
  lcd.print("Set Hour:");
  lcd.setCursor(0,1);
  if(hourupg < 10)
  {
    lcd.print(0);
  }
  lcd.print(hourupg,DEC);
  }
  if(digitalRead(P1) == LOW)
  {
    menu=2;
    delay(debounce);
  }  
}

void DisplaySetMinute()
{
// Setting the minutes

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  if(minupg < 10)
  {
    lcd.print(0);
  }
  lcd.print(minupg,DEC);
  // lcd.blink();
  while(digitalRead(P1) == HIGH){
  if(digitalRead(P2) == LOW)
    {
      if (minupg==59)
      {
        minupg=0;
      }
      else
      {
        minupg=minupg+1;
      }
      delay(debounce);
    }
  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  if(minupg < 10)
  {
    lcd.print(0);
  }
  lcd.print(minupg,DEC);
  // lcd.blink();
  //delay(200);
  }
  if(digitalRead(P1) == LOW)
  {
    menu=3;
    delay(debounce);
  }  
}
  
void DisplaySetYear()
{
// setting the year
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Year:");
  lcd.setCursor(0,1);
  lcd.print(yearupg,DEC);
  // lcd.blink();
  while(digitalRead(P1) == HIGH){
  if(digitalRead(P2) == LOW)
    {
      if(yearupg == 2029)
      {
        yearupg= 2000;
      }
      else
      {
        yearupg=yearupg+1;
      }    
    }
    delay(debounce);
    lcd.setCursor(0,0);
    lcd.print("Set Year:");
    lcd.setCursor(0,1);
    // lcd.blink();
    lcd.print(yearupg,DEC);
  }
  if(digitalRead(P1) == LOW)
  {
    menu=4;
    delay(debounce);
  }  
}

void DisplaySetMonth()
{
// Setting the month
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Month:");
  lcd.setCursor(0,1);
  if(monthupg <10)
  {
    lcd.print(0);
  }
  lcd.print(monthupg,DEC);
  // lcd.blink();
  while(digitalRead(P1) == HIGH){
  if(digitalRead(P2) == LOW)
    {
      if (monthupg==12)
      {
        monthupg=1;
      }
      else
      {
        monthupg=monthupg+1;
      }
    }
  
  lcd.setCursor(0,0);
  lcd.print("Set Month:");
  lcd.setCursor(0,1);
  if(monthupg<10)
  {
    lcd.print(0);
  }
  lcd.print(monthupg,DEC);
  // lcd.blink();
  delay(debounce);
  }
  if(digitalRead(P1) == LOW)
  {
    menu=5;
    delay(debounce);
  }  
}

void DisplaySetDate()
{
// Setting the date
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Date:");
  lcd.setCursor(0,1);
  if(dateupg<10)
  {
    lcd.print(0);
  }
  lcd.print(dateupg,DEC);
  // lcd.blink();
  while(digitalRead(P1) == HIGH){
  if(digitalRead(P2) == LOW)
    {
      if (dateupg==31)
      {
        dateupg=1;
      }
      else
      {
        dateupg=dateupg + 1;
      }
    }
    lcd.setCursor(0,0);
    lcd.print("Set Date:");
    lcd.setCursor(0,1);
    if(dateupg<10)
    {
      lcd.print(0);
    }
    lcd.print(dateupg,DEC);
    // lcd.blink();
    delay(debounce);
  }
  if(digitalRead(P1) == LOW)
  {
    menu=6;
    delay(debounce);
  }  
}
void StoreAgg()
{
// Variable saving
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(0,0);
  lcd.print("SAVING");
  lcd.setCursor(0,1);
  lcd.print("IN");
  lcd.setCursor(0,2);
  lcd.print("PROGRESS");
         
  rtc.setTime(hourupg, minupg, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(dateupg, monthupg, yearupg);   // Set the date to October 3th, 2010
  rtc.setDOW();           // Set Day-of-Week to SUNDAY
  delay(500);
}
