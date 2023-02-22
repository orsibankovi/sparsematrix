//
// Created by banko on 2022. 10. 06..
//

#ifndef KHF3_OUTOFBOUNDEXCEPTION_H
#define KHF3_OUTOFBOUNDEXCEPTION_H

#include <exception>

class OutOfBoundException : public std::exception
{
public:
    const char *what() const noexcept { return "Tulindexeles!"; }
};

#endif //KHF3_OUTOFBOUNDEXCEPTION_H
