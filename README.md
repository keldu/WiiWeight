# WiiWeight
Short input code which processes the wii balance board input and calculates a bad estimate of weight with the simplest possible kalman filter.  

Currently just an upload of old hacky code done over the weekend on a congress.  
Calibration was done at one weight point with another scale,  
so it really isn't calibrated.  

Compile with `gcc -o wiiweight wiiweight.c`.  

To use this the wii balance board has to be connected and you need the assigned number by ev.  
Use `evtest` to check which number `$X` is assigned to the balance board and pass it with `./wiiweight /dev/input/event$X`.  

The program will show the current measurement and the kalman filtered measurement in cout.  
