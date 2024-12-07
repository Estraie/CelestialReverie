# include <iostream>
# include <simulation/celestial_reverie.h>
using namespace std;

int main(){
    celestial_reverie sim;
    celestial_body* a = new celestial_body(1.0e10, 1, {10, 0, 0}, {0, 10, 0});
    celestial_body* b = new celestial_body(1.0e10, 1, {-10, 0, 0}, {0, -10, 0});
    sim.add_celestial_body(a);
    sim.add_celestial_body(b);
    sim.set_simulate_algorithm<barnes_hut>();
    sim.set_time_step(0.01);
    for(int i = 0; i < 1000; i++) {
        celestial_system* sys = sim.simulate();
        cout << "Time: " << sys->get_time() << endl;
        cout << *sys << endl;
        char p;
        p = getchar();
        if(p == '#') {
            break;
        }
    }
}