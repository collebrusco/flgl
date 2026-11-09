#include "UI.h"


void RootUIelement::center() {
    layout = UIlayout(
        UIpin::CENTERCENTER,
        UIpin::CENTERCENTER,
        glm::vec2(0.f),
        glm::vec2(window.frame)
    );
}

RootUIelement::RootUIelement() : UIelement() {
    center();
}

void RootUIelement::on_layout() {
    center();
}

