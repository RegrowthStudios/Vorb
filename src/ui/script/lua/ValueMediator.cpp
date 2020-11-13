#include "Vorb/stdafx.h"
#include "Vorb/ui/script/lua/ValueMediator.h"

#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/WindowEventDispatcher.h"
#include "Vorb/ui/widgets/Widget.h"

/************************************************************************/
/* Clipping                                                             */
/************************************************************************/

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




/************************************************************************/
/* Dock                                                                 */
/************************************************************************/

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




/************************************************************************/
/* WidgetFlags                                                          */
/************************************************************************/

vui::WidgetFlags vscript::lua::ValueMediator<vui::WidgetFlags, void>::defaultValue() {
    return vui::WidgetFlags{ false, false, false, false, false, false, false, false, false, false };
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
    bool result[10];
    // Try to pop the length.
    bool success = ValueMediator<bool>::tryPop(state, result[9])
                && ValueMediator<bool>::tryPop(state, result[8])
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
            result[5], result[6], result[7], result[8], result[9]
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
    bool result[10];
    // Try to pop the underlying enum type.
    bool success = ValueMediator<bool>::tryRetrieve(state, index, result[9])
                && ValueMediator<bool>::tryRetrieve(state, index, result[8])
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
            result[5], result[6], result[7], result[8], result[9]
        };
    }
    // Return success of pop.
    return success;
}




/************************************************************************/
/* Length                                                               */
/************************************************************************/

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




/************************************************************************/
/* Length2                                                              */
/************************************************************************/

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




/************************************************************************/
/* Length4                                                              */
/************************************************************************/

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




/************************************************************************/
/* MouseEvent                                                           */
/************************************************************************/

vui::MouseEvent vscript::lua::ValueMediator<vui::MouseEvent, void>::defaultValue() {
    return vui::MouseEvent{ 0, 0 };
}

i32 vscript::lua::ValueMediator<vui::MouseEvent, void>::getValueCount() {
    return 2;
}

i32 vscript::lua::ValueMediator<vui::MouseEvent, void>::push(Handle state, const vui::MouseEvent& mouseEvent) {
    return ValueMediator<i32>::push(state, mouseEvent.x)
         + ValueMediator<i32>::push(state, mouseEvent.y);
}

vui::MouseEvent vscript::lua::ValueMediator<vui::MouseEvent, void>::pop(Handle state) {
    vui::MouseEvent mouseEvent;

    mouseEvent.y = ValueMediator<i32>::pop(state);
    mouseEvent.x = ValueMediator<i32>::pop(state);

    return mouseEvent;
}

bool vscript::lua::ValueMediator<vui::MouseEvent, void>::tryPop(Handle state, OUT vui::MouseEvent& mouseEvent) {
    vui::MouseEvent result;
    // Try to pop the length.
    bool success = ValueMediator<i32>::tryPop(state, result.y)
                && ValueMediator<i32>::tryPop(state, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseEvent = result;
    }
    // Return success of pop.
    return success;
}

vui::MouseEvent vscript::lua::ValueMediator<vui::MouseEvent, void>::retrieve(Handle state, ui32 index) {
    vui::MouseEvent mouseEvent;

    mouseEvent.y = ValueMediator<i32>::retrieve(state, index);
    mouseEvent.x = ValueMediator<i32>::retrieve(state, index);

    return mouseEvent;
}

bool vscript::lua::ValueMediator<vui::MouseEvent, void>::tryRetrieve(Handle state, ui32 index, OUT vui::MouseEvent& mouseEvent) {
    vui::MouseEvent result;
    // Try to pop the length.
    bool success = ValueMediator<i32>::tryRetrieve(state, index, result.y)
                && ValueMediator<i32>::tryRetrieve(state, index, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseEvent = result;
    }
    // Return success of pop.
    return success;
}




/************************************************************************/
/* MouseButtonEvent                                                     */
/************************************************************************/

