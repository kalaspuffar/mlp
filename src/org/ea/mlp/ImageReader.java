package org.ea.mlp;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class ImageReader {
    public int[] readImage(String filename) {
        try {
            BufferedImage bi = ImageIO.read(new File(filename));
            int[] data = bi.getData().getPixels(0, 0, bi.getWidth(), bi.getHeight(), new int[bi.getWidth() * bi.getHeight()]);
            bi.flush();
            return data;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return new int[] {};
    }
}
