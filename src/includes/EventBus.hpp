#pragma once
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>

struct IEventBuffer {
    virtual ~IEventBuffer() = default;
    virtual void Clear() = 0;
};

template <typename T>
class EventBuffer : public IEventBuffer {
public:
    std::vector<T> events;

    void Clear() override {
        events.clear(); 
    }
};

class EventBus {
private:
    std::unordered_map<std::type_index, std::unique_ptr<IEventBuffer>> m_buffers;

    template <typename T>
    EventBuffer<T>* GetOrCreateBuffer() {
        std::type_index typeId = typeid(T);
        auto it = m_buffers.find(typeId);
        
        if (it == m_buffers.end()) {
            auto newBuffer = std::make_unique<EventBuffer<T>>();
            auto* ptr = newBuffer.get();
            m_buffers[typeId] = std::move(newBuffer);
            return ptr;
        }
        
        return static_cast<EventBuffer<T>*>(it->second.get());
    }

public:
    template <typename T>
    void Publish(const T& event) {
        GetOrCreateBuffer<T>()->events.push_back(event);
    }

    template <typename T>
    const std::vector<T>& Read() {
        return GetOrCreateBuffer<T>()->events;
    }
    void ClearAll() {
        for (auto& [type, buffer] : m_buffers) {
            buffer->Clear();
        }
    }
};