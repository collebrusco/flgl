/** 
 * Driver.h 
 * poker
 * created 03/17/25 by frank collebrusco
 */
#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <cstddef>
#include <flgl.h>
#include <flgl/tools.h>
#include "../util/stopwatch.h"

/* usage: DRIVER_MAIN_FUNCTION(main, <name of your implementation>); */
#define DRIVER_MAIN_FUNCTION(main_name, Classname) int main_name() {Classname *driver = new Classname(); driver->start(); delete driver; return 0;}

class Driver {
protected:
	void close();
public:
    void start();
    void loop();
    Driver();
    inline virtual ~Driver() {}

    inline float dt() const {return _dt;};
    inline Stopwatch const& launch_timer() {return _launch_timer;}

    glm::vec2 world_mouse(glm::vec2 mp, Camera& cam) const;

private:
	bool _close;
	float _dt;
	Stopwatch _launch_timer;
    Stopwatch delta_timer;
    virtual void user_create() = 0;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) = 0;
    virtual void user_render() = 0;
    virtual void user_destroy() = 0;
    void exit();
};


#endif /* DRIVER_H */
