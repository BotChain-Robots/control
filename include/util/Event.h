
#ifndef CONTROL_EVENT_H
#define CONTROL_EVENT_H

#include <functional>

class Event;

class CallbackHandle {
  public:
    CallbackHandle(Event *evt, int id) : evt(evt), id(id) {
    }
    ~CallbackHandle();

  private:
    Event *evt;
    int id;
};

class Event {
  public:
    using Callback = std::function<void()>;

    CallbackHandle addListener(Callback cb) {
        int id = nextId++;
        callbacks[id] = std::move(cb);
        return CallbackHandle(this, id);
    }

    void remove(int id) {
        callbacks.erase(id);
    }

    void fire() {
        for (auto &kv : callbacks)
            kv.second();
    }

  private:
    friend class CallbackHandle;
    std::unordered_map<int, Callback> callbacks;
    int nextId = 0;
};

CallbackHandle::~CallbackHandle() {
    if (evt)
        evt->remove(id);
}

#endif // CONTROL_EVENT_H
