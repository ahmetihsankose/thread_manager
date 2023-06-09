/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once

namespace TEST
{
    class ModuleBase
    {
    public:
        virtual ~ModuleBase() = default;
        virtual void initialize() = 0;
        virtual void tick() = 0;
        virtual void terminate() = 0;
    };
} // namespace TEST
