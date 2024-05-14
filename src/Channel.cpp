#include "Channel.hpp"

Channel::Channel(/* args */)
{
}

Channel::~Channel(){}

Channel::Channel(const Channel& src)
{
    *this = src;
}

Channel& Channel::operator=(const Channel& src)
{

    return *this;
}