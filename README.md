# battery_alarm

This application sounds an alarm if the (laptop) battery goes below 40%.

Currently it checks battery level every 200 seconds, but you many want to change this depending upon how fast your laptop/device drains battery. 

How to make:


     gcc `sdl-config --cflags --libs` battery_alarm.c -o battery_alarm -lSDL_mixer -lsmpeg -lSDL

