#include "stdafx.h"
#include "graphics/IResource.h"

#include "graphics/IContext.h"

void vg::IResource::dispose() {
    m_owner->remove(this);
    disposeInternal();
    m_owner->free(this);
}

vorb::graphics::IResource::IResource(IContext* owner) :
    m_owner(owner) {
    // The owner is notified of this new resource
    owner->add(this);
}
