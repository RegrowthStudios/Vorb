#include "stdafx.h"
#include "UIStyle.h"
#include "UIUtils.h"

vui::UIStyle::UIStyle() {

}

vui::UIStyle::~UIStyle() {

}

int vui::UIStyle::getFlags() {
    return this->flags;
}

void vui::UIStyle::setFlags(int flags) {
    this->flags = flags;
}

vui::UIStyle operator+(vui::UIStyle p, const vui::UIStyle &q) {
    vui::UIStyle* a = new vui::UIStyle();
    color4 cThis = p.color;
    color4 cQ = q.color;

    int flags = p.getFlags();

    if (vui::UIUtils::checkFlag(&flags, vui::StyleFlags::COLOR)) {
        if (vui::UIUtils::checkFlag(&flags, vui::StyleFlags::R)) {
            a->color.r = (cThis.r + cQ.r) <= 255 ? (cThis.r + cQ.r) : 255;
        }

        if (vui::UIUtils::checkFlag(&flags, vui::StyleFlags::G)) {
            a->color.g = (cThis.g + cQ.g) <= 255 ? (cThis.g + cQ.g) : 255;
        }

        if (vui::UIUtils::checkFlag(&flags, vui::StyleFlags::B)) {
            a->color.b = (cThis.b + cQ.b) <= 255 ? (cThis.b + cQ.b) : 255;
        }

        if (vui::UIUtils::checkFlag(&flags, vui::StyleFlags::A)) {
            a->color.a = (cThis.a + cQ.a) <= 255 ? (cThis.a + cQ.a) : 255;
        }

        if (vui::UIUtils::checkFlag(&flags, vui::StyleFlags::RGB)) {
            a->color.r = (cThis.r + cQ.r) <= 255 ? (cThis.r + cQ.r) : 255;
            a->color.g = (cThis.g + cQ.g) <= 255 ? (cThis.g + cQ.g) : 255;
            a->color.b = (cThis.b + cQ.b) <= 255 ? (cThis.b + cQ.b) : 255;
        }

        if (vui::UIUtils::checkFlag(&flags, vui::StyleFlags::RGBA)) {
            a->color.r = (cThis.r + cQ.r) <= 255 ? (cThis.r + cQ.r) : 255;
            a->color.g = (cThis.g + cQ.g) <= 255 ? (cThis.g + cQ.g) : 255;
            a->color.b = (cThis.b + cQ.b) <= 255 ? (cThis.b + cQ.b) : 255;
            a->color.a = (cThis.a + cQ.a) <= 255 ? (cThis.a + cQ.a) : 255;
        }
    }

    return *a;
}