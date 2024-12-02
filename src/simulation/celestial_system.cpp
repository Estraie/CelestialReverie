# include <vector>
# include <simulation/celestial_body.h>
# include <simulation/celestial_system.h>

class celestial_system{
public:
    double time;
    std::vector<celestial_body*> bodies;
    celestial_system();
    ~celestial_system(){
        for(auto body : bodies){
            delete body;
        }
    }
    void add_body(celestial_body* body){
        bodies.push_back(body);
    }
    void set_time(double new_time){
        time = new_time;
    }
    double get_time(){
        return time;
    }
    celestial_system* duplicate(){
        celestial_system* new_system = new celestial_system();
        for(auto body : bodies){
            new_system->add_body(body->duplicate());
        }
        return new_system;
    }
};