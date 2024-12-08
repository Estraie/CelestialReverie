#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <glm/glm.hpp>
#include <iostream>

class celestial_body {
public:
    const double c = 299792458;
    
    celestial_body(
        double mass = 0, 
        double radius = 0, 
        glm::dvec3 position = glm::dvec3(0), 
        glm::dvec3 velocity = glm::dvec3(0), 
        glm::dvec3 polar_position = glm::dvec3(0), 
        glm::dvec3 acceleration = glm::dvec3(0)
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

    celestial_body* duplicate() const;

    friend std::ostream& operator<<(std::ostream& os, const celestial_body& cb);

    // Special Relativity fix
    double gamma;
    double mass_rela;
    void sr_update();

};
#endif // CELESTIAL_BODY_H