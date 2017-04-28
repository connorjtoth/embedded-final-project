#include <reg932.h>
#include "uart.h" //Needed for serial communications

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
void SerialDisplay();

//Characters representing each location's status
// ' ' means Not taken
// 'O' means taken by O
// 'X' means taken by X

//First 3 characters are top row left->right
//Next 3 are middle row left->right
//Last 3 are bottom row left->right
char gameStatus[9];

//Boolean variable to track if the game should continue
bit gameEnd;

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
void main(){
  char input; //The input from a specific polling sequence
  EA = 1;  //Enalbes interrupts

  //Setup I/O ports
  //TODO - Using bidirectional for now, does serial need something else?
  P2M1 = 0x00;
  P1M1 = 0x00;
  P0M1 = 0x00;
SerialDisplay();  //Just for testings
  
  //Loop forever until power off
  while(1){
    StartGame();


	//Game loop - run until victory
	while(!gameEnd){

      //Check for input
	  input = PollButtons();
	  if(input != 0){
        if(gameStatus[input - 1] != ' '){
          //Record new game input
          gameStatus[input - 1] = 'X';

		  //Check for win condition
		  if(CheckWin){
		    //Victory
            gameEnd = 1;
		  }
	    }
	  }
	}   
  }
}

void StartGame(){
  int i;
  gameEnd = 0;
  
  for(i = 0; i < 9; i++){
    gameStatus[i] = ' ';
  }

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
}

bit CheckWin(){
  return 0;
}

void SerialDisplay()
{
  uart_init(); //Initializes serial transmission
  uart_transmit('H');
  uart_transmit('E');
  uart_transmit('L');
  uart_transmit('L');
  uart_transmit('O');
  uart_transmit('\n');

  
  uart_transmit('H');
  uart_transmit('E');
  uart_transmit('L');
  uart_transmit('L');
  uart_transmit('O');
  uart_transmit('\n');



  return;
}
