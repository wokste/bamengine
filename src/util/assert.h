#pragma once

struct Precondition{};
struct ComputationError{};

template <typename X, typename A>
inline void Assert(A assertion)
{
    if( !assertion ) throw X();
}
