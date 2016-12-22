package org.ea.mlp;

import java.util.*;

public class TestApplication {
    static Map<String, Integer> imageMap = new HashMap<String, Integer>();

    public static void main(String[] arg) {
/*
        ImageReader ir2 = new ImageReader();
        int[] array = ir2.readImage("images/img0.bmp");

        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {
                System.out.print(array[i*8+j]);
                System.out.print(",");
            }
            System.out.println();
        }

        System.exit(0);
*/

        imageMap.put("images/img00.bmp", 1);
        imageMap.put("images/img01.bmp", 2);
        imageMap.put("images/img02.bmp", 7);
        imageMap.put("images/img03.bmp", 3);
        imageMap.put("images/img04.bmp", 4);
        imageMap.put("images/img05.bmp", 5);
        imageMap.put("images/img06.bmp", 6);
        imageMap.put("images/img07.bmp", 7);
        imageMap.put("images/img08.bmp", 8);
        imageMap.put("images/img09.bmp", 9);
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

        /*
        hiddenNeurons = 24
        hiddenLayers = 3
        teachingStep = 0.04
        leastMSE = 0.01
        momentum = 0.4
        */

        List<String> keys = new ArrayList(imageMap.keySet());
        Collections.sort(keys);

        int correct = 0;

        /*
        for(String key : keys) {

            int answer = imageMap.get(key);
            imageMap.remove(key);

            MultiLayerPerceptron mlp = new MultiLayerPerceptron(24, 3);
            if(!mlp.trainNetwork(0.04f, 0.01f, 0.4f, imageMap, 10000)) {
                System.out.println("There was an error while training ... Quitting\n\r");
                System.exit(0);
            }
            ImageReader ir = new ImageReader();
            correct += mlp.recallNetwork(key, ir.readImage(key), answer);

            imageMap.put(key, answer);
        }
        */


        MultiLayerPerceptron mlp = new MultiLayerPerceptron(24, 3);
        if(!mlp.trainNetwork(0.04f, 0.01f, 0.4f, imageMap, 10000)) {
            System.out.println("There was an error while training ... Quitting\n\r");
            System.exit(0);
        }

        mlp.save("test.data");

        MultiLayerPerceptron mlp2 = new MultiLayerPerceptron(24, 3);
        mlp2.load("test.data");

        for(String key : keys) {
            int answer = imageMap.get(key);
            ImageReader ir = new ImageReader();
            correct += mlp2.recallNetwork(key, ir.readImage(key), answer);
        }
        System.out.println(correct + " / 22");

/*
        int mb = 1024*1024;
        Runtime runtime = Runtime.getRuntime();
        System.out.println("##### Heap utilization statistics [MB] #####");
        System.out.println("Used Memory:" + (runtime.totalMemory() - runtime.freeMemory()) / mb);
        System.out.println("Free Memory:" + runtime.freeMemory() / mb);
        System.out.println("Total Memory:" + runtime.totalMemory() / mb);
        System.out.println("Max Memory:" + runtime.maxMemory() / mb);
*/
    }
}
