# Ledstrip-with-Tail
Addition of a tail to the earlier Catenary project
Hang your WS2812 LED strip from each end so it forms a natuaral catenary and the Arduino software will use LEDs to simulate two balls dropping down to the cenre, crossing over, then travelling up the other side. However due to 'frictional losses' they won't climb all the way back up. Gradually they lose energy and languish at the bottom and disappear. The cycle then repeats. Motion incorporates a sine function to simulate simple harmonic motion.

There is also a second LED strip to simulate new balls being generated and falling down.

There is a sound option (defaults ON) to make it more intersting. This is a simply digital output pin, use a resistor chain 
to 'pot in down' and feed into an external amp of some sort, or headphones. 
Suitable resistor values would be 10K and 470R - very approx. 

HC-05 Bluetooth control with a single character provides the following:
   u - up the brightness
   d - dim the brightness
   S - toggle sound output
   f - faster
   s - slower
   
Recommended Android Bluetooth app is 'Arduino Bluetooth Controller'

Demo Youtube video at https://www.youtube.com/watch?v=tzAXXbkTaBU
