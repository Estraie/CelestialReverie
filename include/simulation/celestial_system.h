# include <vector>
# include "celestial_body.h"

class celestial_system{
public:
    double time;
    celestial_system();
    ~celestial_system();
    std::vector<celestial_body*> bodies;
    void add_body(celestial_body* body);
    void simulate(double time_step);
    void set_time(double time);
    double get_time();
    celestial_system* duplicate();
};