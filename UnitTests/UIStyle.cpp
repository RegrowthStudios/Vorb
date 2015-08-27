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

vui::UIStyle& vui::UIStyle::operator+(const vui::UIStyle &q) const {
    UIStyle* a = new UIStyle();
    color4 cThis = this->color;
    color4 cQ = q.color;

    int flags = this->flags;

    if (UIUtils::checkFlag(&flags, StyleFlags::COLOR)) {
        if (UIUtils::checkFlag(&flags, StyleFlags::R)) {
            a->color.r = (cThis.r + cQ.r) <= 255 ? (cThis.r + cQ.r) : 255;
        }

        if (UIUtils::checkFlag(&flags, StyleFlags::G)) {
            a->color.g = (cThis.g + cQ.g) <= 255 ? (cThis.g + cQ.g) : 255;
        }

        if (UIUtils::checkFlag(&flags, StyleFlags::B)) {
            a->color.b = (cThis.b + cQ.b) <= 255 ? (cThis.b + cQ.b) : 255;
        }

        if (UIUtils::checkFlag(&flags, StyleFlags::A)) {
            a->color.a = (cThis.a + cQ.a) <= 255 ? (cThis.a + cQ.a) : 255;
        }

        if (UIUtils::checkFlag(&flags, StyleFlags::RGB)) {
            a->color.r = (cThis.r + cQ.r) <= 255 ? (cThis.r + cQ.r) : 255;
            a->color.g = (cThis.g + cQ.g) <= 255 ? (cThis.g + cQ.g) : 255;
            a->color.b = (cThis.b + cQ.b) <= 255 ? (cThis.b + cQ.b) : 255;
        }

        if (UIUtils::checkFlag(&flags, StyleFlags::RGBA)) {
            a->color.r = (cThis.r + cQ.r) <= 255 ? (cThis.r + cQ.r) : 255;
            a->color.g = (cThis.g + cQ.g) <= 255 ? (cThis.g + cQ.g) : 255;
            a->color.b = (cThis.b + cQ.b) <= 255 ? (cThis.b + cQ.b) : 255;
            a->color.a = (cThis.a + cQ.a) <= 255 ? (cThis.a + cQ.a) : 255;
        }
    }

    return *a;
}