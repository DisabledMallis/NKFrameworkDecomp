#pragma once

#include "EventType.h"
#include "Observer.h"

#include <boost/thread/pthread/recursive_mutex.hpp>

#include <atomic>
#include <deque>
#include <map>
#include <vector>

class CEventManager {
public:
	CEventManager() = default;
	~CEventManager() = default;

	bool _SendEvent(IEvent* event, bool own); // I called the bool 'own' because it looks like if its true the func is expected to free the event...
	void AddPendingObservers();
	void AddPendingRemovingObserver(IObserver* observer, const std::type_info& type);
	void AddPendingSubscribingObserver(IObserver* observer, const std::type_info& type);
	void Clear();
	void ProcessQueue();
	void QueueEvent(IEvent* event);
	void RemovePendingObservers();
	void SendEvent(IEvent* event);
	void Subscribe(IObserver* observer, const std::type_info& type);
	void Unsubscribe(IObserver* observer, const std::type_info& type);

	//std::map<CEventType, std::vector<std::pair<bool, IObserver*>>> mObserverMap{};
	std::atomic_int mEventsInFlight = 0;
	boost::recursive_mutex mMutex{};
	std::array<std::deque<IEvent*>, 2> mEventQueue{};
	std::atomic_bool mQueueFlag = false;
};