#include <iostream>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

int main() {
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    std::cout << "Vector: (" << v.x << ", " << v.y << ", " << v.z << ")\n";

    glm::mat4 identity = glm::mat4(1.0f);
    std::cout << "Identity Matrix:\n";
    const float* pMatrix = glm::value_ptr(identity);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << pMatrix[j + i * 4] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
