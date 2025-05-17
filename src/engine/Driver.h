/** 
 * Driver.h 
 * poker
 * created 03/17/25 by frank collebrusco
 * this framework is not really great, in that it's a very simple single update loop glued into sync with the window update. works for most simple things and helps get them going quickly with an accurate dt. TODO upgrade this to lend itself better to multithreading or at least detached tick / render threads
 */
#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <cstddef>
#include <flgl.h>
#include <flgl/misc.h>

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
