/** 
 * geometry.h 
 * flgl
 * created 04/15/25 by frank collebrusco
 */
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <algorithm>
#include <cmath>
#include <flgl/glm.h>

/**
 * CAUTION: this is not extensively tested atm TODO test
 */

struct Ray;
struct AABB;

struct LineIntersect {
    Ray const* incoming;
    float t;
    bool hit;

    LineIntersect(Ray const& ray, float t_val, bool did_hit);

    glm::vec3 point() const;
    static inline LineIntersect miss();
};

struct DualLineIntersect {
    glm::vec3 point1;
    glm::vec3 point2;
    float distance;
    float u;
    float v;

    DualLineIntersect(glm::vec3 const& p1, glm::vec3 const& p2, float dist, float u_, float v_);
    glm::vec3 vec() const;
    bool intersects() const;
};

struct Sphere {
    glm::vec3 center;
    float rad;

    AABB get_aabb() const;
};

struct AABB {
    glm::vec3 min, max;

    AABB() : min(1e38f), max(-1e38f) {}
    AABB(glm::vec3 m, glm::vec3 M) : min(m), max(M) {}

    bool inside(glm::vec3 mpos) const;
    void merge(AABB const& other);
    void expand(glm::vec3 const& p);
    bool isect(AABB const& other) const;
    bool contains(AABB const& other) const;
    bool degen() const;
    glm::vec3 size() const;
    glm::vec3 extent() const;
    glm::vec3 middle() const;

    static AABB from_merge(AABB const& a, AABB const& b);
    static AABB from_isect(AABB const& a, AABB const& b);
    static AABB from_minmax(glm::vec3 min, glm::vec3 max);
    static AABB from_minsize(glm::vec3 min, glm::vec3 size);
    static inline AABB null() { return AABB(); }
};

struct PointToPlane {
    float distance;
    glm::vec3 vec;
};

struct Plane {
    glm::vec3 point, normal;

    static Plane from_point_normal(glm::vec3 const& p, glm::vec3 const& n);
    PointToPlane intersect(glm::vec3 const& point) const;
};

struct Triangle {
    glm::vec3 a, b, c;

    Triangle(glm::vec3 const& a_, glm::vec3 const& b_, glm::vec3 const& c_);

    glm::vec3 normal() const;
    float area() const;
    glm::vec3 barycentric(glm::vec3 const& p) const;
    bool contains_point(glm::vec3 const& p) const;
};

struct Disc {
    glm::vec3 center;
    glm::vec3 normal;
    float radius;
};

struct Segment {
    glm::vec3 start;
    glm::vec3 end;
    glm::vec3 dir;

    Segment(glm::vec3 const& s, glm::vec3 const& e);

    float length() const;
    float squared_length() const;
    AABB get_aabb() const;

    glm::vec3 at(float u) const;

    static Segment from_pos_dir_len(glm::vec3 const& p, glm::vec3 const& d, float l);
    static Segment from_ray_len(Ray const& ray, float length);
    static Segment from_start_end(glm::vec3 const& s, glm::vec3 const& e);

    static DualLineIntersect closest_between(Segment const& seg1, Segment const& seg2);
};

struct Cylinder {
    glm::vec3 p1;
    glm::vec3 p2;
    float radius;

    Segment axis() const;
};

struct Ray {
    glm::vec3 pos;
    glm::vec3 dir;

    Ray(glm::vec3 const& p, glm::vec3 const& d);

    static Ray from_pos_dir(glm::vec3 const& pos, glm::vec3 const& dir);
    static Ray from_start_end(glm::vec3 const& start, glm::vec3 const& end);

    glm::vec3 at(float t) const;
    Segment segment_of(float len) const;

    glm::vec3 closest_to(glm::vec3 const& point) const;
    DualLineIntersect closest_to(Segment const& seg) const;
    DualLineIntersect closest_to(Ray const& ray) const;

    LineIntersect intersect(AABB const& aabb) const;
    LineIntersect intersect(Sphere const& sphere) const;
    LineIntersect intersect(Triangle const& tri) const;
    LineIntersect intersect(Disc const& disc) const;
    LineIntersect intersect(Cylinder const& cyl) const;
    LineIntersect intersect(Plane const& plane) const;
};

#endif /* GEOMETRY_H */
