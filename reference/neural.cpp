/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Multi-layer perceptron tutorial
* A practical application of an MLP in optical character
* recognition.
* written by Lefteris of Real Intelligence Project
*
* you can email for questions/or suggestions
* at :  lefteris@realintelligence.net
* or:   lefkar@msn.com
*
* This is the code for the MLP neural network. At the moment it
* has not been compiled in Linux but it should work fine apart from
* the fact that pdCurses library is not used, for console input to
* make up for windows and <conio.h>. It is in the TODO list.
*
* @author Lefteris
* @date 28/05/2009
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "Neural.h"

//Multi-layer perceptron constructor
MLP::MLP(int hL,int hN)
{
    //initialize the filereader
    reader = new FileReader();


    outputN = 10; //the 9 possible numbers and zero
    hiddenL = hL;
    hiddenN = hN;

    //initialize the filereader
    reader = new FileReader();

    //read the first image to see what kind of input will our net have
    inputN = reader->getBitmapDimensions();

    if(inputN == -1)
    {
        printf("There was an error detecting img0.bmp\n\r");
        return ;
    }



    //let's allocate the memory for the weights
    weights.reserve(inputN*hiddenN+(hiddenN*hiddenN*(hiddenL-1))+hiddenN*outputN);

    //also let's set the size for the neurons vector
    inputNeurons.resize(inputN);
    hiddenNeurons.resize(hiddenN*hiddenL);
    outputNeurons.resize(outputN);


    //randomize weights for inputs to 1st hidden layer
    for(int i = 0; i < inputN*hiddenN; i++)
    {
        weights.push_back( (( (float)rand() / ((float)(RAND_MAX)+(float)(1)) )) - 0.5 );//[-0.5,0.5]

    }

    //if there are more than 1 hidden layers, randomize their weights
    for(int i=1; i < hiddenL; i++)
    {
        for(int j = 0; j <  hiddenN*hiddenN; j++)
        {
            weights.push_back( (( (float)rand() / ((float)(RAND_MAX)+(float)(1)) )) - 0.5   );//[-0.5,0.5]
        }
    }

    //and finally randomize the weights for the output layer
    for(int i = 0; i < hiddenN*outputN; i ++)
    {
         weights.push_back( (( (float)rand() / ((float)(RAND_MAX)+(float)(1)) )) - 0.5   );//[-0.5,0.5]
    }

}



//destructor
MLP::~MLP()
{
    delete reader;

    weights.clear();
    inputNeurons.clear();
    hiddenNeurons.clear();
    outputNeurons.clear();
}

void MLP::calculateNetwork()
{
    //let's propagate towards the hidden layer
    for(int hidden = 0; hidden < hiddenN; hidden++)
    {
        hiddenAt(1,hidden) = 0;

        for(int input = 0 ; input < inputN; input ++)
        {
            hiddenAt(1,hidden) += inputNeurons.at(input)*inputToHidden(input,hidden);
        }

        //and finally pass it through the activation function
        hiddenAt(1,hidden) = sigmoid(hiddenAt(1,hidden));
    }



    //now if we got more than one hidden layers
    for(int i = 2; i <= hiddenL; i ++)
    {

        //for each one of these extra layers calculate their values
        for(int j = 0; j < hiddenN; j++)//to
        {
            hiddenAt(i,j) = 0;

            for(int k = 0; k <hiddenN; k++)//from
            {
               hiddenAt(i,j) += hiddenAt(i-1,k)*hiddenToHidden(i,k,j);
            }

            //and finally pass it through the activation function
            hiddenAt(i,j) = sigmoid(hiddenAt(i,j));
        }
    }

    int i;
    //and now hidden to output
    for(i =0; i< outputN; i ++)
    {
        outputNeurons.at(i) = 0;


        for(int j = 0; j <hiddenN; j++)
        {
           outputNeurons.at(i) += hiddenAt(hiddenL,j) * hiddenToOutput(j,i);
        }

         //and finally pass it through the activation function
         outputNeurons.at(i) = sigmoid( outputNeurons.at(i) );
    }



}

