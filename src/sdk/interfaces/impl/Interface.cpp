#include "Interface.h"
#include <string>
#include <format>
#include "../../cheat.h"

void Interface::print() const
{
    auto remove_leading_zeros = [](const std::string& input) -> std::string
    {
        size_t start = input.find_first_not_of("0x");
        if (start != std::string::npos)
            return "0x" + input.substr(start);

        return "0x0";
    };

    std::string ptr_str = std::format("{:p}", m_obj);

    std::string str = std::format("{:s}: {:s}", m_name, remove_leading_zeros(ptr_str));

    cheat::console().println(str.c_str());
}
