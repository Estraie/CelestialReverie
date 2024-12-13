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