/*
 *
 * Copyright (c) [2020] by InvenSense, Inc.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
 
#include "ICM45688P.h"

// Instantiate an ICM456XX with LSB address set to 0
ICM456xx IMU(Wire,0);

const char* axis_str[3] = {"X", "Y", "Z"}; 
const char* direction_str[2] = {"+", "-"}; 
volatile bool irq_received = false;

void irq_handler(void) {
  irq_received = true;
}

void setup() {
  int ret;
  Serial.begin(115200);
  while(!Serial) {}

  // Initializing the ICM456XX
  ret = IMU.begin();
  if (ret != 0) {
    Serial.print("ICM456xx initialization failed: ");
    Serial.println(ret);
    while(1);
  }
  
  // APEX Tap enabled, irq on pin 2
  IMU.startTap(2,irq_handler);

}

void loop() {
  if(irq_received)
  {
    uint8_t tap_count=0;
    uint8_t axis=0;
    uint8_t direction=0;
    irq_received = false;
    IMU.getTap(tap_count,axis,direction);
    Serial.print("Tap count:");
    Serial.println(tap_count);
    Serial.print("Axis:");
    Serial.println(axis_str[axis]);
    Serial.print("Direction:");
    Serial.println(direction_str[direction]);
  }
}
