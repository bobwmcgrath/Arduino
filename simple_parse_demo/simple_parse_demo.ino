// Receive with start- and end-markers combined with parsing https://forum.arduino.cc/index.php?topic=288234.60

const byte numChars = 9;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use by strtok() function

      // variables to hold the parsed data
//char messageFromPC[numChars] = {0};
int integerFromPC = 0;
int int2FromPC = 0;

boolean newData = false;

//============

void setup() {
    Serial.begin(115200);
    Serial3.begin(115200);
    //Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <12,24>  ");
    Serial.println();
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
    //delay(1000);
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial3.available() > 0 && newData == false) {
        rc = Serial3.read();

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

//============

void parseData() {

      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(tempChars, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    int2FromPC = atoi(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("Integer ");
    Serial.println(integerFromPC);
    Serial.print("Int2 ");
    Serial.println(int2FromPC);
}
