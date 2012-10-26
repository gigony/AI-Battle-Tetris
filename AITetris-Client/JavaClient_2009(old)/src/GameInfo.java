import java.nio.ByteBuffer;


public class GameInfo {

    public Board   board         = new Board();
    public Board   targetBoard   = new Board();
    public int     nextBlock[]   = new int[7];  //Block info.(index 0 is current block)
    public int     nowCombo;                   // # of current combo       
    public int     holdBlockNum;               // # of current hold block(empty==0)
    public boolean isOnHold;                   // Can¡¯t use 'hold' if this value is 1  
    public int     numOfGarbageLines;          // # of garbage lines to be pushed
    public int[]   holePositionX = new int[5];  // The x-position of the hole in garbage lines.


    public GameInfo(ByteBuffer byteBuffer) {

        for (int i = 0; i < 22; i++)
            for (int j = 0; j < 10; j++)
                board.board[i][j] = byteBuffer.get();
        for (int i = 0; i < 22; i++)
            for (int j = 0; j < 10; j++)
                targetBoard.board[i][j] = byteBuffer.get();
        for (int i = 0; i < 7; i++)
            nextBlock[i] = byteBuffer.get();
        nowCombo = byteBuffer.get();
        holdBlockNum = byteBuffer.get();
        isOnHold = byteBuffer.get() == (byte) 1 ? true : false;
        numOfGarbageLines = byteBuffer.get();
        for (int i = 0; i < 5; i++)
            holePositionX[i] = byteBuffer.get();
    }
}
