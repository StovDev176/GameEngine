#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <memory>

template<typename T> 
class ComponentPool {
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
    std::unordered_map<std::type_index, std::unique_ptr<ComponentPool>>
}