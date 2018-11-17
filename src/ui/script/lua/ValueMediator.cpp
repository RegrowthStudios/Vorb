#include "Vorb/stdafx.h"
#include "Vorb/ui/script/lua/ValueMediator.h"

#include "Vorb/ui/Widget.h"

// Provides support for pushing and popping Clipping.
vui::Clipping vscript::lua::ValueMediator<vui::Clipping, void>::defaultValue() {
    return vui::DEFAULT_CLIPPING;
}

i32 vscript::lua::ValueMediator<vui::Clipping, void>::getValueCount() {
    return 4;
}

i32 vscript::lua::ValueMediator<vui::Clipping, void>::push(Handle state, vui::Clipping clipping) {
    return ValueMediator<vui::ClippingState>::push(state, clipping.left)
         + ValueMediator<vui::ClippingState>::push(state, clipping.top)
         + ValueMediator<vui::ClippingState>::push(state, clipping.right)
         + ValueMediator<vui::ClippingState>::push(state, clipping.bottom);
}

vui::Clipping vscript::lua::ValueMediator<vui::Clipping, void>::pop(Handle state) {
    vui::Clipping clipping;

    clipping.bottom = ValueMediator<vui::ClippingState>::pop(state);
    clipping.right  = ValueMediator<vui::ClippingState>::pop(state);
    clipping.top    = ValueMediator<vui::ClippingState>::pop(state);
    clipping.left   = ValueMediator<vui::ClippingState>::pop(state);

    return clipping;
}

bool vscript::lua::ValueMediator<vui::Clipping, void>::tryPop(Handle state, OUT vui::Clipping& clipping) {
    vui::ClippingState result[4];
    // Try to pop the length.
    bool success = ValueMediator<vui::ClippingState>::tryPop(state, result[3])
                && ValueMediator<vui::ClippingState>::tryPop(state, result[2])
                && ValueMediator<vui::ClippingState>::tryPop(state, result[1])
                && ValueMediator<vui::ClippingState>::tryPop(state, result[0]);
    // If successful, set value to the popped Clipping.
    if (success) {
        clipping = vui::Clipping{
            result[0], result[1], result[2], result[3]
        };
    }
    // Return success of pop.
    return success;
}

vui::Clipping vscript::lua::ValueMediator<vui::Clipping, void>::retrieve(Handle state, ui32 index) {
    vui::Clipping clipping;

    clipping.bottom = ValueMediator<vui::ClippingState>::retrieve(state, index);
    clipping.right  = ValueMediator<vui::ClippingState>::retrieve(state, index);
    clipping.top    = ValueMediator<vui::ClippingState>::retrieve(state, index);
    clipping.left   = ValueMediator<vui::ClippingState>::retrieve(state, index);

    return clipping;
}

bool vscript::lua::ValueMediator<vui::Clipping, void>::tryRetrieve(Handle state, ui32 index, OUT vui::Clipping& clipping) {
    vui::ClippingState result[4];
    // Try to pop the underlying enum type.
    bool success = ValueMediator<vui::ClippingState>::tryRetrieve(state, index, result[3])
                && ValueMediator<vui::ClippingState>::tryRetrieve(state, index, result[2])
                && ValueMediator<vui::ClippingState>::tryRetrieve(state, index, result[1])
                && ValueMediator<vui::ClippingState>::tryRetrieve(state, index, result[0]);
    // If successful, set value to the Type casted result of the pop.
    if (success) {
        clipping = vui::Clipping{
            result[0], result[1], result[2], result[3]
        };
    }
    // Return success of pop.
    return success;
}

// Provides support for pushing and popping Dock.
vui::Dock vscript::lua::ValueMediator<vui::Dock, void>::defaultValue() {
    return vui::Dock{ vui::DockState::NONE, 0.0f };
}

i32 vscript::lua::ValueMediator<vui::Dock, void>::getValueCount() {
    return 2;
}

i32 vscript::lua::ValueMediator<vui::Dock, void>::push(Handle state, vui::Dock dock) {
    return ValueMediator<vui::DockState>::push(state, dock.state)
         + ValueMediator<f32>::push(state, dock.size);
}

