Valentine-Arduino
=================

For driving an array of LEDs with a Valentine's message.

Currently this is configured for a 5x5 array of pixels.

I used Adafruit's RGB Pixels which have a WS2801 driving them, and a 5V 2A power supply.

There's a class (LEDFader) here which wraps the WS2801 library and provides the ability to animate pixel color changes, which makes for a smoother color flow.

For the hardware, I followed http://www.instructables.com/id/Yet-Another-Daft-Punk-Coffee-TableDisplay/#step0, though of course I replaced the individual LEDs with the Pixels. I also thickened the box, by 3cm; this added a hidden back area with space for the wiring and arduino.

I ordered a translucent cover from Blick art materials (failed to find anything locally). http://www.dickblick.com/products/grafix-drafting-film/. I found the 2-sided version of this better at hiding the LEDs within, there's more blurring going on.

Photos
------

![LED array next to Arduino](gperks.github.com/Valentine-Arduino/pics/IMG_3159.jpg)
