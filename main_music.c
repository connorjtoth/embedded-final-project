#include <REG932.H>

#define FREQUENCY_DIVISOR 50 // this value divides each frequency so notes can
                             // be stored in one byte instead of two. This also
                             // enables use of 8 bit auto reload mode

#define TIMER_0 1            // timer 0 interrupt code
#define TIMER_1 3            // timer 1 interrupt code

#define DURATION_32_NOTE -48001
#define HALF_TIME_BOOL 1     // determines whether we are in half time

//---------------------
// NOTE LOOKUP TABLE
//---------------------

#define NOTE_C2 564
#define NOTE_CS2 532
#define NOTE_D2 502
#define NOTE_DS2 474
#define NOTE_E2 447
#define NOTE_F2 422
#define NOTE_FS2 399
#define NOTE_G2 376
#define NOTE_GS2 355
#define NOTE_A2 335
#define NOTE_AS2 316
#define NOTE_B2 299
#define NOTE_C3 282
#define NOTE_CS3 266
#define NOTE_D3 251
#define NOTE_DS3 237
#define NOTE_E3 224
#define NOTE_F3 211
#define NOTE_FS3 199
#define NOTE_G3 188
#define NOTE_GS3 178
#define NOTE_A3 168
#define NOTE_AS3 158
#define NOTE_B3 149
#define NOTE_C4 141
#define NOTE_CS4 133
#define NOTE_D4 126
#define NOTE_DS4 118
#define NOTE_E4 112
#define NOTE_F4 106
#define NOTE_FS4 100
#define NOTE_G4 94
#define NOTE_GS4 89
#define NOTE_A4 84
#define NOTE_AS4 79
#define NOTE_B4 75
#define NOTE_C5 70
#define NOTE_CS5 66
#define NOTE_D5 63
#define NOTE_DS5 59
#define NOTE_E5 56
#define NOTE_F5 53
#define NOTE_FS5 50
#define NOTE_G5 47
#define NOTE_GS5 44
#define NOTE_A5 42
#define NOTE_AS5 40
#define NOTE_B5 37


#define NOTE_REST 0
#define DUR_8 8

//-----------------------





//start									1		2			3			4		5			6		7		8			9			10		11			12			13		14		15			16			17		18			19		   20		21			22			23			24		25			26			27		28			29			30			31		32			33
unsigned char song_main_buzzer1[] =   {NOTE_A4, NOTE_REST, NOTE_A4, NOTE_REST, NOTE_C4, NOTE_REST, NOTE_C4, NOTE_REST, NOTE_A4,   NOTE_REST, NOTE_A4, NOTE_C5,   NOTE_REST, NOTE_A4, NOTE_REST, NOTE_REST, NOTE_G4,  NOTE_REST, NOTE_A4,  NOTE_REST, NOTE_F4,  NOTE_REST, NOTE_D4,    NOTE_F4   ,NOTE_REST, NOTE_G4,  NOTE_REST, NOTE_F4,    NOTE_REST, NOTE_REST, NOTE_F4,  NOTE_REST, NOTE_REST };
//unsigned char song_main_buzzer2[] = {NOTE_E4, NOTE_REST, NOTE_E4, NOTE_REST, NOTE_E4, NOTE_REST, NOTE_E4, NOTE_REST, NOTE_E4,   NOTE_REST, NOTE_E4, NOTE_G4,   NOTE_REST, NOTE_E4, NOTE_REST, NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_AS3,   NOTE_REST };
//unsigned char song_main_buzzer3[] = {NOTE_C4, NOTE_REST, NOTE_C4, NOTE_REST, NOTE_G4, NOTE_REST, NOTE_C4, NOTE_REST, NOTE_C4,   NOTE_REST, NOTE_C4, NOTE_E4,   NOTE_REST, NOTE_C4, NOTE_G4,   NOTE_REST, NOTE_AS3, NOTE_REST, NOTE_AS3, NOTE_REST, NOTE_AS3, NOTE_REST, NOTE_REST,  NOTE_REST };
//unsigned char song_main_buzzer4[] = {NOTE_C3, NOTE_REST, NOTE_A2, NOTE_REST, NOTE_G2, NOTE_REST, NOTE_A2, NOTE_C3,   NOTE_REST, NOTE_C3,   NOTE_A2, NOTE_REST, NOTE_C3,   NOTE_G2, NOTE_A2,   NOTE_F4,   NOTE_AS2, NOTE_REST, NOTE_G2,  NOTE_REST, NOTE_F2,  NOTE_REST, NOTE_G2,    NOTE_AS2  };
unsigned char song_main_duration[]=   {DUR_8,   DUR_8,     DUR_8,   DUR_8,     DUR_8,   DUR_8,     DUR_8,   DUR_8,     DUR_8,     DUR_8,     DUR_8,   DUR_8,     DUR_8,     DUR_8,   DUR_8,     DUR_8,     DUR_8,    DUR_8,     DUR_8,    DUR_8,     DUR_8,    DUR_8,     DUR_8,      DUR_8     ,DUR_8,     DUR_8,    DUR_8,     DUR_8,      DUR_8,     DUR_8,     DUR_8,    DUR_8,     DUR_8     };

