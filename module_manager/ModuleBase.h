#pragma once

class ModuleBase
{
public:
    virtual ~ModuleBase() = default;
    virtual void initialize() = 0;
    virtual void tick() = 0;
    virtual void terminate() = 0;
};
