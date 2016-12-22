package org.ea.mlp;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class TestFloat {
    public static void main(String args[]) {
        float[] disparity=new float[640*480];

        disparity[1]=1.5f;
        disparity[2]=4.566f;

        //WRITE
        try{
            RandomAccessFile aFile = new RandomAccessFile("demo.data", "rw");
            FileChannel outChannel = aFile.getChannel();

            //one float 4 bytes
            ByteBuffer buf = ByteBuffer.allocate(4*640*480);
            buf.clear();
            buf.asFloatBuffer().put(disparity);

            while(buf.hasRemaining()) {
                outChannel.write(buf);
            }

            //outChannel.close();
            buf.rewind();

            float[] out=new float[3];
            buf.asFloatBuffer().get(out);

            outChannel.close();

        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }

        //READ
        float[] readback=new float[640*480];
        try{

            RandomAccessFile rFile = new RandomAccessFile("demo.data", "rw");
            FileChannel inChannel = rFile.getChannel();
            ByteBuffer buf_in = ByteBuffer.allocate(640*480*4);
            buf_in.clear();

            inChannel.read(buf_in);

            buf_in.rewind();
            buf_in.asFloatBuffer().get(readback);

            inChannel.close();

            System.out.println(readback[1]);
            System.out.println(readback[2]);

        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }
}
