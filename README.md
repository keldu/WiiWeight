# WiiWeight
Short input code which processes the wii balance board input and calculates a bad estimate of weight with the simplest possible kalman filter.  

Currently just an upload of old hacky code done over the weekend on a congress.  
Calibration was done at one weight point with another scale,  
so it really isn't calibrated.  

Compile with 'gcc wiiweight.c -o wiiweight'. It's not that hard.  

To use this the wii balance board has to be connected and you have to know which number it has assigned in udev.  
Use 'evtest' to check which number is assigned to the balance board.  
