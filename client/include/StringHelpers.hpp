#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

#include <sstream>
#include <vector>
#include <iostream>

class StringHelpers
{
    public:
        template <typename T>
        static std::string toString(const T& value)
        {
            std::stringstream stream;
            stream << value;
            return stream.str();
        }

        static std::vector<std::string> splitString(std::string str, const std::string& delimiter)
        {
            size_t pos = 0;
            std::vector<std::string> tokens;
            while ((pos = str.find(delimiter)) != std::string::npos)
            {
                tokens.push_back(str.substr(0, pos));
                str.erase(0, pos + delimiter.length());
            }
            tokens.push_back(str);
            return tokens;
        }

    protected:

    private:
        StringHelpers(){};
        virtual ~StringHelpers(){};
};

#endif // STRINGHELPERS_H
