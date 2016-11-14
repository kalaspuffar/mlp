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
* this is the main file of the tutorial where everything is called.
* The correct usage of the program is to call it with its name.exe
* in Windows, without .exe(of course) in Linux and add 6 arguments.
* These are:
* -Number of Hidden Neurons(int)
* -Number of hidden Layers(int)
* -Number of Bitmap files to read(int)
* -Teaching Step for training(float)
* -least Mean Square error for training(float)
* -momentum for training(float)
*
* @author Lefteris
* @date 28/05/2009
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "neural.h"

#include <string>
#include <stdlib.h>




//checks input stream to see if it's numeric
bool streamIsNumeric(char inp[1024]);

int main(int argc,  const char* argv[] )
{
    int eleos = sizeof(std::string);

    //will hold the console input
    char inp[1024];

    if(argc != 7)
    {
        printf("An incorrect number of arguments has been given\n\r\n\r");
        printf("Usage is: %s hiddenNeurons(int) hiddenLayers(int) numberOfBitmaps(int) teachingStep(float) leastMSE(float) momentum(float) \n\r",argv[0]);

       printf("\n\rPress any key to continue and exit the program \n\r");
       getchar();

        return -1;
    }

    int hiddenNeurons = atoi(argv[1]);
    int hiddenLayers = atoi(argv[2]);
    int noOfBitmaps = atoi(argv[3]);
    float teachingStep = atof(argv[4]);
    float leastMSE = atof(argv[5]);
    float momentum = atof(argv[6]);





    MLP* neuralnet = new MLP(hiddenLayers,hiddenNeurons);


    if(!(neuralnet->trainNetwork(teachingStep,leastMSE,momentum,noOfBitmaps)))
    {
        printf("There was an error while training ... Quitting\n\r");
        printf("\n\rPress any key to continue and exit the program \n\r");
        getchar();
        return -1;
    }



    int number = 5;

    while(number != -1)
    {
        printf("Now that the training is complete, give \n\rthe number of the bitmap you want to test(0 - %d), or -1 to exit\n\r\n\r",noOfBitmaps-1);
        cin.getline(inp,1024);

        //if it's not a number
        if(!streamIsNumeric(inp))
            continue;//ask again


        number = atoi(inp);

        if(number == -1)
            break;

        //if the recalling goal file does not exit
        if(number < 0 || number > (noOfBitmaps-1))
            continue;//ask again

        //if we get here, recall the net
        neuralnet->recallNetwork(number);
    }

    printf("\n\rPress any key to continue and exit the program \n\r");
    getchar();

}

//checks input stream to see if it's numeric
bool streamIsNumeric(char inp[1024])
{
    for(int i = 0; i < 1024; i ++)
    {
        //if it the string terminating character
        if(inp[i] == '\0')
           break;//terminate

        //if it is not a digit, return false
        if(!isdigit(inp[i]))
            return false;

    }

    return true;
}
