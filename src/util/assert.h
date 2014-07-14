#pragma once
#include <string>

struct Exception{
	virtual std::string describe() = 0;
};

struct Precondition{};
struct ComputationError{};
struct UnknownBlock : Exception{
	std::string mTag;
	UnknownBlock(const std::string& tag){mTag = tag;}

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
