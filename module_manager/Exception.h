/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    explicit Exception(const std::string &message)
        : message(message)
    {
    }

    const char *what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

class ConsoleOutputWriteException : public Exception
{
    public:
        explicit ConsoleOutputWriteException(const std::string &message)
            : Exception("Console output write error: " + message)
        {
        }
};

class FileOutputWriteException : public Exception
{
    public:
        explicit FileOutputWriteException(const std::string &message)
            : Exception("File output write error: " + message)
        {
        }
};

class FileNotFoundException : public Exception
{
public:
    explicit FileNotFoundException(const std::string &filename)
        : Exception("File not found: " + filename)
    {
    }
};

class InvalidArgumentException : public Exception
{
public:
    explicit InvalidArgumentException(const std::string &argument)
        : Exception("Invalid argument: " + argument)
    {
    }
};

class InvalidOperationException : public Exception
{
public:
    explicit InvalidOperationException(const std::string &operation)
        : Exception("Invalid operation: " + operation)
    {
    }
};
