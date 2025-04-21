#include "stopwatch.h"
#include <chrono>
#include <ctime>

Stopwatch::Stopwatch(TimeUnit tu) : stopwatch_start_time(0), stopwatch_stop_time(0) {
    props = {.unit = tu, ._running = false, ._reset = false};
}

void Stopwatch::setUnit(TimeUnit tu) {
    props.unit = tu;
}

void Stopwatch::start() {
    if (!props._running) {
        props._running = true;
        props._reset = false;
        stopwatch_start_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    }
}

void Stopwatch::reset() {
    props._reset = true;
    props._running = false;
    stopwatch_start_time = 0;
    stopwatch_stop_time = 0;
}

bool Stopwatch::running() const {
    return props._running;
}

double Stopwatch::read() const {
    return read(props.unit);
}

double Stopwatch::read(TimeUnit tu) const {
    double divisor;
    switch (tu) {
    case SECONDS:
        divisor = 1000000000.;
        break;
    case MILLISECONDS:
        divisor = 1000000.;
        break;
    case MICROSECONDS:
        divisor = 1000.;
        break;
    case NANOSECONDS:
        divisor = 1.;
        break;
    }
    if (props._running) {
        auto current_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
        return (current_time - stopwatch_start_time) / divisor;
    }
    if (!props._reset)
        return (stopwatch_stop_time - stopwatch_start_time) / divisor;
    return 0;
}

double Stopwatch::stop() {
    return stop(props.unit);
}

double Stopwatch::stop(TimeUnit tu) {
    if (props._running) {
        props._running = false;
        stopwatch_stop_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    }
    return read(tu);
}

void Stopwatch::reset_start() {
    reset();
    start();
}

double Stopwatch::stop_reset() {
    return stop_reset(props.unit);
}

double Stopwatch::stop_reset(TimeUnit tu) {
    double elapsed = stop(tu);
    reset();
    return elapsed;
}

double Stopwatch::stop_reset_start() {
    return stop_reset_start(props.unit);
}

double Stopwatch::stop_reset_start(TimeUnit tu) {
    double elapsed = stop(tu);
    reset_start();
    return elapsed;
}
