#pragma once
#include "ConfigJSON.h"
#include <iostream>
#include <string>

namespace KJ
{
class debug_output
{
  public:
    enum class ConsoleColor
    {
        RED = 1,
        GREEN = 2,
        YELLOW = 3,
        WHITE = 4,
        PINK = 5,
    };

    enum class MessageType
    {
        FATAL = 0,   // exits the program
        ERROR = 1,   // returned always
        WARNING = 2, // returned always
        GOOD = 3,    // returned if verbose is set to 1
        INFO = 4,    // ONLY returned if verbose is set to 2
        T_NULL = 99,
    };

    static void print(const std::string s_filename, const std::string output,
                      const MessageType type = MessageType::T_NULL)
    {
        std::string filename = "";
        size_t lastSlashPos = s_filename.find_last_of('\\');
        if (lastSlashPos != std::string::npos)
        {
            filename = s_filename.substr(lastSlashPos + 1);
        }

        if (type == MessageType::T_NULL)
        {
            print_to_console("FIXME ----- " + filename + " : " + output, ConsoleColor::PINK);
            return;
        }

        if (type == MessageType::FATAL)
        {
            print_to_console(filename + " : " + output, ConsoleColor::RED);
            exit(1);
        }
        else if (type == MessageType::ERROR)
        {
            print_to_console(filename + " : " + output, ConsoleColor::RED);
        }
        else if (type == MessageType::WARNING)
        {
            print_to_console(filename + " : " + output, ConsoleColor::YELLOW);
        }
        else if (type == MessageType::GOOD)
        {
            if (ConfigJSON::getVerboseDebugging() == 1)
            {
                print_to_console(filename + " : " + output, ConsoleColor::GREEN);
            }
        }
        else if (type == MessageType::INFO)
        {
            if (ConfigJSON::getVerboseDebugging() > 1)
            {
                print_to_console("V-Verbose - " + filename + " : " + output, ConsoleColor::WHITE);
            }
        }
    }

  private:
    static void print_to_console(std::string output, ConsoleColor lineColour)
    {
        colour(lineColour);
        std::cout << output << std::endl;
        clear();
    }

    static void colour(ConsoleColor color)
    {
        switch (color)
        {
        case ConsoleColor::RED:
            std::cout << "\033[1;31m"; // Set color to red
            std::cout << "ERROR: ";    // Print the error tag
            break;
        case ConsoleColor::GREEN:
            std::cout << "\033[1;32m"; // Set color to green
            std::cout << "GOOD: ";     // Print the good tag
            break;
        case ConsoleColor::YELLOW:
            std::cout << "\033[1;33m"; // Set color to yellow
            std::cout << "WARNING: ";  // Print the warning tag
            break;
        case ConsoleColor::PINK:
            std::cout << "\033[1;35m"; // Set color to pink
            break;
        case ConsoleColor::WHITE:
        default:
            std::cout << "\033[1;37m"; // Set color to white
            break;
        }
    }

    static void clear()
    {
        std::cout << "\033[0m";
    }
};
} // namespace KJ
