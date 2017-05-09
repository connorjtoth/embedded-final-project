#include <reg932.h>
#include "uart.h" //Needed for serial communications
#include "main_music.h"



// -----------------------------------------------------------------------------
// Port and Pin Variable Definitions
// -----------------------------------------------------------------------------

// b<row><column> = <port-pin>
// Refers to each button built-in to the simon board 
// and its corresponding port pin based on location
sbit bTopLeft    = P2^0;
sbit bTopMid     = P0^1;
sbit bTopRight   = P2^3;
sbit bMidLeft    = P0^2;
sbit bMidMid     = P1^4;
sbit bMidRight   = P0^0;
sbit bBotLeft    = P2^1;
sbit bBotMid     = P0^3;
sbit bBotRight   = P2^2;

// l<row><column> = <port-pin>
// Refers to each LED built-in to the simon board 
// and its corresponding port pin based on location
sbit lTopLeft    = P2^4;
sbit lTopMid     = P0^5;
sbit lTopRight   = P2^7;
sbit lMidLeft    = P0^6;
sbit lMidMid     = P1^6;
sbit lMidRight   = P0^4;
sbit lBotLeft    = P2^5;
sbit lBotMid     = P0^7;
sbit lBotRight   = P2^6;

// p1_pulse and p2_pulse are used for sending
// high pulses to Arduino UNO used for displaying
// and counting scores. p1_pulse for Player 'X', 
// p2_pulse for player 'O'
sbit p1_pulse    = P1^2;
sbit p2_pulse    = P1^3;
// Refers to the piezo buzzer built-in to the simon
// board and its corresponding port pin
sbit SPKR        = P1^7;



// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------

// these values are maximum wait values for 16-bit
// timers, and are utilized in the LED flashing function
// for display 'O' cells on the LED grid.
#define LED_FLASH_TIME_HIGH -65535 >> 8
#define LED_FLASH_TIME_LOW  -65535

// This message is sent over serial communication
// upon initialization of the program to ensure serial
// communication is working
char welcome_msg[] = "HELLO!\n\0";

// This null-terminating character array is used for
// displaying the board. (it is the horizontal divider
// for rows) and is used in printGameStatus
char line[] = "-----";



// -----------------------------------------------------------------------------
// Regular-use Variable Definitions
// -----------------------------------------------------------------------------

// these iterators are variables that can be used for iteration
// they must be reset each time they are used by programmer
unsigned int iterator;
unsigned char char_iterator;


// length is used in the music playing subroutine and
// is the upper limit of the total note length
unsigned char length;


unsigned char pulse_counter;


//Characters representing each location's status
// ' ' means Not taken
// 'O' means taken by O
// 'X' means taken by X

/* 
  Given a tic-tac-toe board,

    0 | 1 | 2 
   -----------
    3 | 4 | 5
   -----------
    6 | 7 | 8

  the values in each cell correspond to its
  index in gameStatus. gameStatus has a '\0'
  at its end in the event a programmer wants
  to output gameStatus for debugging purposes.
*/
char gameStatus[10];


// boolean flag used in main program for determining
// when the game has concluded
bit gameEnd;


// boolean to determine if introduction music is done
bit introduction_flag;


// char for iterating through each character in a
// null-terminating character array in SerialDisplay
// subroutine
char msg_i = 0;


// char to store input from a specific polling sequence
// returned from under the PollButtons function
unsigned char input = 0x00;


// char that cycles each game turn between 'X' and
// 'O' representing the player whose piece will be
// played next
char current_player = 'X';

// game_output is a null-terminating character array
// that provides a nicely formatted row of tic-tac-toe
// used in printGameStatus
char game_output[] = " | | ";

// row is used to iterate through each row of the board
// in printGameStatus
char row;

// col is used to iterate through each column of each row
// of the board in printGameStatus
char col;

// when AI_flag is 1, then a computer plays 'O's and
// human plays 'X's. Otherwise, humans control both 'O's
// and 'X's
bit AI_flag = 0;

// note_its counts the number of times the timer controlling
// frequency oscillation for notes has been triggered. When it 
// reaches FREQUENCY_DIVISOR, it resets to 0.
unsigned char note_its;

// duration counts the number of times the timer controlling
// note duration has been triggered. When it reaches 'length',
// it moves to the next note or quits music.
unsigned char duration;

