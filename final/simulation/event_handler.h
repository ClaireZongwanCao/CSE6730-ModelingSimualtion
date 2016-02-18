#ifndef EVENT_H
#define EVENT_H

#include <queue>
#include <iostream>
#include <string>
#include <algorithm>

#include "contents.h"

typedef enum {EnterLane, ExitLane, SignalCheck, SignalChange, SignalChangeTrigger, EVENT_SIZE} EventType;
const std::vector<std::string> EventName= {"Enter Lane", "Exit Lane", "Signal Check", "Signal Change"};
const std::vector<std::string> H = {"North", "South", "East", "West"};
const std::vector<std::string> L = {"L", "G", "R"};

class Event {
public:
	unsigned int time_stamp;
	EventType e;
	

	friend std::ostream& operator<<(std::ostream& os, const Event& e) {
		os << "Time: " << e.time_stamp << std::endl;
		os << "Event: " << EventName[e.e] << std::endl;		
		return os;
	}

// 	std::ostream& operator<<(std::ostream& os) {
// 		os << "Time: " << this->time_stamp << std::endl;
// 		os << "Event: " << EventName[this->e] << std::endl;		
// 		return os;
// 	}
};



class Event_EnterLane : public Event {
public:
	unsigned int lane_num;
	unsigned int vehicle_id;
	DirType dir;

	friend std::ostream& operator<<(std::ostream& os, const Event_EnterLane& e) {
		os << "Time: " << e.time_stamp << std::endl;
		os << "Event: " << EventName[e.e] << std::endl;	
		os << "\t\t\t\tLane no: " << e.lane_num << std::endl;
		os << "\t\t\t\tDir: " << H[e.dir] <<  std::endl << std::endl;
		return os;
	}
};

class Event_ExitLane : public Event {
public:
	unsigned int lane_num;
	DirType dir;

	friend std::ostream& operator<<(std::ostream& os, const Event_ExitLane& e) {
		os << "Time: " << e.time_stamp << std::endl;
		os << "Event: " << EventName[e.e] << std::endl;	
		os << "\t\t\t\tLane no: " << e.lane_num << std::endl;
		os << "\t\t\t\tDir: " << H[e.dir] <<  std::endl << std::endl;
		return os;
	}

};

class Event_SignalCheck : public Event {
public:
	unsigned int signal_num;
	
	// direction in which car is going before hitting the signal
	DirType dir;

	friend std::ostream& operator<<(std::ostream& os, const Event_SignalCheck& e) {
		os << "Time: " << e.time_stamp << std::endl;
		os << "Event: " << EventName[e.e] << std::endl;	
		os << "\t\t\t\tSignal no: " << e.signal_num << std::endl;
		os << "\t\t\t\tDir: " << H[e.dir] <<  std::endl << std::endl;
		return os;
	}

};
	
class Event_SignalChange : public Event {
public:
	unsigned int signal_num;
	/*some more params telling how to change signal*/

	friend std::ostream& operator<<(std::ostream& os, const Event_SignalChange& e) {
		os << "Time: " << e.time_stamp << std::endl;
		os << "Event: " << EventName[e.e] << std::endl;	
		os << "\t\t\t\tSignal no: " << e.signal_num << std::endl;
		return os;
	}

};

class Event_SignalChangeTrigger : public Event {
public:
	// Think what should go in the class
	// basically, if there is a car in any of the lanes, schedule 10 more
	unsigned int signal_num;
	unsigned int left_green_time;
	unsigned int green_time;
};

class CompareTime {
public:
	bool operator() (const Event* a, const Event* b) const {
		if (a->time_stamp > b->time_stamp) {
			return true;
		}
		return false;
	}
};

class EventHandler {
public:	
	
	// vectors of objects to be delt with
	std::vector<Vehicle> *Vptr=nullptr;
	std::vector<std::vector<Lane> > *Lptr=nullptr;
	std::vector<Signal> *Sptr=nullptr;
	
	// delay at each intersection
	unsigned int t_signal_delay;
	
	std::priority_queue<Event*, std::vector<Event*>, CompareTime> EventQueue;
	
	Event* min_timestamp_event();
	void ProcessEvent(Event *);
	void ScheduleEvent(Event *);
	void PrintEventInfo(Event *);
};

#endif // EVENT_H
