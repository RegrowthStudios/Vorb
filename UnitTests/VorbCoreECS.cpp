#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Core_ECS_

#include <include/ecs/ECS.h>
#include <include/ecs/ComponentTable.hpp>

TEST(Creation) {
    vecs::ECS ecs;
    return true;
}

TEST(MakeEntity) {
    vecs::ECS ecs;
    vecs::EntityID e = ecs.addEntity();
    assert(e == 1);
    return true;
}

struct Component {
public:
    int x = 10;
};
class CTTest : public vecs::ComponentTable<Component> {
    virtual void update(const vecs::EntityID& eID, const vecs::ComponentID& cID, Component& component) {
        component.x = eID;
    }
};

TEST(MakeComponent) {
    vecs::ECS ecs;
    
    CTTest ct;
    assert(ct.getDefaultData().x == 10);
    
    ecs.addComponentTable("C1", &ct);

    auto table = ecs.getComponentTable("C1");
    assert(table);

    return true;
}