# include <glm/glm.hpp>
# include <simulation/celestial_body.h>
// This is the base class for all celestial bodies in the simulation.
celestial_body::celestial_body(
    double mass, 
    double radius, 
    glm::dvec3 position, 
    glm::dvec3 velocity, 
    glm::dvec3 polar_position, 
    glm::dvec3 acceleration
) : mass(mass), radius(radius), 
    position(position), velocity(velocity), 
    polar_position(polar_position), acceleration(acceleration) {}

celestial_body* celestial_body::duplicate() const {
    celestial_body* new_body = new celestial_body;
    new_body->mass = mass;
    new_body->radius = radius;
    new_body->position = position;
    new_body->velocity = velocity;
    new_body->polar_position = polar_position;
    new_body->acceleration = acceleration;
    return new_body;
}

std::ostream& operator<<(std::ostream& os, const celestial_body& body) {
    os << "Mass: " << body.mass << std::endl;
    os << "Radius: " << body.radius << std::endl;
    os << "Position: " << body.position.x << ", " << body.position.y << ", " << body.position.z << std::endl;
    os << "Velocity: " << body.velocity.x << ", " << body.velocity.y << ", " << body.velocity.z << std::endl;
    os << std::endl;
    return os;
}
// class celestial_body {
// public:
//     celestial_body() {}
//     virtual ~celestial_body() {}
//     double mass;
//     double radius;
//     glm::dvec3 position;
//     glm::f64 &x = position.x;
//     glm::f64 &y = position.y;
//     glm::f64 &z = position.z;

//     glm::dvec3 velocity;
//     glm::f64 &vx = velocity.x;
//     glm::f64 &vy = velocity.y;
//     glm::f64 &vz = velocity.z;

//     glm::dvec3 polar_position;
//     glm::f64 &r = polar_position.x;
//     glm::f64 &theta = polar_position.y;
//     glm::f64 &phi = polar_position.z;

//     glm::dvec3 acceleration;
//     glm::f64 &ax = acceleration.x;
//     glm::f64 &ay = acceleration.y;
//     glm::f64 &az = acceleration.z;

//     celestial_body* duplicate() const {
//         celestial_body* new_body = new celestial_body();
//         new_body->mass = mass;
//         new_body->radius = radius;
//         new_body->position = position;
//         new_body->velocity = velocity;
//         new_body->polar_position = polar_position;
//         new_body->acceleration = acceleration;
//         return new_body;
//     }
// };

// class planet : public celestial_body {
// public:
//     planet() {}
//     virtual ~planet() {}
// };

void celestial_body::sr_update(){

    acceleration.x = 0;
    acceleration.y = 0;
    acceleration.z = 0;

    double v = glm::length(velocity);
    
    gamma = 1/glm::sqrt(1 - glm::pow(v/c, 2));

    mass_rela = mass  / gamma;
}