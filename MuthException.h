#ifndef MUTH_MUTH_EXCEPTION_H
#define MUTH_MUTH_EXCEPTION_H

#include <exception>
#include <string>

namespace Muth
{

    class MuthException : public std::exception
    {
    protected:
        std::string msg;

    public:
        MuthException(const std::string& msg = "") : msg(msg) {};
        const char * what() const noexcept { return msg.c_str(); }
    };

    class MuthOutOfRangeException : public MuthException
    {
    public:
        MuthOutOfRangeException(const std::string& msg = "") { this->msg = msg; };
    };

    class MuthExceptionInvalidOperation : public MuthException
    {
    public:
        MuthExceptionInvalidOperation(const std::string& msg = "") { this->msg = msg; };
    };

} // namespace Muth

#endif