#include <reg932.h>
#include "uart.h" //Needed for serial communications
#include "main_music.h"

//Button definitions
sbit bTopLeft = P2^0;
sbit bTopMid = P0^1;
sbit bTopRight = P2^3;
sbit bMidLeft = P0^2;
sbit bMidMid = P1^4;
sbit bMidRight = P0^0;
sbit bBotLeft = P2^1;
sbit bBotMid = P0^3;
sbit bBotRight = P2^2;

//Description: The display() outputs the current state of the game
//to the serial port.
//Pre:  gameStatus[] must be populated or cleared
//Post: Current state of the game will be displayed.
void SerialDisplay(char *);
//Light definitions
sbit lTopLeft = P2^4;
sbit lTopMid = P0^5;
sbit lTopRight = P2^7;
sbit lMidLeft = P0^6;
sbit lMidMid = P1^6;
sbit lMidRight = P0^4;
sbit lBotLeft = P2^5;
sbit lBotMid = P0^7;
sbit lBotRight = P2^6;


#define LED_FLASH_TIME_HIGH = -50000 >> 8
#define LED_FLASH_TIME_LOW =  -50000

unsigned char length;



// ----------------------------
// Connor's variables start
// ----------------------------

//-----------------------

//start									1		2			3			4		5			6		7		8			9			10		11			12			13		14		15			16			17***lastone		18			19		   20		21			22			23			24		25			26			27		28			29			30			31		32			33
unsigned char song_main_buzzer1[]   = {NOTE_REST, NOTE_A6, NOTE_REST, NOTE_A6, NOTE_REST, NOTE_G6, NOTE_REST, NOTE_G6, NOTE_REST, NOTE_A6,   NOTE_REST, NOTE_A6, NOTE_C7,   NOTE_REST, NOTE_A6, NOTE_REST, NOTE_REST, NOTE_G6,  NOTE_REST, NOTE_A5,  NOTE_REST, NOTE_F5,  NOTE_REST, NOTE_D5,    NOTE_F5   ,NOTE_REST, NOTE_G5,  NOTE_REST, NOTE_F5,    NOTE_REST, NOTE_REST, NOTE_F5,  NOTE_REST, NOTE_REST };
//unsigned char song_main_buzzer2[] = {NOTE_E4, NOTE_REST, NOTE_E4, NOTE_REST, NOTE_E4, NOTE_REST, NOTE_E4, NOTE_REST, NOTE_E4,   NOTE_REST, NOTE_E4, NOTE_G4,   NOTE_REST, NOTE_E4, NOTE_REST, NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_D4,  NOTE_REST, NOTE_AS3,   NOTE_REST };
//unsigned char song_main_buzzer3[] = {NOTE_C4, NOTE_REST, NOTE_C4, NOTE_REST, NOTE_G4, NOTE_REST, NOTE_C4, NOTE_REST, NOTE_C4,   NOTE_REST, NOTE_C4, NOTE_E4,   NOTE_REST, NOTE_C4, NOTE_G4,   NOTE_REST, NOTE_AS3, NOTE_REST, NOTE_AS3, NOTE_REST, NOTE_AS3, NOTE_REST, NOTE_REST,  NOTE_REST };
//unsigned char song_main_buzzer4[] = {NOTE_C3, NOTE_REST, NOTE_A2, NOTE_REST, NOTE_G2, NOTE_REST, NOTE_A2, NOTE_C3,   NOTE_REST, NOTE_C3,   NOTE_A2, NOTE_REST, NOTE_C3,   NOTE_G2, NOTE_A2,   NOTE_F4,   NOTE_AS2, NOTE_REST, NOTE_G2,  NOTE_REST, NOTE_F2,  NOTE_REST, NOTE_G2,    NOTE_AS2  };
//unsigned char song_main_duration[]= {DUR_8, DUR_8, DUR_8,   DUR_8,     DUR_8,   DUR_8,     DUR_8,   DUR_8,     DUR_8,   DUR_8,     DUR_8,     DUR_8,     DUR_8,   DUR_8,     DUR_8,     DUR_8,   DUR_8,     DUR_8,     DUR_8,    DUR_8,     DUR_8,    DUR_8,     DUR_8,    DUR_8,     DUR_8,      DUR_8     ,DUR_8,     DUR_8,    DUR_8,     DUR_8,      DUR_8,     DUR_8,     DUR_8,    DUR_8,     DUR_8     };
//unsigned char song_main_duration[] = {DUR_8};
// first ending
//unsigned char song_ending1_buzzer1[] = {NOTE_REST, NOTE_G4,  NOTE_REST, NOTE_F4,    NOTE_REST, NOTE_REST, NOTE_F4,  NOTE_REST, NOTE_REST };
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


