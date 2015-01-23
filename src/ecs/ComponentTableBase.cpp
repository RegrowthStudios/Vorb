#include "stdafx.h"
#include "ecs/ComponentTableBase.h"

vecs::ComponentTableBase::ComponentTableBase() :
    onEntityAdded(this),
    onEntityRemoved(this) {
    // Empty
}

vecs::ComponentID vecs::ComponentTableBase::add(EntityID eID) {
    // Check that this entity does not exist
    auto cBind = _components.find(eID);
    if (cBind != _components.end()) {
        char buf[256];
        sprintf(buf,
                "Entity <0x%08lX> already contains component <0x%08lX>",
                static_cast<unsigned long>(eID),
                static_cast<unsigned long>(cBind->second));
        throw std::runtime_error(buf);
    }

    // Generate a new component
    bool shouldPush = false;
    ComponentID id = _genComponent.generate(&shouldPush);
    _components[eID] = id;

    if (shouldPush) {
        // Add a new component
        addComponent(id, eID);
    } else {
        // Recycle an old component
        setComponent(id, eID);
    }

    // Perform initialization operation
    initComponent(id, eID);

    // Signal addition
    onEntityAdded(id, eID);

    return id;
}
bool vecs::ComponentTableBase::remove(EntityID eID) {
    // Find the entity
    auto cBind = _components.find(eID);
    if (cBind == _components.end()) return false;

    // Signal removal
    onEntityRemoved(cBind->second, eID);

    // Perform disposal operations
    disposeComponent(cBind->second, eID);

    // Component is cleared
    _genComponent.recycle(cBind->second);
    setComponent(cBind->second, ID_GENERATOR_NULL_ID);
    _components.erase(cBind);

    return true;
}
