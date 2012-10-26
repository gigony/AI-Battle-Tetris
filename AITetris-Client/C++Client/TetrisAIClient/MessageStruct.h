typedef struct
{
	BYTE board[22][10];       //My game board
	BYTE targetBoard[22][10];	//Enemy's game board
	BYTE nextBlock[7];      //Block info.(index 0 is current block)
	BYTE nowCombo; 	        // # of current combo
	BYTE holdBlockNum;	    // # of current hold block(empty==0)
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
	BYTE reserved[2];  //align struct(don't use)
}DECISION_INFO;
