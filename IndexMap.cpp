//----------------------------------------------------------
// CS260 Assignment Starter Code
// Copyright Andrew Scholer (ascholer@chemeketa.edu)
// Neither this code, nor any works derived from it
//    may not be republished without approval.
//----------------------------------------------------------

#include <functional>
#include <iostream>
#include "IndexMap.h"

using namespace std;

//Construct HashMap with given number of buckets
IndexMap::IndexMap(int startingBuckets){
    numBuckets = startingBuckets;
    buckets = new IndexRecord[numBuckets];
    keyCount = 0;
}

//Destructor
IndexMap::~IndexMap(){
    delete [] buckets;
}

//Copy constructor and assignment operators
IndexMap::IndexMap(const IndexMap &other){

}

IndexMap& IndexMap::operator=(const IndexMap& other){

}


//Returns true if indicated key is in the map
bool IndexMap::contains(const std::string& key) const{
    hash<string> hasher;
    for(unsigned int i = hasher(key) % numBuckets; i < numBuckets; i++){
        if(buckets[i].word == key){
            return true;
        }
    }
    return false;
}

//Add indicated location to the map.
//  If the key does not exist in the map, add an IndexRecord for it
//  If the key does exist, add a Location to its IndexRecord
void IndexMap::add(const std::string& key, int pageNumber, int wordNumber){
    string tempkey = key;
    tempkey.at(0) = tolower(tempkey.at(0));
    if(keyCount > numBuckets * 0.75){
        grow();
    }
    hash<string> hasher;
    unsigned int hashValue = hasher(tempkey) % numBuckets;
    if(this->contains(tempkey)){
        IndexRecord temp = get(tempkey);
        temp.addLocation(IndexLocation(pageNumber, wordNumber));
        unsigned int index = getLocationFor(tempkey);
        buckets[index] = temp;
    }
    else{
        IndexRecord temp(tempkey);
        temp.addLocation(IndexLocation(pageNumber, wordNumber));
        while(buckets[hashValue].word != "?"){
            hashValue++;
            hashValue = hashValue % numBuckets;
        }
        buckets[hashValue] = temp;
        keyCount++;
    }
}

//get number of keys contained in the map
int IndexMap::numKeys() const{
    return keyCount;
}

//print out the contents of the hash table
void IndexMap::print() const{
    for(int i = 0; i < numBuckets; i++){
        if(buckets[i].word != "?"){
            cout << buckets[i];
            cout << endl;
        }
    }
}

//get the IndexRecord associated with a particular word
IndexRecord IndexMap::get(const std::string& word) const{
    hash<string> hasher;
    unsigned int hashValue = hasher(word) % numBuckets;
    while(buckets[hashValue].word != word){
        hashValue++;
    }
    return buckets[hashValue];

}

//find all locations where one word is directly followed on the same page by a second
//  print them to cout
void IndexMap::findWordPairs(const std::string& key1, const std::string& key2) const{
    if(this->contains(key1) && this->contains(key2)){
        IndexRecord firstWord = get(key1);
        IndexRecord secondWord = get(key2);
        cout << "\"" << firstWord.word << " " << secondWord.word << "\" : " << endl;
        for(int i = 0; i < firstWord.locations.size(); i++){
            for(int j = 0; j < secondWord.locations.size(); j++){
                if(firstWord.locations.at(i).pageNum < secondWord.locations.at(j).pageNum){ //this should prevent some unnecessary searches
                    continue;
                }
                if(firstWord.locations.at(i).pageNum == secondWord.locations.at(j).pageNum && firstWord.locations.at(i).wordNum+1 == secondWord.locations.at(j).wordNum){
                    cout << firstWord.locations.at(i).pageNum << "-" <<
                            firstWord.locations.at(i).wordNum << " " <<
                            secondWord.locations.at(j).pageNum << "-" <<
                            secondWord.locations.at(j).wordNum << endl;
                }
            }
        }
    }

}

//returns the first word on the indicated page
std::string IndexMap::firstWordOnPage(int pageNumber) const{
    for(int i = 0; i < numBuckets; i++){
        for(unsigned int j = 0; j < buckets[i].locations.size(); j++){
            if(buckets[i].locations.at(j).pageNum == pageNumber && buckets[i].locations.at(j).wordNum == 1){
                return buckets[i].word;
            }
        }
    }
    return "?";
}



//-----------------------------------
// private functions
//-----------------------------------

//handle resizing the hash table into a new array with twice as many buckets
void IndexMap::grow(){
    keyCount = 0;
    IndexRecord* tempBuckets = buckets;
    int oldNumBuckets = numBuckets;
    numBuckets *= 2;
    buckets = new IndexRecord[numBuckets];
    for(int i = 0; i < oldNumBuckets; i++){
        if(tempBuckets[i].word != "?"){
            for(unsigned int j = 0; j < tempBuckets[i].locations.size(); j++){
                add(tempBuckets[i].word, tempBuckets[i].locations.at(j).pageNum, tempBuckets[i].locations.at(j).wordNum);
            }
        }
    }
    delete [] tempBuckets;
}

//Get the location this key should be placed at.
//  Will either containt IndexRecord with that key or an empty IndexRecord
int IndexMap::getLocationFor(const std::string& key) const{
    hash<string> hasher;
    for(unsigned int i = hasher(key) % numBuckets; i < numBuckets; i++){
        if(buckets[i].word == key){
            return i;
        }
    }
    return -1;
}
