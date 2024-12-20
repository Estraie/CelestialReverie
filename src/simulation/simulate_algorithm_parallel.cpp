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
    omp_set_num_threads(10);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < sys->bodies.size(); i++) {
        // std::cout << "Thread " << omp_get_thread_num() << " processes iteration " << i << ".\n";
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

barnes_hut_parallel& barnes_hut_parallel::get_instance() {
    static barnes_hut_parallel instance;
    return instance;
}

void barnes_hut_parallel::simulate(celestial_system*& sys) {
    node* root = new node();
    upper_bound = glm::dvec3(-std::numeric_limits<double>::infinity());
    lower_bound = glm::dvec3(std::numeric_limits<double>::infinity());
    omp_set_num_threads(10);

    #pragma omp parallel for
    for (size_t i = 0; i < sys->bodies.size(); i++) {
        const auto& body = sys->bodies[i];

        #pragma omp critical
        {
            upper_bound.x = std::max(upper_bound.x, body->position.x);
            upper_bound.y = std::max(upper_bound.y, body->position.y);
            upper_bound.z = std::max(upper_bound.z, body->position.z);
            lower_bound.x = std::min(lower_bound.x, body->position.x);
            lower_bound.y = std::min(lower_bound.y, body->position.y);
            lower_bound.z = std::min(lower_bound.z, body->position.z);
        }

        // Reset acceleration (no reduction needed)
        sys->bodies[i]->acceleration = glm::dvec3(0.0);
    }

    root->center = (upper_bound + lower_bound) / 2.0;
    root->semi_edge = (upper_bound - lower_bound) / 2.0;

    for(size_t i = 0; i < sys->bodies.size(); i++) {
        add_body(root, sys->bodies[i]);
    }

    #pragma omp parallel for
    for(size_t i = 0; i < sys->bodies.size(); i++) {
        add_gravity(root, sys->bodies[i]);
    }

    delete root;
}