// when using half-time mode, mod increases the amount of
// time necessary for duration to be incremented
char mod;

// rest_played is a flag that indicates whether or not
// the mandatory short pause between notes has been completed
char rest_played;

// note_num counts the number of notes that have been played
// and is used to find frequency and duration of the current
// note in arrays.
unsigned char note_num = 0;

// num_notes is the length of the notes/duration arrays storing
// musical information
unsigned char num_notes = 32;


// note_ptr points to the array containing the number n 
// that corresponds to 111*n = number of machine cycles 
// per oscillation for a given note
char idata * note_ptr;

// duration_ptr points to the array containing the 
// number n that corresponds to (n / 32)th note 
// when playing at a given tempo
char idata * duration_ptr;

// introduction_flag is 1 both enables the music functions
// to be utilized and implies that they are being used
// as introduction_flag is automatically set to 0 at the end
// of use of music functions
bit introduction_flag = 1;

// nbc_flag is 1 enables the use of the NBC chimes for music
// it is automatically set to 1 after the initial song is played
// i.e. Maxine Nightingale's Gotta Get Right Back.
bit nbc_flag = 0;



// -----------------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------------

// ------------------------------------
// restart_timer1
// Desc: Resets values for 16-bit timer in music mode
// Post: Timer is turned on and has 
// ------------------------------------
void restart_timer1 ( );

// ------------------------------------
// restart_timer1
// Desc:
// Pre:
// Post:
// ------------------------------------
void SerialDisplay(char *);

// ------------------------------------
// restart_timer1
// Desc:
// Pre:
// Post:
// ------------------------------------
void StartGame();

// ------------------------------------
// restart_timer1
// Desc:
// Pre:
// Post:
// ------------------------------------
char PollButtons();

// ------------------------------------
// restart_timer1
// Desc:
// Pre:
// Post:
// ------------------------------------
void play_sound_byte();

// ------------------------------------
// restart_timer1
// Desc:
// Pre:
// Post:
// ------------------------------------
bit CheckWin();


//Description: The display() outputs the current state of the game
//to the serial port.
//Pre:  gameStatus[] must be populated or cleared
//Post: Current state of the game will be displayed.












//Loops through and checks each button
//Returns (as a number) the location of the first
//button pressed
// 0 is returned if no button is pressed



// -----------------------------------------------------------------------------
// Interrupt Service Routines (ISRs)
// -----------------------------------------------------------------------------


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


/*
  timer 0 interrupt service routine
  when introduction_flag == 0:
    ...
  when introduction_flag == 1:
*/
void display ( ) interrupt TIMER_0
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
  else
  {
    TR0 = 0;
    if(gameStatus[0] == 'X')
    lTopLeft = 0;
  else if(gameStatus[0] == 'O')
    lTopLeft = ~lTopLeft;
  if(gameStatus[1] == 'X')
    lTopMid = 0;
  else if(gameStatus[1] == 'O')
    lTopMid = ~lTopMid;
  if(gameStatus[2] == 'X')
    lTopRight = 0;
  else if(gameStatus[2] == 'O')
    lTopRight = ~lTopRight;
  if(gameStatus[3] == 'X')
    lMidLeft = 0;
    else if(gameStatus[3] == 'O')
    lMidLeft = ~lMidLeft;
  if(gameStatus[4] == 'X')
    lMidMid = 0;
  else if(gameStatus[4] == 'O')
    lMidMid = ~lMidMid;
  if(gameStatus[5] == 'X')
    lMidRight = 0;
  else if(gameStatus[5] == 'O')
    lMidRight = ~lMidRight;
  if(gameStatus[6] == 'X')
    lBotLeft = 0;
  else if(gameStatus[6] == 'O')
    lBotLeft = ~lBotLeft;
  if(gameStatus[7] == 'X')
    lBotMid = 0;
  else if(gameStatus[7] == 'O')
    lBotMid = ~lBotMid;
  if(gameStatus[8] == 'X')
    lBotRight = 0;
  else if(gameStatus[8] == 'O')
    lBotRight = ~lBotRight;
  TH0= LED_FLASH_TIME_HIGH;
  TL0 = LED_FLASH_TIME_LOW;
  TR0 = 1;
  }
  return;
}




// -----------------------------------------------------------------------------
// Function Definitions
// -----------------------------------------------------------------------------


void shortDelay()
{
  for (pulse_counter = 0; pulse_counter < 255; pulse_counter++);
}

