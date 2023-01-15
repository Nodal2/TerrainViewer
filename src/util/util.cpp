#include "util.hpp"

bool util::read_file(const std::string &path, std::string &content)
{
    std::ifstream file;
    file.open(path);
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            content.append(line);
            content.append("\n");
        }
        file.close();
        return true;
    }
    return false;
}