// TODO(Matthew): Pass MouseButton enum consts to env.

vui::MouseButtonEvent vscript::lua::ValueMediator<vui::MouseButtonEvent, void>::defaultValue() {
    return vui::MouseButtonEvent{ 0, 0, vui::MouseButton::UNKNOWN, 0 };
}

i32 vscript::lua::ValueMediator<vui::MouseButtonEvent, void>::getValueCount() {
    return 4;
}

i32 vscript::lua::ValueMediator<vui::MouseButtonEvent, void>::push(Handle state, const vui::MouseButtonEvent& mouseButtonEvent) {
    return ValueMediator<i32>::push(state, mouseButtonEvent.x)
         + ValueMediator<i32>::push(state, mouseButtonEvent.y)
         + ValueMediator<vui::MouseButton>::push(state, mouseButtonEvent.button)
         + ValueMediator<ui8>::push(state, mouseButtonEvent.clicks);
}

vui::MouseButtonEvent vscript::lua::ValueMediator<vui::MouseButtonEvent, void>::pop(Handle state) {
    vui::MouseButtonEvent mouseButtonEvent;

    mouseButtonEvent.clicks = ValueMediator<ui8>::pop(state);
    mouseButtonEvent.button = ValueMediator<vui::MouseButton>::pop(state);
    mouseButtonEvent.y      = ValueMediator<i32>::pop(state);
    mouseButtonEvent.x      = ValueMediator<i32>::pop(state);

    return mouseButtonEvent;
}

bool vscript::lua::ValueMediator<vui::MouseButtonEvent, void>::tryPop(Handle state, OUT vui::MouseButtonEvent& mouseButtonEvent) {
    vui::MouseButtonEvent result;
    // Try to pop the length.
    bool success = ValueMediator<ui8>::tryPop(state, result.clicks)
                && ValueMediator<vui::MouseButton>::tryPop(state, result.button)
                && ValueMediator<i32>::tryPop(state, result.y)
                && ValueMediator<i32>::tryPop(state, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseButtonEvent = result;
    }
    // Return success of pop.
    return success;
}

vui::MouseButtonEvent vscript::lua::ValueMediator<vui::MouseButtonEvent, void>::retrieve(Handle state, ui32 index) {
    vui::MouseButtonEvent mouseButtonEvent;

    mouseButtonEvent.clicks = ValueMediator<ui8>::retrieve(state, index);
    mouseButtonEvent.button = ValueMediator<vui::MouseButton>::retrieve(state, index);
    mouseButtonEvent.y      = ValueMediator<i32>::retrieve(state, index);
    mouseButtonEvent.x      = ValueMediator<i32>::retrieve(state, index);

    return mouseButtonEvent;
}

bool vscript::lua::ValueMediator<vui::MouseButtonEvent, void>::tryRetrieve(Handle state, ui32 index, OUT vui::MouseButtonEvent& mouseButtonEvent) {
    vui::MouseButtonEvent result;
    // Try to pop the length.
    bool success = ValueMediator<ui8>::tryRetrieve(state, index, result.clicks)
                && ValueMediator<vui::MouseButton>::tryRetrieve(state, index, result.button)
                && ValueMediator<i32>::tryRetrieve(state, index, result.y)
                && ValueMediator<i32>::tryRetrieve(state, index, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseButtonEvent = result;
    }
    // Return success of pop.
    return success;
}




/************************************************************************/
/* MouseMotionEvent                                                     */
/************************************************************************/

// TODO(Matthew): Pass MouseButton enum consts to env.

vui::MouseMotionEvent vscript::lua::ValueMediator<vui::MouseMotionEvent, void>::defaultValue() {
    return vui::MouseMotionEvent{ 0, 0, 0, 0 };
}

i32 vscript::lua::ValueMediator<vui::MouseMotionEvent, void>::getValueCount() {
    return 4;
}

