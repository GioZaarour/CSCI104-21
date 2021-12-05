#include <vector>
#include <iostream>

template <class T>
struct object {
    std::string key;
    T value;
};

template <class T>
class Hashtable {

private:
    unsigned int size;
    bool debug; 
	int numObjects; //how many objects in the table so far
	double loadFactor; 

    //dynamically allocated so we can choose and change the size later/during runtime
    object* hashTable;

    void resize () {
        //should resize the array, rehash the contents, and set a new size, numObjects, and loadFactor

        
    }

    int hash (std::string k) const {

    }

public:
    //base constructor, set debug to false and size to 11
    Hashtable () {
        size = 11;
        debug = false;

        //array of "object" structs
        //initialize with nullptr
        hashTable = new object[size](nullptr);
    }

    //override constructor 1
    Hashtable (bool debug) {
        this->debug = debug;
        size = 11;

        hashTable = new object*[size](nullptr);
    }

    /*override constructor 2
    Hashtable (bool debug, unsigned int size) {
        this->debug = debug;
        this->size = size;

        hashTable = new object[size];
    } */

    ~Hashtable () {
        delete [] hashTable;
    }

    /*If k is already in the Hashtable, then do nothing. If it is new, add it to the Hashtable with the 
    specified value. Returns the number of probes required to place k (if no probes are required, return 0). */
    int add (std::string k, const T& val) {

        //first make sure a new insert won't increase load factor above 0.5
        loadFactor = (numObjects+1) / (double)size;
        if (loadFactor > 0.5) {
            resize();
        }

        //use hash to find index it belongs in
        int index = hash(k);

        //if k is new and index is unoccupied
        if (hashTable[index] == nullptr) {
            object newObj;
            newObj.key = k;
            newObj.value = val;

            hashTable[index] = newObj;

            return 0;
        }
        //if k is already in the Hashtable, then do nothing.
        else if (k == hashTable[index].key) {
            return 0;
        }
        //if the index is occupied by something else
        else {
            //quadratic probing
            //try index+1, index+4, index+9, and so on
            int i = 1;
            
            //while the current index isn't fillable and isn't already K
            while (hashTable[index].key != k && hashTable[index] != nullptr) {
                //if the NEXT quadratic index IS fillable
                if(hashTable[ (index+(i*i)) % size ] == nullptr) {
                    object newObj;
                    newObj.key = k;
                    newObj.value = val;
                    
                    hashTable[ (index+(i*i)) % size ] = newObj;

                    return i;
                }
                //or if the next quadratic index is already k
                else if (hashTable[ (index+(i*i)) % size ].key ==k) {
                    return i;
                }

                i++;
            }
        }

    }

    /*Returns the value associated with k. Don’t crash if k is not in the Hashtable, but you can return whatever
    T value you like (possibly something else in the Hashtable, or a garbage value). */
    const T& lookup (std::string k) {

        int index = hash(k);

        if (hashTable[index].key == k) {
            return hashTable[index].value;
        }
        //iterate through the secondary clump
        for (int i = 1; i <= size; i++) {
            if ( hashTable[ (index+(i*i)) % size ].key == k ) {
                return hashTable[ (index+(i*i)) % size].value;
            }
        }

        //if nothing found
        return nullptr;

    }

    void reportAll (ostream & os) const {

        for(int i = 0; i < size; i++){
            if(hashTable[i] != nullptr){
                os << hashTable[i].key << " " << hashTable[i].value << endl;
            }
        }
        //NOTE: DEFINE << OPERATOR FOR TYPE T

    }

};