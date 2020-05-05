#include <iostream>

using namespace std;

struct cacheValues
{
    int valid = 0;
    int tag = 0;
    int data = 0;
};

struct cacheBlocks //For 2 way set associate cache
{
    cacheValues blockOne;
    cacheValues blockTwo;
    int leastOne = 0; //Keeps track of LRU
    int leastTwo = 0;
};

void printData(int data) //Binary data conversion
{
    int mask = 1;
    mask <<= 31; //Shift left 31 positions

    for(int i = 1; i <= 32; i++)
    {
        if((data & mask) == 0 )
        {
            cout << "0";
        }
        else
        {
            cout << "1";
        }
        data <<= 1; //Shift left 1
    }
   // cout << endl;
}

void printTag(int data) //Binary tag conversion
{
    int mask = 1;
    mask <<= 3; //Shift left 31 positions

    for(int i = 1; i <= 4; i++)
    {
        if((data & mask) == 0 )
        {
            cout << "0";
        }
        else
        {
            cout << "1";
        }
        data <<= 1; //Shift left 1
    }
   // cout << endl;
}

int main()
{
    int mainMemory[64];
    cacheValues cacheTable[16];
    cacheBlocks twoWayTable[8];
    int sequence[16] = {1,4,8,5,20,17,19,56,9,11,4,43,5,6,9,17};


    cout << "Main Memory content " <<  endl;
    for(int i = 0; i < 64; i++) //Initializing main memory content
    {
        mainMemory[i] = i + 5;
        cout << "Index " << i << ": " << mainMemory[i] << " ";
        cout << endl;
    }

    cout << endl;
    cout << endl;

    cout << "Sequence array" << endl;
    for(int i = 0; i < 16; i++)
    {
        cout << sequence[i] << " ";
    }

    cout << endl;
    cout << endl;


    cout << "Cache Table Direct Mapping" << endl;
    for(int i = 0; i < 16; i++)
    {
        int modResult, divResult;

        modResult = sequence[i] % 16;
        divResult = sequence[i] / 16;

        cacheTable[modResult].data = mainMemory[sequence[i]];
        cacheTable[modResult].tag = divResult;
        cacheTable[modResult].valid = 1;

    }


    for(int i = 0; i < 16; i++) //Display cache content
    {
        cout << "Index: " << i << " Data: ";
        printData(cacheTable[i].data);
        cout << "   Tag: ";
        printTag(cacheTable[i].tag);
        cout << "   Valid: " << cacheTable[i].valid << endl;

    }


    cout << endl;
    cout << "2 way set associative cache" << endl;
    for(int i = 0; i < 16; i++)
    {
        int modR, divR;
        modR = sequence[i] % 8;
        divR = sequence[i] / 8;

        if(twoWayTable[modR].blockOne.valid == 0 && twoWayTable[modR].blockTwo.valid == 0) //If both blocks are empty then fill block one first
        {
            twoWayTable[modR].blockOne.data = mainMemory[sequence[i]];
            twoWayTable[modR].blockOne.tag = divR;
            twoWayTable[modR].blockOne.valid = 1;
        }

        else if(twoWayTable[modR].blockOne.valid == 1 && twoWayTable[modR].blockTwo.valid == 0) //If first block is filled then fill second block
        {
            if(twoWayTable[modR].blockOne.data == mainMemory[sequence[i]]) //Data is already in cache do not need to fill or replace
            {
               twoWayTable[modR].leastOne = 0;
               twoWayTable[modR].leastTwo = 0;
            }
            else //If not then fill block two
            {
                twoWayTable[modR].blockTwo.data = mainMemory[sequence[i]];
                twoWayTable[modR].blockTwo.tag = divR;
                twoWayTable[modR].blockTwo.valid = 1;
                twoWayTable[modR].leastOne = 1; //LRU is block one
                twoWayTable[modR].leastTwo = 0;
            }

        }
        else //Both blocks are filled so replace least recently used
        {
            if(twoWayTable[modR].blockOne.data == mainMemory[sequence[i]]) //Data is already in cache do not need to fill or replace
            {
               twoWayTable[modR].leastOne = 0;
               twoWayTable[modR].leastTwo = 1;
            }
            else if(twoWayTable[modR].blockTwo.data == mainMemory[sequence[i]]) //Data is already in cache do not need to fill or replace
            {
               twoWayTable[modR].leastOne = 1;
               twoWayTable[modR].leastTwo = 0;
            }

            else if(twoWayTable[modR].leastOne == 1) //Data is not in cache so Replace LRU
            {
                 twoWayTable[modR].blockOne.data = mainMemory[sequence[i]];
                 twoWayTable[modR].blockOne.tag = divR;
                 twoWayTable[modR].blockOne.valid = 1;
                 twoWayTable[modR].leastOne = 0;
                 twoWayTable[modR].leastTwo = 1; //LRU is now block two
            }
            else //Replace block two
            {
                twoWayTable[modR].blockTwo.data = mainMemory[sequence[i]];
                twoWayTable[modR].blockTwo.tag = divR;
                twoWayTable[modR].blockTwo.valid = 1;
                twoWayTable[modR].leastOne = 1; //LRU is now block one
                twoWayTable[modR].leastTwo = 0;
            }
        }

    }


     for(int i = 0; i < 8; i++) //Display cache content
    {
        cout << "Index: " << i << " Data: ";
        printData(twoWayTable[i].blockOne.data);
        cout << "   Tag: ";
        printTag(twoWayTable[i].blockOne.tag);
        cout << "   Valid: " << twoWayTable[i].blockOne.valid << endl;

        cout << "Index: " << i << " Data: ";
        printData(twoWayTable[i].blockTwo.data);
        cout << "   Tag: ";
        printTag(twoWayTable[i].blockTwo.tag);
        cout << "   Valid: " << twoWayTable[i].blockTwo.valid << endl;

    }

    cout << endl;
    return 0;
}
