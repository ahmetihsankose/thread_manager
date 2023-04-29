/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include "ModuleBase.h"

class ModuleManager
{
public:
    ModuleManager() = default;
    ~ModuleManager() = default;

    template <typename T, typename... Args>
    std::enable_if_t<std::is_base_of_v<ModuleBase, T>, std::shared_ptr<T>> createModule(Args &&... args)
    {
        std::shared_ptr<T> module = std::make_shared<T>(std::forward<Args>(args)...);
        addModule(module);
        return module;
    }

    template <typename T>
    std::enable_if_t<std::is_base_of_v<ModuleBase, T>, void> removeModule()
    {
        for (auto it = mModules.begin(); it != mModules.end();)
        {
            if (std::dynamic_pointer_cast<T>(*it))
            {
                it = mModules.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    template <typename T>
    std::enable_if_t<std::is_base_of_v<ModuleBase, T>, std::shared_ptr<T>> getModule()
    {
        for (auto &module : mModules)
        {
            if (auto castedModule = std::dynamic_pointer_cast<T>(module))
            {
                return castedModule;
            }
        }
        return nullptr;
    }

    void initializeModules()
    {
        for (const auto &module : mModules)
        {
            module->initialize();
        }
    }

    void terminateModules()
    {
        for (const auto &module : mModules)
        {
            module->terminate();
        }
    }


private:
    std::vector<std::shared_ptr<ModuleBase>> mModules;

    void addModule(const std::shared_ptr<ModuleBase> &module)
    {
        mModules.push_back(module);
    }
};
