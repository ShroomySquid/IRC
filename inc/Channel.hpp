#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel
{
    private:
        Channel();
    public:
        ~Channel();
        Channel& operator=(const Channel& src);
        Channel(const Channel& src);
};

#endif