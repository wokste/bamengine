#include "tokenstream.h"
#include <cstdlib>
#include "assert.h"

//
// TokenStream
//

TokenStream::TokenStream(const std::string& data) : mData(data), mPos(0){
	trimToNext();
	mHasArguments = true;
}

std::string TokenStream::readString(){
	Assert<StreamException>(mHasArguments, "Missing argument", mData, mPos);
	int start = mPos;
	while(!isSpecialChar(peek()) && !isWhitespace(peek())){
		mPos++;
	}
	int length = mPos - start;

	trimToNext();
	mHasArguments = readSpecialChar(',');
	return mData.substr(start, length);
}

int TokenStream::readInt(){
	std::string type = readString();
	return std::atoi(type.c_str());
}

float TokenStream::readFloat(){
	std::string type = readString();
	return std::atof(type.c_str());
}

bool TokenStream::readBool(){
	std::string type = readString();
	return (type[0] == 't' || type[0] == 'T' || type[0] == 'y' || type[0] == 'Y');
}

bool TokenStream::hasArguments(){
	return mHasArguments;
}

bool TokenStream::push(){
	Assert<StreamException>(readSpecialChar('('), "Missing (", mData, mPos);

	mHasArguments = (peek() != ')');
	return true;
}

bool TokenStream::pop(){
	Assert<StreamException>(readSpecialChar(')'), "Missing ) or too many arguments", mData, mPos);
	mHasArguments = readSpecialChar(',');
	return true;
}

char TokenStream::peek(){
	char c = mData[mPos];
	return c;
}

bool TokenStream::isSpecialChar(char c){
	return (c == '(' || c == ')' || c == ',');
}

bool TokenStream::isWhitespace(char c){
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

bool TokenStream::readSpecialChar(char c){
	if (peek() == c){
		mPos++;
		trimToNext();
		return true;
	}
	return false;
}

void TokenStream::trimToNext(){
	while(isWhitespace(mData[mPos])){
		mPos++;
	}
}

//
// TokenStreamPusher
//

TokenStreamPusher::TokenStreamPusher(TokenStream& stream) : mStream(stream){
	mStream.push();
}

TokenStreamPusher::~TokenStreamPusher(){
	mStream.pop();
}
