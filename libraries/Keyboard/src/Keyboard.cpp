/*
  Keyboard.cpp

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Keyboard.h"

#if defined(_USING_HID)

#define kbd_en_us

/*
#define kbd_es_es
*/

#ifdef kbd_be_be
#include "be_be.h"
#endif
#ifdef kbd_cz_cz
#include "cz_cz.h"
#endif
#ifdef kbd_da_dk
#include "da_dk.h"
#endif
#ifdef kbd_de_de
#include "de_de.h"
#endif
#ifdef kbd_es_es
#include "es_es.h"
#endif
#ifdef kbd_fi_fi
#include "fi_fi.h"
#endif
#ifdef kbd_fr_fr
#include "fr_fr.h"
#endif
#ifdef kbd_it_it
#include "it_it.h"
#endif
#ifdef kbd_pt_pt
#include "pt_pt.h"
#endif
#ifdef kbd_tr_tr
#include "tr_tr.h"
#endif
#ifndef _kbd_lang
#include "en_us.h"
#endif



//================================================================================
//================================================================================
//	Keyboard

static const uint8_t _hidReportDescriptor[] PROGMEM = {

  //  Keyboard
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
   
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    
  0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    
  0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION
};

Keyboard_::Keyboard_(void) 
{
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
}

void Keyboard_::begin(void)
{
}

void Keyboard_::end(void)
{
}

void Keyboard_::sendReport(KeyReport* keys)
{
	HID().SendReport(2,keys,sizeof(KeyReport));
}

/*
extern
const uint8_t _asciimap[256] PROGMEM;

#define SHIFT 0x80
#define ALTGR 0x40
const uint8_t _asciimap[256] =
{
  0x00,             // NUL
  0x00,             // SOH
  0x00,             // STX
  0x00,             // ETX
  0x00,             // EOT
  0x00,             // ENQ
  0x00,             // ACK
  0x00,             // BEL
  0x2a,      // BS  Backspace
  0x2b,      // TAB  Tab
  0x28,      // LF  Enter
  0x00,             // VT
  0x00,             // FF
  0x00,             // CR
  0x00,             // SO
  0x00,             // SI
  0x00,             // DEL
  0x00,             // DC1
  0x00,             // DC2
  0x00,             // DC3
  0x00,             // DC4
  0x00,             // NAK
  0x00,             // SYN
  0x00,             // ETB
  0x00,             // CAN
  0x00,             // EM
  0x00,             // SUB
  0x00,             // ESC
  0x00,             // FS
  0x00,             // GS
  0x00,             // RS
  0x00,             // US
  0x2c,          //  ' ' (space)
  0x1e|SHIFT,    // !
  0x1f|SHIFT,    // "
  0x20|ALTGR,    // #
  0x21|SHIFT,    // $
  0x22|SHIFT,    // %
  0x23|SHIFT,    // &
  0x2d,          // '
  0x25|SHIFT,    // (
  0x26|SHIFT,    // )
  0x30|SHIFT,    // *
  0x30,          // +
  0x36,          // ,
  0x38,          // -
  0x37,          // .
  0x24|SHIFT,    // /
  0x27,          // 0
  0x1e,          // 1
  0x1f,          // 2
  0x20,          // 3
  0x21,          // 4
  0x22,          // 5
  0x23,          // 6
  0x24,          // 7
  0x25,          // 8
  0x26,          // 9
  0x37|SHIFT,      // :
  0x36|SHIFT,      // ;
  0x03,            // < //KEY_NON_US_100
  0x27|SHIFT,      // =
  0x03|SHIFT,      // > //KEY_NON_US_100 + SHIFT
  0x2d|SHIFT,      // ?
  0x1f|ALTGR,      // @
  0x04|SHIFT,      // A
  0x05|SHIFT,      // B
  0x06|SHIFT,      // C
  0x07|SHIFT,      // D
  0x08|SHIFT,      // E
  0x09|SHIFT,      // F
  0x0a|SHIFT,      // G
  0x0b|SHIFT,      // H
  0x0c|SHIFT,      // I
  0x0d|SHIFT,      // J
  0x0e|SHIFT,      // K
  0x0f|SHIFT,      // L
  0x10|SHIFT,      // M
  0x11|SHIFT,      // N
  0x12|SHIFT,      // O
  0x13|SHIFT,      // P
  0x14|SHIFT,      // Q
  0x15|SHIFT,      // R
  0x16|SHIFT,      // S
  0x17|SHIFT,      // T
  0x18|SHIFT,      // U
  0x19|SHIFT,      // V
  0x1a|SHIFT,      // W
  0x1b|SHIFT,      // X
  0x1c|SHIFT,      // Y
  0x1d|SHIFT,      // Z
  0x2f|ALTGR,      // [
  0x35,          // bslash
  0x30|ALTGR,      // ]
  0x2f|SHIFT,    // ^
  0x38|SHIFT,    // _
  0x2f, 	 // `
  0x04,          // a
  0x05,          // b
  0x06,          // c
  0x07,          // d
  0x08,          // e
  0x09,          // f
  0x0a,          // g
  0x0b,          // h
  0x0c,          // i
  0x0d,          // j
  0x0e,          // k
  0x0f,          // l
  0x10,          // m
  0x11,          // n
  0x12,          // o
  0x13,          // p
  0x14,          // q
  0x15,          // r
  0x16,          // s
  0x17,          // t
  0x18,          // u
  0x19,          // v
  0x1a,          // w
  0x1b,          // x
  0x1c,          // y
  0x1d,          // z
  0x34|ALTGR,    // {
  0x1e|ALTGR,    // |
  0x32|ALTGR,    // }
  0x21|ALTGR,    // ~
  0x00,        // DEL
  0x00,             // Ç Start extended ASCII
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,             // ç
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x33,             // ñ
  0x33|SHIFT,       // Ñ
  0x00,
  0x00,
  0x2e|SHIFT,       // ¿
  0x00,
  0x00,
  0x00,
  0x00,
  0x2e,             // ¡
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,

};



// Init var         
bool _altGrMap[128];
bool _altFine = false;

// Individually define all needed char
void initAltGr() {
  _altFine = true;

  _altGrMap[126] = true; // ~
  _altGrMap[123] = true; // {
  _altGrMap[91] = true;  // [
  _altGrMap[93] = true;  // ]
  _altGrMap[125] = true; // }
  _altGrMap[92] = true;  // bslash
  _altGrMap[124] = true; // |
  _altGrMap[64] = true;  // @
  _altGrMap[35] = true; // #
}*/



