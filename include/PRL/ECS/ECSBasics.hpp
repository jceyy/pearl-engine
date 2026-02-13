#ifndef ECS_BASICS_HPP
#define ECS_BASICS_HPP

#include <bitset>
#include <algorithm>
#include <assert.h>

namespace ECS {
    constexpr std::size_t maxComponents = 32;
    constexpr std::size_t maxGroups = 32;
    constexpr std::size_t maxSystems = 32;
}

using EntityGroup = std::size_t;
using EntityGroupBitSet = std::bitset<ECS::maxGroups>;

class ComponentID {
public:
    ComponentID() = default;
    ComponentID(std::size_t id) : id_(id) {}

    constexpr inline explicit operator std::size_t() const noexcept { return id_; }

    inline bool operator==(std::size_t id) const { return id_ == id; }
    inline bool operator!=(std::size_t id) const { return id_ != id; }
    inline bool operator<(std::size_t id) const { return id_ < id; }
    inline bool operator>(std::size_t id) const { return id_ > id; }
    inline bool operator<=(std::size_t id) const { return id_ <= id; }
    inline bool operator>=(std::size_t id) const { return id_ >= id; }
    inline std::size_t operator++() { return ++id_; }
    inline std::size_t operator++(int) { return id_++; }

    //! Static function to generate new component type IDs
    inline static ComponentID getNewComponentTypeID(){
        static ComponentID lastID = 0u;
        assert(lastID < ECS::maxComponents);
        return lastID++;
    }

    //! Static function to get the component type ID for a specific type
    template<typename T> inline static ComponentID getComponentTypeID() noexcept{
        static ComponentID typeID = ComponentID::getNewComponentTypeID();
        return typeID;
    }

    constexpr static std::size_t maxComponentID = ECS::maxComponents;

private:
    std::size_t id_;
};


class ComponentSignature {
public:
    ComponentSignature() : bitset_(0) {}
    ComponentSignature(std::size_t id) : bitset_(id) {}
    ~ComponentSignature() = default;

    inline bool matches(const ComponentSignature& other) const {
        return (bitset_ & other.bitset_) == other.bitset_;
    }
    
    inline bool all() const { return bitset_.all(); }
    inline bool any() const { return bitset_.any(); }
    inline bool none() const { return bitset_.none(); }
    
    inline void set(ComponentID id) {
        assert(id < ECS::maxComponents);
        bitset_.set(static_cast<size_t>(id));
    }

    inline const std::bitset<ECS::maxComponents>& bitset() const { return bitset_; }

    inline bool operator[] (ComponentID id) const { return bitset_[static_cast<size_t>(id)]; }
    inline std::bitset<ECS::maxComponents>::reference operator[] (ComponentID id) { 
        return bitset_[static_cast<size_t>(id)];
    }
    
    //! Assignment operator to set the signature from a bitset
    inline ComponentSignature& operator=(const std::bitset<ECS::maxComponents>& bitset) {
        this->bitset_ = bitset;
        return *this;
    }

    //! Operators for bitwise operations
    inline ComponentSignature& operator|=(const ComponentSignature& other) {
        this->bitset_ |= other.bitset_;
        return *this;
    }
    inline ComponentSignature& operator&=(const ComponentSignature& other) {
        this->bitset_ &= other.bitset_;
        return *this;
    }
    friend ComponentSignature operator|(ComponentSignature lhs, const ComponentSignature& rhs) {
        lhs |= rhs;
        return lhs;
    }
    friend ComponentSignature operator&(ComponentSignature lhs, const ComponentSignature& rhs) {
        lhs &= rhs;
        return lhs;
    }
    
    //! Static function that creates a signature
    template <typename... ComponentTypes>
    static ComponentSignature create() {
        ComponentSignature signature;
        int assemble[] = { (signature.set(ComponentID::getComponentTypeID<ComponentTypes>()), 0)... };
        (void)assemble;
        return signature;
    }
   
private:
    //! Bitset representing the presence of components in an entity or the requirements of a system
    std::bitset<ECS::maxComponents> bitset_; 
};

class System;

class SystemID {
public:
    SystemID() : id_(0u) {}
    SystemID(std::size_t id) : id_(id) {}
    
    //! Allow conversion to size_t for easy indexing
    constexpr inline explicit operator std::size_t() const noexcept { return id_; }
    
    //! Comparison operators
    inline bool operator==(std::size_t id) const { return id_ == id; }
    inline bool operator!=(std::size_t id) const { return id_ != id; }
    inline bool operator<(std::size_t id) const { return id_ < id; }
    inline bool operator>(std::size_t id) const { return id_ > id; }
    inline bool operator<=(std::size_t id) const { return id_ <= id; }
    inline bool operator>=(std::size_t id) const { return id_ >= id; }
    inline std::size_t operator++() { return ++id_; }
    inline std::size_t operator++(int) { return id_++; }

    //! Assignment operator to set the system ID from a size_t
    inline SystemID& operator=(std::size_t id) {
        this->id_ = id;
        return *this;
    }

    //! Static function to generate new system type IDs
    template<typename T> static inline std::size_t getNewSystemTypeID() {
        static_assert(std::is_base_of<System, T>::value, "T must inherit from System");
        static SystemID lastID = 0u;
        assert(lastID < maxSystemID);
        return lastID++;
    }

    //! Get the system type ID for a specific type
    template<typename T> static inline std::size_t getSystemTypeID() {
        static_assert(std::is_base_of<System, T>::value, "T must inherit from System");
        static std::size_t typeID = getNewSystemTypeID<T>();
        return typeID;
    }

    //! Maximum number of systems that can be registered
    constexpr static std::size_t maxSystemID = ECS::maxSystems;

private:
    std::size_t id_;
};

#endif // ECS_BASICS_HPP
