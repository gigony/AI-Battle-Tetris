import java.nio.ByteBuffer;


public class Util {

    public static ByteBuffer getByteBuffer(String text, int len) {

        ByteBuffer byteBuf = ByteBuffer.allocateDirect(256);
        byte[] buf;
        try {
            buf = text.getBytes("UTF-16LE");
            if (buf.length > len - 2)
                throw new Exception("Text is too long");
            byteBuf.put(buf);
            byteBuf.position(0);
            byteBuf.limit(len);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
        return byteBuf;
    }

}
