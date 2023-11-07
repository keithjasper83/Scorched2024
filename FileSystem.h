#pragma once
#include <fstream>
#include <string>

class file_system
{
  public:
    file_system() = default;

    static bool file_exists(const std::string& file_name);
};
