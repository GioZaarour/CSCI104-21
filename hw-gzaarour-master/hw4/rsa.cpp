#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

//note: encryption currently only works for single words without spaces
//example of a way to test: ./rsa 1217 1223 and pass 1488391 as the n for encryption

//declare and set e
const int e = 65537;

//letters array that will help us parse the unencrypted message inputted by the user
const char letters[27] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

//digit strings 00-26 representing each element from letters[]. what we will concatenate to M in the encryption function
const std::string digits[27] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26"};

//function prototypes
void encrypt(std::string, long, std::string);
long ModularExponentiation(long, long, long);
void decrypt();
int decryptionKey(int, int);

int main (int argc, char* argv[]) {

    int p;
    int q;

    //checking command line parameters
    try{
        if (argc < 3){
            throw std::invalid_argument("Error: Must run with two paramaters (two large prime numbers)");
        }

        //set our p and q values
        p = atoi(argv[1]);
        q = atoi(argv[2]);

    }
    catch(const std::invalid_argument& ex) {

        std::cout << std::endl << ex.what() << std::endl;

        return 0;
    }

    //declare and set our n value as a product of p and q
    long trueN = p*q;

    //command menu
    std::string command = "";
    do {
        
        /*note: encrypt should work with the n given in command line, and decrypt should work with the n 
        calculated above as a product of p and q. This way anyone can encrypt a message if they have 
        someone's public key n, but only the person running the program with p and q can decrypt */
        std::cout << "Enter command EXIT, DECRYPT [input] [output], or ENCRYPT [filename] [n] [message]" 
        << std::endl << "Note: Case sensitive" << std::endl;

        std::cin >> command;

        if (command == "ENCRYPT") {
            std::string filename;
            long inputN; //whatever n the encryptor wants to use (not necissarily the correct n of the program)
            std::string message;
            std::string addMessage = ""; //to add more words to the message if needed

            std::cin >> filename;
            std::cin >> inputN;
            std::cin >> message; //takes only one word but works for everything
            //std::getline (std::cin,message); //takes multiple words but sometimes doesn't work for certain inputs

            encrypt(filename, inputN, message);
        }
        else if (command == "DECRYPT") {

        }


    } while (command != "EXIT");

}

//encrypt
void encrypt(std::string filename, long n, std::string message){

    try {
        if(n < 27){
            throw std::invalid_argument("Error: N must be greater than 27");
        }

        int x = 1 + ( log( n / (double)27 ) / log(100) ) ;

        //where the encryption magic happens
        while (!message.empty()) {
            /* M is the string we will populate with digits corresponding to letters from the message. 
            On every loop we want it to reset to empty string since we are outputting the encryption to the file
            in increments of size x, then erasing those x characters from message*/
            std::string M = "";

            /*If there are less than x characters remaining in message, populate the rest of the string with 
            whitespace so we have a string of length x*/
            if (message.length() < x) {
                for (int i=0; i<(x-message.length()); i++) {
                    message += " ";
                }

                message += '\0';
            }

            /* for every character in message that we want to encrypt in this iteration (i.e. the first x chars),
            we want to see what letter it is from letters[] and then append the corresponding digits[] value to 
            our digit string M*/
            for(int j=0; j<x; j++) {
                for(int i=0; i<27; i++) {
                    if ( tolower(message.at(j)) == letters[i]) {
                        M += digits[i];
                    }
                }
            }

            //erase the first x chars from message since we just finished parsing them
            message.erase(0, x);

            //turn digit string M into a long, numM
            std::stringstream readM(M);
            long numM;
            readM >> numM;

            //use modular exponantiation to calculate C = M^e mod n
            long C = ModularExponentiation(numM, (long)e, n);

            //output the encrypted value C to output file
            std::ofstream encryptFile;
            encryptFile.open(filename.c_str());
            encryptFile << C << " ";
        }
    }
    catch (const std::invalid_argument& ex){
        std::cout << std::endl << ex.what() << std::endl;

        exit(EXIT_FAILURE);
     }

}

/*void decrypt(){

}

int decryptionKey(int p, int q){

} */

//function to calculate M^e mod n
//also to calculate C^d mod n
long ModularExponentiation (long M, long exponent, long n){

    long x = 1;
    long power = M % n;
    for (int i = 0; i < 16; i++)
    {
        int mask =  1 << i;
        int masked_e = exponent & mask;
        int bit = masked_e >> i;

        if(bit == 1){
            x = (x*power) % n;
        }
        power = power *power;
    }

    return x;

}