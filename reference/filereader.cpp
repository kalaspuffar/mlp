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
* This is the class that read the bitmap files. They need to be in
* the same directory as the program's executable and be named 'imgX.bmp'
* where X is the image number. Moreover there should exist a goals.txt
* file in there which will have a number in each line, depicting the
* corresponding desired output for each bitmap. You can change the goal's
* filename by changing the macro below should you wish to.
*
* @author Lefteris
* @date 28/05/2009
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "filereader.h"

FileReader::FileReader()
{
    //a DWORD
    check = (char*) malloc(sizeof(char)*4);
    firstImageRead = true;

}


FileReader::~FileReader()
{
    free(imgBuffer);
    free(check);
}


//Reads a bitmap file, and stores actual data
//returns false if it's not our type of bitmap
bool FileReader::readBitmap(int fileNum)
{

    int dataOffset;
    int dataSize;
    string fileName;



    //used for quick int to string conversion
    ostringstream stringstream;

    stringstream<<fileNum;

    string fileNumber = stringstream.str();


    fileName = "img"+fileNumber+".bmp";



    fs.open(fileName.c_str(),ios::in|ios::binary);



    fs.read(check,2);

    //checking if it is a bitmap file
    if(*check != 'B' && *(check+1) != 'M')
    {
        printf("Image %d is not a bitmap file \n\r",fileNum);
        return false;
    }

    fs.seekg(BMPBPP);
    fs.read(check,1);

    //if it is not a monochrome bitmap
    if(((int)(*check)) != 1)
    {
        printf("Image %d is not a monochrome bitmap\n\r",fileNum);
        return false;
    }

    //get the dataoffset
    fs.seekg(DATAOFFSET);
    fs.read(check,4);

    dataOffset = bytesToInt(check,4);

    //get the data size in bytes
    fs.seekg(DATASIZEINBYTES);
    fs.read(check,4);

    dataSize = bytesToInt(check,4);

    //if this is the first image we read
    if(firstImageRead)
    {
        //allocate the image buffer
        imgBuffer = (char*) malloc(dataSize);

        //and make sure it does not get re-allocated
        firstImageRead = false;
    }
    else
    {
        fs.seekg(BMPWIDTH);
        fs.read(check,2);

        if(bytesToInt(check,2) != width)
        {
            printf("Image %d width does not match initalising image's width\n\r",fileNum);
            return false;
        }

        fs.seekg(BMPHEIGHT);
        fs.read(check,2);

        if(bytesToInt(check,2) != height)
        {
            printf("Image %d height does not match initalising image's height\n\r",fileNum);
            return false;
        }
    }


    //get the actual bitmap data

    //go to where it is
    fs.seekg(dataOffset);
    //and get it
    fs.read(imgBuffer,dataSize);


    fs.close();
    return true;

}

//reads the first bitmap file, the one designated with a '0'
//and gets each dimensions. All other .bmp are assumed with
//equal and identical dimensions
int FileReader::getBitmapDimensions()
{

    ifstream stream("img0.bmp",ios::in|ios::binary);


    stream.read(check,2);

    //checking if it is a bitmap file
     //checking if it is a bitmap file
    if(*check != 'B' && *(check+1) != 'M')
        return -1;

    //now go and get its width from the header
    stream.seekg(BMPWIDTH);
    stream.read(check,2);

    width = bytesToInt(check,2);

    //and finally its height
    stream.seekg(BMPHEIGHT);
    stream.read(check,2);

    height = bytesToInt(check,2);

    stream.close();

    return width*height;
}


 //returns a pointer to the currently read data
char* FileReader::getImgData()
{
    return imgBuffer;
}


//returns a pointer to integers with all the goals
//that each bitmap should have
int* FileReader::getImgGoals()
{
    ifstream stream(GOALFILENAME);

    char number;
    int lineCount = 0;


    int n;
    //first let's get the number of lines, hence the number of goals
    while(!stream.eof())
    {
        stream>>n;
        lineCount++;
    }

    stream.close();

    //allocate the ints for the goals
    int* imgGoals = (int*) malloc(sizeof(int)*(lineCount+1));


    ifstream stream2(GOALFILENAME);


    //the size of the int array will be
    //the first int in the pointer
    *imgGoals = lineCount;
    lineCount = 1;
    //keep reading until we find the end of file or lineCount
    //surpasses the number of goals in the file
    while(!stream2.eof()  && lineCount < *(imgGoals))
    {
        stream2>> *(imgGoals+lineCount);

        lineCount++;
    }


    stream2.close();
    return imgGoals;

}


//helper function convering bytes to an int
int FileReader::bytesToInt(char* bytes,int number)
{
    int n;
    if(number == 4)
    {
        n = (int)( *(bytes+3) << 24 | *(bytes+2) << 16 | *(bytes+1) << 8 | *bytes);
    }
    else if(number == 2)
    {
         n = (int)( *(bytes+1) << 8 | *bytes);
    }
    else
        return -1;

    return n;
}


