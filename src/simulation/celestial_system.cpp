# include <vector>
# include <simulation/celestial_body.h>
# include <simulation/celestial_system.h>

celestial_system::~celestial_system(){
    for(auto body : bodies){
        delete body;
    }
}

celestial_system::celestial_system(){
    time = 0.0;
}

void celestial_system::add_body(celestial_body* body){
    bodies.push_back(body);
}

void celestial_system::set_time(double new_time){
    time = new_time;
}

double celestial_system::get_time(){
    return time;
}

celestial_system* celestial_system::duplicate(){
    celestial_system* new_system = new celestial_system();
    for(auto body : bodies){
        new_system->add_body(body->duplicate());
    }
    return new_system;
}

// class celestial_system{
// public:
//     double time;
//     std::vector<celestial_body*> bodies;
//     celestial_system();
//     ~celestial_system(){
//         for(auto body : bodies){
//             delete body;
//         }
//     }
//     void add_body(celestial_body* body){
//         bodies.push_back(body);
//     }
//     void set_time(double new_time){
//         time = new_time;
//     }
//     double get_time(){
//         return time;
//     }
//     celestial_system* duplicate(){
//         celestial_system* new_system = new celestial_system();
//         for(auto body : bodies){
//             new_system->add_body(body->duplicate());
//         }
//         return new_system;
//     }
// };