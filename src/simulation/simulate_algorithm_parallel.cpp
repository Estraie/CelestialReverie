#include <simulation/simulate_algorithm_parallel.h>

pure_newtonian_parallel& pure_newtonian_parallel::get_instance() {
    static pure_newtonian_parallel instance;
    return instance;
}

void pure_newtonian_parallel::add_gravity(celestial_body*& body, celestial_body*& other, glm::dvec3& local_acceleration) {
    auto dist_vec = other->position - body->position;
    double G = sim_constants::get_G();
    double dist_squared = glm::dot(dist_vec, dist_vec);
    if (dist_squared > 1e-6) {
        glm::dvec3 force_dir = glm::normalize(dist_vec);
        local_acceleration += G * other->mass * force_dir / dist_squared;
    }
}

void pure_newtonian_parallel::simulate(celestial_system*& sys) {
    std::vector<glm::dvec3> local_accelerations(sys->bodies.size(), glm::dvec3(0.0));

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < sys->bodies.size(); i++) {
        glm::dvec3 local_acceleration(0.0);
        for (int j = 0; j < sys->bodies.size(); j++) {
            if (i != j) {
                add_gravity(sys->bodies[i], sys->bodies[j], local_acceleration);
            }
        }
        local_accelerations[i] = local_acceleration;
    }
    
    for (int i = 0; i < sys->bodies.size(); i++) {
        sys->bodies[i]->acceleration = local_accelerations[i];
    }
}
