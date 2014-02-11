Valentine-Arduino
=================

For driving an array of LEDs with a Valentine's message.

Currently this is configured for a 5x5 array of pixels.

I used Adafruit's RGB Pixels (http://www.adafruit.com/products/322) which have a WS2801 driving them.

There's a class (LEDFader) here which wraps the WS2801 library and provides the ability to animate pixel color changes, which makes for a smoother color flow. Rather than the pixel snapping from color to color, you can specify a duration to setPixelColor over which the pixel's color will animate.

For the hardware, I followed http://www.instructables.com/id/Yet-Another-Daft-Punk-Coffee-TableDisplay/#step0, though of course I replaced the individual LEDs with the Pixels. I also thickened the box, by 3cm; this added a hidden back area with space for the wiring and arduino.

I ordered a translucent cover from Blick art materials (failed to find anything locally). http://www.dickblick.com/products/grafix-drafting-film/. I found the 2-sided version of this better at hiding the LEDs within, there's more blurring going on.

Video
-----
http://vimeo.com/86373164

Photos
------

![LED array next to Arduino](http://gperks.github.com/Valentine-Arduino/pics/IMG_3159.jpg)

Here you can see the rear of the array. The Arduino is connected to the array, and the 5V power to both the pixel array and directly to the Arduino's 5V pin. That goes off via a power adapter (http://www.adafruit.com/products/368) to an inline power switch (http://www.adafruit.com/products/1125) and then to a 5V 2A power supply (http://www.adafruit.com/products/276).

![Rear of LED array](http://gperks.github.com/Valentine-Arduino/pics/IMG_3160.jpg)

This shows the box surrounding the pixels. Part way down are the stops where the pixel layer rests, with space beneath for wires & arduino.

![Case](http://gperks.github.com/Valentine-Arduino/pics/IMG_3161.jpg)

![A heart](http://gperks.github.com/Valentine-Arduino/pics/IMG_3164.jpg)

![Blue lights](http://gperks.github.com/Valentine-Arduino/pics/IMG_3162.jpg)

![Rainbow](http://gperks.github.com/Valentine-Arduino/pics/IMG_3165.jpg)

![Rainbow again](http://gperks.github.com/Valentine-Arduino/pics/IMG_3166.jpg)

With the double-sided matte film.
![Double sided matte](http://gperks.github.com/Valentine-Arduino/pics/IMG_3167.jpg)

With the single-sided matte film.
![Double sided matte](http://gperks.github.com/Valentine-Arduino/pics/IMG_3168.jpg)

With two layers of single-sided matte film.
![Double sided matte](http://gperks.github.com/Valentine-Arduino/pics/IMG_3169.jpg)
