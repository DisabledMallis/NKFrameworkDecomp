#pragma once

class IEvent;

class IObserver {
public:
	IObserver() = default;
	virtual ~IObserver() = default;
	virtual void HandleEvent_Internal(IEvent* event, bool& cancel) = 0;
	virtual void ManagerDestroyed() = 0;
};

template<typename event_t>
class CObserver : public IObserver {
public:
	virtual ~CObserver() = default;
	void HandleEvent_Internal(IEvent* event, bool& cancel) override {
		if (typeid(event_t) != typeid(event)) {
			return;
		}
		this->HandleEvent(event, cancel);
	}
	void ManagerDestroyed() override {
		//TODO: impl
	};
	virtual void HandleEvent(event_t* event, bool& cancel) {
		this->HandleEvent(event);
	}
	virtual void HandleEvent(event_t*) {};
};