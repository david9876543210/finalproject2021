#include <SoftwareSerial.h> // Includes the Software Serial library to allow serial device communication
#include <SPI.h> // Includes outside SPI library to allow serial device communication
#include <Wire.h> // Includes outside Wire library to allow I2C communication
#include <Adafruit_GFX.h> // Includes outside Adafruit_GFX library that enables graphics to be displayed on-screen
#include <Adafruit_SSD1306.h> // Includes outside Adafruit_SSD1306 library which installs drivers for the display

#define SCREEN_WIDTH 128 // Declaring OLED display width, in pixels
#define SCREEN_HEIGHT 32 // Declaring OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

#define RE 8 // Define RE pin on board
#define DE 7 // Define DE pin on board

const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xb5, 0xcc}; // Create read-only array to store Nitrogen input from sensor
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xe4, 0x0c}; // Create read-only array to store Phosphorous input from sensor
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0}; // Create read-only array to store Potassium input from sensor

byte values[11];
SoftwareSerial mod(2,3);

#define LOGO_HEIGHT   16 // Create LOGO_HEIGHT variable
#define LOGO_WIDTH    16 // Create LOGO_WIDTH variable 
static const unsigned char PROGMEM logo_bmp[] = // Store information in flash memory
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

void setup() { // Setup function running once
  Serial.begin(9600); // Sets the data rate in bits per second for serial data transmission
  mod.begin(9600); // Sets the data rate in bits per second for serial data transmission
  pinMode (RE, OUTPUT); // Configure RE to be an output pin
  pinMode (DE, OUTPUT); // Configure DE to be an output pin
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display(); // Activates the display 
  delay(2000); // Pause for 2 seconds

  display.clearDisplay(); // Clear display buffer

  display.drawPixel(10, 10, SSD1306_WHITE); // Draw a single pixel in white

  // Show the display buffer on the screen
  display.display(); // Activates the display
  delay(2000); // Pause for 2 seconds
  testdrawline(); // Draw many lines
  testdrawstyles(); // Draw 'stylized' characters
  display.invertDisplay(true); // Invert and restore display, pausing in-between
  delay(1000); // Pause for 1 second
  display.invertDisplay(false); // Invert and restore display, pausing in-between
  delay(1000); // Pause for 1 second
}

void loop() { // Loops the program
  byte val1,val2,val3; // Create three variables to store chemical values
  val1 = nitrogen(); // Initialise variable
  delay(250); // Pause
  val2 = phosphorous(); // Initialise variable
  delay(250); // Pause
  val3 = potassium(); // Initialise variable
  delay(250); // Pause

//The following code is to display the data on the OLED screen
  display.clearDisplay(); // Clear display buffer
  display.setTextSize(1); // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0); // Set cursor position
  display.print("N: "); // Print N:
  display.print(val1); // Print value
  display.print(" mg/kg"); // Print mg/kg

  display.setTextSize(1); // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,10); // Set cursor position
  display.print("P: "); // Print P:
  display.print(val2); // Print value
  display.print(" mg/kg"); // Print mg/kg

  display.setTextSize(1); // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,20); // Set cursor position
  display.print("K: "); // Print K:
  display.print(val3); // Print value
  display.print(" mg/kg"); // Print mg/kg

  display.display(); // Update screen
}

// Three functions for reading the NPK values respectively
byte nitrogen(){ // Nitrogen function
  digitalWrite(DE,HIGH); // Set pin voltage to 3.3V
  digitalWrite(RE,HIGH); // Set pin voltage to 3.3V
  delay(10); // Pause
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW); // Set pin to 0V
    digitalWrite(RE,LOW); // Set pin to 0V
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println(); // Print data to port
  }
  return values[4]; // Return Nitrogen value, storing it in array
}

byte phosphorous(){ // Phosphorous function
  digitalWrite(DE,HIGH); // Set pin voltage to 3.3V
  digitalWrite(RE,HIGH); // Set pin voltage to 3.3V
  delay(10); // Pause
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW); // Set pin to 0V
    digitalWrite(RE,LOW); // Set pin to 0V
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println(); // Print data to port
  }
  return values[4]; // Return Phosphorous value, storing it in array
}
 
byte potassium(){ // Potassium function
  digitalWrite(DE,HIGH); // Set pin voltage to 3.3V
  digitalWrite(RE,HIGH); // Set pin voltage to 3.3V
  delay(10); // Pause
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW); // Set pin to 0V
    digitalWrite(RE,LOW); // Set pin to 0V
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println(); // Print data to port
  }
  return values[4]; // Return Potassium value, storing it in array
}

void testdrawline() { // Draw line function
  int16_t i; // Initialise 16 bit integer

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1); // Pause for 1 millisecond
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1); // Pause for 1 millisecond
  }
  delay(250); // Pause for 250 milliseconds

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1); // Pause for 1 millisecond
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1); // Pause for 1 millisecond
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawstyles(void) {
  display.clearDisplay(); // Clear display buffer

  display.setTextSize(1); // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0); // Start at top-left corner
  display.println(F("Your reading is..")); // Display text
  display.display(); // Update screen with new text
  delay(2000); // Pause for 2 seconds
}
