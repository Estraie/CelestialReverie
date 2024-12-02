#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <glm/glm.hpp>

class celestial_body {
public:
    celestial_body();
    virtual ~celestial_body();
    double mass;
    double radius;
    glm::dvec3 position;
    glm::f64 &x = position.x;
    glm::f64 &y = position.y;
    glm::f64 &z = position.z;

    glm::dvec3 velocity;
    glm::f64 &vx = velocity.x;
    glm::f64 &vy = velocity.y;
    glm::f64 &vz = velocity.z;

    glm::dvec3 polar_position;
    glm::f64 &r = polar_position.x;
    glm::f64 &theta = polar_position.y;
    glm::f64 &phi = polar_position.z;

    glm::dvec3 acceleration;
    glm::f64 &ax = acceleration.x;
    glm::f64 &ay = acceleration.y;
    glm::f64 &az = acceleration.z;

    celestial_body* duplicate();
};
#endif // CELESTIAL_BODY_H