void play_sound_byte ( )
{
  // set the music to be the NBC clip
  //note_ptr =     nbc_notes;
  //duration_ptr = nbc_durations;

  // ------------------------------------------------------
  // set up the timers
  // ------------------------------------------------------

  // Timer 0 ==> 8-bit auto-reload
  // Timer 1 ==> 16-bit mode
  TMOD = 0x12;

  // Enable Timer 0, Timer 1 interrupts
  // Disable Serial interrupts
  IEN0 |= 0x8A;

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








void restart_timer1 ( )
{
  TH1 = DURATION_32_NOTE >> 8;
  TL1 = DURATION_32_NOTE;
  TR1 = 1;
}





// ------------------------------------
// print Game Status
// ------------------------------------
void printGameStatus ( )
{

  for (row = 0; row < 3; row++)
  {   
    for (col = 0; col < 3; col++)
    {
      game_output[col * 2] = gameStatus[3 * row + col];
    }
    SerialDisplay(game_output);
    
    if (row != 2)
      SerialDisplay(line);
  }

  uart_transmit('\n');
}

char ai_input ( )
{
  for (iterator = 0; iterator < 9; iterator++)
  {
    if (gameStatus[iterator] == ' ')
    {
      return (char) iterator + 1;
    }
  }
  return 0;
}





void StartGame(){
  char i;
  gameEnd = 0;
  
  for(i = 0; i < 9; i++)
  {
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
  TMOD = 0x01;
  TH0 = LED_FLASH_TIME_HIGH;
  TL0 = LED_FLASH_TIME_LOW;
  IEN0 |= 0x82;
  TR0 = 1;

  //Player 1 should always go first
  current_player = 'X';

  return;
}

char PollButtons ( )
{
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

bit CheckWin ( )
{
  //Check winning by rows
  int i = 0;
  bit tie = 1;
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

  //Check for tie
  for(i = 0; i < 9; i++)
  {
    if(gameStatus[i] == ' ')
	{
      tie = 0;
	}
  }
  if(tie == 1)
  {
    return 1;
  }


   return 0;
}

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





// -----------------------------------------------------------------------------
// Main function body
// -----------------------------------------------------------------------------

void main ( )
{ 
  P2M1 = 0x00;
  P1M1 = 0x00;
  P0M1 = 0x00;
  
  p1_pulse = 0;
  p2_pulse = 0;

  gameStatus[9] = 0; //null terminating char array
  
  introduction_flag = 1;
  note_ptr =     maxine_buzzer1;
  duration_ptr = 0;
  num_notes = 32;
  
  play_sound_byte();
  
  uart_init(); //Initializes serial transmission
  SerialDisplay(welcome_msg);  //Just for testings
  
  //char input = 0; 
  EA = 1;  //Enalbes interrupts

  //Setup I/O ports
  //TODO - Using bidirectional for now, does serial need something else?

  
  //Loop forever until power off
  while(1)
  {

    // if the top-left button is pressed before
    // a game starts, then the player is activating
    // human vs. computer mode, otherwise, it is
    // human vs. human mode
    AI_flag = 0;
    if ( !bTopLeft )
    {
        AI_flag = 1;
    }

    while ( bTopLeft == 0 );   
    
    StartGame();


  //Game loop - run until victory
  while(!gameEnd)
    {

      //Check for input
    do
    {
        if ( AI_flag && current_player == 'O' )
        {
          input = ai_input();
        }
        else
        {
          input = PollButtons();
        }
    }
      while(input == 0 || gameStatus[input - 1] != ' ');
      
      //Record new game input
      gameStatus[input - 1] = current_player;

      printGameStatus();

      //Check for win condition
      if(CheckWin())
    {
      //Victory
        gameEnd = 1;
        //This is for Jonathan's individual part.
        if (current_player == 'X')
        {
            p1_pulse = 1;
            shortDelay();
            p1_pulse = 0;
        }
        else
        {  
            p2_pulse = 1;
            shortDelay();
            p2_pulse = 0;
        }
          
      }

      // switches players
    if (current_player == 'X')
      {
        current_player = 'O';
      }
      else
      {
        current_player = 'X';
      }
    //Wait for buttons to release
    while(PollButtons() != 0);
    }
    // Game is now over!! Play NBC
    introduction_flag = 1;
    play_sound_byte();
    
  }
}

