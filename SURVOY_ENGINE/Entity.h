#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include "Component.h"

namespace ECS {
    class Entity {
    private:
        int id;
        std::unordered_map<std::type_index, std::shared_ptr<Component>> _components;

    public:
        Entity(int id) : id(id) {}

        int getId() const {
            return id;
        }

        template<typename T, typename... TArgs>
        void addComponent(TArgs&&... args) {
            std::shared_ptr<T> component = std::make_shared<T>(std::forward<TArgs>(args)...);
            _components[typeid(T)] = component;
        }

        template<typename T>
        T* getComponent() const {
            auto it = _components.find(typeid(T));
            if (it != _components.end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        template<typename T>
        bool hasComponent() const {
            return _components.find(typeid(T)) != _components.end();
        }
    };
}