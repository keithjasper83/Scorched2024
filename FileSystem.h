#pragma once
#include <string>
#include <fstream>


class FileSystem
{
  public:
    FileSystem() = default;

    ~FileSystem() = default;

    static bool fileExists(std::string FileName);
};
