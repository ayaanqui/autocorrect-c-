#include "autocorrect.h"

int main()
{
    Autocorrect ob;
    std::string input;

    while (true)
    {
        std::cout << "Enter phrase (type -1 to quit): ";
        std::getline(std::cin, input);

        if (input == "-1")
            break;
        else
        {
            ob.suggest(input);
            std::cout << std::endl;
        }
    }
    return 0;
}