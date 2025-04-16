#include "geometry.h"
#include <iostream>
#include <cassert>

// LineIntersect
LineIntersect::LineIntersect(Ray const& ray, float t_val, bool did_hit)
    : incoming(&ray), t(t_val), hit(did_hit) {}

glm::vec3 LineIntersect::point() const {
    return incoming->at(t);
}

LineIntersect LineIntersect::miss() {
    return LineIntersect(Ray(glm::vec3(0.0f), glm::vec3(0.0f)), 0.0f, false);
}

// DualLineIntersect
DualLineIntersect::DualLineIntersect(glm::vec3 const& p1, glm::vec3 const& p2, float dist, float u_, float v_)
    : point1(p1), point2(p2), distance(dist), u(u_), v(v_) {}

glm::vec3 DualLineIntersect::vec() const {
    return point2 - point1;
}

bool DualLineIntersect::intersects() const {
    return distance < 1e-5f;
}

// Sphere
AABB Sphere::get_aabb() const {
    return AABB(center - glm::vec3(rad), center + glm::vec3(rad));
}

// AABB
bool AABB::inside(glm::vec3 mpos) const {
    return all(greaterThanEqual(mpos, min)) && all(lessThanEqual(mpos, max));
}

void AABB::merge(AABB const& other) {
    min = glm::min(min, other.min);
    max = glm::max(max, other.max);
}

void AABB::expand(glm::vec3 const& p) {
    min = glm::min(min, p);
    max = glm::max(max, p);
}

bool AABB::isect(AABB const& other) const {
    return all(lessThanEqual(min, other.max)) && all(greaterThanEqual(max, other.min));
}

bool AABB::contains(AABB const& other) const {
    return all(lessThanEqual(min, other.min)) && all(greaterThanEqual(max, other.max));
}

bool AABB::degen() const {
    return glm::any(glm::lessThanEqual(max - min, glm::vec3(0.0f)));
}

glm::vec3 AABB::size() const {
    return max - min;
}

glm::vec3 AABB::extent() const {
    return 0.5f * size();
}

glm::vec3 AABB::middle() const {
    return 0.5f * (min + max);
}

AABB AABB::from_merge(AABB const& a, AABB const& b) {
    return AABB(glm::min(a.min, b.min), glm::max(a.max, b.max));
}

AABB AABB::from_isect(AABB const& a, AABB const& b) {
    return AABB(glm::max(a.min, b.min), glm::min(a.max, b.max));
}

AABB AABB::from_minmax(glm::vec3 min, glm::vec3 max) {
    return AABB(min, max);
}

AABB AABB::from_minsize(glm::vec3 min, glm::vec3 size) {
    return AABB(min, min + size);
}

// Plane
Plane Plane::from_point_normal(glm::vec3 const& p, glm::vec3 const& n) {
    return Plane{p, glm::normalize(n)};
}

PointToPlane Plane::intersect(glm::vec3 const& point) const {
    glm::vec3 v = point - this->point;
    float d = glm::dot(v, normal);
    return {d, v};
}

// Triangle
Triangle::Triangle(glm::vec3 const& a_, glm::vec3 const& b_, glm::vec3 const& c_)
    : a(a_), b(b_), c(c_) {}

glm::vec3 Triangle::normal() const {
    return glm::normalize(glm::cross(b - a, c - a));
}

float Triangle::area() const {
    return 0.5f * glm::length(glm::cross(b - a, c - a));
}

glm::vec3 Triangle::barycentric(glm::vec3 const& p) const {
    glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    return glm::vec3(u, v, w);
}

bool Triangle::contains_point(glm::vec3 const& p) const {
    glm::vec3 bary = barycentric(p);
    return bary.x >= 0.0f && bary.y >= 0.0f && bary.z >= 0.0f;
}

// Segment
Segment::Segment(glm::vec3 const& s, glm::vec3 const& e)
    : start(s), end(e), dir(glm::normalize(e - s)) {}

float Segment::length() const {
    return glm::length(end - start);
}

float Segment::squared_length() const {
    return glm::dot(end - start, end - start);
}

AABB Segment::get_aabb() const {
    return AABB(glm::min(start, end), glm::max(start, end));
}

glm::vec3 Segment::at(float u) const {
    return start + dir * u;
}

Segment Segment::from_pos_dir_len(glm::vec3 const& p, glm::vec3 const& d, float l) {
    return Segment(p, p + d * l);
}

Segment Segment::from_ray_len(Ray const& ray, float length) {
    return Segment(ray.pos, ray.at(length));
}

Segment Segment::from_start_end(glm::vec3 const& s, glm::vec3 const& e) {
    return Segment(s, e);
}

