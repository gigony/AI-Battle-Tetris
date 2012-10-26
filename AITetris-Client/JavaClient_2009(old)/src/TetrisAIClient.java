import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

public class TetrisAIClient {

    public static final boolean DEBUG_ = true;


    public static SocketChannel createSocketChannel(String hostName, int port) throws IOException {

        /* this implementation is referenced by http://www.exampledepot.com/egs/java.nio/NbClientSocket.html*/
        // Create a non-blocking socket channel
        SocketChannel sChannel = SocketChannel.open();
        sChannel.configureBlocking(false);

        // Send a connection request to the server; this method is non-blocking
        sChannel.connect(new InetSocketAddress(hostName, port));
        return sChannel;
    }


    public static void main(String[] args) throws InterruptedException {

        SocketChannel sChannel = null;

        try {
            System.out.print("Enter server address:");
            Scanner scanner = new Scanner(System.in);
            String ipAddress = scanner.next();
            System.out.print("Enter port number:");
            int portNumber = scanner.nextInt();


            sChannel = createSocketChannel(ipAddress, portNumber);

            // Before the socket is usable, the connection must be completed
            // by calling finishConnect(), which is non-blocking
            while (!sChannel.finishConnect()) {
                Thread.sleep(5);

            }
            sChannel.configureBlocking(false);


            //Send team name
            ByteBuffer teamNameBuf = Util.getByteBuffer(AI.TEAM_NAME, 256);
            boolean ret = sendData(sChannel, teamNameBuf, 256);
            if (!ret) {
                sChannel.close();
                System.exit(1);
            }

            AI ai = new AI();

            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(1024);
            while (true) {
                //receive message
                byteBuffer.position(0);
                ret = recvData(sChannel, byteBuffer, 456);
                if (!ret)
                    break;

                byteBuffer.flip();
                GameInfo gameInfo = new GameInfo(byteBuffer);


                if (DEBUG_)
                    System.out.println("Received message");

                //#############calculate#################
                DecisionInfo decisionInfo = ai.decision(gameInfo);
                //##############################


                //send message
                ret = sendData(sChannel, decisionInfo.getByteBuffer(), 264);
                if (!ret)
                    break;

                if (DEBUG_)
                    System.out.println("Sended message");

            }
            sChannel.close();


        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


    private static boolean sendData(SocketChannel sChannel, ByteBuffer byteBuffer, int len) throws IOException, InterruptedException {

        int count = 0;
        while (true) {
            int numBytesWritten = sChannel.write(byteBuffer);
            if (numBytesWritten == -1)
                return false;
            count += numBytesWritten;
            if (count == len)
                return true;
            Thread.sleep(5);
        }
    }


    private static boolean recvData(SocketChannel sChannel, ByteBuffer byteBuffer, int len) throws IOException, InterruptedException {

        int count = 0;
        while (true) {
            int numBytesRead = sChannel.read(byteBuffer);
            if (numBytesRead == -1)
                return false;
            count += numBytesRead;
            if (count == len)
                return true;
            Thread.sleep(5);
        }
    }

}