vui::Dock vscript::lua::ValueMediator<vui::Dock, void>::pop(Handle state) {
    vui::Dock dock;

    dock.size  = ValueMediator<f32>::pop(state);
    dock.state = ValueMediator<vui::DockState>::pop(state);

    return dock;
}

bool vscript::lua::ValueMediator<vui::Dock, void>::tryPop(Handle state, OUT vui::Dock& dock) {
    vui::Dock result;
    // Try to pop the length.
    bool success = ValueMediator<f32>::tryPop(state, result.size)
                && ValueMediator<vui::DockState>::tryPop(state, result.state);
    // If successful, set value to the popped Dock.
    if (success) {
        dock = result;
    }
    // Return success of pop.
    return success;
}

vui::Dock vscript::lua::ValueMediator<vui::Dock, void>::retrieve(Handle state, ui32 index) {
    vui::Dock dock;

    dock.size  = ValueMediator<f32>::retrieve(state, index);
    dock.state = ValueMediator<vui::DockState>::retrieve(state, index);

    return dock;
}

bool vscript::lua::ValueMediator<vui::Dock, void>::tryRetrieve(Handle state, ui32 index, OUT vui::Dock& dock) {
    vui::Dock result;
    // Try to pop the underlying enum type.
    bool success = ValueMediator<f32>::tryRetrieve(state, index, result.size)
                && ValueMediator<vui::DockState>::tryRetrieve(state, index, result.state);
    // If successful, set value to the Type casted result of the pop.
    if (success) {
        dock = result;
    }
    // Return success of pop.
    return success;
}

// Provides support for pushing and popping WidgetFlags.
vui::WidgetFlags vscript::lua::ValueMediator<vui::WidgetFlags, void>::defaultValue() {
    return vui::WidgetFlags{ false, false, false, false, false, false, false, false, false };
}

i32 vscript::lua::ValueMediator<vui::WidgetFlags, void>::getValueCount() {
    return 9;
}

i32 vscript::lua::ValueMediator<vui::WidgetFlags, void>::push(Handle state, vui::WidgetFlags flags) {
    return ValueMediator<bool>::push(state, flags.isClicking)
         + ValueMediator<bool>::push(state, flags.isEnabled)
         + ValueMediator<bool>::push(state, flags.isMouseIn)
         + ValueMediator<bool>::push(state, flags.ignoreOffset)
         + ValueMediator<bool>::push(state, flags.needsDimensionUpdate)
         + ValueMediator<bool>::push(state, flags.needsZIndexReorder)
         + ValueMediator<bool>::push(state, flags.needsDockRecalculation)
         + ValueMediator<bool>::push(state, flags.needsClipRectRecalculation)
         + ValueMediator<bool>::push(state, flags.needsDrawableRecalculation);
}

vui::WidgetFlags vscript::lua::ValueMediator<vui::WidgetFlags, void>::pop(Handle state) {
    vui::WidgetFlags flags;

    flags.needsDrawableRecalculation = ValueMediator<bool>::pop(state);
    flags.needsClipRectRecalculation = ValueMediator<bool>::pop(state);
    flags.needsDockRecalculation     = ValueMediator<bool>::pop(state);
    flags.needsZIndexReorder         = ValueMediator<bool>::pop(state);
    flags.needsDimensionUpdate       = ValueMediator<bool>::pop(state);
    flags.ignoreOffset               = ValueMediator<bool>::pop(state);
    flags.isMouseIn                  = ValueMediator<bool>::pop(state);
    flags.isEnabled                  = ValueMediator<bool>::pop(state);
    flags.isClicking                 = ValueMediator<bool>::pop(state);

    return flags;
}

