#ifndef DISABLE_TIMER
#include <mbgl/util/timer.hpp>
#include <mbgl/util/time.hpp>
#include <mbgl/platform/log.hpp>

#include <iostream>
#include <atomic>

using namespace mbgl::util;

timer::timer(Event event)
    : event(event), start(now()) {}

timer::timer(EventSeverity severity, Event event)
    : severity(severity), event(event), start(now()) {}

timer::timer(const std::string &name, Event event)
    : name(name), event(event), start(now()) {}

timer::timer(const std::string &name, EventSeverity severity, Event event)
    : name(name), severity(severity), event(event), start(now()) {}

void timer::report(const std::string &name) {
    timestamp duration = now() - start;

    Log::Record(severity, event, name + ": " + std::to_string((double)(duration) / 1_millisecond) + "ms");
    start += duration;
}

timer::~timer() {
    if (name.size()) {
        report(name);
    }
}
#endif
