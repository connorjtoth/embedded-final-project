
#ifndef MAIN_MUSIC_H
#define MAIN_MUSIC_H

#define FREQUENCY_DIVISOR 25 //111-1=110 // this value divides each frequency so notes can
                             // be stored in one byte instead of two. This also
                             // enables use of 8 bit auto reload mode

#define TIMER_0 1            // timer 0 interrupt code
#define TIMER_1 3            // timer 1 interrupt code

#define DURATION_32_NOTE -48001
#define HALF_TIME_BOOL 1     // determines whether we are in half time

//---------------------
// NOTE LOOKUP TABLE
//---------------------
#define NOTE_C2 1691
#define NOTE_CS2 1596
#define NOTE_D2 1506
#define NOTE_DS2 1422
#define NOTE_E2 1342
#define NOTE_F2 1267
#define NOTE_FS2 1196
#define NOTE_G2 1129
#define NOTE_GS2 1065
#define NOTE_A2 1005
#define NOTE_AS2 949
#define NOTE_B2 896
#define NOTE_C3 845
#define NOTE_CS3 798
#define NOTE_D3 753
#define NOTE_DS3 711
#define NOTE_E3 671
#define NOTE_F3 633
#define NOTE_FS3 598
#define NOTE_G3 564
#define NOTE_GS3 533
#define NOTE_A3 503
#define NOTE_AS3 474
#define NOTE_B3 448
#define NOTE_C4 423
#define NOTE_CS4 399
#define NOTE_D4 377
#define NOTE_DS4 355
#define NOTE_E4 336
#define NOTE_F4 317
#define NOTE_FS4 299
#define NOTE_G4 282
#define NOTE_GS4 266
#define NOTE_A4 251
#define NOTE_AS4 237
#define NOTE_B4 224
#define NOTE_C5 211
#define NOTE_CS5 199
#define NOTE_D5 188
#define NOTE_DS5 178
#define NOTE_E5 168
#define NOTE_F5 158
#define NOTE_FS5 149
#define NOTE_G5 141
#define NOTE_GS5 133
#define NOTE_A5 126
#define NOTE_AS5 119
#define NOTE_B5 112
#define NOTE_C6 106
#define NOTE_CS6 100
#define NOTE_D6 94
#define NOTE_DS6 89
#define NOTE_E6 84
#define NOTE_F6 79
#define NOTE_FS6 75
#define NOTE_G6 71
#define NOTE_GS6 67
#define NOTE_A6 63
#define NOTE_AS6 59
#define NOTE_B6 56
#define NOTE_C7 53
#define NOTE_CS7 50
#define NOTE_D7 47
#define NOTE_DS7 44
#define NOTE_E7 42
#define NOTE_F7 40
#define NOTE_FS7 37
#define NOTE_G7 35
#define NOTE_GS7 33
#define NOTE_A7 31
#define NOTE_AS7 30
#define NOTE_B7 28
#define NOTE_C8 26
#define NOTE_CS8 25
#define NOTE_D8 24
#define NOTE_DS8 22
#define NOTE_E8 21
#define NOTE_F8 20
#define NOTE_FS8 19





#define NOTE_REST 0
#define DUR_8 8

#endif
