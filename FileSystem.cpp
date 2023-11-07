#include "FileSystem.h"

/// <summary>
/// Checks if a file exists
/// </summary>
/// <param name="file_name">The name of the file to check</param>
/// <returns>True if the file exists, false if it does not</returns>
/// <remarks>
/// This function checks if a file exists
/// </remarks>
bool file_system::file_exists(const std::string &file_name)
{
    const std::ifstream file(file_name); // Open the file
    if (file.good())
    {
        return true; // File exists
    }
    else
    {
        printf("FileSystem - Non-Fatal Error: File %s does not exist\n", file_name.c_str());
        return false; // File does not exist
    }
}