i32 vscript::lua::ValueMediator<vui::MouseMotionEvent, void>::push(Handle state, const vui::MouseMotionEvent& mouseMotionEvent) {
    return ValueMediator<i32>::push(state, mouseMotionEvent.x)
         + ValueMediator<i32>::push(state, mouseMotionEvent.y)
         + ValueMediator<i32>::push(state, mouseMotionEvent.dx)
         + ValueMediator<i32>::push(state, mouseMotionEvent.dy);
}

vui::MouseMotionEvent vscript::lua::ValueMediator<vui::MouseMotionEvent, void>::pop(Handle state) {
    vui::MouseMotionEvent mouseMotionEvent;

    mouseMotionEvent.dy = ValueMediator<i32>::pop(state);
    mouseMotionEvent.dx = ValueMediator<i32>::pop(state);
    mouseMotionEvent.y  = ValueMediator<i32>::pop(state);
    mouseMotionEvent.x  = ValueMediator<i32>::pop(state);

    return mouseMotionEvent;
}

bool vscript::lua::ValueMediator<vui::MouseMotionEvent, void>::tryPop(Handle state, OUT vui::MouseMotionEvent& mouseMotionEvent) {
    vui::MouseMotionEvent result;
    // Try to pop the length.
    bool success = ValueMediator<i32>::tryPop(state, result.dy)
                && ValueMediator<i32>::tryPop(state, result.dx)
                && ValueMediator<i32>::tryPop(state, result.y)
                && ValueMediator<i32>::tryPop(state, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseMotionEvent = result;
    }
    // Return success of pop.
    return success;
}

vui::MouseMotionEvent vscript::lua::ValueMediator<vui::MouseMotionEvent, void>::retrieve(Handle state, ui32 index) {
    vui::MouseMotionEvent mouseMotionEvent;

    mouseMotionEvent.dy = ValueMediator<i32>::retrieve(state, index);
    mouseMotionEvent.dx = ValueMediator<i32>::retrieve(state, index);
    mouseMotionEvent.y  = ValueMediator<i32>::retrieve(state, index);
    mouseMotionEvent.x  = ValueMediator<i32>::retrieve(state, index);

    return mouseMotionEvent;
}

bool vscript::lua::ValueMediator<vui::MouseMotionEvent, void>::tryRetrieve(Handle state, ui32 index, OUT vui::MouseMotionEvent& mouseMotionEvent) {
    vui::MouseMotionEvent result;
    // Try to pop the length.
    bool success = ValueMediator<i32>::tryRetrieve(state, index, result.dy)
                && ValueMediator<i32>::tryRetrieve(state, index, result.dx)
                && ValueMediator<i32>::tryRetrieve(state, index, result.y)
                && ValueMediator<i32>::tryRetrieve(state, index, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseMotionEvent = result;
    }
    // Return success of pop.
    return success;
}




/************************************************************************/
/* MouseWheelEvent                                                     */
/************************************************************************/

vui::MouseWheelEvent vscript::lua::ValueMediator<vui::MouseWheelEvent, void>::defaultValue() {
    return vui::MouseWheelEvent{ 0, 0, 0, 0, 0, 0 };
}

i32 vscript::lua::ValueMediator<vui::MouseWheelEvent, void>::getValueCount() {
    return 6;
}

i32 vscript::lua::ValueMediator<vui::MouseWheelEvent, void>::push(Handle state, const vui::MouseWheelEvent& mouseWheelEvent) {
    return ValueMediator<i32>::push(state, mouseWheelEvent.x)
         + ValueMediator<i32>::push(state, mouseWheelEvent.y)
         + ValueMediator<i32>::push(state, mouseWheelEvent.dx)
         + ValueMediator<i32>::push(state, mouseWheelEvent.dy)
         + ValueMediator<i32>::push(state, mouseWheelEvent.sx)
         + ValueMediator<i32>::push(state, mouseWheelEvent.sy);
}

