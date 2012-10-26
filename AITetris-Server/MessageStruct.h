/*
 Copyright (c) 2005-2012 Gigon Bae

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE, DAMMIT.
*/
typedef struct
{
	BYTE board[22][10];       //My game board
	BYTE targetBoard[22][10];	//Enemy's game board
	BYTE nextBlock[7];      //Block info.(index 0 is current block)
	BYTE nowCombo; 	        // # of combo		
	BYTE holdBlockNum;	    // # of current hold block(empty==-1)
	BYTE isOnHold;	        // Can¡¯t use 'hold' if this value is 1	
	BYTE numOfGarbageLines;  // # of garbage lines to be pushed
	BYTE holePositionX[5];  // The x-position of the hole in garbage lines.
}GAME_INFO;


typedef struct
{
	int posX;      // X position of dropping block
	BYTE turn;		// # of rotation
	BYTE useHold;	// Use hold if useHold is not 0
	BYTE message[256];  //Message to show
	BYTE reserved[2];  //align struct
}DECISION_INFO;
