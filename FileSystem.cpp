#pragma once
#include "FileSystem.h"


/// <summary>
/// Checks if a file exists
/// </summary>
/// <param name="FileName">The name of the file to check</param>
/// <returns>True if the file exists, false if it does not</returns>
/// <remarks>
/// This function checks if a file exists
/// </remarks>
bool FileSystem::fileExists(std::string FileName)
{
    std::ifstream file(FileName); // Open the file
    if (file.good())
    {
        return true; // File exists
    }
    else
    {
        printf("FileSystem - Non-Fatal Error: File %s does not exist\n", FileName.c_str());
        return false; // File does not exist
    }
}