// first ending
unsigned char song_ending1_buzzer1[] = {NOTE_REST, NOTE_G4,  NOTE_REST, NOTE_F4,    NOTE_REST, NOTE_REST, NOTE_F4,  NOTE_REST, NOTE_REST };
//unsigned char song_ending1_buzzer2[] = {NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_D4,    NOTE_REST, NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_REST };
//unsigned char song_ending1_buzzer3[] = {NOTE_REST, NOTE_AS3, NOTE_REST, NOTE_AS3,   NOTE_REST, NOTE_G4,   NOTE_AS3, NOTE_REST, NOTE_REST };
//unsigned char song_ending1_buzzer4[] = {NOTE_REST, NOTE_AS2, NOTE_G2,   NOTE_REST,  NOTE_F2,   NOTE_F2,   NOTE_G2,  NOTE_F2,   NOTE_REST };
//unsigned char song_ending1_duration[]= {DUR_8,     DUR_8,    DUR_8,     DUR_8,      DUR_8,     DUR_8,     DUR_8,    DUR_8,     DUR_8     };




sbit SPKR = P1^7;            // port used for speaker

// Temporary variables
sbit led1 = P2^4;
sbit led2 = P0^5;

unsigned char note_MCs;
unsigned char note_its;
unsigned char duration;


unsigned char nbc_notes[] =     {NOTE_G4, NOTE_E4, NOTE_C3};
unsigned char nbc_durations[] = {32, 32, 64};

unsigned char note_num = 0;
unsigned char num_notes = 32;//23; //24 for main_buzzer


// note_ptr points to the array containing the number n that corresponds to
// 111*n = number of machine cycles per oscillation for a given note
char idata *note_ptr;

// duration_ptr points to the array containing the number n that corresponds to
// (n / 32)th note when using a given tempo
char idata *duration_ptr;

char mod;
char rest_played;

// -----------------------------------------------------------------------------
// begin main function
// -----------------------------------------------------------------------------

void restart_timer1 ( );

void main ( )
{
  // set the music to be the NBC clip
  //note_ptr =     nbc_notes;
  //duration_ptr = nbc_durations;
  note_ptr =     song_main_buzzer1;
  duration_ptr = song_main_duration;

  // set P0, P1, P2 to be bi-directional
  P0M1 = 0; 
  P1M1 = 0;
  P2M1 = 0;

  // ------------------------------------------------------
  // set up the timers
  // ------------------------------------------------------

  // Timer 0 ==> 8-bit auto-reload
  // Timer 1 ==> 16-bit mode
  TMOD = 0x12;

  // Enable Timer 0, Timer 1 interrupts
  IEN0 = 0x8A;

  // Prioritize Timer 1 over Timer 0
  IP0 = 0x08;

  // Timer 0 will raise a flag based on the note frequency
  TH0 = note_ptr[0];
  TL0 = note_ptr[0];

  // Timer 1 will raise a flag every time the duration of a 32nd note is played
  TH1 = DURATION_32_NOTE >> 8;
  TL1 = DURATION_32_NOTE;

  mod = 1;
  rest_played = 0;

  duration = 0;  // set for 8 32nd notes
  note_its = 0;  // reset of count of iterations (to 111)


  TF1 = 1; // start timer 1
  TR0 = 1; // start timer 2
  led1 = 1;
  led2 = 1;
  while(1);
}


// the Timer0 ISR is responsible for pulse oscillation under
// the duration of the note.

void timer0_isr ( ) interrupt TIMER_0
{
  /*
     This function oscillates the speaker when the flag is raised
     the FREQUENCY_DIVISOR times. Since the flag is based on the timer
     which triggers based on what the current note is, we can vary the
     pitch by changing the timer amount. When we oscillate the speaker,
     we also reset note_its so it can start counting again.
  */

  // if triggered FREQUENCY_DIVISOR times
  if (note_its == FREQUENCY_DIVISOR)
  {
    // oscillate speaker and reset count
    SPKR = ~SPKR;
    note_its = 0;
  }

  // otherwise, mark it and wait until reflagged
  else
  {
    note_its++;
  }
}



// The Timer1 ISR is responsible for providing the duration
// of the note being played

void timer1_isr ( ) interrupt TIMER_1
{
  // turn off the timer while working
  TR1 = 0;

  // if we have not yet reached the note's full duration
  if ( duration != duration_ptr[note_num] )
  {
    
    // if we are using half time
    if (HALF_TIME_BOOL)
    {
      // every other trigger, we increment duration
      mod = (++mod) % 10;
      if ( mod != 9 )
      {
        duration++;
      }
    }
    // otherwise
    else
    {
      // always increment duration
      duration++;
    }

	restart_timer1();
  }

  // if we have reached the note's full duration
   else
  {
    // stop playing the current note
    TR0 = 0;
    TF0 = 0;
    SPKR = 0;

    // make sure we have rested adequately
    if ( rest_played != 5 )
    {
      // acknowledge that this code has been executed
      rest_played++;

      // wait the smallest possible note length
      restart_timer1();
      return;
    }
    
    // if we are at the last note, we light the LED
    if ( note_num == num_notes )
    {
      led1 = 0;
    }

    // otherwise, if there are still notes in the song
    else
    {
      // move on to the next note
      note_num++;

      // reset the duration
      duration = 0;

      // reset the mod, so we don't get left out if in half time
      mod = 1;

      // if the note is stored as 0, do not worry about oscillating
      // b/c it is a rest!
      if ( note_ptr[note_num] == 0 )
      {
        // turn off timer 0
        TR0 = 0;
      }

      // otherwise, the note is real... we must worry
      else
      {
        // set the frequency of the note by pushing to TH0
        // turn on the timer 0;
        TH0 = note_ptr[note_num];
        TL0 = TH0;
        TR0 = 1;
      }

      // ensure that since there are more notes, we are still timing their
      // durations
      TR1 = 1;

      // notice that the rest for the other note has not yet been played
      // so we must reset this variable
      rest_played = 0;
    }
  }
}


void restart_timer1 ( )
{
  TH1 = DURATION_32_NOTE >> 8;
  TL1 = DURATION_32_NOTE;
  TR1 = 1;
}
