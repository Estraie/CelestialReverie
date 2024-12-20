# include <glm/glm.hpp>
# include <simulation/celestial_body.h>
// This is the base class for all celestial bodies in the simulation.
celestial_body::celestial_body(
    double mass, 
    double radius, 
    glm::dvec3 position, 
    glm::dvec3 velocity, 
    glm::dvec3 acceleration,
    glm::dvec3 color
) : mass(mass), radius(radius), 
    position(position), velocity(velocity), 
    acceleration(acceleration), color(color) {}

celestial_body* celestial_body::duplicate() const {
    celestial_body* new_body = new celestial_body;
    new_body->mass = mass;
    new_body->radius = radius;
    new_body->position = position;
    new_body->velocity = velocity;
    new_body->polar_position = polar_position;
    new_body->acceleration = acceleration;
    new_body->color = color;
    return new_body;
}

// std::ostream& operator<<(std::ostream& os, const celestial_body& body) {
//     os << 0 << ',' << body.mass << ',';
//     os << body.radius << ',';
//     os << body.position.x << ',' << body.position.y << ',' << body.position.z << ',';
//     os << body.velocity.x << ',' << body.velocity.y << ',' << body.velocity.z << ',';
//     os << body.color.x << ',' << body.color.y << ',' << body.color.z << std::endl;
//     return os;
// }

std::ostream& celestial_body::operator<<(std::ostream& os) const {
    os << 0 << ',' << mass << ',';
    os << radius << ',';
    os << position.x << ',' << position.y << ',' << position.z << ',';
    os << velocity.x << ',' << velocity.y << ',' << velocity.z << ',';
    os << acceleration.x << ',' << acceleration.y << ',' << acceleration.z << ',';
    os << color.x << ',' << color.y << ',' << color.z << std::endl;
    return os;
}

bool celestial_body::is_emissive() {
    return false;
}

// std::ostream& operator<<(std::ostream& os, const planet& body) {
//     os << 1 << ',' << body.mass << ',';
//     os << body.radius << ',';
//     os << body.position.x << ',' << body.position.y << ',' << body.position.z << ',';
//     os << body.velocity.x << ',' << body.velocity.y << ',' << body.velocity.z << ',';
//     os << body.color.x << ',' << body.color.y << ',' << body.color.z << std::endl;
//     return os;
// }

std::ostream& planet::operator<<(std::ostream& os) const {
    os << 1 << ',' << mass << ',';
    os << radius << ',';
    os << position.x << ',' << position.y << ',' << position.z << ',';
    os << velocity.x << ',' << velocity.y << ',' << velocity.z << ',';
    os << acceleration.x << ',' << acceleration.y << ',' << acceleration.z << ',';
    os << color.x << ',' << color.y << ',' << color.z << std::endl;
    return os;
}

celestial_body* planet::duplicate() const {
    planet* new_body = new planet;
    new_body->mass = mass;
    new_body->radius = radius;
    new_body->position = position;
    new_body->velocity = velocity;
    new_body->polar_position = polar_position;
    new_body->acceleration = acceleration;
    new_body->color = color;
    return new_body;
}

bool planet::is_emissive() {
    return false;
}

// std::ostream& operator<<(std::ostream& os, const star& body) {
//     os << 2 << ',' << body.mass << ',';
//     os << body.radius << ',';
//     os << body.position.x << ',' << body.position.y << ',' << body.position.z << ',';
//     os << body.velocity.x << ',' << body.velocity.y << ',' << body.velocity.z << ',';
//     os << body.color.x << ',' << body.color.y << ',' << body.color.z << std::endl;
//     return os;
// }

std::ostream& star::operator<<(std::ostream& os) const {
    os << 2 << ',' << mass << ',';
    os << radius << ',';
    os << position.x << ',' << position.y << ',' << position.z << ',';
    os << velocity.x << ',' << velocity.y << ',' << velocity.z << ',';
    os << acceleration.x << ',' << acceleration.y << ',' << acceleration.z << ',';
    os << color.x << ',' << color.y << ',' << color.z << std::endl;
    return os;
}

star::star(
    double mass, 
    double radius, 
    glm::dvec3 position, 
    glm::dvec3 velocity, 
    glm::dvec3 acceleration,
    glm::dvec3 color
) : celestial_body(mass, radius, position, velocity, acceleration, color) {}

celestial_body* star::duplicate() const {
    star* new_body = new star;
    new_body->mass = mass;
    new_body->radius = radius;
    new_body->position = position;
    new_body->velocity = velocity;
    new_body->polar_position = polar_position;
    new_body->acceleration = acceleration;
    new_body->color = color;
    return new_body;
}

bool star::is_emissive() {
    return true;
}
