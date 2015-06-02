#include "stdafx.h"
#include "graphics/IResource.h"

#include "graphics/IContext.h"

void vg::IResource::dispose() {
    m_owner->remove(this);
    disposeInternal();
    m_owner->free(this);
}
