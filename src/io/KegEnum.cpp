#include "Vorb/stdafx.h"
#include "Vorb/io/KegEnum.h"

#include "Vorb/io/KegEnvironment.h"

keg::Enum::Enum(size_t sizeInBytes, const nString& name, Environment* env) :
_sizeInBytes(sizeInBytes) {
    switch (_sizeInBytes) {
    case 1:
        _fSetter = setValue<ui8>;
        _fGetter = getValue<ui8>;
        break;
    case 2:
        _fSetter = setValue<ui16>;
        _fGetter = getValue<ui16>;
        break;
    case 4:
        _fSetter = setValue<ui32>;
        _fGetter = getValue<ui32>;
        break;
    default:
        _fSetter = setValue<ui64>;
        _fGetter = getValue<ui64>;
        break;
    }
    if (env) env->addEnum(name, this);
    getGlobalEnvironment()->addEnum(name, this);
}
