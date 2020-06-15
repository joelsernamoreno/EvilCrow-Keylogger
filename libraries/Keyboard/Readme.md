Arduino Keyboard library with multiple layout support
=====================================================
 
Based on Arduino Keyboard library 1.0.1

This library allows an Arduino board with USB capabilites to act as a Keyboard.

For more information about this library please visit http://www.arduino.cc/en/Reference/Keyboard

Modified by Ernesto Sanchez to support multiple keyboard layout 

Supported layouts:
* be_be
* cz_cz
* da_dk
* de_de
* en_us (default)
* es_es
* fi_fi
* fr_fr
* it_it
* pt_pt
* tr_tr

__NOTE:__ Only en_us and es_es are tested at june 2017. 

Download and installation
=========================
- Click "Clone or download" -> "Download ZIP"
- Unzip downloaded file in Arduino/libraries/ directory

Use
===
To configure the keyboard layout it just add #define kbd_lang after #include <Keyboard.h>

Example:
```
#define kbd_es_es
#include <Keyboard.h>
```

You can use:
* kbd_be_be
* kbd_cz_cz
* kbd_da_dk
* kbd_de_de
* kbd_en_us
* kbd_es_es
* kbd_fi_fi
* kbd_fr_fr
* kbd_it_it
* kbd_pt_pt
* kbd_tr_tr

If none is especified en_us is used by default.

Version History
===============
```
(Date format: DD/MM/YYYY)
* 8/7/2017  Fix backslash problem in es_es.h
* 14/6/2017 First commit

```

TO DO
=====
- Test all layouts
- Implement a solution for extended ascii characters


Contact
=======
Open an issue, ask me on twitter to [@ernesto_xload](http://www.twitter.com/ernesto_xload/) or visit www.sanchezpano.info
