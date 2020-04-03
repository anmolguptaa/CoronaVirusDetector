#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>

#include "Adafruit_NeoPixel.h"

#include "MLX90615.h"

#include "WT2003S_Player.h"

#ifdef __AVR__
#include <SoftwareSerial.h>
SoftwareSerial SSerial(2, 3); // RX, TX
#define COMSerial SSerial
#define ShowSerial Serial

WT2003S<SoftwareSerial> Mp3Player;
#endif

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define COMSerial Serial1
#define ShowSerial SerialUSB

WT2003S<Uart> Mp3Player;
#endif

#ifdef ARDUINO_ARCH_STM32F4
#define COMSerial Serial
#define ShowSerial SerialUSB

WT2003S<HardwareSerial> Mp3Player;
#endif



#define BY_PINS // Uncomment this to use pins for every MLX
// #define BY_ADDR // Uncomment this to use single bus

#ifdef BY_PINS // USING ADDITIONAL PINS
byte sda_1 = 5;
byte scl_1 = 4;
byte sda_2 = 7;
byte scl_2 = 6;

SoftI2cMaster i2c_1(sda_1, scl_1);
MLX90615 mlx90615_1(DEVICE_ADDR, &i2c_1);
SoftI2cMaster i2c_2(sda_2, scl_2);
MLX90615 mlx90615_2(DEVICE_ADDR, &i2c_2);
#endif // BY_PINS


#ifdef BY_ADDR
// TODO: Update with your real addresses and quantity of MLXs!
#define DEVICE1_ADDR DEVICE_ADDR
#define DEVICE2_ADDR DEVICE_ADDR+1

// #define INCLUDED_I2C

#ifdef INCLUDED_I2C // Using included I2C Library
#define SDA_PIN SDA //define the SDA pin
#define SCL_PIN SCL //define the SCL pin
SoftI2cMaster i2c_bus(SDA_PIN, SCL_PIN);
MLX90615 mlx90615_1(DEVICE1_ADDR, &i2c_bus);
MLX90615 mlx90615_2(DEVICE2_ADDR, &i2c_bus);
#else // Using Wire
MLX90615 mlx90615_1(DEVICE1_ADDR, &Wire);
MLX90615 mlx90615_2(DEVICE2_ADDR, &Wire);
#endif // WITH_WIRE defined
#endif // BY_ADDR defined


//LED
#define PIN 8
#define NUMPIXELS 24
#define BRIGHTNESS 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500; // delay for half a






int button1 = 14; //trigger button A0

//MP3
uint8_t vol = 70;
uint32_t spi_flash_songs = 0;
uint32_t sd_songs = 0;
STROAGE workdisk = SD;


float temperatureObj1 = 0;
float temperatureObj2 = 0;





void setup() {
Wire.begin();
SeeedGrayOled.init(SH1107G); //initialize SEEED OLED display
SeeedGrayOled.clearDisplay(); //Clear Display.
SeeedGrayOled.setNormalDisplay(); //Set Normal Display Mode
SeeedGrayOled.setVerticalMode(); // Set to vertical mode for displaying text

COMSerial.begin(9600);
Mp3Player.init(COMSerial);
Mp3Player.volume(vol);

Serial.begin(115200);
while (!Serial); // Only for native USB serial
delay(2000); // Additional delay to allow open the terminal to see setup() messages
Serial.println("Setup...");


pinMode(button1, INPUT);


strip.setBrightness(BRIGHTNESS);
strip.begin();
strip.show(); // Initialize all pixels to 'off'

#ifdef WITH_WIRE
Wire.begin();
#endif // WITH_WIRE defined
}

void loop() {
temperatureObj1 = mlx90615_1.getTemperature(MLX90615_OBJECT_TEMPERATURE);
temperatureObj1=temperatureObj1+3;

if(digitalRead(button1)){
temperatureObj2 = mlx90615_2.getTemperature(MLX90615_OBJECT_TEMPERATURE);
temperatureObj2=temperatureObj2+3;
if(temperatureObj2<38){
mode1();
}
else{
Mp3Player.playSDRootSong(0x0001);
mode2();
}

}

delay(1000);

// Serial.print("Temp_1: ");
// Serial.print(temperatureObj1);
// Serial.print("°C ");

SeeedGrayOled.setTextXY(2,0); //set Cursor to ith line, 0th column
SeeedGrayOled.setGrayLevel(5); //Set Grayscale level. Any number between 0 - 15.
SeeedGrayOled.putFloat(temperatureObj1); //Print Hello World





}


void mode1(void){

for(int i=0;i<NUMPIXELS;i++){
// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
strip.setPixelColor(i, strip.Color(0,255,0)); // Moderately bright green color.
strip.show(); // This sends the updated pixel color to the hardware.
// delay(250); // Delay for a period of time (in milliseconds).
}
}

void mode2(void){

for(int i=0;i<NUMPIXELS;i++){
// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
strip.setPixelColor(i, strip.Color(255,0,0)); // Moderately bright green color.
strip.show(); // This sends the updated pixel color to the hardware.
// delay(250); // Delay for a period of time (in milliseconds).
}
}