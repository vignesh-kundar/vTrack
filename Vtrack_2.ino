#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device:: actually it represents the pins but refer the pins , go through the pin diagram and follow it up.

// WIFI PASSWORD AND SSID
const char* ssid = "Enter_your_wifi_ssid_here"; //ssid of your wifi
const char* password = "Password_paadle_anna";  //password of your wifi

//LATITUDE AND LONGITUDE DATA WILL BE STORED IN THIS VARIABLE
float latitude , longitude; //YOU CANNOT USE THIS BUT SHOULD BE CONVERTED TO STRING

// EXTRA INFORMATION
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;
WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //connecting to wifi
  while (WiFi.status() != WL_CONNECTED)// while wifi not connected
  {
    delay(500);
    Serial.print("."); //print "...."
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());  // Print the IP address
}


void loop()
{
  while (ss.available() > 0)    //while data is available
    if (gps.encode(ss.read()))  //read gps data
    {
      // IN THIS PART YOU WILL BE GETTING THE LATITUDE AND LONGITUDE IS ALL CREDENTIALS ARE MET
      if (gps.location.isValid()) //check whether gps location is valid
      {
        Serial.println();
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude location is stored in a string
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);//longitude location is stored in a string
      }
      
      //NOTE 
      
      //IN THE ABOVE CODE `lat_str` and `lng_str` THESE TWO CAN BE USED TO UPDATE THE DATA IN THE CLOUD
      
      if (gps.date.isValid()) //check whether gps date is valid
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
        if (date < 10)
          date_str = '0';
        date_str += String(date);   // values of date,month and year are stored in a string
        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);  // values of date,month and year are stored in a string
        date_str += " / ";
        if (year < 10)
          date_str += '0';
        date_str += String(year);   // values of date,month and year are stored in a string
      }
      if (gps.time.isValid())       //check whether gps time is valid
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        minute = (minute + 30);     // converting to IST -> INDIA STANDARD TIME
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;        // converting to IST
        if (hour >= 12)            // checking whether AM or PM
          pm = 1;
        else
          pm = 0;
        hour = hour % 12;
        if (hour < 10)
          time_str = '0';
        time_str += String(hour); //values of hour,minute and time are stored in a string
        time_str += " : ";
        if (minute < 10)
          time_str += '0';
        time_str += String(minute); //values of hour,minute and time are stored in a string
        time_str += " : ";
        if (second < 10)
          time_str += '0';
        time_str += String(second); //values of hour,minute and time are stored in a string
        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";
      }
    }
  
  
  //ONCE YOU GET THE LAT_STRING AND LONG_STRING
  // ITS UP TO YOU WHERE YOU PUT THE LATITUDE AND LONGITUDE INTO.
 
  
 // YOUL GET ONE IP ADDRESS YOU HAVE TO SEE THAT IN THE ARDUINO APPLICATION :)
 // ASK HELP FROM EC PPL IF IAM UNABLE TO THERE YOU SHOULD GET IP ADRESS IN SPECIFIC PORT IF ITS NOT COMMING CHECK OUT SERIAL MONITOR ONCE CONNECTED AND ALSO USE RESET BUTTON IN THE NODE MCU IT WILL RESTART THE PROCESS. 
 WiFiClient client = server.available(); // Check if a client has connected
  if (!client)
  {
    return;
  }
  // Prepare the response
  // BELOW CODE IS TO RENDER A WEB PAGE WHICH WILL BE CREATED LOCALLY
  // HINT PASS THE DATA THROUGH ONE API TO STORE THE LOCATION INFORMATION :)
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS DATA</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS DATA</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += date_str;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += time_str;
  s += "</td>  </tr> </table> ";
  s += "<a href="https://www.google.com/maps/place/12%C2%B049'25.5%22N+74%C2%B050'35.9%22E/@12.8238907,74.842751,17.78z/">Click here to reddirect to Google Maps</a>"
  s += "</body> </html>";

  client.print(s); // all the values are send to the webpage
  delay(100);
}
