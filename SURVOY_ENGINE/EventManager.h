#pragma once

#include <vector>
#include "IObserver.h"

class EventManager {
    std::vector<IObserver*> observers;
public:
    void subscribe(IObserver* observer) {
        observers.push_back(observer);
    }

    void unsubscribe(IObserver* observer) {
        // Remove observer from the list
    }

    void notifyAll(const Event& event) {
        for (auto* observer : observers) {
            observer->onNotify(event);
        }
    }
};
