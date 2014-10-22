// Helen Chac & Tony Seto.

/*

    Line size = 8
    # of entries = 32

    There are several cases that we have to follow
      1. If it's in the cache, just write/read from it
      2. If it's not in cache, we have to get it from memory
            a. we have to write to memory, then write from memory to cache -> only if it's dirty
            b. if it's not dirty, we just put the tag into the cache. 


*/

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

class Cache{
    
    public:
    int tag;
    char data[8];
    bool dirty;
    Cache():tag(-1), dirty(false)
    {
        for (int i = 0; i < 8; i++) 
            data[i] = 0;
    } // initializes the caches tag as -1 (because it starts at 0 -> 2^8) 
};

void write(Cache line[32], int address, int index, int data){
    //cout << data << endl;
    line[index].data[address & 7] = data;
    line[index].dirty = true;
    
}//write


void read(int hit, bool dirty, Cache line[32], int address, int index){

        cout << setw(2) << hex << uppercase << setfill('0')
        << (((int) (line[index].data[address & (7)])) & 0xFF)
        << ' ' << hit << ' ' << dirty << endl;
}//read


int main(int argc, char** argv){
    const int linesize = 8, entry_size = 32;
    int address, readwrite, data, tag, index, offset;
    bool hit,dirty;
    Cache line[entry_size];
    char memory[0xFFFF];
    ifstream file(argv[1]);
    
    while( file >> hex >> address >> readwrite >> data ){
        index = (address / linesize ) % entry_size;
        tag = ( address ) / entry_size;
        
        if(line[index].tag == tag){
            hit = true;
        }//if there's a hit
        else{
            hit = false;
        }// else
        dirty = line[index].dirty; // taking the case of if it's dirty or not before you go in.
        //cout << dirty << endl;
        //cout << line[index].dirty<< endl;
        if (hit == false){
            if (line[index].dirty){
                for (int i = 0; i < linesize; i++){
                    (&memory[line[index].tag * linesize])[i] = line[index].data[i];
                } // go inside the memory and store what's in the cache into the memory
            }// if there's something in memory
            for (int i = 0; i < linesize; i++){
                line[index].data[i] = (&memory[tag * linesize])[i];
            }
            line[index].tag = tag;
            line[index].dirty = false; // since you replaced it, you now say it's not dirty
        } //if it's not in the cache
        if (readwrite){
            write(line, address, index, data);
            //line[index].data[address & (8-1)] = data;

        }
        else{
//            cout << line[index].data[address & 7] << endl;
/*
        cout << setw(2) << hex << uppercase << setfill('0')
        << (((int) ((line[index]).data[address & (7)])) & 0xFF)
        << ' ' << hit << ' ' << dirty << endl;
  */          

        read(hit, dirty, line, address, index);
        }
        
    }//while
    /*
    cout << "checking for dirty" << endl ; 
    for (int i = 0; i < 32; i++){
        //for (int j = 0; j < 8; j++){
            cout << line[i].dirty << " ";
        //}
        cout << endl;
    }
    */
    /*
    cout << "checking for stuff in cache" << endl;
    for (int i = 0; i < 32; i++){
        
        for (int j = 0; j < 8; j++){
            cout << line[i].data[j] << " " ;
        }
        cout << endl;
        
    }
    */
    file.close(); //closing file
    return 0;
} // main
