#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "logger.h"

#define ENCODE_OUTPUT "encoded.txt"
#define DECODE_OUTPUT "decoded.txt"

int progress;
int inputSize;

char base46_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

char* base64_encode(char* plain);
char* base64_decode(char* cipher);
char*(*generateOutput)(void *);

void sigHandler(int sig){
	if(sig == SIGINT){
		warnf("Process was interrupted at %.2f%%\n",(progress * 100.0) / inputSize);
		exit(7);
	} else if(sig == SIGUSR1){
		infof("Current progress is %.2f%%\n",(progress * 100.0) / inputSize);
	}
}

int main(int argc, char **argv){

	progress = 0;
	inputSize = 1;

	if(signal(SIGUSR1,sigHandler) == SIG_ERR){
		warnf("Could not catch SIGUSR1 signal\n");
	}

	if(signal(SIGINT,sigHandler) == SIG_ERR){
		warnf("Could not catch SIGINT signal\n");
	}

	if(argc < 3){
		errorf("Error. Correct usage: ./base64 <--encode | --decode> <file>\n");
		return 1;
	} else if(strcmp(argv[1],"--encode") * strcmp(argv[1],"--decode") != 0) {
		errorf("Error. Flag was not recognized\n");
		return 2;
	}

	char *filename = argv[2];
	int inputFileDesc = open(filename, O_RDONLY);
	if(inputFileDesc < 0){
		errorf("Error. Something went wrong while opening the input file\n");
		return 3;
	}

	char *outputF;

	if(strcmp(argv[1],"--encode") == 0){
		outputF = ENCODE_OUTPUT;
		generateOutput = (char*(*)(void *))(&base64_encode);
	} else{
		outputF = DECODE_OUTPUT;
		generateOutput = (char*(*)(void *))(&base64_decode);
	}

	int outputFileDesc = open(outputF, O_WRONLY|O_TRUNC|O_CREAT,0664);
	if(outputFileDesc < 0){
		errorf("Error. Something went wrong while opening the output file\n");
		return 4;
	}

	inputSize = lseek(inputFileDesc,0,SEEK_END);

	if(inputSize < 0){
		errorf("Error. Could not get input file size\n");
		return 8;
	}

	if(lseek(inputFileDesc, -inputSize, SEEK_END) < 0){
		errorf("Error. Could not return to start of input file\n");
		return 9;
	}

	int progressBytes = -1;
	char inputBuffer[129];
	char *outputText;

	while((progressBytes = read(inputFileDesc, inputBuffer, 128)) > 0){
		inputBuffer[progressBytes] = '\0';
		outputText = generateOutput(inputBuffer);

		if(write(outputFileDesc,outputText, strlen(outputText)) < 0){
			errorf("Error. Could not properly write into file\n");
			return 11;
		}

		progress += progressBytes;

	}

	if(progressBytes < 0){
		errorf("Error. Could not properly read file\n");
		return 10;
	}

	if(close(inputFileDesc) < 0){
		errorf("Error. Something went wrong while closing the input file\n");
		return 5;
	}

	if(close(outputFileDesc) < 0){
		errorf("Error. Something went wrong while closing the output file\n");
		return 6;
	}

	infof("Successfully generated file %s\n", outputF);

    	return 0;
}

/***********************************************************
    * Base64 library implementation                            *
    * @author Ahmed Elzoughby                                  *
    * @date July 23, 2017                                      *
    ***********************************************************/

char* base64_encode(char* plain) {

    char counts = 0;
    char buffer[3];
    char* cipher = malloc(strlen(plain) * 4 / 3 + 4);
    int i = 0, c = 0;

    for(i = 0; plain[i] != '\0'; i++) {
        buffer[counts++] = plain[i];
        if(counts == 3) {
            cipher[c++] = base46_map[buffer[0] >> 2];
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            cipher[c++] = base46_map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if(counts > 0) {
        cipher[c++] = base46_map[buffer[0] >> 2];
        if(counts == 1) {
            cipher[c++] = base46_map[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        } else {                      // if counts == 2
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0';   /* string padding character */
    return cipher;
}


char* base64_decode(char* cipher) {

    char counts = 0;
    char buffer[4];
    char* plain = malloc(strlen(cipher) * 3 / 4);
    int i = 0, p = 0;

    for(i = 0; cipher[i] != '\0'; i++) {
        char k;
        for(k = 0 ; k < 64 && base46_map[k] != cipher[i]; k++);
        buffer[counts++] = k;
        if(counts == 4) {
            plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
                plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if(buffer[3] != 64)
                plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    plain[p] = '\0';    /* string padding character */
    return plain;
}
