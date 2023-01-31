#include <FastLED.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define RTD_PIN    9
#define PROGRESS_PIN    11
#define TREND_PIN    10
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    9
#define BRIGHTNESS          100
#define FRAMES_PER_SECOND  10

CRGB leds_rtd[NUM_LEDS];
CRGB leds_progress[NUM_LEDS];
CRGB leds_progress_current[NUM_LEDS];


const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
char messageFromPC[numChars] = {0};
boolean newData = false;

int r = 0; //rtd hue
int p11 = 0; //current progress 1-9
int p12 = 0; //fine step 0-255
int p21 = 0; //current step progress 1-9
int p22 = 0; //fine step 0-255

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index
    strtokIndx = strtok(tempChars,","); // get the first part - the string
    r = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");  // this continues where the previous call left off
    p11 = atoi(strtokIndx);     
    strtokIndx = strtok(NULL, ","); 
    p12 = atoi(strtokIndx);     
    strtokIndx = strtok(NULL, ","); 
    p21 = atoi(strtokIndx);     
    strtokIndx = strtok(NULL, ","); 
    p22 = atoi(strtokIndx);     

}

void updateLED()
{
  FastLED.clear();
  fill_solid(leds_rtd, 9, CHSV(r, 192,192));
  fill_solid(leds_progress, p11, CHSV(0, 0, 255));
  fill_solid(leds_progress+p11, 1 , CHSV(0, 0, p12));
  fill_solid(leds_progress_current, p21, CHSV(0, 0, 255));
  fill_solid(leds_progress_current+p21, 1 , CHSV(0, 0, p22));
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}
void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(115200);
  Serial.setTimeout(1);
  FastLED.addLeds<LED_TYPE,RTD_PIN,COLOR_ORDER>(leds_rtd, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,PROGRESS_PIN,COLOR_ORDER>(leds_progress, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,TREND_PIN,COLOR_ORDER>(leds_progress_current, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

//uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        updateLED();
        newData = false;
    }

}
