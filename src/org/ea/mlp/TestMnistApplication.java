package org.ea.mlp;

/**
 * Created by woden on 2016-12-30.
 */
public class TestMnistApplication {
    public static void main(String[] arg) {

        MnistReader mr = new MnistReader(
                "mnist/train-labels.idx1-ubyte",
                "mnist/train-images.idx3-ubyte");

        MnistMultiLayerPerceptron mlp = new MnistMultiLayerPerceptron(768, 2);
        if(!mlp.trainNetwork(0.04f, 0.01f, 0.3f, mr, 1000)) {
            System.out.println("There was an error while training ... Quitting\n\r");
            System.exit(0);
        }


        MnistReader mrT10K = new MnistReader(
                "mnist/t10k-labels.idx1-ubyte",
                "mnist/t10k-images.idx3-ubyte");

        int correct = 0;
        for(int i=0; i<mrT10K.size(); i++) {
            try {
                correct += mlp.recallNetwork(mrT10K.readNextImage(), mrT10K.readNextLabel());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        System.out.println(correct + " / " + mrT10K.size());
    }

}
