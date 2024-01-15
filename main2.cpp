#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>

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


bool verifyHead(char *filename1, char*filename2,int length){
	std::ifstream file1(filename1,std::ios::binary);
	std::ifstream file2(filename2,std::ios::binary);
	if(!file1){
		return false;
	}
	if(!file2){
		return false;
	}
	unsigned char *buffer1=new unsigned char[length];
	unsigned char *buffer2=new unsigned char[length];
	file1.read((char*)buffer1,length);
	file2.read((char*)buffer2,length);
	file1.close();
	file2.close();
	bool verified=verify(buffer1,buffer2,length);

	delete[] buffer1;
	delete[] buffer2;

	return verified;
}

int verifiedCopy(char *src, char* dst){
	unsigned char buffer[BUFFER_SIZE];
	unsigned char cbuffer[BUFFER_SIZE];
    std::ifstream input( src, std::ios::binary );
    std::fstream output( dst, std::ios::binary |  std::ios::in | std::ios::out | std::ios::trunc );

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

int main(int argc, char*argv[])
{

	if(argc<4){
		std::cerr<<"Usage: "<<argv[0]<<" copy src dest"<<std::endl;
		std::cerr<<"Usage: "<<argv[0]<<" verify src dest numBytesToCheck"<<std::endl;
		std::cerr<<"Usage: "<<argv[0]<<" copyIfNotVerified src dest numBytesToCheck"<<std::endl;
		return 1;
	}

	if(!strcmp(argv[1],"copy")){
		return verifiedCopy(argv[2],argv[3]);
	}else if(!strcmp(argv[1],"verify")){
		if(argc<5){
			std::cerr<<"Usage: "<<argv[0]<<"verify src dest numBytesToCheck"<<std::endl;
			return 1;
		}
		if(verifyHead(argv[2],argv[3],atoi(argv[4]))){
			std::cerr<<"verified"<<std::endl;
		}else{
			std::cerr<<"notverified"<<std::endl;
		}
	
		return 0;
	}else if(!strcmp(argv[1],"copyIfNotVerified")){
		if(argc<5){
			std::cerr<<"Usage: "<<argv[0]<<"copyIfNotVerified src dest numBytesToCheck"<<std::endl;
			return 1;
		}
		if(verifyHead(argv[2],argv[3],atoi(argv[4]))){
			std::cerr<<"verified. No copy needed"<<std::endl;
			return 0;
		}else{
			return verifiedCopy(argv[2],argv[3]);
		}
	}else{

		std::cerr<<"No function "<<argv[1]<<std::endl;
		return 1;
	}
}