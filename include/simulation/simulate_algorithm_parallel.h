#ifndef SIMULATE_ALGORITHM_PARALLEL_H
#define SIMULATE_ALGORITHM_PARALLEL_H

#include <simulation/simulate_algorithm.h>
#include <omp.h>

class pure_newtonian_parallel : public pure_newtonian {
protected:
    static pure_newtonian_parallel instance;
    void add_gravity(celestial_body*& body, celestial_body*& other, glm::dvec3& local_acceleration);
    pure_newtonian_parallel() = default;
public:
    virtual void simulate(celestial_system*& system) override;
    static pure_newtonian_parallel& get_instance();
};

class barnes_hut_parallel : public barnes_hut {
protected:
    static barnes_hut_parallel instance;
    barnes_hut_parallel() = default;
public:
    virtual void simulate(celestial_system*& system) override;
    static barnes_hut_parallel& get_instance();
};

#endif