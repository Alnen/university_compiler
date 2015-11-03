#ifndef IINPUTCHECKER_H
#define IINPUTCHECKER_H

#include <iosfwd>

namespace Lexer {

enum class InputCheckerType
{
    CHAR_CHECK,
    CHAR_CLASS_CHECK
};

class IInputChecker
{
public:
    IInputChecker(InputCheckerType type): m_type(type)
    {

    }

    virtual bool operator()(char input) const = 0;

    virtual bool operator==(const IInputChecker& inputChecker) const
    {
        return m_type == inputChecker.m_type;
    }

    virtual bool operator<(const IInputChecker& inputChecker) const
    {
        return m_type < inputChecker.m_type;
    }


    virtual void print(std::ostream& out) const = 0;

    virtual ~IInputChecker()
    {

    }

protected:
    InputCheckerType m_type;
};

std::ostream& operator << (std::ostream& out, IInputChecker* check);

}

#endif // IINPUTCHECKER_H
