import java.util.Random;


public class AI {
    public final static String TEAM_NAME = "Å×Æ®¸®½º ½Å";
    public final static Random random    = new Random();


    public DecisionInfo decision(GameInfo gameInfo) {

        DecisionInfo result = new DecisionInfo();

        Board myBoard = gameInfo.board; //myboard
        Board targetBoard = gameInfo.targetBoard; //enemy's board

//        myBoard.printBoard();

        int currentBlock = gameInfo.nextBlock[0];
        int maxTurn = Board.MaxTurn[currentBlock];


        int maxScore = -999999999;

        for (int x = -2; x < 10; x++) //for possible X
        {
            for (int turn = 0; turn < maxTurn; turn++) { //for possible rotation
                Board tmp = myBoard.makeClone();
                int posY = tmp.dropBlock(x, 0, currentBlock, turn);
                if (posY == 0)
                    continue;

                int filledLine = tmp.countFilledLine();

                int score = calculate(tmp);
                score += filledLine;
//                System.out.format("[[%d::( %d, %d ) ]]    ", score, x, turn);

                if (score > maxScore) {
                    maxScore = score;
                    result.posX = x;
                    result.turn = turn;
                    result.useHold = false;

                }
            }
        }
        System.out.println();
        System.out.format("put block on...  posX:%d turn: %d\n", result.posX, result.turn);

        int randomMessage = random.nextInt(4);

        switch (randomMessage) {
        case 0:
            result.message = "hello~\r\nhi~";
            break;
        case 1:
            result.message = "100ÄÞº¸!\r\n100ÄÞº¸!";
            break;
        case 2:
            //clear message
            result.message = " ";
            break;
        case 3:
            //Reuse previous message(if all data is blank, server uses previous message)
            result.message = "";
            break;
        }


/*      myBoard.printBoard();
        System.out.println();
        myBoard.dropBlock(result.posX,0,gameInfo.nextBlock[0],result.turn);
        myBoard.deleteFilledLine();
        myBoard.printBoard();       
 */

/*
        result.posX=random.nextInt(10);
        result.turn=random.nextInt(4);
        result.useHold=true;
        result.message="simple test";
 */        
        return result;
    }


    public int calculate(Board board) {

        return board.sampleHeuristicValue();
    }


}
