/**
 * MIT License
 * 
 * Copyright (c) 2020 MIT E-Vent
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * Buttons.cpp
 */

#include "Buttons.h"


namespace buttons {


/// PressHoldButton ///

void PressHoldButton::update() {
  const unsigned long time_now = millis();
  const unsigned long time_since_last_low = time_now - last_pressed_time_;
  const bool press_lost = time_since_last_low > kDebounceDelay;
  if (pin_ == true) {
    if (!press_lost) {
      current_hold_time_ += time_since_last_low;
    }
    last_pressed_time_ = time_now;
  }
  else if (press_lost) {
    current_hold_time_ = 0;
  }
}


/// DebouncedButton ///

DebouncedButton::DebouncedButton(volatile bool & pin): pin_(pin) {}

//void DebouncedButton::begin() {pinMode(pin_, INPUT_PULLUP);}

bool DebouncedButton::isPressed() {
  bool pressed_value = false;
  const unsigned long time_now = millis();
  if (pin_ == true) {
    if ((time_now - last_pressed_time_) > kDebounceDelay) {
      pressed_value = true;
    }
    last_pressed_time_ = time_now;
  }
  return pressed_value;
}


}  // namespace buttons
