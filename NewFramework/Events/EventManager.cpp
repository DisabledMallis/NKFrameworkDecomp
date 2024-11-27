#include "EventManager.h"

bool CEventManager::_SendEvent(IEvent* event, bool own) {
	mEventsInFlight++;
	auto& observers = mObserverMap[typeid(event)];
	bool cancelled = false;
	for (auto& [flag, observer] : observers) {
		observer->HandleEvent_Internal(event, cancelled);
	}
	if (own) {
		delete event;
	}
	mEventsInFlight--;
	return cancelled;
}
