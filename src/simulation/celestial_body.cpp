# include <glm/glm.hpp>

// This is the base class for all celestial bodies in the simulation.
class celestial_body {
public:
    celestial_body() {}
    virtual ~celestial_body() {}
    long double mass;
    long double radius;
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
};

class planet : public celestial_body {
public:
    planet() {}
    virtual ~planet() {}
};