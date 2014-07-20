#pragma once
#include <string>

class TokenStream{
public:
	const std::string& mData;
	int mPos;
	bool mHasArguments;

	TokenStream(const std::string& data);
	std::string readString();
	int readInt();
	float readFloat();
	bool readBool();
	bool hasArguments();
	bool push();
	bool pop();

private:
	char peek();
	bool isSpecialChar(char c);
	bool isWhitespace(char c);
	bool readSpecialChar(char c);
	void trimToNext();
};

class TokenStreamPusher{
	TokenStream& mStream;
public:
	TokenStreamPusher(TokenStream& stream);
	~TokenStreamPusher();
};
