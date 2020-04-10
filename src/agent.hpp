#ifndef AGENT_HPP_INCLUDED
#define AGENT_HPP_INCLUDED

struct agent
{
    int gold = 0;
};

struct ai_agent_tag
{

};

struct player_agent_tag
{
    //Temporary, networking not implemented yet
    uint64_t pid = -1;
};

#endif // AGENT_HPP_INCLUDED
