# include <glm/glm.hpp>
class view { // TODO
private:
    glm::dvec3 position;
	glm::dvec3 direction;
    int width, height;
public:
    view(glm::dvec3 position, glm::dvec3 direction) : position(position), direction(direction) {}
    glm::dvec3 get_position() const { return position; }
    glm::dvec3 get_direction() const { return direction; }
    void set_position(glm::dvec3 position) { this->position = position; }
    void set_direction(glm::dvec3 direction) { this->direction = direction; }
};

