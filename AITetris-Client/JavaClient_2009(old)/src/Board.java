import java.util.Arrays;

public class Board {
    public byte               board[][]        = new byte[22][10];                                                     //Board 

    //all block can rotate 0~3 times, but somtimes rotate makes same shape. Using 'maxTurn' value can reduce search space.
    public static final int[] MaxTurn          = { 0, 2, 4, 1, 2, 2, 4, 4 };

    private static final int  NOTHING          = 0;
    private static final int  WALL             = 1;
    private static final int  COLLISION        = 2;

    private static final byte BLOCK_NULL       = 0;
    private static final byte BLOCK_1          = 1;
    private static final byte BLOCK_2          = 2;
    private static final byte BLOCK_3          = 3;
    private static final byte BLOCK_4          = 4;
    private static final byte BLOCK_5          = 5;
    private static final byte BLOCK_6          = 6;
    private static final byte BLOCK_7          = 7;
    private static final byte BLOCK_DEAD       = 8;

    public static byte        BlockData[][][]  = {
            { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
            { { 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 }, { 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },
            { { 0, 0, 2, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 2, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, { 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0 } },
            { { 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
            { { 0, 4, 4, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 4, 4, 0, 4, 4, 0, 0, 0, 0, 0, 0 }, { 4, 0, 0, 0, 4, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0 } },
            { { 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0 }, { 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0 } },
            { { 6, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 6, 6, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0 }, { 0, 6, 0, 0, 0, 6, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0 } },
            { { 0, 7, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 7, 0, 0, 0, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 7, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0 }, { 0, 7, 0, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0 } } };
    public static byte        BlockDataX[][][] = new byte[8][4][4];                                                    //BlockDataX[blockNum][turn][i]=x-position of the block's ith component
    public static byte        BlockDataY[][][] = new byte[8][4][4];                                                    //BlockDataX[blockNum][turn][i]=y-position of the block's ith component

    static {
        int i, j, k, l;
        int value;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 4; j++) {
                int count = 0;
                for (k = 0; k < 4; k++)
                    for (l = 0; l < 4; l++) {
                        value = BlockData[i][j][k * 4 + l];
                        if (value > 0) {
                            BlockDataX[i][j][count] = (byte) l;
                            BlockDataY[i][j][count] = (byte) k;
                            count++;
                        }
                    }
            }
        }
    }


    public Board makeClone() {

        Board result = new Board();
        multiArrayCopy(board, result.board);
        return result;
    }


    public void multiArrayCopy(byte[][] source, byte[][] destination) {

        for (int i = 0; i < source.length; i++) {
            System.arraycopy(source[i], 0, destination[i], 0, source[i].length);
        }
    }


    public void clearBoard() {

        for (int i = 0; i < 22; i++)
            Arrays.fill(board[i], (byte) 0);
    }


    /*
     * Return # of filled lines
     */
    public int countFilledLine() {

        int i, j, count = 0;
        boolean sw;
        for (i = 2; i < 22; i++) {
            sw = true;
            for (j = 0; j < 10; j++)
                if (board[i][j] == 0) {
                    sw = false;
                    break;
                }
            if (sw)
                count++;
        }
        return count;
    }


    /* 
     * Delete filled lines and return # of deleted lines
     */
    public int deleteFilledLine() {

        int j, count = 0;
        boolean sw;
        int fillStart, chkStart;
        fillStart = 21;
        chkStart = 21;
        while (chkStart >= 0) {
            sw = true;
            do {

                for (j = 0; j < 10; j++) {
                    if (board[chkStart][j] == 0) {
                        sw = false;
                        break;
                    }
                }
                if (sw) {
                    count++;
                    chkStart--;
                }
            } while (sw);
            if (chkStart != fillStart) {
                System.arraycopy(board[chkStart], 0, board[fillStart], 0, 10);
            }
            fillStart--;
            chkStart--;
        }

        for (int i = fillStart; i >= 0; i--)
            Arrays.fill(board[fillStart], (byte) 0);
        return count;
    }


    /* 
     * Check if block is crashed against
     */
    public int chkBlock(int x, int y, int blockNum, int turn) {

        int j;
        int value;
        for (j = 0; j < 4; j++) {
            value = x + BlockDataX[blockNum][turn][j];
            if (value > 9 || value < 0)
                return WALL;
        }

        for (j = 0; j < 4; j++) {
            int pieceX = x + BlockDataX[blockNum][turn][j];
            int pieceY = y + BlockDataY[blockNum][turn][j];
            if (pieceY >= 0 && ((pieceY) > 21 || board[pieceY][pieceX] > 0))
                return COLLISION;
        }
        return NOTHING;
    }


    /* Find proper y-position where block can be dropped
     *    ##If you can store the height of dropped blocks in advance, you can save processing time using parameter 'y' 
     * [return value]
     *      0     : can't find proper y-position
     *   otherwise: proper y-position where block can be dropped     
     */

    public int chkPutY(int x, int y, int blockNum, int turn) {

        int j;

        //check if block is crashed against wall
        int value;
        for (j = 0; j < 4; j++) {
            value = x + BlockDataX[blockNum][turn][j];
            if (value > 9 || value < 0)
                return 0;
        }


        for (j = 0; j < 4; j++) {
            int pieceX = x + BlockDataX[blockNum][turn][j];
            int pieceY = BlockDataY[blockNum][turn][j];
            if (pieceY >= 0 && board[pieceY][pieceX] > 0)
                return 0;
        }

        for (; y <= 21; y++) {
            boolean sw;
            sw = false;
            for (j = 0; j < 4; j++) {

                int pieceX = x + BlockDataX[blockNum][turn][j];
                int pieceY = y + BlockDataY[blockNum][turn][j];

                if ((pieceY) > 21 || board[pieceY][pieceX] > 0) // if crashed
                {
                    sw = true;
                    break;
                }
            }
            if (sw) {
                y--;
                return y;
            }
        }
        return 0;
    }


    /* Drop block on board(similar to chkPutY)
     *    ##If you can store the height of dropped blocks in advance, you can save processing time using parameter 'y' 
     * [return value]
     *      0     : can't drop block on board
     *   otherwise: proper y-position where block can be dropped     
     */
    public int dropBlock(int x, int y, int blockNum, int turn) {

        int j;
        int value;
        //side wall crash check
        for (j = 0; j < 4; j++) {
            value = x + BlockDataX[blockNum][turn][j];

            if (value > 9 || value < 0)
                return 0;
        }

        //crash check
        for (j = 0; j < 4; j++) {
            int pieceX = x + BlockDataX[blockNum][turn][j];
            int pieceY = BlockDataY[blockNum][turn][j];
            if (pieceY >= 0 && board[pieceY][pieceX] > 0)
                return 0;
        }

        for (; y <= 21; y++) {
            boolean sw;
            sw = false;
            for (j = 0; j < 4; j++) {

                int pieceX = x + BlockDataX[blockNum][turn][j];
                int pieceY = y + BlockDataY[blockNum][turn][j];


                if ((pieceY) > 21 || board[pieceY][pieceX] > 0)// if touched
                {
                    sw = true;
                    break;
                }
            }

            if (sw) {
                y--;
                //drop block
                for (j = 0; j < 4; j++) {
                    int pieceX = BlockDataX[blockNum][turn][j];
                    int pieceY = BlockDataY[blockNum][turn][j];

                    board[y + pieceY][x + pieceX] = (byte) blockNum;
                }
                return y;
            }
        }
        return 0;
    }


    public void printBoard() {

        for (int i = 0; i < 22; i++) {
            for (int j = 0; j < 10; j++) {
                System.out.print(board[i][j] + " ");
            }
            System.out.println();
        }
    }


//####################################################################################################################################


    public int sampleHeuristicValue() {

        int score = 0;
        int topPosY = 22;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 22; j++) {
                if (board[j][i] != BLOCK_NULL) {
                    if (j < topPosY)
                        topPosY = j;
                    break;
                } else
                    score++;
            }
        }
        return score + topPosY * 10;
    }


}