uint8_t USBPutChar(uint8_t c);

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way 
// USB HID works, the host acts like the key remains pressed until we 
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t Keyboard_::press(uint8_t k) 
{
	uint8_t i;

	if(k>=0xB0 && k<=0xDA){			//it's a non-printing key
		if(k>=0xB5 && k<=0xBE){		//0xB5-0xBE reserved for special non printing keys asigned manually
			if(k==0xB5) k=0x65;	//0xB5 ==> 0x76 (MENU key)
			if(k==0xB6) k=0x46;	//0xB6 ==> 0x46 (PRINT Screen key)
		}
		else{
			k = k - 136;
		}
	}
	else {
	if(k>=0x80 && k<=0x87){			//it's a modifier
		_keyReport.modifiers |= (1<<(k-128));
		k = 0;
	}
	else{					//it's a printable key

	k = pgm_read_byte(_asciimap + k);

	if (k & 0x80) {				// it's a capital letter or other character reached with shift
		_keyReport.modifiers |= 0x02;	// the left shift modifier
		k &= 0x7F;
	}
	if (k & 0x40) {				// altgr modifier (RIGHT_ALT)
		_keyReport.modifiers |= 0x40;	// the left shift modifier
		k &= 0x3F;
	}
	if (k == 0x03) { // special case 0x64
		k = 0x64;
	}
	}
	}

	// Add k to the key report only if it's not already present
	// and if there is an empty slot.
	if (_keyReport.keys[0] != k && _keyReport.keys[1] != k &&
		_keyReport.keys[2] != k && _keyReport.keys[3] != k &&
		_keyReport.keys[4] != k && _keyReport.keys[5] != k) {
		
		for (i=0; i<6; i++) {
			if (_keyReport.keys[i] == 0x00) {
				_keyReport.keys[i] = k;
				break;
			}
		}
		if (i == 6) {
			setWriteError();
			return 0;
		}	
	}
	sendReport(&_keyReport);
	return 1;
}


