#include "stdafx.h"
#include "voxel/VoxCommon.h"

vvox::Cardinal vvox::toCardinal(const Axis& a, const bool& positive) {
    return (Cardinal)(a << 1) | (positive ? Cardinal::POSITIVE : Cardinal::NEGATIVE);
}
vvox::Axis vvox::toAxis(const Cardinal& a) {
    return (Axis)(a >> 1);
}