DualLineIntersect Segment::closest_between(Segment const& seg1, Segment const& seg2) {
    glm::vec3 const& p1 = seg1.start;
    glm::vec3 const& p2 = seg2.start;
    glm::vec3 d1 = seg1.end - seg1.start;
    glm::vec3 d2 = seg2.end - seg2.start;
    glm::vec3 r = p1 - p2;

    float a = glm::dot(d1, d1);
    float e = glm::dot(d2, d2);
    float f = glm::dot(d2, r);

    float b = glm::dot(d1, d2);
    float c = glm::dot(d1, r);
    float denom = a * e - b * b;

    float s = 0.0f, t = 0.0f;

    if (denom > 1e-5f) {
        s = (b * f - c * e) / denom;
        t = (a * f - b * c) / denom;
    }

    s = glm::clamp(s, 0.0f, 1.0f);
    t = glm::clamp(t, 0.0f, 1.0f);

    glm::vec3 pt1 = p1 + s * d1;
    glm::vec3 pt2 = p2 + t * d2;

    return DualLineIntersect(pt1, pt2, glm::length(pt1 - pt2), s, t);
}

// Cylinder
Segment Cylinder::axis() const {
    return Segment(p1, p2);
}

// Disc: No methods

LineIntersect Ray::intersect(AABB const& aabb) const {
    float tmin = -1e38f, tmax = 1e38f;
    for (int i = 0; i < 3; ++i) {
        float inv_d = 1.0f / dir[i];
        float t1 = (aabb.min[i] - pos[i]) * inv_d;
        float t2 = (aabb.max[i] - pos[i]) * inv_d;
        if (t1 > t2) std::swap(t1, t2);
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmax < tmin) return LineIntersect(*this, 0.0f, false);
    }
    return LineIntersect(*this, tmin, true);
}

LineIntersect Ray::intersect(Sphere const& sphere) const {
    glm::vec3 oc = pos - sphere.center;
    float a = glm::dot(dir, dir);
    float b = 2.0f * glm::dot(oc, dir);
    float c = glm::dot(oc, oc) - sphere.rad * sphere.rad;
    float disc = b * b - 4 * a * c;
    if (disc < 0.0f) return LineIntersect(*this, 0.0f, false);
    float t = (-b - std::sqrt(disc)) / (2.0f * a);
    return LineIntersect(*this, t, t >= 0.0f);
}

LineIntersect Ray::intersect(Plane const& plane) const {
    float denom = glm::dot(plane.normal, dir);
    if (std::abs(denom) < 1e-6f) return LineIntersect(*this, 0.0f, false);
    float t = glm::dot(plane.point - pos, plane.normal) / denom;
    return LineIntersect(*this, t, t >= 0.0f);
}

LineIntersect Ray::intersect(Triangle const& tri) const {
    glm::vec3 edge1 = tri.b - tri.a;
    glm::vec3 edge2 = tri.c - tri.a;
    glm::vec3 h = glm::cross(dir, edge2);
    float a = glm::dot(edge1, h);
    if (std::abs(a) < 1e-6f) return LineIntersect(*this, 0.0f, false);
    float f = 1.0f / a;
    glm::vec3 s = pos - tri.a;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return LineIntersect(*this, 0.0f, false);
    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(dir, q);
    if (v < 0.0f || u + v > 1.0f) return LineIntersect(*this, 0.0f, false);
    float t = f * glm::dot(edge2, q);
    return LineIntersect(*this, t, t >= 0.0f);
}

LineIntersect Ray::intersect(Disc const& disc) const {
    Plane plane{disc.center, disc.normal};
    LineIntersect hit = intersect(plane);
    if (!hit.hit) return hit;
    glm::vec3 p = hit.point();
    if (glm::length(p - disc.center) <= disc.radius)
        return hit;
    return LineIntersect(*this, 0.0f, false);
}

