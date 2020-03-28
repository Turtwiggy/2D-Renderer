#ifndef ENTITY_COMMON_HPP_INCLUDED
#define ENTITY_COMMON_HPP_INCLUDED

struct collidable
{
    /// -1 = blocked
    int cost = 0;
};

struct team
{
    int t = 0;
};

struct battle_tag{};
struct overworld_tag{};

#endif // ENTITY_COMMON_HPP_INCLUDED