bool vscript::lua::ValueMediator<vui::WidgetFlags, void>::tryPop(Handle state, OUT vui::WidgetFlags& flags) {
    bool result[9];
    // Try to pop the length.
    bool success = ValueMediator<bool>::tryPop(state, result[8])
                && ValueMediator<bool>::tryPop(state, result[7])
                && ValueMediator<bool>::tryPop(state, result[6])
                && ValueMediator<bool>::tryPop(state, result[5])
                && ValueMediator<bool>::tryPop(state, result[4])
                && ValueMediator<bool>::tryPop(state, result[3])
                && ValueMediator<bool>::tryPop(state, result[2])
                && ValueMediator<bool>::tryPop(state, result[1])
                && ValueMediator<bool>::tryPop(state, result[0]);
    // If successful, set value to the popped WidgetFlags.
    if (success) {
        flags = vui::WidgetFlags{
            result[0], result[1], result[2], result[3], result[4],
            result[5], result[6], result[7], result[8]
        };
    }
    // Return success of pop.
    return success;
}

vui::WidgetFlags vscript::lua::ValueMediator<vui::WidgetFlags, void>::retrieve(Handle state, ui32 index) {
    vui::WidgetFlags flags;

    flags.needsDrawableRecalculation = ValueMediator<bool>::retrieve(state, index);
    flags.needsClipRectRecalculation = ValueMediator<bool>::retrieve(state, index);
    flags.needsDockRecalculation     = ValueMediator<bool>::retrieve(state, index);
    flags.needsZIndexReorder         = ValueMediator<bool>::retrieve(state, index);
    flags.needsDimensionUpdate       = ValueMediator<bool>::retrieve(state, index);
    flags.ignoreOffset               = ValueMediator<bool>::retrieve(state, index);
    flags.isMouseIn                  = ValueMediator<bool>::retrieve(state, index);
    flags.isEnabled                  = ValueMediator<bool>::retrieve(state, index);
    flags.isClicking                 = ValueMediator<bool>::retrieve(state, index);

    return flags;
}

bool vscript::lua::ValueMediator<vui::WidgetFlags, void>::tryRetrieve(Handle state, ui32 index, OUT vui::WidgetFlags& flags) {
    bool result[9];
    // Try to pop the underlying enum type.
    bool success = ValueMediator<bool>::tryRetrieve(state, index, result[8])
                && ValueMediator<bool>::tryRetrieve(state, index, result[7])
                && ValueMediator<bool>::tryRetrieve(state, index, result[6])
                && ValueMediator<bool>::tryRetrieve(state, index, result[5])
                && ValueMediator<bool>::tryRetrieve(state, index, result[4])
                && ValueMediator<bool>::tryRetrieve(state, index, result[3])
                && ValueMediator<bool>::tryRetrieve(state, index, result[2])
                && ValueMediator<bool>::tryRetrieve(state, index, result[1])
                && ValueMediator<bool>::tryRetrieve(state, index, result[0]);
    // If successful, set value to the Type casted result of the pop.
    if (success) {
        flags = vui::WidgetFlags{
            result[0], result[1], result[2], result[3], result[4],
            result[5], result[6], result[7], result[8]
        };
    }
    // Return success of pop.
    return success;
}

// Provides support for pushing and popping Length.
vui::Length vscript::lua::ValueMediator<vui::Length, void>::defaultValue() {
    return vui::Length{ 0, { vui::DimensionType::PIXEL } };
}

i32 vscript::lua::ValueMediator<vui::Length, void>::getValueCount() {
    return 2;
}

i32 vscript::lua::ValueMediator<vui::Length, void>::push(Handle state, vui::Length value) {
    return ValueMediator<f32>::push(state, value.x)
         + ValueMediator<vui::DimensionType>::push(state, value.dimension.x);
}

vui::Length vscript::lua::ValueMediator<vui::Length, void>::pop(Handle state) {
    vui::Length length;

    length.dimension.x = ValueMediator<vui::DimensionType>::pop(state);
    length.x           = ValueMediator<f32>::pop(state);

    return length;
}

bool vscript::lua::ValueMediator<vui::Length, void>::tryPop(Handle state, OUT vui::Length& value) {
    vui::Length result;
    // Try to pop the length.
    bool success = ValueMediator<vui::DimensionType>::tryPop(state, result.dimension.x)
                && ValueMediator<f32>::tryPop(state, result.x);
    // If successful, set value to the popped Length.
    if (success) {
        value = result;
    }
    // Return success of pop.
    return success;
}

