#include "Bezier.h"

#include <iostream>
#include <glm/ext/quaternion_common.hpp>

Bezier::Bezier(const glm::vec2 &start, const glm::vec2 &control0, const glm::vec2 &control1, const glm::vec2 &end):
    _start(start),
    _control0(control0),
    _control1(control1),
    _end(end) {
}

Bezier::Bezier(const glm::vec2 &control0, const glm::vec2 &control1):
    Bezier({0.0, 0.0}, control0, control1, {1.0, 1.0}){
}

double Bezier::x(double t) const {
    if (t <= 0.0) return _start.x;
    if (t >= 1.0) return _end.x;

    const double t2 = t * t;
    const double t3 = t2 * t;
    const double mt = 1.0 - t;
    const double mt2 = mt * mt;
    const double mt3 = mt2 * mt;

    return mt3 * _start.x
        + 3.0 * mt2 * t *_control0.x
        + 3.0 * mt * t2 *_control1.x
        + t3 *_end.x;
}

double Bezier::y(double t) const {
    if (t <= 0.0) return _start.y;
    if (t >= 1.0) return _end.y;

    const double t2 = t * t;
    const double t3 = t2 * t;
    const double mt = 1.0 - t;
    const double mt2 = mt * mt;
    const double mt3 = mt2 * mt;

    return mt3 * _start.y
        + 3.0 * mt2 * t *_control0.y
        + 3.0 * mt * t2 *_control1.y
        + t3 *_end.y;
}

double Bezier::derivative_x(double t) const {
    if (t <= 0.0) return 3.0 * (_control0.x - _start.x);
    if (t >= 1.0) return 3.0 * (_end.x - _control1.x);

    const double t2 = t * t;
    const double mt = 1.0 - t;
    const double mt2 = mt * mt;

    return 3.0 * mt2 * (_control0.x - _start.x)
        + 6.0 * mt * t * (_control1.x - _control0.x)
        + 3.0 * t2 * (_end.x - _control1.x);
}

double Bezier::derivative_y(double t) const {
    if (t <= 0.0) return 3.0 * (_control0.y - _start.y);
    if (t >= 1.0) return 3.0 * (_end.y - _control1.y);

    const double t2 = t * t;
    const double mt = 1.0 - t;
    const double mt2 = mt * mt;

    return 3.0 * mt2 * (_control0.y - _start.y)
        + 6.0 * mt * t * (_control1.y - _control0.y)
        + 3.0 * t2 * (_end.y - _control1.y);
}

double Bezier::inverse_x(double t, size_t accuracy) const {
    if (t <= 0) return _start.x;
    if (t >= 1.0) return _end.x;
    double l{_start.x}, r{_end.x};
    double root{};
    for (uint8_t i = 0; i < 3; i++) {
        root = l + (r - l) / 2;
        double value = x(root) - t;
        if (value > 0.0) {
            r = root;
        } else {
            l = root;
        }
    }

    for (size_t i = 0; i < accuracy; i++) {
        double current_x = x(root) - t;
        double current_x_ = derivative_x(root);
        root = root - current_x / current_x_;
    }
    return root;
}

double Bezier::inverse_y(double t, size_t accuracy) const {
    if (t <= 0) return _start.y;
    if (t >= 1.0) return _end.y;
    double l{_start.y}, r{_end.y};
    double root{};
    for (uint8_t i = 0; i < 3; i++) {
        root = l + (r - l) / 2;
        double value = y(root) - t;
        if (value > 0.0) {
            r = root;
        } else {
            l = root;
        }
    }

    for (size_t i = 0; i < accuracy; i++) {
        double current_y = y(root) - t;
        double current_y_ = derivative_y(root);
        root = root - current_y / current_y_;
    }

    return root;
}


glm::vec2 Bezier::get(double t) const {
    return {x(t), y(t)};
}

glm::vec2 Bezier::operator[](double t) const {
    return {x(t), y(t)};
}