vui::MouseWheelEvent vscript::lua::ValueMediator<vui::MouseWheelEvent, void>::pop(Handle state) {
    vui::MouseWheelEvent mouseWheelEvent;

    mouseWheelEvent.sy = ValueMediator<i32>::pop(state);
    mouseWheelEvent.sx = ValueMediator<i32>::pop(state);
    mouseWheelEvent.dy = ValueMediator<i32>::pop(state);
    mouseWheelEvent.dx = ValueMediator<i32>::pop(state);
    mouseWheelEvent.y  = ValueMediator<i32>::pop(state);
    mouseWheelEvent.x  = ValueMediator<i32>::pop(state);

    return mouseWheelEvent;
}

bool vscript::lua::ValueMediator<vui::MouseWheelEvent, void>::tryPop(Handle state, OUT vui::MouseWheelEvent& mouseWheelEvent) {
    vui::MouseWheelEvent result;
    // Try to pop the length.
    bool success = ValueMediator<i32>::tryPop(state, result.sy)
                && ValueMediator<i32>::tryPop(state, result.sx)
                && ValueMediator<i32>::tryPop(state, result.dy)
                && ValueMediator<i32>::tryPop(state, result.dx)
                && ValueMediator<i32>::tryPop(state, result.y)
                && ValueMediator<i32>::tryPop(state, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseWheelEvent = result;
    }
    // Return success of pop.
    return success;
}

vui::MouseWheelEvent vscript::lua::ValueMediator<vui::MouseWheelEvent, void>::retrieve(Handle state, ui32 index) {
    vui::MouseWheelEvent mouseWheelEvent;

    mouseWheelEvent.sy = ValueMediator<i32>::retrieve(state, index);
    mouseWheelEvent.sx = ValueMediator<i32>::retrieve(state, index);
    mouseWheelEvent.dy = ValueMediator<i32>::retrieve(state, index);
    mouseWheelEvent.dx = ValueMediator<i32>::retrieve(state, index);
    mouseWheelEvent.y  = ValueMediator<i32>::retrieve(state, index);
    mouseWheelEvent.x  = ValueMediator<i32>::retrieve(state, index);

    return mouseWheelEvent;
}

bool vscript::lua::ValueMediator<vui::MouseWheelEvent, void>::tryRetrieve(Handle state, ui32 index, OUT vui::MouseWheelEvent& mouseWheelEvent) {
    vui::MouseWheelEvent result;
    // Try to pop the length.
    bool success = ValueMediator<i32>::tryRetrieve(state, index, result.sy)
                && ValueMediator<i32>::tryRetrieve(state, index, result.sx)
                && ValueMediator<i32>::tryRetrieve(state, index, result.dy)
                && ValueMediator<i32>::tryRetrieve(state, index, result.dx)
                && ValueMediator<i32>::tryRetrieve(state, index, result.y)
                && ValueMediator<i32>::tryRetrieve(state, index, result.x);
    // If successful, set value to the popped Clipping.
    if (success) {
        mouseWheelEvent = result;
    }
    // Return success of pop.
    return success;
}




/************************************************************************/
/* WindowResizeEvent                                                    */
/************************************************************************/

vui::WindowResizeEvent vscript::lua::ValueMediator<vui::WindowResizeEvent, void>::defaultValue() {
    return vui::WindowResizeEvent{ 0, 0 };
}

i32 vscript::lua::ValueMediator<vui::WindowResizeEvent, void>::getValueCount() {
    return 2;
}

i32 vscript::lua::ValueMediator<vui::WindowResizeEvent, void>::push(Handle state, const vui::WindowResizeEvent& windowResizeEvent) {
    return ValueMediator<ui32>::push(state, windowResizeEvent.w)
         + ValueMediator<ui32>::push(state, windowResizeEvent.h);
}

