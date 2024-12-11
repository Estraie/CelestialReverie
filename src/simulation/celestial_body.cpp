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
