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

struct Ray;

struct LineIntersect {
    Ray incoming;
    float t;
    bool hit;
    glm::vec3 point() const;
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
};

struct AABB {
    bool inside(glm::vec3 mpos) const;
    void merge(AABB const& other);
    bool isect(AABB const& other);
    bool contains(AABB const& other);
    bool degen() const;
    glm::vec3 size() const;
    glm::vec3 middle() const;
    static AABB from_merge(AABB const& a, AABB const& b);
    static AABB from_isect(AABB const& a, AABB const& b);
    static AABB from_minmax(glm::vec3 min, glm::vec3 max);
    static AABB from_minsize(glm::vec3 min, glm::vec3 size);
    static inline AABB null() {return AABB();}
    AABB() : min(1e100), max(-1e100) {}
    AABB(glm::vec3 m, glm::vec3 M) : min(m), max(M) {}
    glm::vec3 min, max;
};

struct PointToPlane {
    float distance;
    glm::vec3 vec;
};

struct Plane {
    glm::vec3 point, normal;
    PointToPlane intersect(glm::vec3 const& point) const;
    LineIntersect intersect(Ray const& ray) const;

};

struct Segment {
    glm::vec3 start;
    glm::vec3 end;
    glm::vec3 dir;

    float length() const;

    Segment(glm::vec3 const& s, glm::vec3 const& e);
    static Segment from_pos_dir_len(glm::vec3 const& p, glm::vec3 const& d, float l);
    static Segment from_ray_len(Ray const& ray, float length);
    static Segment from_start_end(const glm::vec3& s, const glm::vec3& e);

    glm::vec3 at(float u) const;

    static DualLineIntersect closest_between(const Segment& seg1, const Segment& seg2);
};

struct Ray {
    glm::vec3 pos;
    glm::vec3 dir;

    Ray(glm::vec3 const& p, glm::vec3 const& d);
    static Ray from_pos_dir(glm::vec3 const& pos, glm::vec3 const& dir);
    static Ray from_start_end(glm::vec3 const& start, glm::vec3 const& end);

    glm::vec3 at(float t) const;

    glm::vec3 closest_to(glm::vec3 const& point) const;
    DualLineIntersect closest_to(Segment const& seg) const;
    DualLineIntersect closest_to(Ray const& ray) const;

    LineIntersect intersect(AABB const& aabb) const;
    LineIntersect intersect(Sphere const& sphere) const;
};




#endif /* GEOMETRY_H */
