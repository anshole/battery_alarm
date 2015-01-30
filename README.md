# battery_alarm

This application sounds an alarm if the (laptop) battery goes below 40%.

How to make:


     gcc `sdl-config --cflags --libs` battery_alarm.c -o battery_alarm -lSDL_mixer -lsmpeg -lSDL

