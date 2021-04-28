#pragma once
#include<iostream>

class LOG
{
public:
    LOG(){ }

    template <class T>
    LOG& operator<<(const T& msg)
    {
        #ifdef _DEBUG
                std::cout << msg;
        #endif // _DEBUG

        return *this;
    }

    ~LOG()
    {
        std::cout << std::endl;
    }
};
