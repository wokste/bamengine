#pragma once
#include <string>
#include <cmath>
#include <algorithm>

class Exception{
protected:
	std::string mError;
public:
	Exception(const std::string& error) : mError(error){}
	std::string describe() {return mError;}
	virtual ~Exception(){}
};

struct Precondition{};
struct ComputationError{};

struct FileException : public Exception{
	FileException(const std::string& fileName) : Exception("could not load/save file " + fileName){}
};

struct EnumException : public Exception{
	EnumException(const std::string& enumName, const std::string& value) : Exception("Unknown enum value " + value + " for enum " + enumName){}
};

struct UnknownBlock : public Exception{
	UnknownBlock(const std::string& value) : Exception("Unknown block " + value){}
};

struct StreamException : public Exception{
	StreamException(const std::string& error, const std::string& source, int pos) : Exception(""){
		int start = std::max(0, pos - 5);
		int end = std::min(pos + 5, static_cast<int>(source.length()));

		mError = error + "\nin " + source + "\nnear '" + source.substr(start,end) + "'";
	}
};

template <typename X, typename A, typename... Args>
inline void Assert(A assertion, Args... args)
{
#ifdef NDEBUG
    if( !assertion && std::is_base_of<Exception, A>::value) throw X(args...);
#else
    if( !assertion ) throw X(args...);
#endif
}