LineIntersect Ray::intersect(Cylinder const& cyl) const {
    glm::vec3 d = cyl.p2 - cyl.p1;
    glm::vec3 m = pos - cyl.p1;
    glm::vec3 n = dir;

    float md = glm::dot(m, d);
    float nd = glm::dot(n, d);
    float dd = glm::dot(d, d);

    glm::vec3 a = m - d * (md / dd);
    glm::vec3 b = n - d * (nd / dd);

    float A = glm::dot(b, b);
    float B = 2.0f * glm::dot(a, b);
    float C = glm::dot(a, a) - cyl.radius * cyl.radius;

    float disc = B * B - 4.0f * A * C;
    if (disc < 0.0f) return LineIntersect(*this, 0.0f, false);

    float sqrt_disc = std::sqrt(disc);
    float t0 = (-B - sqrt_disc) / (2.0f * A);
    float t1 = (-B + sqrt_disc) / (2.0f * A);

    float z0 = glm::dot((at(t0) - cyl.p1), d) / dd;
    float z1 = glm::dot((at(t1) - cyl.p1), d) / dd;

    bool z0in = (z0 >= 0.0f && z0 <= 1.0f);
    bool z1in = (z1 >= 0.0f && z1 <= 1.0f);

    if (t0 >= 0.0f && z0in) return LineIntersect(*this, t0, true);
    if (t1 >= 0.0f && z1in) return LineIntersect(*this, t1, true);

    Plane ptop = Plane::from_point_normal(cyl.p2, glm::normalize(d));
    Plane pbot = Plane::from_point_normal(cyl.p1, -glm::normalize(d));

    LineIntersect hit_top = intersect(ptop);
    LineIntersect hit_bot = intersect(pbot);

    bool hit_top_valid = hit_top.hit && glm::length(hit_top.point() - cyl.p2) <= cyl.radius;
    bool hit_bot_valid = hit_bot.hit && glm::length(hit_bot.point() - cyl.p1) <= cyl.radius;

    if (hit_top_valid && (!hit_bot_valid || hit_top.t < hit_bot.t)) return hit_top;
    if (hit_bot_valid) return hit_bot;

    return LineIntersect(*this, 0.0f, false);
}

Ray::Ray(glm::vec3 const& p, glm::vec3 const& d) : pos(p), dir(glm::normalize(d)) {}

Ray Ray::from_pos_dir(glm::vec3 const& pos, glm::vec3 const& dir) {
    return Ray(pos, dir);
}

Ray Ray::from_start_end(glm::vec3 const& start, glm::vec3 const& end) {
    return Ray(start, end - start);
}

glm::vec3 Ray::at(float t) const {
    return pos + t * dir;
}

Segment Ray::segment_of(float len) const {
    return Segment(pos, at(len));
}

glm::vec3 Ray::closest_to(glm::vec3 const& point) const {
    glm::vec3 v = point - pos;
    float t = std::max(0.0f, glm::dot(v, dir));
    return at(t);
}

DualLineIntersect Ray::closest_to(Segment const& seg) const {
    glm::vec3 const& p1 = pos;
    glm::vec3 const& p2 = seg.start;
    glm::vec3 d1 = dir;
    glm::vec3 d2 = seg.end - seg.start;
    glm::vec3 r = p1 - p2;

    float a = glm::dot(d1, d1);
    float b = glm::dot(d1, d2);
    float c = glm::dot(d2, d2);
    float d = glm::dot(d1, r);
    float e = glm::dot(d2, r);

    float denom = a * c - b * b;

    float u = 0.0f, v = 0.0f;

    if (denom > 1e-6f) {
        u = (b * e - c * d) / denom;
        v = (a * e - b * d) / denom;
    } else {
        v = (b > c) ? d / b : e / c;
    }

    u = std::max(u, 0.0f);
    v = std::clamp(v, 0.0f, 1.0f);

    glm::vec3 pt1 = p1 + u * d1;
    glm::vec3 pt2 = p2 + v * d2;

    return DualLineIntersect(pt1, pt2, glm::length(pt1 - pt2), u, v);
}

DualLineIntersect Ray::closest_to(Ray const& other) const {
    glm::vec3 const& p1 = pos;
    glm::vec3 const& p2 = other.pos;
    glm::vec3 d1 = dir;
    glm::vec3 d2 = other.dir;
    glm::vec3 r = p1 - p2;

    float a = glm::dot(d1, d1);
    float e = glm::dot(d2, d2);
    float f = glm::dot(d2, r);

    float b = glm::dot(d1, d2);
    float c = glm::dot(d1, r);
    float denom = a * e - b * b;

    float s = 0.0f, t = 0.0f;

    if (denom > 1e-6f) {
        s = (b * f - c * e) / denom;
        t = (a * f - b * c) / denom;
    }

    s = std::max(0.0f, s);
    t = std::max(0.0f, t);

    glm::vec3 pt1 = p1 + s * d1;
    glm::vec3 pt2 = p2 + t * d2;

    return DualLineIntersect(pt1, pt2, glm::length(pt1 - pt2), s, t);
}




// int main() {
//     using glm::vec3;
//     const float eps = 1e-5f;

