#ifndef Stopwatch_h
#define Stopwatch_h

// construct these as little simple timers
// init with time unit, default is seconds
class Stopwatch {
public:
    enum TimeUnit {
        SECONDS = 0,
        MILLISECONDS = 1,
        MICROSECONDS = 2,
        NANOSECONDS = 3
    };
    Stopwatch(TimeUnit tu = SECONDS);
    void setUnit(TimeUnit);
    void start();
    void reset();
    bool running() const;
    double read() const;
    double read(TimeUnit) const;
    double stop();
    double stop(TimeUnit);
    void reset_start();
    double stop_reset();
    double stop_reset(TimeUnit);
    double stop_reset_start();
    double stop_reset_start(TimeUnit);
private:
    double stopwatch_start_time;
    double stopwatch_stop_time;
    struct {
        TimeUnit unit       : 6;
        bool _running       : 1;
        bool _reset         : 1;
    } props;
};


#endif /* Stopwatch_h */
