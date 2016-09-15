#include "stdafx.h"
#include "ecs/ECS.h"

#include "ecs/ComponentTableBase.h"

vecs::ECS::ECS() :
    onEntityAdded(this),
    onEntityRemoved(this),
    onComponentAdded(this) {
    // Empty
}

vecs::EntityID vecs::ECS::addEntity() {
    // Generate a new entity
    EntityID id = m_genEntity.generate();
    m_entities.emplace(id);

    // Check for bit-table insertion
    if (id > m_eidHighest) {
        m_entityComponents.addRows(id - m_eidHighest);
        m_eidHighest = id;
    } else {
        // Erase previous entity's component values (why?)
        m_entityComponents.setRowFalse(id - 1);
    }

    // Signal a newly created entity
    onEntityAdded(id);

    // Return the ID of the newly created entity
    return id;
}
bool vecs::ECS::deleteEntity(EntityID id) {
    // Check for a correct ID
    auto entity = m_entities.find(id);
    if (entity == m_entities.end()) return false;

    // Recycle the ID
    m_entities.erase(entity);
    m_genEntity.recycle(id);

    // Signal an entity must be destroyed
    onEntityRemoved(id);

    // Remove all the components that this entity has
    vecs::BitArray components = m_entityComponents.getRow(id - 1);
    for (TableID c = m_entityComponents.getBitColumnCount(); c > 0;) {
        c--;
        if (components.valueOf(c)) m_componentList[c]->remove(id);
    }

    return true;
}

vecs::TableID vecs::ECS::addComponentTable(nString name, vecs::ComponentTableBase* table) {
    TableID id = (TableID)m_componentList.size() + 1;
    table->m_id = id;
    m_components[name] = id;

    m_entityComponents.addColumns(1);
    m_componentList.push_back(table);

    onComponentAdded(NamedComponent(name, table));

    return id;
}
vecs::TableID vecs::ECS::getComponentTableID(const nString& name) const {
    auto kvp = m_components.find(name);
    return (kvp == m_components.end()) ? 0 : kvp->second;
}
vecs::ComponentTableBase* vecs::ECS::getComponentTable(nString name) const {
    TableID tid = getComponentTableID(name);
    if (tid == 0) return nullptr;
    return getComponentTable(getComponentTableID(name));
}
vecs::ComponentTableBase* vecs::ECS::getComponentTable(TableID id) const {
    return m_componentList[id - 1];
}

vecs::ComponentID vecs::ECS::addComponent(nString name, EntityID id) {
    ComponentTableBase* table = getComponentTable(name);
    if (!table) return ID_GENERATOR_NULL_ID;

    // Can't have multiple of the same component
    if (hasComponent(table->getID(), id)) return ID_GENERATOR_NULL_ID;
    m_entityComponents.setTrue(id - 1, table->getID() - 1);
    return table->add(id);
}
bool vecs::ECS::deleteComponent(nString name, EntityID id) {
    ComponentTableBase* table = getComponentTable(name);
    if (!table) return false;
    if (!hasComponent(table->getID(), id)) return false;
    // TODO: Delete component dependencies
    m_entityComponents.setFalse(id - 1, table->getID() - 1);
    return table->remove(id);
}

bool vecs::ECS::hasComponent(const TableID& tableID, const EntityID& id) const {
    return m_entityComponents.valueOf(id - 1, tableID - 1);
}
bool vecs::ECS::hasComponent(const nString& name, const EntityID& id) const {
    TableID tid = getComponentTableID(name);
    if (tid == 0) return false;
    return hasComponent(tid, id);
}