//assigns values to the input neurons
bool MLP::populateInput(int fileNum)
{
    char* data;
    //let's read the image file
    if(!reader->readBitmap(fileNum))
        return false;

    //get its data
    data = reader->getImgData();

    for(int i = 0; i < inputN; i ++)
    {
        //if the specific pixel is on
        //set the corresponding neuron
        if(data[i/8] & (0x01<<(7-(i%8))))
            inputNeurons.at(i) = 1.0;
        else
            inputNeurons.at(i) = 0.0;//the neuron is off
    }

    return true;
}


//trains the network according to our parameters
bool MLP::trainNetwork(float teachingStep,float lmse,float momentum,int trainingFiles)
{

    float mse = 999.0;
    int tCounter = 0;
    int epochs = 1;
    float error = 0.0;
    //the delta of the output layer
    float* odelta = (float*) malloc(sizeof(float)*outputN);
    //the delta of each hidden layer
    float* hdelta = (float*)  malloc(sizeof(float)*hiddenN*hiddenL);

    //a buffer for the weights
    std::vector<float> tempWeights = weights;
    //used to keep the previous weights before modification, for momentum
    std::vector<float> prWeights = weights;

    //let's get the goals for each image from the file
    //since it is malloced by the reader, we have the repsponsibility
    //of freeing its memory after training
    int* goals = reader->getImgGoals();


    //First let's do a check if the goals number
    //is the same as the training files the user gave
    if((*goals) != trainingFiles)
    {
        printf("The number of goals in the goals file doesn't\
        match the number of training files that were given\n\r");

        return false;
    }






//For as long as the mse is less than the least mean square error
#ifdef _WIN32
    while(fabs(mse-lmse) > 0.0001  && !_kbhit() )
    {
#else//LINUX , TO DO : Use pdCurses library to do the same thing I accomplish with <conio.h> and _kbhit()
    //until then we will be stopping to ask if you want to continue every 400 epochs
     while(fabs(mse-lmse) > 0.0001)
    {
#endif

        //for each epoch reset the mean square error
        mse = 0.0;

        //for each file
        while(tCounter < trainingFiles)
        {
            //first populate the input neurons
           if(!populateInput(tCounter))
           {
               printf("An error has been encountered while reading a bitmap file \n\r");
                return false;
           }


            //then calculate the network
            calculateNetwork();

            //let's get the value of this particular goal
            int target = *(goals+tCounter+1);

            //Now we have calculated the network for this iteration
            //let's back-propagate following the back-propagation algorithm


            for(int i = 0; i < outputN; i ++)
            {
                //let's get the delta of the output layer
                //and the accumulated error
                if(i != target)
                {
                     outputDeltaAt(i) = (0.0 - outputNeurons[i])*dersigmoid(outputNeurons[i]);
                     error += (0.0 - outputNeurons[i])*(0.0-outputNeurons[i]);
                }
                else
                {
                     outputDeltaAt(i) = (1.0 - outputNeurons[i])*dersigmoid(outputNeurons[i]);
                     error += (1.0 - outputNeurons[i])*(1.0-outputNeurons[i]);
                }

            }


            //we start popagating backwards now, to get the error of each neuron
            //in every layer

            //let's get the delta of the last hidden layer first
            for(int i = 0; i < hiddenN; i++)
            {
                hiddenDeltaAt(hiddenL,i) = 0;//zero the values from the previous iteration

                //add to the delta for each connection with an output neuron
                for(int j = 0; j < outputN; j ++)
                {
                    hiddenDeltaAt(hiddenL,i) += outputDeltaAt(j) * hiddenToOutput(i,j) ;
                }

               //The derivative here is only because of the
                //delta rule weight adjustment about to follow
                hiddenDeltaAt(hiddenL,i) *= dersigmoid(hiddenAt(hiddenL,i));
            }



            //now for each additional hidden layer, provided they exist
            for(int i = hiddenL-1; i >0; i--)
            {
                //add to each neuron's hidden delta
                for(int j = 0; j < hiddenN; j ++)//from
                {

                    hiddenDeltaAt(i,j) = 0;//zero the values from the previous iteration

                    for(int k = 0; k <hiddenN; k++)//to
                    {
                        //the previous hidden layers delta multiplied by the weights
                        //for each neuron
                       hiddenDeltaAt(i,j) +=  hiddenDeltaAt(i+1,k) * hiddenToHidden(i+1,j,k);
                    }

                     //The derivative here is only because of the
                    //delta rule weight adjustment about to follow
                    hiddenDeltaAt(i,j) *= dersigmoid(hiddenAt(i,j));
                }
            }


            //Weights modification
            tempWeights = weights;//keep the previous weights somewhere, we will need them


            //hidden to Input weights
            for(int i = 0; i < inputN; i ++)
            {
                for(int j = 0; j < hiddenN; j ++)
                {
                    inputToHidden(i,j) +=   momentum*(inputToHidden(i,j) - _prev_inputToHidden(i,j)) +
                                            teachingStep* hiddenDeltaAt(1,j) * inputNeurons[i];
                }
            }



            //hidden to hidden weights, provided more than 1 layer exists
            for(int i = 2; i <=hiddenL; i++)
            {

                for(int j = 0; j < hiddenN; j ++)//from
                {
                    for(int k =0; k < hiddenN; k ++)//to
                    {
                        hiddenToHidden(i,j,k) += momentum*(hiddenToHidden(i,j,k) - _prev_hiddenToHidden(i,j,k)) +
                                                 teachingStep * hiddenDeltaAt(i,k) * hiddenAt(i-1,j);
                    }
                }
            }

             //last hidden layer to output weights
            for(int i = 0; i < outputN; i++)
            {
                for(int j = 0; j < hiddenN; j ++)
                {
                    hiddenToOutput(j,i) += momentum*(hiddenToOutput(j,i) - _prev_hiddenToOutput(j,i)) +
                                           teachingStep * outputDeltaAt(i) * hiddenAt(hiddenL,j);
                }
            }

            prWeights = tempWeights;


            //add to the total mse for this epoch
            mse += error/(outputN+1);
            //zero out the error for the next iteration
            error = 0;


            tCounter++;

        }

        //reset the counter
        tCounter = 0;
#ifdef _WIN32
        printf("Press any key to stop training,\n\r");
#else
        char reply;
        if((epochs%1000) == 0)
        {
            printf("We are at epoch %d , would you like to continue training? (N for no, any other key to continue)\n\r",epochs);
            cin>>reply;
        }

        if(reply == 'N')
            break;

        printf("You will be prompted every 400 epochs if you want to continue training\n\r");

#endif

        printf("Mean square error for epoch %d is:  %.4f \n\r",epochs,mse);

        epochs++;


    }

    printf("\n\r");

    //free the goals array
    //free(goals);
    //free the deltas
   // free(odelta);
    //free(hdelta);

    return true;
}

void MLP::recallNetwork(int fileNum)
{
    //first populate the input neurons
    populateInput(fileNum);

    //then calculate the network
    calculateNetwork();

    float winner = -1;
    int index = 0;

    //find the best fitting output
    for(int i =0; i < outputN; i++)
    {
        if(outputNeurons[i] > winner)
        {
            winner = outputNeurons[i];
            index = i;
        }
    }

    //output it
    printf("The neural network thinks that image %d represents a \n\r\n\r \t\t----->| %d |<------\t\t \n\r\n\r",fileNum,index);

    //now let's the exact percentages of what it thnks
    printf("Analyzing what the network thinks about bitmap %d we see that it thinks it is: \n\r\
          | 0 with %d%% probability |\n\r\
          | 1 with %d%% probability |\n\r\
          | 2 with %d%% probability |\n\r\
          | 3 with %d%% probability |\n\r\
          | 4 with %d%% probability |\n\r\
          | 5 with %d%% probability |\n\r\
          | 6 with %d%% probability |\n\r\
          | 7 with %d%% probability |\n\r\
          | 8 with %d%% probability |\n\r\
          | 9 with %d%% probability |\n\r\n\r",fileNum,(int)(outputNeurons[0]*100),\
           (int)(outputNeurons[1]*100),(int)(outputNeurons[2]*100),(int)(outputNeurons[3]*100),\
           (int)(outputNeurons[4]*100),(int)(outputNeurons[5]*100),(int)(outputNeurons[6]*100),\
           (int)(outputNeurons[7]*100),(int)(outputNeurons[8]*100),(int)(outputNeurons[9]*100));

}

