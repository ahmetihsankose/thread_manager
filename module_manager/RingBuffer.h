/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <array>
#include <map>
#include <mutex>

template <typename T, size_t BufferSize>
class RingBuffer
{
public:
    // Add an element to the back of the buffer
    void push_back(const T &value)
    {
        std::lock_guard<std::mutex> lock(mtx);
        buffer[end] = value;
        end = (end + 1) % BufferSize;

        // If the buffer is full, overwrite the oldest element
        if (end == start)
        {
            start = (start + 1) % BufferSize;
        }
    }

    // Return the number of elements in the buffer
    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (end >= start)
        {
            return end - start;
        }
        else
        {
            return BufferSize - (start - end);
        }
    }

    // Access the first element in the buffer
    T front() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (size() == 0)
        {
            throw std::out_of_range("Buffer is empty");
        }
        return buffer[start];
    }

    // Access an element in the buffer by index
    T operator[](size_t index) const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return buffer[(start + index) % BufferSize];
    }

    // Access an element in the buffer by index, with range checking
    T at(size_t index) const
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (index >= size())
        {
            throw std::out_of_range("Index out of range");
        }
        return buffer[(start + index) % BufferSize];
    }

    // Clear the buffer and set all elements to their default value
    void clear()
    {
        std::lock_guard<std::mutex> lock(mtx);
        start = 0;
        end = 0;
        buffer.fill(T{});
    }

    // Get a vector containing all elements in the buffer, in order
    std::vector<T> get_all() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<T> result;
        for (size_t i = 0; i < size(); i++)
        {
            result.push_back(buffer[(start + i) % BufferSize]);
        }
        return result;
    }

    // Check if a given value is in the buffer
    bool contains(const T &value) const
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < size(); i++)
        {
            if (buffer[(start + i) % BufferSize] == value)
            {
                return true;
            }
        }
        return false;
    }

    // Remove the first occurrence of a given value from the buffer
    void remove(const T &value)
    {
        std::lock_guard<std::mutex> lock(mtx);

        for (size_t i = 0; i < size(); i++)
        {
            if (buffer[(start + i) % BufferSize] == value)
            {
                size_t index = (start + i) % BufferSize;
                for (size_t j = i; j < size() - 1; j++)
                {
                    buffer[(start + j) % BufferSize] = buffer[(start + j + 1) % BufferSize];
                }
                end = (end - 1 + BufferSize) % BufferSize;
                if (index == start)
                {
                    start = (start + 1) % BufferSize;
                }
                break;
            }
        }
    }

private:
    std::array<T, BufferSize> buffer;
    size_t start = 0;
    size_t end = 0;
    mutable std::mutex mtx;
};