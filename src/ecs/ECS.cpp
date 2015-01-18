#include "stdafx.h"
#include "ecs/ECS.h"

#include "ecs/ComponentTableBase.h"

vcore::ECS::ECS() :
    onEntityAdded(this),
    onEntityRemoved(this),
    onComponentAdded(this) {
    // Empty
}

vcore::EntityID vcore::ECS::addEntity() {
    // Generate a new entity
    EntityID id = _genEntity.generate();
    _entities.emplace(id);

    // Check for bit-table insertion
    if (id > m_eidHighest) {
        do {
            m_eidHighest++;
            m_entityComponents.addRow();
        } while (id > m_eidHighest);
    } else {
        // Erase previous entity's component values
        m_entityComponents.setRowFalse(id - 1);
    }

    // Signal a newly created entity
    onEntityAdded(id);

    // Return the ID of the newly created entity
    return id;
}
bool vcore::ECS::deleteEntity(EntityID id) {
    // Check for a correct ID
    if (id > m_eidHighest) return false;
    auto entity = _entities.find(id);
    if (entity == _entities.end()) return false;

    // Recycle the ID
    _entities.erase(entity);
    _genEntity.recycle(id);

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

vcore::TableID vcore::ECS::addComponentTable(nString name, vcore::ComponentTableBase* table) {
    TableID id = (TableID)m_componentList.size() + 1;
    table->m_id = id;
    _components[name] = id;

    m_entityComponents.addColumn();
    m_componentList.push_back(table);

    onComponentAdded(NamedComponent(name, table));

    return id;
}
vcore::TableID vcore::ECS::getComponentTableID(const nString& name) const {
    auto kvp = _components.find(name);
    return (kvp == _components.end()) ? 0 : kvp->second;
}
vcore::ComponentTableBase* vcore::ECS::getComponentTable(nString name) const {
    TableID tid = getComponentTableID(name);
    if (tid == 0) return nullptr;
    return getComponentTable(getComponentTableID(name));
}
vcore::ComponentTableBase* vcore::ECS::getComponentTable(TableID id) const {
    return m_componentList[id - 1];
}

vcore::ComponentID vcore::ECS::addComponent(nString name, EntityID id) {
    ComponentTableBase* table = getComponentTable(name);
    if (!table) return ID_GENERATOR_NULL_ID;

    // Can't have multiple of the same component
    if (hasComponent(table->getID(), id)) return ID_GENERATOR_NULL_ID;
    m_entityComponents.setTrue(id - 1, table->getID() - 1);
    return table->add(id);
}
bool vcore::ECS::deleteComponent(nString name, EntityID id) {
    ComponentTableBase* table = getComponentTable(name);
    if (!table) return false;
    if (!hasComponent(table->getID(), id)) return false;
    // TODO: Delete component dependencies
    m_entityComponents.setFalse(id - 1, table->getID() - 1);
    return table->remove(id);
}

bool vcore::ECS::hasComponent(const TableID& tableID, const EntityID& id) const {
    return m_entityComponents.valueOf(id - 1, tableID - 1);
}
bool vcore::ECS::hasComponent(const nString& name, const EntityID& id) const {
    TableID tid = getComponentTableID(name);
    if (tid == 0) return false;
    return hasComponent(tid, id);
}
