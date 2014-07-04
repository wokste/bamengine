#pragma once
#include <string>

struct Precondition{};
struct ComputationError{};
struct UnknownBlock{
	std::string mTag;
	UnknownBlock(const std::string& tag){mTag = tag;}
};

template <typename X, typename A, typename... Args>
inline void Assert(A assertion, Args... args)
{
    if( !assertion ) throw X(args...);
}