vui::WindowResizeEvent vscript::lua::ValueMediator<vui::WindowResizeEvent, void>::pop(Handle state) {
    vui::WindowResizeEvent windowResizeEvent;

    windowResizeEvent.h  = ValueMediator<ui32>::pop(state);
    windowResizeEvent.w  = ValueMediator<ui32>::pop(state);

    return windowResizeEvent;
}

bool vscript::lua::ValueMediator<vui::WindowResizeEvent, void>::tryPop(Handle state, OUT vui::WindowResizeEvent& windowResizeEvent) {
    vui::WindowResizeEvent result;
    // Try to pop the length.
    bool success = ValueMediator<ui32>::tryPop(state, result.h)
                && ValueMediator<ui32>::tryPop(state, result.w);
    // If successful, set value to the popped Clipping.
    if (success) {
        windowResizeEvent = result;
    }
    // Return success of pop.
    return success;
}

vui::WindowResizeEvent vscript::lua::ValueMediator<vui::WindowResizeEvent, void>::retrieve(Handle state, ui32 index) {
    vui::WindowResizeEvent windowResizeEvent;

    windowResizeEvent.h  = ValueMediator<ui32>::retrieve(state, index);
    windowResizeEvent.w  = ValueMediator<ui32>::retrieve(state, index);

    return windowResizeEvent;
}

bool vscript::lua::ValueMediator<vui::WindowResizeEvent, void>::tryRetrieve(Handle state, ui32 index, OUT vui::WindowResizeEvent& windowResizeEvent) {
    vui::WindowResizeEvent result;
    // Try to pop the length.
    bool success = ValueMediator<ui32>::tryRetrieve(state, index, result.h)
                && ValueMediator<ui32>::tryRetrieve(state, index, result.w);
    // If successful, set value to the popped Clipping.
    if (success) {
        windowResizeEvent = result;
    }
    // Return success of pop.
    return success;
}




/************************************************************************/
/* WindowFileEvent                                                      */
/************************************************************************/

vui::WindowFileEvent vscript::lua::ValueMediator<vui::WindowFileEvent, void>::defaultValue() {
    return vui::WindowFileEvent{ nullptr };
}

i32 vscript::lua::ValueMediator<vui::WindowFileEvent, void>::getValueCount() {
    return 1;
}

i32 vscript::lua::ValueMediator<vui::WindowFileEvent, void>::push(Handle state, const vui::WindowFileEvent& windowFileEvent) {
    return ValueMediator<const cString>::push(state, windowFileEvent.file);
}

vui::WindowFileEvent vscript::lua::ValueMediator<vui::WindowFileEvent, void>::pop(Handle state) {
    vui::WindowFileEvent windowFileEvent;

    windowFileEvent.file = ValueMediator<const cString>::pop(state);

    return windowFileEvent;
}

bool vscript::lua::ValueMediator<vui::WindowFileEvent, void>::tryPop(Handle state, OUT vui::WindowFileEvent& windowFileEvent) {
    vui::WindowFileEvent result;
    // Try to pop the length.
    bool success = ValueMediator<const cString>::tryPop(state, result.file);
    // If successful, set value to the popped Clipping.
    if (success) {
        windowFileEvent = result;
    }
    // Return success of pop.
    return success;
}

vui::WindowFileEvent vscript::lua::ValueMediator<vui::WindowFileEvent, void>::retrieve(Handle state, ui32 index) {
    vui::WindowFileEvent windowFileEvent;

    windowFileEvent.file = ValueMediator<const cString>::retrieve(state, index);

    return windowFileEvent;
}

bool vscript::lua::ValueMediator<vui::WindowFileEvent, void>::tryRetrieve(Handle state, ui32 index, OUT vui::WindowFileEvent& windowFileEvent) {
    vui::WindowFileEvent result;
    // Try to pop the length.
    bool success = ValueMediator<const cString>::tryRetrieve(state, index, result.file);
    // If successful, set value to the popped Clipping.
    if (success) {
        windowFileEvent = result;
    }
    // Return success of pop.
    return success;
}
