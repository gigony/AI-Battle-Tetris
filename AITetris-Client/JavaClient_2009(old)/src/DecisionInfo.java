import java.nio.ByteBuffer;


public class DecisionInfo {

    public int     posX    = 3;
    public int     turn;
    public boolean useHold;
    public String  message = "";


    public ByteBuffer getByteBuffer() {

        ByteBuffer result = ByteBuffer.allocateDirect(512);
        result.order(java.nio.ByteOrder.LITTLE_ENDIAN);
        result.putInt(posX);
        result.put((byte) turn);
        result.put(useHold ? (byte) 1 : (byte) 0);
        result.put(Util.getByteBuffer(message, 256));
        result.put((byte) 0);
        result.put((byte) 0);
        result.flip();
        return result;
    }


}
