# include <glm/glm.hpp>
# include <simulation/celestial_body.h>
// This is the base class for all celestial bodies in the simulation.
class celestial_body {
public:
    celestial_body() {}
    virtual ~celestial_body() {}
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

    celestial_body* duplicate() const {
        celestial_body* new_body = new celestial_body();
        new_body->mass = mass;
        new_body->radius = radius;
        new_body->position = position;
        new_body->velocity = velocity;
        new_body->polar_position = polar_position;
        new_body->acceleration = acceleration;
        return new_body;
    }
};

class planet : public celestial_body {
public:
    planet() {}
    virtual ~planet() {}
};