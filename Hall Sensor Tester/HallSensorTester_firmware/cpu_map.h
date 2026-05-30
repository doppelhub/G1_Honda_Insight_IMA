//Copyright 2022-2023(c) John Sullivan


//cpu_map.h - CPU and pin mapping configuration file
//central pin mapping selection file for different processor types

#ifndef cpu_map_h
#define cpu_map_h

  #ifdef CPU_MAP_ATMEGA328p
      #if defined HW_REV0
            #define PIN_HALL_C 2 //1k0 external pullup to 5V required
            #define PIN_HALL_B 3 //1k0 external pullup to 5V required
            #define PIN_HALL_A 4 //1k0 external pullup to 5V required
      #endif
  #endif

#endif