//     // Ray-Sphere intersect (hit)
//     {
//         Ray r(vec3(0, 0, -5), vec3(0, 0, 1));
//         Sphere s{vec3(0), 2.0f};
//         LineIntersect i = r.intersect(s);
//         assert(i.hit);
//         assert(std::abs(glm::length(i.point()) - 2.0f) < 0.1f);
//     }

//     // Ray-Sphere intersect (miss)
//     {
//         Ray r(vec3(5, 5, -5), vec3(0, 0, 1));
//         Sphere s{vec3(0), 1.0f};
//         LineIntersect i = r.intersect(s);
//         assert(!i.hit);
//     }

//     // Ray-Plane intersect (hit)
//     {
//         Plane p = Plane::from_point_normal(vec3(0), vec3(0, 1, 0));
//         Ray r(vec3(0, 3, 0), vec3(0, -1, 0));
//         LineIntersect i = r.intersect(p);
//         assert(i.hit);
//         assert(std::abs(i.point().y) < eps);
//     }

//     // Ray-Plane intersect (parallel miss)
//     {
//         Plane p = Plane::from_point_normal(vec3(0), vec3(0, 1, 0));
//         Ray r(vec3(0, 3, 0), vec3(1, 0, 0));
//         LineIntersect i = r.intersect(p);
//         assert(!i.hit);
//     }

//     // Ray-Triangle intersect (hit)
//     {
//         Triangle tri(vec3(0), vec3(1, 0, 0), vec3(0, 1, 0));
//         Ray r(vec3(0.2f, 0.2f, -1), vec3(0, 0, 1));
//         LineIntersect i = r.intersect(tri);
//         assert(i.hit);
//         assert(std::abs(i.point().z) < eps);
//     }

//     // Ray-Triangle intersect (miss)
//     {
//         Triangle tri(vec3(0), vec3(1, 0, 0), vec3(0, 1, 0));
//         Ray r(vec3(2, 2, -1), vec3(0, 0, 1));
//         LineIntersect i = r.intersect(tri);
//         assert(!i.hit);
//     }

//     // Ray-Disc intersect (hit)
//     {
//         Disc d{vec3(0), vec3(0, 1, 0), 1.0f};
//         Ray r(vec3(0, 5, 0), vec3(0, -1, 0));
//         LineIntersect i = r.intersect(d);
//         assert(i.hit);
//         assert(glm::length(i.point()) < 1.0f + eps);
//     }

//     // Ray-Disc intersect (miss outside radius)
//     {
//         Disc d{vec3(0), vec3(0, 1, 0), 1.0f};
//         Ray r(vec3(2, 5, 0), vec3(0, -1, 0));
//         LineIntersect i = r.intersect(d);
//         assert(!i.hit);
//     }

//     // Ray-Cylinder intersect (side hit)
//     {
//         Cylinder cyl{vec3(0, 0, -1), vec3(0, 0, 1), 1.0f};
//         Ray r(vec3(2, 0, 0), vec3(-1, 0, 0));
//         LineIntersect i = r.intersect(cyl);
//         assert(i.hit);
//         assert(std::abs(i.point().x - 1.0f) < 0.01f);
//     }

//     // Ray-Cylinder intersect (cap hit)
//     {
//         Cylinder cyl{vec3(0, 0, -1), vec3(0, 0, 1), 1.0f};
//         Ray r(vec3(0, 2, 0), vec3(0, -1, 0));
//         LineIntersect i = r.intersect(cyl);
//         assert(i.hit);
//         assert(std::abs(i.point().y - 1.0f) < 0.01f);
//     }

//     // Ray-Cylinder intersect (miss)
//     {
//         Cylinder cyl{vec3(0, 0, -1), vec3(0, 0, 1), 1.0f};
//         Ray r(vec3(2, 2, 0), vec3(1, 0, 0));
//         LineIntersect i = r.intersect(cyl);
//         assert(!i.hit);
//     }

//     // Ray-Segment closest
//     {
//         Ray r(vec3(0), vec3(1, 0, 0));
//         Segment seg(vec3(0, 1, 0), vec3(1, 1, 0));
//         DualLineIntersect i = r.closest_to(seg);
//         assert(i.distance - 1.0f < eps);
//         assert(i.u >= 0.0f);
//         assert(i.v >= 0.0f && i.v <= 1.0f);
//     }

//     // Ray-Ray closest
//     {
//         Ray r1(vec3(0), vec3(1, 0, 0));
//         Ray r2(vec3(0, 1, 0), vec3(1, 0, 0));
//         DualLineIntersect i = r1.closest_to(r2);
//         assert(!i.intersects());
//         assert(std::abs(i.distance - 1.0f) < eps);
//     }

//     std::cout << "[geometry] test suite passed\n";
//     return 0;
// }