unsigned char nbc_notes[] =     {NOTE_G5, NOTE_E5, NOTE_C4};
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

bit introduction_flag = 1;
bit nbc_flag = 0;

void restart_timer1 ( );
void play_sound_byte ( )
{
  // set the music to be the NBC clip
  //note_ptr =     nbc_notes;
  //duration_ptr = nbc_durations;

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
  TH0 = FREQUENCY_DIVISOR;
  TL0 = FREQUENCY_DIVISOR;

  // Timer 1 will raise a flag every time the duration of a 32nd note is played
  TH1 = DURATION_32_NOTE >> 8;
  TL1 = DURATION_32_NOTE;

  mod = 1;
  rest_played = 0;

  duration = 0;  // set for 8 32nd notes
  note_its = 0;  // reset of count of iterations (to 111)


  TF1 = 1; // start timer 1

  /*if (note_ptr != 1)
  {
    TR0 = 1;
  }*/

  SPKR = 0;
  while(introduction_flag);
}


// The Timer1 ISR is responsible for providing the duration
// of the note being played

void timer1_isr ( ) interrupt TIMER_1
{
  // turn off the timer while working
  TR1 = 0;

  // if we have not yet reached the note's full duration
  length = DUR_8;
  if ( nbc_flag )
  {
    length = duration_ptr[note_num];
  }
  
  if ( duration != length )
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
	note_its = 0;
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

    //no matter what
    
    // reset the duration
    duration = 0;

    // reset the mod, so we don't get left out if in half time
    mod = 1;
 
    // if we are at the last note, we light the LED
    if ( note_num == num_notes )
    {
      introduction_flag = 0;
      note_ptr = nbc_notes;
      duration_ptr = nbc_durations;
      num_notes = 3;
      note_num = 0;
    }

    // otherwise, if there are still notes in the song
    else
    {
      // move on to the next note
      note_num++;

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

// --------------------
// Connors variables end
// --------------------




//Characters representing each location's status
// ' ' means Not taken
// 'O' means taken by O
// 'X' means taken by X

//First 3 characters are top row left->right
//Next 3 are middle row left->right
//Last 3 are bottom row left->right
char gameStatus[10];

//Boolean variable to track if the game should continue
bit gameEnd;

// boolean to determine if introduction music is done
bit introduction_flag;

//Handles 1-time initialization code
void StartGame();

//Loops through and checks each button
//Returns (as a number) the location of the first
//button pressed
// 0 is returned if no button is pressed

//Location grid layout

//------Top------//
// 1  |  2  |  3 //
// 4  |  5  |  6 //
// 7  |  8  |  9 //
//-----Bottom----//
char PollButtons();

bit CheckWin();

void LEDDisplay();




/*
  timer 0 interrupt service routine
  when introduction_flag == 0:
    ...
  when introduction_flag == 1:
*/
void display ( ) interrupt 1
{

  if (introduction_flag)
  {
     /*
     This function oscillates the speaker when the flag is raised
     the FREQUENCY_DIVISOR times. Since the flag is based on the timer
     which triggers based on what the current note is, we can vary the
     pitch by changing the timer amount. When we oscillate the speaker,
     we also reset note_its so it can start counting again.
    */

    // if triggered FREQUENCY_DIVISOR times
    if (note_its == note_ptr[note_num])
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
  
  /*for(i = 0; i < 9; i++)
  {
    if(gameStatus[i] == 'X')
    {
      lTopLeft = ~lTopLeft;
    }
    else if(gameStatus[i] == 'O')
    {
    }
  }*/
  return;
}

unsigned char input = 0x00;//The input from a specific polling sequence


char welcome_msg[] = "HELLO!\n\0";



int iterator = 0;

char current_player = 'X';



char game_output[] = " | | "; // used in printGameStatus
char line[] = "-----";       // used in printGameStatus
char row;                     // used in printGameStatus
char col;                    // used in printGameStatus



// ------------------------------------
// print Game Status
// ------------------------------------
void printGameStatus ( )
{
  //led1 = 0;

  for (row = 0; row < 3; row++)
  {	  
    for (col = 0; col < 3; col++)
    {
      game_output[2 * col] = gameStatus[row * 3 + col];
    }
    
    //SerialDisplay(game_output);
//problem area
	SerialDisplay(line);
  }
  //led1 = 1;
}


void main ( )
{ 
  P2M1 = 0x00;
  P1M1 = 0x00;
  P0M1 = 0x00;
  gameStatus[9] = 0; //null terminating char array
  //introduction_flag = 1;
  note_ptr =     nbc_notes;//song_main_buzzer1;
  duration_ptr = nbc_durations;
  num_notes = 2;
  //play_sound_byte();
  
  //char input = 0; 
  EA = 1;  //Enalbes interrupts

  //Setup I/O ports
  //TODO - Using bidirectional for now, does serial need something else?
  uart_init(); //Initializes serial transmission
  SerialDisplay(welcome_msg);  //Just for testings

  
  //Loop forever until power off
  while(1)
  {
    StartGame();


	//Game loop - run until victory
	while(!gameEnd)
    {

        //Check for input
	  do
	  {
	    input = PollButtons();
	  }
      while(input == 0 || gameStatus[input - 1] != ' ');
      led1 = 0;
      //Record new game input
      gameStatus[input - 1] = current_player;

      //Show new display
      LEDDisplay();

      printGameStatus();

      //Check for win condition
      if(CheckWin())
	  {
	    //Victory
        gameEnd = 1;
  	  }
      
	  if (current_player == 'X')
        current_player = 'O';
      else
        current_player = 'X';

	  //Wait for buttons to release
	  while(PollButtons() != 0);
	led1 = 0;
    }
	led1 = 1;
	led2 = 0;
    // Game is now over!! Play NBC
    introduction_flag = 1;
    play_sound_byte();
    
  }
}

void StartGame(){
  char i;
  gameEnd = 0;
  
  for(i = 0; i <= 9; i++){
    gameStatus[i] = ' ';
  }

  //Reset lights
  lTopLeft = 1;
  lTopMid = 1;
  lTopRight = 1;
  lMidLeft = 1;
  lMidMid = 1;
  lMidRight = 1;
  lBotLeft = 1;
  lBotMid = 1;
  lBotRight = 1;

  //Initialize timer 0 for LED calculations
  /*TMOD &= 0x10;
  TH1 = LED_FLASH_TIME_HIGH;
  TL1 = LED_FLASH_TIME_LOW;
  IEN0 &= 0x82;
  TR0 = 1;*/
  uart_init(); //Initializes serial transmission

  return;
}

char PollButtons(){
  //Loop through and check each button
  if(!bTopLeft)
    return 1;
  if(!bTopMid)
    return 2;
  if(!bTopRight)
    return 3;
  if(!bMidLeft)
    return 4;
  if(!bMidMid)
    return 5;
  if(!bMidRight)
    return 6;
  if(!bBotLeft)
    return 7;
  if(!bBotMid)
    return 8;
  if(!bBotRight)
    return 9;

  return 0;
}

bit CheckWin(){
  //Check winning by rows
  int i = 0;
  for(i = 0; i < 3; i++)
  {
    if(gameStatus[3 * i] == gameStatus[(3*i) + 1] &&
	   gameStatus[3 * i] == gameStatus[(3*i) + 2] &&
	   gameStatus[3 * i] != ' ')
	   
	   return 1;
  }

  //Check winning by columns
  for(i = 0; i < 3; i++)
  {
    if(gameStatus[i] == gameStatus[i + 3] &&
	   gameStatus[i] == gameStatus[i + 6] &&
	   gameStatus[i] != ' ')
	   
	   return 1;
  }

  //Check diagonals
  if(gameStatus[0] == gameStatus[4] &&
     gameStatus[0] == gameStatus[8] &&
	 gameStatus[0] != ' ')
	 
	 return 1;

  if(gameStatus[2] == gameStatus[4] &&
     gameStatus[2] == gameStatus[6] &&
	 gameStatus[2] != ' ')

	 return 1;


   return 0;
}

void LEDDisplay() {
  if(gameStatus[0] != ' ')
  	lTopLeft = 0;
  if(gameStatus[1] != ' ')
  	lTopMid = 0;
  if(gameStatus[2] != ' ')
  	lTopRight = 0;
  if(gameStatus[3] != ' ')
  	lMidLeft = 0;
  if(gameStatus[4] != ' ')
  	lMidMid = 0;
  if(gameStatus[5] != ' ')
  	lMidRight = 0;
  if(gameStatus[6] != ' ')
  	lBotLeft = 0;
  if(gameStatus[7] != ' ')
  	lBotMid = 0;
  if(gameStatus[8] != ' ')
  	lBotRight = 0;
}

char msg_i = 0;
void SerialDisplay(char s_msg[])
{
  msg_i = 0;
  while (s_msg[msg_i] != '\0')
  {
    uart_transmit(s_msg[msg_i]);
    msg_i++;
  }

  for(iterator = 0; iterator < 10000; iterator++);
  return;
}
