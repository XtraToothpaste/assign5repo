//----------------------------------------------------------
// CS260 Assignment Starter Code
// Copyright Andrew Scholer (ascholer@chemeketa.edu)
// Neither this code, nor any works derived from it
//    may not be republished without approval.
//----------------------------------------------------------
#include <iostream>
#include <fstream>
#include <ctime>

#include "IndexRecord.h"
#include "IndexMap.h"

using namespace std;

int main(){

    cout << "------------------------Section 3------------------------------" << endl;



//    IndexMap test(10);
//    test.add("banana", 0, 1);
//    test.add("Banana", 2, 3);
//    test.print();


    int pageNum = 1;
    int wordNum = 1;
    int x = 0;
    IndexMap clusterFudgeOfWords(4);
    ifstream myFile ("GreatExpectations.txt");
    if (myFile.is_open()){
        string line;
        clock_t start = clock();
        while(myFile >> line){

            if(line == "----------------------------------------"){
                pageNum++;
                wordNum = 1;
            }
            else {
                clusterFudgeOfWords.add(line, pageNum, wordNum);
                wordNum++;
            }
        }
        clock_t end = clock() - start;
        double seconds = 1.0 * end / CLOCKS_PER_SEC;
        cout << seconds << ": Seconds to build the index" << endl;
        cout << clusterFudgeOfWords.numKeys() << ": Number of keys stored in the index (number of unique words in book)" << endl;
        IndexRecord fathersRecord = clusterFudgeOfWords.get("fathers");
        cout << fathersRecord << endl;
        //clusterFudgeOfWords.print();


        cout << "------------------------Section 4------------------------------" << endl;

        cout << "Word Pairs: " << endl;

        clusterFudgeOfWords.findWordPairs("great", "expectations");

        cout << "------------------------Section 5------------------------------" << endl;

        cout << clusterFudgeOfWords.firstWordOnPage(100) << ": First word on page 100" << endl;

    }
    else
        cout << "Unable to open file";
}
