#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <glm/glm.hpp>
#include <iostream>

class celestial_body {
public:
    celestial_body(
        double mass = 0, 
        double radius = 0, 
        glm::dvec3 position = glm::dvec3(0), 
        glm::dvec3 velocity = glm::dvec3(0), 
        glm::dvec3 polar_position = glm::dvec3(0), 
        glm::dvec3 acceleration = glm::dvec3(0), 
        glm::dvec3 color = glm::dvec3(0.2, 0.2, 0.6)
    );
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

    glm::dvec3 color; // RGB color

    celestial_body* duplicate() const;
    virtual std::ostream& operator<<(std::ostream& os) const;
    virtual bool is_emissive();
};

class planet : public celestial_body {
    using celestial_body::celestial_body;
    virtual std::ostream& operator<<(std::ostream& os) const;
    virtual bool is_emissive();
};

class star : public celestial_body {
public:
    star(
        double mass = 0, 
        double radius = 0, 
        glm::dvec3 position = glm::dvec3(0), 
        glm::dvec3 velocity = glm::dvec3(0), 
        glm::dvec3 polar_position = glm::dvec3(0), 
        glm::dvec3 acceleration = glm::dvec3(0), 
        glm::dvec3 color = glm::dvec3(0.99, 0.95, 0.90)
    );
    virtual std::ostream& operator<<(std::ostream& os) const;
    virtual bool is_emissive();
};
#endif // CELESTIAL_BODY_H