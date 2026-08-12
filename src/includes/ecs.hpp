#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cstdint>
#include <vector>

using Entity = uint32_t;

class IComponentPool {
public:
    virtual ~IComponentPool() = default;
};

template<typename T>
class ComponentPool : public IComponentPool {
public:
    std::vector<T> dense;
    std::vector<uint32_t> denseIds;
    std::unordered_map<uint32_t, uint32_t> sparseSet;

    void addData(T data, uint32_t entityId) {
        dense.push_back(data);
        denseIds.push_back(entityId);
        sparseSet[entityId] = dense.size() - 1;
    }

    void removeData(uint32_t entityId) {
        if (sparseSet.count(entityId) == 0) return; 
        
        uint32_t denseIdx = sparseSet[entityId]; 
        uint32_t lastEntityId = denseIds.back(); 
        
        dense[denseIdx] = dense.back(); 
        denseIds[denseIdx] = lastEntityId;
        
        sparseSet[lastEntityId] = denseIdx; 
        
        dense.pop_back();
        denseIds.pop_back();
        sparseSet.erase(entityId);
    }

    T* getComponent(uint32_t entityId) {
        if (sparseSet.count(entityId) == 0) return nullptr;
        uint32_t denseIdx = sparseSet[entityId];
        return &dense[denseIdx];
    }
};

class Registry {
private:
    Entity nextEntity = 0;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> pools;

public:
    Entity create() {
        return nextEntity++;
    }

    template<typename T, typename... Args>
    void addComponent() {
        std::type_index typeId(typeid(T));
        if (pools.find(typeId) == pools.end()) {
            pools[typeId] = std::make_unique<ComponentPool<T>>();
        }
        ComponentPool<T>* pool = static_cast<ComponentPool<T>*>(pools[typeId].get());
    }

    template<typename T>
    ComponentPool<T>* getComponentPool() {
        std::type_index typeId(typeid(T));
        if (pools.find(typeId) == pools.end()) {
            pools[typeId] = std::make_unique<ComponentPool<T>>();
        }
        return static_cast<ComponentPool<T>*>(pools[typeId].get());
    }
};