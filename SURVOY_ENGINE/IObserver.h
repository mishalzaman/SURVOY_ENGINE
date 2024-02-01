#pragma once

/*
IObserver class
---------------

1. Create an Event class that inherits event
    E.g. CameraViewProjectionEvent 

2. In the subscriber add the onNotify method (see MeshRenderSystem)

3. In the event sender use _eventManager.notifyAll(CameraViewProjectionEvent(camera->View, camera->Projection));
*/

#include "Event.h"

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotify(const Event& event) = 0;
};
