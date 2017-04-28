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

<<<<<<< HEAD

//Description: The display() outputs the current state of the game
//to the serial port.
//Pre:  gameStatus[] must be populated or cleared
//Post: Current state of the game will be displayed.
void SerialDisplay();
=======
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

>>>>>>> 5c9b10e2604c25b23b9e0bb3d5ecd3ee395cae05

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

void LEDDisplay();

void main()
{
  char input; //The input from a specific polling sequence
  EA = 1;  //Enalbes interrupts

  //Setup I/O ports
  //TODO - Using bidirectional for now, does serial need something else?
  P2M1 = 0x00;
  P1M1 = 0x00;
  P0M1 = 0x00;
SerialDisplay();  //Just for testings
  
  //Loop forever until power off
  while(1)
  {
    StartGame();


	//Game loop - run until victory
<<<<<<< HEAD
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
=======
	while(!gameEnd)
	{
	  if(!gameEnd)
	  {
        //Check for input
	    do
	    {
	      input = PollButtons();
	    }while(input == 0 || gameStatus[input - 1] != ' ');
        //Record new game input
        gameStatus[input - 1] = 'X';

        //Show new display
	    LEDDisplay();

	    //Check for win condition
        if(CheckWin())
	    {
	      //Victory
          gameEnd = 1;
  	    }
	  }
      
	  if(!gameEnd)
	  {
	    //Player 2 Logic
	    //Check for input
	    do
	    {
	      input = PollButtons();
	    }while(input == 0 || gameStatus[input - 1] != ' ');
        //Record new game input
        gameStatus[input - 1] = 'O';

        //Show new display
  	    LEDDisplay();

	    //Check for win condition
        if(CheckWin())
	    {
	      //Victory
          gameEnd = 1;
>>>>>>> 5c9b10e2604c25b23b9e0bb3d5ecd3ee395cae05
	    }
	  }

	  //Wait for buttons to release
	  while(PollButtons() != 0);
	}   
  }
}

void StartGame(){
  int i;
  gameEnd = 0;
  
  for(i = 0; i < 9; i++){
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
