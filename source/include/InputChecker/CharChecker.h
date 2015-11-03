#ifndef CHARCLASSCHECKER_H
#define CHARCLASSCHECKER_H

#include "IInputHandler.h"

namespace Lexer {

class CharChecker : public IInputChecker
{
public:
    CharChecker(char check): IInputChecker(InputCheckerType::CHAR_CHECK), m_check(check)
    {
    }

    virtual bool operator()(char input) const override
    {
        return input == m_check;
    }

    virtual bool operator<(const IInputChecker& inputChecker) const override
    {
        if ((this)->IInputChecker::operator ==(inputChecker))
        {
            return m_check < static_cast<const CharChecker&>(inputChecker).m_check;
        }
        else
        {
            return (this)->IInputChecker::operator <(inputChecker);
        }
    }

    virtual bool operator==(const IInputChecker& inputChecker) const override
    {
        if ((this)->IInputChecker::operator ==(inputChecker))
        {
            return m_check == static_cast<const CharChecker&>(inputChecker).m_check;
        }
        else
        {
            return false;
        }
    }

    virtual void print(std::ostream& out) const override
    {
        if (m_check == ' ')
        {
            out << "'" << m_check << "'";
        }
        else
        {
            out << m_check;
        }

    }

    virtual ~CharChecker()
    {

    }

protected:
    char m_check;
};

}

#endif // CHARCLASSCHECKER_H