vui::Length vscript::lua::ValueMediator<vui::Length, void>::retrieve(Handle state, ui32 index) {
    vui::Length length;

    length.dimension.x = ValueMediator<vui::DimensionType>::retrieve(state, index);
    length.x           = ValueMediator<f32>::retrieve(state, index);

    return length;
}

bool vscript::lua::ValueMediator<vui::Length, void>::tryRetrieve(Handle state, ui32 index, OUT vui::Length& value) {
    vui::Length result;
    // Try to pop the underlying enum type.
    bool success = ValueMediator<vui::DimensionType>::tryRetrieve(state, index, result.dimension.x)
                && ValueMediator<f32>::tryRetrieve(state, index, result.x);
    // If successful, set value to the Type casted result of the pop.
    if (success) {
        value = result;
    }
    // Return success of pop.
    return success;
}

// Provides support for pushing and popping Length2.
vui::Length2 vscript::lua::ValueMediator<vui::Length2, void>::defaultValue() {
    return vui::Length2{ 0, 0, { vui::DimensionType::PIXEL, vui::DimensionType::PIXEL } };
}

i32 vscript::lua::ValueMediator<vui::Length2, void>::getValueCount() {
    return 4;
}

i32 vscript::lua::ValueMediator<vui::Length2, void>::push(Handle state, const vui::Length2& value) {
    return ValueMediator<vui::DimensionType>::push(state, value.dimension.y)
         + ValueMediator<f32>::push(state, value.x)
         + ValueMediator<vui::DimensionType>::push(state, value.dimension.x)
         + ValueMediator<f32>::push(state, value.y);
}

vui::Length2 vscript::lua::ValueMediator<vui::Length2, void>::pop(Handle state) {
    vui::Length2 result;

    result.dimension.y = ValueMediator<vui::DimensionType>::pop(state);
    result.y = ValueMediator<f32>::pop(state);
    result.dimension.x = ValueMediator<vui::DimensionType>::pop(state);
    result.x = ValueMediator<f32>::pop(state);

    return result;
}

bool vscript::lua::ValueMediator<vui::Length2, void>::tryPop(Handle state, OUT vui::Length2& value) {
    vui::Length2 result;
    // Try to pop the length.
    bool success = ValueMediator<vui::DimensionType>::tryPop(state, result.dimension.y)
                && ValueMediator<f32>::tryPop(state, result.y)
                && ValueMediator<vui::DimensionType>::tryPop(state, result.dimension.x)
                && ValueMediator<f32>::tryPop(state, result.x);
    // If successful, set value to the popped Length2.
    if (success) {
        value = result;
    }
    // Return success of pop.
    return success;
}

vui::Length2 vscript::lua::ValueMediator<vui::Length2, void>::retrieve(Handle state, ui32 index) {
    vui::Length2 result;

    result.dimension.y = ValueMediator<vui::DimensionType>::retrieve(state, index);
    result.y = ValueMediator<f32>::retrieve(state, index);
    result.dimension.x = ValueMediator<vui::DimensionType>::retrieve(state, index);
    result.x = ValueMediator<f32>::retrieve(state, index);

    return result;
}

bool vscript::lua::ValueMediator<vui::Length2, void>::tryRetrieve(Handle state, ui32 index, OUT vui::Length2& value) {
    vui::Length2 result;
    // Try to pop the underlying enum type.
    bool success = ValueMediator<vui::DimensionType>::tryRetrieve(state, index, result.dimension.y)
                && ValueMediator<f32>::tryRetrieve(state, index, result.y)
                && ValueMediator<vui::DimensionType>::tryRetrieve(state, index, result.dimension.x)
                && ValueMediator<f32>::tryRetrieve(state, index, result.x);
    // If successful, set value to the Type casted result of the pop.
    if (success) {
        value = result;
    }
    // Return success of pop.
    return success;
}

// Provides support for pushing and popping Length4.
vui::Length4 vscript::lua::ValueMediator<vui::Length4, void>::defaultValue() {
    return vui::Length4{ 0, 0, 0, 0, { vui::DimensionType::PIXEL, vui::DimensionType::PIXEL, vui::DimensionType::PIXEL, vui::DimensionType::PIXEL } };
}

i32 vscript::lua::ValueMediator<vui::Length4, void>::getValueCount() {
    return 8;
}

