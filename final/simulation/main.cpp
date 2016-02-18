#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <cstdlib>
#include <algorithm>

#include "event_handler.h"
#include "contents.h"

void PrintUsage() {
	std::cerr << "Usage: simulation <param_file>" << std::endl;
}

void lanes_init(std::vector<std::vector<Lane> >&);
void signals_init(std::vector<Signal>&);
void PrintSignalInfo(std::vector<Signal>);
void PrintLaneInfo(std::vector<std::vector<Lane> >);

// Schedule all signal changing event at the start
// params in order:
// ref to event handler
// ref to signal vector
// diff21
// diff32
// green time
// left green time
void ScheduleSignalEvents(EventHandler& , std::vector<Signal>&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

int main(int argc, char **argv) {

	if (argc!=2) {
		PrintUsage();
		exit(0);
	}
	
	// read input
	std::ifstream infile;
	infile.open(argv[1]);
	if (!(infile.good() && infile.is_open())) {
		std::cerr << "Unable to open input file" << std::endl;
		exit(0);
	}
	
	// sync flag for sync/unsync run
	unsigned int sync_flag;
	
	// total number of cars in the system
	unsigned int num_cars;
	
	// total number of time units for which system will run
	unsigned int max_time;
	
	// time for which green light is active
	unsigned int green_time;
	
	// time for which left green light is active
	unsigned int left_green_time;
	
	// There is no red time, that is derrived by green times
//	unsigned int red_time;
	
	// time taken by a car to pass an signal
	unsigned int intersect_delay;
	
	// sync parameters
	// diff in timings of an event between signal 2 and 1
	unsigned int diff21;
	
	// corr for 3 and 2
	unsigned int diff32;
	
	// read input
	infile >> sync_flag >> num_cars >> max_time;
	
	// form vehicle objects
	std::vector<Vehicle> V(num_cars);
	for (unsigned int i=0; i < num_cars; i++) {
		infile >> V[i].id >> V[i].time_entry >> V[i].src >> V[i].dest;
	}
	
	// read other params
	infile >> green_time;
	infile >> left_green_time;
//	infile >> red_time;
	infile >> intersect_delay;
	infile >> diff21;
	infile >> diff32;	
	
	// warm up time for signals, schedule traffic only after this time
	unsigned int warm_up = diff21 + diff32 + 1;
	
	// form signal objects
	std::vector<Signal> S(3);
	for (unsigned int i=0;i<3;i++) {
		S[i].ref.resize(10);
	}
	
	// set signal params
	signals_init(S);
	
	
	// form lane objects
	std::vector<std::vector<Lane> > L;
	L.resize(10);
	for (unsigned int i=0; i<L.size(); i++) {
		L[i].resize(2);
	}
	
	// manually enter front and back ends of lanes;
	lanes_init(L);
	

	
	// for event object and push events into queue
	EventHandler E;
	
	E.Lptr = &L;
	E.Vptr = &V;
	E.Sptr = &S;
	E.t_signal_delay = intersect_delay;
	
	// push enterlane events in queue
	Event_EnterLane * E_enter;
	for (unsigned int i=0; i < num_cars; i++) {
		E_enter = new Event_EnterLane;
		
		V[i].time_entry+=warm_up;
		
		E_enter->time_stamp = V[i].time_entry;
		E_enter->e = EnterLane;
		E_enter->lane_num = V[i].src;
		E_enter->vehicle_id = V[i].id;
		
		if (V[i].src == 0) { E_enter->dir = North; }
		else if (V[i].src == 3) { E_enter->dir = South; }
		else if (V[i].src <=6) { E_enter->dir = West; }
		else { E_enter->dir = East; }
		
		E.ScheduleEvent(E_enter);
	}
	
	// push signal change events in queue, for all three signals.
	// make a function
	
	ScheduleSignalEvents(E, S, diff21, diff32, green_time, left_green_time, max_time);
	
//	std::cout << "Event queue size :" << E.EventQueue.size() << std::endl;
	
	// Start processing events
	unsigned int check_infi_loop=0;
	
	Event * ETop=nullptr;
	while(!E.EventQueue.empty() and check_infi_loop++!=5000) {
		ETop = E.min_timestamp_event();
// 		E.PrintEventInfo(ETop);
		E.ProcessEvent(ETop);
		delete ETop;
	}
	
	for (unsigned int i=0; i<num_cars; i++) {
		std::cout << V[i].id << "\t" << V[i].src << "\t" << V[i].dest << "\t" << V[i].time_entry << "\t" << V[i].time_exit << std::endl;
	}
			
	return 0;
}

void ScheduleSignalEvents(EventHandler& Eptr, std::vector<Signal>& S, unsigned int diff21, unsigned int diff32, unsigned int green_time, unsigned int left_green_time, unsigned int max_time)
{
	// fill function, but first implement the functions you will use here
	Event_SignalChange * E;
	std::vector<unsigned int> delay = {0, diff21, diff21 + diff32};
	
	// this should schedule all signal changing events, recheck the logic
	for (unsigned int snum=0; snum<3; snum++) {
		unsigned int t;
		for (t=0; t<max_time;) {
			E = new Event_SignalChange;
			E->time_stamp = t + delay[snum];
			E->e = SignalChange;
			E->signal_num = snum;
			
			Eptr.ScheduleEvent(E);
			
			// assuming that the initial stage will be LLRR
			t=t+left_green_time;
			
			E = new Event_SignalChange;
			E->time_stamp = t + delay[snum];
			E->e = SignalChange;
			E->signal_num = snum;
			
			Eptr.ScheduleEvent(E);
			
			t=t+green_time;
		}
		Event_SignalChangeTrigger *Es = new Event_SignalChangeTrigger;
		Es->time_stamp = t;
		Es->e = SignalChangeTrigger;
		Es->signal_num = snum;
		Es->green_time = green_time;
		Es->left_green_time = left_green_time;
		
		Eptr.ScheduleEvent(Es);
	}
}



void lanes_init(std::vector<std::vector<Lane> >& L) {
	L[0][0].neigh_signals = {0, NO_SIGNAL};
	L[1][0].neigh_signals = {1,0};
	L[2][0].neigh_signals = {2,1};
	L[3][0].neigh_signals = {NO_SIGNAL, 2};
	L[4][0].neigh_signals = {NO_SIGNAL, 0};
	L[5][0].neigh_signals = {NO_SIGNAL, 1};
	L[6][0].neigh_signals = {NO_SIGNAL, 2};
	L[7][0].neigh_signals = {0, NO_SIGNAL};
	L[8][0].neigh_signals = {1, NO_SIGNAL};
	L[9][0].neigh_signals = {2, NO_SIGNAL};
	
	for (unsigned int i=0; i < 10; i++) {
		L[i][1].neigh_signals = L[i][0].neigh_signals;
		std::reverse(L[i][1].neigh_signals.begin(), L[i][1].neigh_signals.end());
	}
}

void PrintLaneInfo(std::vector<std::vector<Lane> > L)
{
	for (unsigned int i=0; i < L.size(); i++) {
		for (unsigned int j=0; j < L[i].size(); j++) {
			for (unsigned int k=0; k<L[i][j].neigh_signals.size(); k++) {
				std::cout << L[i][j].neigh_signals[k] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}


void signals_init(std::vector<Signal>& S) {
	// Enter ref chart manually, depends on the lane structure.
	S[0].ref = {South, North, North, North, East, North, North, West, North, North};
	S[1].ref = {South, South, North, North, South, East, North, South, West, North};
	S[2].ref = {South, South, South, North, South, South, East, South, South, West};
	
	S[0].neigh_lanes = {0,1,7,4};
	S[1].neigh_lanes = {1,2,8,5};
	S[2].neigh_lanes = {2,3,9,6};
	
	for (unsigned int i=0; i<3; i++) {
		S[i].schedule_check=false;
	}
}

void PrintSignalInfo(std::vector<Signal> S) {
	for (unsigned int i=0; i<S.size(); i++) {
		for (unsigned int j=0; j<S[i].ref.size(); j++) {
			std::cout << S[i].ref[j] << " ";
		}
		std::cout << std::endl;
	}
	
	for (unsigned int i=0; i<S.size(); i++) {
		for (unsigned int j=0; j<S[i].neigh_lanes.size(); j++) {
			std::cout << S[i].neigh_lanes[j] << " ";
		}
		std::cout << std::endl;
	}
}


