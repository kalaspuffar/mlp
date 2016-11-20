package org.ea.mlp;


import java.util.ArrayList;
import java.util.List;

/**
 * Created by woden on 2016-11-20.
 */
public class MultiLayerPerceptron {
    float[] inputNeurons;
    float[] hiddenNeurons;
    float[] outputNeurons;
    float[] weights;

    //a buffer for the weights
    float[] tempWeights;
    //used to keep the previous weights before modification, for momentum
    float[] prWeights;

    int inputN,outputN,hiddenN,hiddenL;

    public MultiLayerPerceptron(int hiddenL, int hiddenN) {
        outputN = 10; //the 9 possible numbers and zero
        this.hiddenL = hiddenL;
        this.hiddenN = hiddenN;

        int weightsSize = inputN*hiddenN+(hiddenN*hiddenN*(hiddenL-1))+hiddenN*outputN;

        //let's allocate the memory for the weights
        weights = new float[weightsSize];

        //also let's set the size for the neurons vector
        inputNeurons = new float[inputN];
        hiddenNeurons = new float[hiddenN*hiddenL];
        outputNeurons = new float[outputN];

        for(int i = 0; i < weightsSize; i++) {
            weights[i] = (float)(Math.random() - 0.5f);
        }
    }

    //assigns values to the input neurons
    public void populateInput(int[] data) {
        for(int i = 0; i < inputN; i++) {
            if(data[i] > 0) {
                inputNeurons[i] = 1.0f;
            } else {
                inputNeurons[i] = 0.0f;
            }
        }
    }

    //calculates the whole network, from input to output
    public void calculateNetwork() {
        //let's propagate towards the hidden layer
        for(int hidden = 0; hidden < hiddenN; hidden++)
        {
            hiddenNeurons[hidden] = 0; // Layer one neuron.

            for(int input = 0 ; input < inputN; input ++)
            {
                hiddenNeurons[hidden] += inputNeurons[input] * inputToHidden(input,hidden);
            }

            //and finally pass it through the activation function
            hiddenNeurons[hidden] = sigmoid(hiddenNeurons[hidden]);
        }

        //now if we got more than one hidden layers
        for(int i = 2; i <= hiddenL; i ++) {
            //for each one of these extra layers calculate their values
            for(int j = 0; j < hiddenN; j++) { //to
                hiddenNeurons[(i-1)*hiddenN + j] = 0

                for(int k = 0; k <hiddenN; k++) { //from
                    hiddenNeurons[(i-1)*hiddenN + j] += hiddenNeurons[(i-1)*hiddenN + k] * hiddenToHidden(i,k,j);
                }

                //and finally pass it through the activation function
                hiddenNeurons[(i-1)*hiddenN + j] = sigmoid(hiddenNeurons[(i-1)*hiddenN + j]);
            }
        }

        //and now hidden to output
        for(int i =0; i< outputN; i ++) {
            outputNeurons[i] = 0;

            for(int j = 0; j <hiddenN; j++)
            {
                outputNeurons[i] += hiddenNeurons[(hiddenL-1)*hiddenN + j] * hiddenToOutput(j,i);
            }

            //and finally pass it through the activation function
            outputNeurons[i] = sigmoid( outputNeurons[i] );
        }

    }

    //trains the network according to our parameters
    public boolean trainNetwork(float teachingStep,float lmse,float momentum,int trainingFiles) {

    }

    //recalls the network for a given bitmap file
    public void recallNetwork(int fileNum) {

    }

    private float inputToHidden(int inp, int hid) {
        return weights[inputN*hid+inp];
    }
    private float hiddenToHidden(int toLayer, int fromHid, int toHid) {
        return weights[inputN*hiddenN+ ((toLayer-2)*hiddenN*hiddenN)+hiddenN*fromHid+toHid];
    }
    private float hiddenToOutput(int hid, int out)  {
        return weights[(inputN*hiddenN + (hiddenL-1)*hiddenN*hiddenN + hid*outputN+out)];
    }

    /*Helper macros just as above, but for the previous Weights*/
    private float _prev_inputToHidden(int inp, int hid) {
        return prWeights[inputN*hid+inp];
    }
    private float _prev_hiddenToHidden(int toLayer, int fromHid, int toHid) {
        return prWeights[inputN*hiddenN+ ((toLayer-2)*hiddenN*hiddenN)+hiddenN*fromHid+toHid];
    }
    private float _prev_hiddenToOutput(int hid, int out) {
        return prWeights[inputN*hiddenN + (hiddenL-1)*hiddenN*hiddenN + hid*outputN+out];
    }


    /*helper macro to locate the appropriate hidden neuron*/
    private float hiddenAt(int layer,int hid) {
        return hiddenNeurons[(layer-1)*hiddenN + hid];
    }

    /*helper macros to locate the appropriate neuron's delta*/
    private float outputDeltaAt(out) {
        return (*(odelta+out));
    }
    private float hiddenDeltaAt(layer,hid) {
        return (*(hdelta+(layer-1)*hiddenN+hid));
    }

    /*math help*/
    private float sigmoid(float value) {
        return (float)(1f/(1f+Math.exp(-value)));
    }
    private float dersigmoid(float value) {
        return (value*(1f-value))
    }

}
