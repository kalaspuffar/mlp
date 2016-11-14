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

#include <fstream>
#include <sstream>

#include <stdlib.h>

//BMP file header offsets
#define DATAOFFSET      0xA//10 dec
#define DATASIZEINBYTES 0x22//34 dec
#define BMPWIDTH        0x12
#define BMPHEIGHT       0x16
#define BMPBPP          0x1C


#define GOALFILENAME    "goals.txt"



//This will read monochrome bitmap files.
//The format is as it is in the windows .bmp file,
//have to check the corresponding linux one

using namespace std;

class FileReader
{

    private:
        char* imgBuffer;
        //a DWORD
        char* check;
        bool firstImageRead;
        ifstream fs;


        //image stuff
        int width;
        int height;

    public:
        FileReader();
        ~FileReader();
        bool readBitmap(int fileNum);
        //reads the first bitmap file, the one designated with a '0'
        //and gets the dimensions. All other .bmp are assumed with
        //equal and identical dimensions
        int getBitmapDimensions();
        //returns a pointer to integers with all the goals
        //that each bitmap should have. Reads it from a file
        int* getImgGoals();
        //returns a pointer to the currently read data
        char* getImgData();
        //helper function convering bytes to an int
        int bytesToInt(char* bytes,int number);
        //gets machine's endiannes
        int getEndianness();

};
