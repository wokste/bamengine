#pragma once
#include <string>

struct Exception{
	virtual std::string describe() = 0;
};

struct Precondition{};
struct ComputationError{};

struct FileException : public Exception{
	std::string mFileName;
	FileException(const std::string& fileName) : mFileName(fileName){}

	std::string describe() override{
		return "could not load/save file " + mFileName;
	}
};

struct EnumException : public Exception{
	std::string mValue;
	EnumException(const std::string& value) : mValue(value){}

	std::string describe() override{
		return "Unknown enum value " + mValue;
	}
};


struct UnknownBlock : public Exception{
	std::string mTag;
	UnknownBlock(const std::string& tag) : mTag(tag){}

	std::string describe() override{
		return "unknown block " + mTag;
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