// Joel
size_t Keyboard_::rawpress(uint8_t k, uint8_t rawmodifiers) //
{
	uint8_t i;

/*
	if(k>=0xB0 && k<=0xDA){			//it's a non-printing key
		if(k>=0xB5 && k<=0xBE){		//0xB5-0xBE reserved for special non printing keys asigned manually
			if(k==0xB5) k=0x65;	//0xB5 ==> 0x76 (MENU key)
			if(k==0xB6) k=0x46;	//0xB6 ==> 0x46 (PRINT Screen key)
		}
		else{
			k = k - 136;
		}
	}
	else {
	if(k>=0x80 && k<=0x87){			//it's a modifier
		_keyReport.modifiers |= (1<<(k-128));
		k = 0;
	}
	else{					//it's a printable key

	k = pgm_read_byte(_asciimap + k);

	if (k & 0x80) {				// it's a capital letter or other character reached with shift
		_keyReport.modifiers |= 0x02;	// the left shift modifier
		k &= 0x7F;
	}
	if (k & 0x40) {				// altgr modifier (RIGHT_ALT)
		_keyReport.modifiers |= 0x40;	// the left shift modifier
		k &= 0x3F;
	}
	if (k == 0x03) { // special case 0x64
		k = 0x64;
	}
	}
	}
*/
	// Add k to the key report only if it's not already present
	// and if there is an empty slot.

	_keyReport.modifiers = rawmodifiers;	

	if (_keyReport.keys[0] != k && _keyReport.keys[1] != k &&
		_keyReport.keys[2] != k && _keyReport.keys[3] != k &&
		_keyReport.keys[4] != k && _keyReport.keys[5] != k) {
		
		for (i=0; i<6; i++) {
			if (_keyReport.keys[i] == 0x00) {
				_keyReport.keys[i] = k;
				break;
			}
		}
		if (i == 6) {
			setWriteError();
			return 0;
		}	
	}
	sendReport(&_keyReport);
	return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t Keyboard_::release(uint8_t k)
{
	uint8_t i;

	if(k>=0xB0 && k<=0xDA){			//it's a non-printing key
		if(k>=0xB5 && k<=0xBE){		//0xB5-0xBE reserved for special non printing keys asigned manually
			if(k==0xB5) k=0x65;	//0xB5 ==> 0x76 (MENU key)
			if(k==0xB6) k=0x46;	//0xB6 ==> 0x46 (PRINT Scr key)
		}
		else{
			k = k - 136;
		}
	}
	else {
	if(k>=0x80 && k<=0x87){			//it's a modifier
		_keyReport.modifiers &= ~(1<<(k-128));
		k = 0;
	}
	else{					//it's a printable key

	k = pgm_read_byte(_asciimap + k);

	if (k & 0x80) {					// it's a capital letter or other character reached with shift
		_keyReport.modifiers &= ~(0x02);	// the left shift modifier
		k &= 0x7F;
	}
	if (k & 0x40) {
		_keyReport.modifiers &= ~(0x40);       // the altgr shift modifier
		k &= 0x3F;
	}
	if (k == 0x03) { // special case 0x64
		k = 0x64;
	}

	if (k >= 136) {			// it's a non-printing key (not a modifier)
		k = k - 136;
	}
	}
	}

	// Test the key report to see if k is present.  Clear it if it exists.
	// Check all positions in case the key is present more than once (which it shouldn't be)
	for (i=0; i<6; i++) {
		if (0 != k && _keyReport.keys[i] == k) {
			_keyReport.keys[i] = 0x00;
		}
	}

	sendReport(&_keyReport);
	return 1;
}

//////Joel

size_t Keyboard_::rawrelease(uint8_t k, uint8_t rawmodifiers)
{
	uint8_t i;

/*
	if(k>=0xB0 && k<=0xDA){			//it's a non-printing key
		if(k>=0xB5 && k<=0xBE){		//0xB5-0xBE reserved for special non printing keys asigned manually
			if(k==0xB5) k=0x65;	//0xB5 ==> 0x76 (MENU key)
			if(k==0xB6) k=0x46;	//0xB6 ==> 0x46 (PRINT Scr key)
		}
		else{
			k = k - 136;
		}
	}
	else {
	if(k>=0x80 && k<=0x87){			//it's a modifier
		_keyReport.modifiers &= ~(1<<(k-128));
		k = 0;
	}
	else{					//it's a printable key

	k = pgm_read_byte(_asciimap + k);

	if (k & 0x80) {					// it's a capital letter or other character reached with shift
		_keyReport.modifiers &= ~(0x02);	// the left shift modifier
		k &= 0x7F;
	}
	if (k & 0x40) {
		_keyReport.modifiers &= ~(0x40);       // the altgr shift modifier
		k &= 0x3F;
	}
	if (k == 0x03) { // special case 0x64
		k = 0x64;
	}

	if (k >= 136) {			// it's a non-printing key (not a modifier)
		k = k - 136;
	}
	}
	}
*/

	_keyReport.modifiers = rawmodifiers;

	// Test the key report to see if k is present.  Clear it if it exists.
	// Check all positions in case the key is present more than once (which it shouldn't be)
	for (i=0; i<6; i++) {
		if (0 != k && _keyReport.keys[i] == k) {
			_keyReport.keys[i] = 0x00;
		}
	}

	sendReport(&_keyReport);
	return 1;
}


void Keyboard_::releaseAll(void)
{
	_keyReport.keys[0] = 0;
	_keyReport.keys[1] = 0;	
	_keyReport.keys[2] = 0;
	_keyReport.keys[3] = 0;	
	_keyReport.keys[4] = 0;
	_keyReport.keys[5] = 0;	
	_keyReport.modifiers = 0;
	sendReport(&_keyReport);
}

size_t Keyboard_::write(uint8_t c)
{	
	uint8_t p = press(c);  // Keydown
	release(c);            // Keyup
	return p;              // just return the result of press() since release() almost always returns 1
}

Keyboard_ Keyboard;

#endif
