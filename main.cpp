#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>

#define BUFFER_SIZE 1024
#define NUM_VERIFY_ATTEMPT 3

bool verify(unsigned char*A, unsigned char*B, int length){
	for(int i=0;i<length;i++){
		if(A[i]!=B[i]){
			return false;
		}
	}

	return true;
}

void printBuffer(unsigned char*buf,int length){
	for(int i=0;i<length;i++){
		std::cerr<<(int)buf[i]<<" ";
	}
	std::cerr<<std::endl;
}


int main(int argc, char*argv[])
{

	if(argc<3){
		std::cerr<<"Usage: "<<argv[0]<<" src dest"<<std::endl;
		return 1;
	}

	unsigned char buffer[BUFFER_SIZE];
	unsigned char cbuffer[BUFFER_SIZE];
    std::ifstream input( argv[1], std::ios::binary );
    std::fstream output( argv[2], std::ios::binary |  std::ios::in | std::ios::out | std::ios::trunc );

	int writePos=0;
	int numBytesRead;
	bool verified=false;

	while(input){

		for(int i=0;i<NUM_VERIFY_ATTEMPT;i++){
			input.read((char*)buffer,BUFFER_SIZE);
			numBytesRead=input.gcount();

			output.write((char*)buffer,numBytesRead);
			//verify
			output.seekg(writePos);
			output.read((char*)cbuffer,numBytesRead);



			verified=verify(buffer,cbuffer,numBytesRead);
			if(verified){
				writePos+=numBytesRead;
				break;
			}else{
				input.seekg(writePos);
			}

		}

		if(!verified){
			std::cerr<<"verfication failed at "<<writePos<<". exited."<<std::endl;
			std::cerr<<"buffer"<<std::endl;
			printBuffer(buffer,numBytesRead);
			std::cerr<<"cbuffer"<<std::endl;
			printBuffer(cbuffer,numBytesRead);

			return 1;
		}
	}

	input.close();
	output.close();

	return 0;
}