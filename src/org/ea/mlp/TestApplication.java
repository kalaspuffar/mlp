package org.ea.mlp;

import java.util.HashMap;
import java.util.Map;

public class TestApplication {
    static Map<String, Integer> imageMap = new HashMap<String, Integer>();

    public static void main(String[] arg) {
        imageMap.put("images/img0.bmp", 1);
        imageMap.put("images/img1.bmp", 2);
        imageMap.put("images/img2.bmp", 7);
        imageMap.put("images/img3.bmp", 3);
        imageMap.put("images/img4.bmp", 4);
        imageMap.put("images/img5.bmp", 5);
        imageMap.put("images/img6.bmp", 6);
        imageMap.put("images/img7.bmp", 7);
        imageMap.put("images/img8.bmp", 8);
        imageMap.put("images/img9.bmp", 9);
        imageMap.put("images/img10.bmp", 4);
        imageMap.put("images/img11.bmp", 2);
        imageMap.put("images/img12.bmp", 1);
        imageMap.put("images/img13.bmp", 3);
        imageMap.put("images/img14.bmp", 5);
        imageMap.put("images/img15.bmp", 9);
        imageMap.put("images/img16.bmp", 6);
        imageMap.put("images/img17.bmp", 8);
        imageMap.put("images/img18.bmp", 7);
        imageMap.put("images/img19.bmp", 0);
        imageMap.put("images/img20.bmp", 2);
        imageMap.put("images/img21.bmp", 0);

        MultiLayerPerceptron mlp = new MultiLayerPerceptron(3, 24);
        if(!mlp.trainNetwork(0.04f, 0.01f, 0.4f, imageMap, 1000)) {
            System.out.println("There was an error while training ... Quitting\n\r");
            System.exit(0);
        }

        int mb = 1024*1024;
        Runtime runtime = Runtime.getRuntime();
        System.out.println("##### Heap utilization statistics [MB] #####");
        System.out.println("Used Memory:" + (runtime.totalMemory() - runtime.freeMemory()) / mb);
        System.out.println("Free Memory:" + runtime.freeMemory() / mb);
        System.out.println("Total Memory:" + runtime.totalMemory() / mb);
        System.out.println("Max Memory:" + runtime.maxMemory() / mb);

        ImageReader ir = new ImageReader();
        mlp.recallNetwork("images/img10.bmp", ir.readImage("images/img10.bmp"));

        /*
        hiddenNeurons = 24
        hiddenLayers = 3
        teachingStep = 0.04
        leastMSE = 0.01
        momentum = 0.4
        */
    }
}
