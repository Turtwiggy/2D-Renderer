#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

#include <random>

struct random_state
{
    std::minstd_rand rng;
};

#endif
