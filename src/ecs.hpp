#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <memory>

using Entity = uint32_t;

class IComponentPool {
public:
    virtual ~IComponentPool() = default;
};

template<T typename, typename... Args>
class ComponentPool : public IComponentPool {
private:
    std::vector<T> dense;
    std::vector<uint32_t> denseIds;       
    std::unordered_map<uint32_t, uint32_t> sparseSet;

public:
    void addData(T data, uint32_t entityId) {
        dense.push_back(data);
        denseIds.push_back(entityId);
        uint32_t denseIdx = dense.size() - 1;
        sparseSet[entityId] = denseIdx;
    }

    void removeData(uint32_t entityId) {
        uint32_t denseIdx = sparseSet[entityId]; 
        uint32_t lastEntityId = denseIds.back(); 
        dense[denseIdx] = dense.back(); 
        denseIds[denseIdx] = lastEntityId;
        
        sparseSet[lastEntityId] = denseIdx; 
        dense.pop_back();
        denseIds.pop_back();
        sparseSet.erase(entityId); 
    }
};

class Registry {
private:
    std::unordered_map<std::type_index, std::unique_ptr<ComponentPool>> registers;
    uint32_t nextId = 0;
public:
    void addComponent(Entity entity, Args&&... args) {
        std::type_index typeId(typeid(T)); // creates a type for the component submitted.
        if (pools.find(typeId) == pools.end()) { // Verifies if the key exist in the unordered map.
            pools[typeId] = std::make_unique<ComponentPool<T>>(); 
        }
        IComponentPool* basePtr = pools[typeId].get(); // Creates a pointer.
        ComponentPool<T>* realPool = static_cast<ComponentPool<T>*>(basePtr); // Creating a new pointer based on the IComponentPool one with static_cast.
        realPool->addData(T{args...}, entity); // call the addData function of the pointer.
    }
    ComponentPool<T>* getComponent() {
        std::type_index typeId(typeid(T));
        if (pools.find(typeId) == pools.end()) {
            return pools[typeId] = std::make_unique<ComponentPool<T>>();
        }
        return static_cast<ComponentPool<T>*>(pools[typeId].get());
    }
};