i32 vscript::lua::ValueMediator<vui::Length4, void>::push(Handle state, const vui::Length4& value) {
    return ValueMediator<vui::DimensionType>::push(state, value.dimension.w)
         + ValueMediator<f32>::push(state, value.w)
         + ValueMediator<vui::DimensionType>::push(state, value.dimension.z)
         + ValueMediator<f32>::push(state, value.z)
         + ValueMediator<vui::DimensionType>::push(state, value.dimension.y)
         + ValueMediator<f32>::push(state, value.y)
         + ValueMediator<vui::DimensionType>::push(state, value.dimension.x)
         + ValueMediator<f32>::push(state, value.x);
}

vui::Length4 vscript::lua::ValueMediator<vui::Length4, void>::pop(Handle state) {
    vui::Length4 result;

    result.dimension.w = ValueMediator<vui::DimensionType>::pop(state);
    result.w = ValueMediator<f32>::pop(state);
    result.dimension.z = ValueMediator<vui::DimensionType>::pop(state);
    result.z = ValueMediator<f32>::pop(state);
    result.dimension.y = ValueMediator<vui::DimensionType>::pop(state);
    result.y = ValueMediator<f32>::pop(state);
    result.dimension.x = ValueMediator<vui::DimensionType>::pop(state);
    result.x = ValueMediator<f32>::pop(state);

    return result;
}

bool vscript::lua::ValueMediator<vui::Length4, void>::tryPop(Handle state, OUT vui::Length4& value) {
    vui::Length4 result;
    // Try to pop the length.
    bool success = ValueMediator<vui::DimensionType>::tryPop(state, result.dimension.w)
                && ValueMediator<f32>::tryPop(state, result.w)
                && ValueMediator<vui::DimensionType>::tryPop(state, result.dimension.z)
                && ValueMediator<f32>::tryPop(state, result.z)
                && ValueMediator<vui::DimensionType>::tryPop(state, result.dimension.y)
                && ValueMediator<f32>::tryPop(state, result.y)
                && ValueMediator<vui::DimensionType>::tryPop(state, result.dimension.x)
                && ValueMediator<f32>::tryPop(state, result.x);
    // If successful, set value to the popped Length4.
    if (success) {
        value = result;
    }
    // Return success of pop.
    return success;
}

vui::Length4 vscript::lua::ValueMediator<vui::Length4, void>::retrieve(Handle state, ui32 index) {
    vui::Length4 result;

    result.dimension.w = ValueMediator<vui::DimensionType>::retrieve(state, index);
    result.w = ValueMediator<f32>::retrieve(state, index);
    result.dimension.z = ValueMediator<vui::DimensionType>::retrieve(state, index);
    result.z = ValueMediator<f32>::retrieve(state, index);
    result.dimension.y = ValueMediator<vui::DimensionType>::retrieve(state, index);
    result.y = ValueMediator<f32>::retrieve(state, index);
    result.dimension.x = ValueMediator<vui::DimensionType>::retrieve(state, index);
    result.x = ValueMediator<f32>::retrieve(state, index);

    return result;
}

bool vscript::lua::ValueMediator<vui::Length4, void>::tryRetrieve(Handle state, ui32 index, OUT vui::Length4& value) {
    vui::Length4 result;
    // Try to pop the underlying enum type.
    bool success = ValueMediator<vui::DimensionType>::tryRetrieve(state, index, result.dimension.w)
                && ValueMediator<f32>::tryRetrieve(state, index, result.w)
                && ValueMediator<vui::DimensionType>::tryRetrieve(state, index, result.dimension.z)
                && ValueMediator<f32>::tryRetrieve(state, index, result.z)
                && ValueMediator<vui::DimensionType>::tryRetrieve(state, index, result.dimension.y)
                && ValueMediator<f32>::tryRetrieve(state, index, result.y)
                && ValueMediator<vui::DimensionType>::tryRetrieve(state, index, result.dimension.x)
                && ValueMediator<f32>::tryRetrieve(state, index, result.x);
    // If successful, set value to the Type casted result of the pop.
    if (success) {
        value = result;
    }
    // Return success of pop.
    return success;
}
