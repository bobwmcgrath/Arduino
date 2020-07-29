
#include <stdio.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

void setup() {
  Serial.begin(9600);

}

void loop() {
      char result[10][7];

    Serial.println("Number of rows: %d\n", LEN(result));
    Serial.println("Number of columns: %d\n", LEN(result[0]));
}
