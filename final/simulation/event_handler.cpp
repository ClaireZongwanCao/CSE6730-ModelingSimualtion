#include "event_handler.h"
#include <iostream>

// return the event with min timestamp
Event* EventHandler::min_timestamp_event()
{
	Event* E = EventQueue.top();
	EventQueue.pop();
	return E;
}

void EventHandler::ProcessEvent(Event* E)
{
	
	// handling the enter lane event
	if (E->e == EnterLane) {
		
		// extract values from event
		unsigned int lane = static_cast<Event_EnterLane*>(E)->lane_num;
		unsigned int vehicle = static_cast<Event_EnterLane*>(E)->vehicle_id;
		DirType dir = static_cast<Event_EnterLane*>(E)->dir;
		unsigned int d=((int)dir)%2;
		
		// check if its the first car in the lane and the forward signal is not NO_SIGNAL
		if ((*Lptr)[lane][d].contents.empty() and (*Lptr)[lane][d].neigh_signals[0]!=NO_SIGNAL) {
			
			// schedule a new signal check event
			Event_SignalCheck * En = new Event_SignalCheck;
			En->time_stamp = E->time_stamp;
			En->signal_num = (*Lptr)[lane][d].neigh_signals[0]; // front
			En->e = SignalCheck;
			En->dir = static_cast<Event_EnterLane*>(E)->dir;
			
			ScheduleEvent(En);
			
		}
		if ((*Lptr)[lane][d].neigh_signals[0]!=NO_SIGNAL) {
			(*Lptr)[lane][d].contents.push(vehicle);
		}
		// if I want to enter a lane with no front signal, I'd rather disappear
		else {
			(*Vptr)[vehicle].time_exit = E->time_stamp;
		}
		
		// writing log
// 		std::cout << "Vehicle no " <<  vehicle << " entering " << lane << " in direction " << H[dir] << " at time " << E->time_stamp << std::endl;
		
	}
	
	// handling the exit lane event
	else if (E->e == ExitLane) {
		
		// extracting values from event
		unsigned int lane = static_cast<Event_ExitLane*>(E)->lane_num;
		unsigned int dir = (int)static_cast<Event_ExitLane*>(E)->dir;
		unsigned int d=dir%2;
// 		unsigned int vid = (*Lptr)[lane][d].contents.front();
		
		// put exit time in the vehile structure
// 		(*Vptr)[vid].time_exit = E->time_stamp;
		
		// writing log
//		std::cout << "\t\t\tVehicle no " << vid << " exiting " << lane << " in direction " << H[dir] << " at time " << E->time_stamp << std::endl;
		(*Lptr)[lane][d].contents.pop();
	}
	
	// handling the signal check event
	else if (E->e == SignalCheck) {
		
		// extracting the values from event
		unsigned int signal_num = static_cast<Event_SignalCheck*>(E)->signal_num;
		DirType in_dir = static_cast<Event_SignalCheck*>(E)->dir;
		unsigned int d = in_dir%2;
		unsigned int in_lane = (*Sptr)[signal_num].neigh_lanes[(int)in_dir];
// 		std::cout << "Sig " << signal_num << " " << H[in_dir] << " " << d << " " << "inlane " << in_lane << std::endl;
		
		// if incoming lane is empty, do nothing
		if ((*Lptr)[in_lane][d].contents.empty()) {
// 			std::cout << "Lane " << in_lane << " in direction " << H[in_dir] << " is empty" << std::endl;
		}
		
		// if there is no signal ahead, do nothing
		else if (signal_num==NO_SIGNAL) {
// 			std::cout << "No signal ahead" << std::endl;
		}
		
		
		else {
			
			// extracting values from evant
// 			std::cout << "Queue size of lane " << in_lane << " is " << (*Lptr)[in_lane][d].contents.size() << std::endl;
			unsigned int vehicle = (*Lptr)[in_lane][d].contents.front();
			DirType out_dir;
			out_dir = (*Sptr)[signal_num].ref[(*Vptr)[vehicle].dest];
			unsigned int out_lane=0;

			// the directions are for the incoming lanes in signal, need to be inverted for outgoing
			out_lane = (*Sptr)[signal_num].get_outlane_for_dir(out_dir);
// 			std::cout << "vehicle " << vehicle << ", out lane " << out_lane << std::endl;

			// check if the car can go in desired direction
			if ((*Sptr)[signal_num].SignalCheck(in_dir, out_dir)) {
				
// 				std::cout << "Yay! The signal is green" << std::endl;
				
				// schedule for car to enter new lane
				Event_EnterLane * E1 = new Event_EnterLane;
				E1->time_stamp = E->time_stamp + t_signal_delay;
				E1->e = EnterLane;
				E1->lane_num = out_lane;
				E1->vehicle_id = vehicle;
				E1->dir = out_dir;
				
				ScheduleEvent(E1);
				
				
				// schedule for car to leave current lane
				Event_ExitLane * E2 = new Event_ExitLane;
				E2->time_stamp = E->time_stamp;
				E2->e = ExitLane;
				E2->lane_num = in_lane;
				E2->dir = in_dir;
				
				ScheduleEvent(E2);
				
				
				// schedule for checking the signal again after the current car is gone
				Event_SignalCheck * E3 = new Event_SignalCheck;
				E3->time_stamp = E->time_stamp + t_signal_delay;
				E3->e = SignalCheck;
				E3->signal_num = signal_num;
				E3->dir = in_dir;
				
				ScheduleEvent(E3);
		
			}
			else { // schedule a signal check when the signal changes next time
				
//  				std::cout << "Damn, the signal is RED for " << H[in_dir] << " to " << H[out_dir] << std::endl;
				(*Sptr)[signal_num].schedule_check = true;
				(*Sptr)[signal_num].check_dir.push_back(in_dir);
			}
		}
	}
	else if (E->e == SignalChange) {
		unsigned int signal_num = static_cast<Event_SignalChange*>(E)->signal_num;
		(*Sptr)[signal_num].SignalChange();
		
// 		std::cout << "Lights changed for signal " << signal_num << ": ";
// 		for (unsigned int i=0; i < (*Sptr)[signal_num].light.size(); i++) {
// 			std::cout << L[(*Sptr)[signal_num].light[i]] << " ";
// 		}
// 		std::cout << std::endl;

		if ((*Sptr)[signal_num].schedule_check) {
			
			// remove redundant values from the check_dir vector
			std::vector<DirType>::iterator it = std::unique((*Sptr)[signal_num].check_dir.begin(), (*Sptr)[signal_num].check_dir.end());
			(*Sptr)[signal_num].check_dir.resize(std::distance((*Sptr)[signal_num].check_dir.begin(), it));
			
			Event_SignalCheck *Echeck;
			for (unsigned int i=0; i < (*Sptr)[signal_num].check_dir.size(); i++) {
				Echeck = new Event_SignalCheck;
				Echeck->time_stamp = E->time_stamp;
				Echeck->signal_num = signal_num;
				Echeck->e = SignalCheck;
				Echeck->dir = (*Sptr)[signal_num].check_dir[i];
				
// 				std::cout << "Scheduled re-check for signal " << signal_num << " and dir " << Echeck->dir << std::endl;
				
				ScheduleEvent(Echeck);
			}
			
			(*Sptr)[signal_num].check_dir.clear();
			(*Sptr)[signal_num].schedule_check=false;
		}
	}
	else if (E->e == SignalChangeTrigger) {
		// schedule signal changing events if there are any cars left
		bool no_car=true;
		
		for (unsigned int i=0; i < (*Lptr).size(); i++) {
			if (!(*Lptr)[i][0].contents.empty() or !(*Lptr)[i][1].contents.empty()) {
				no_car=false;
				break;
			}
		}
		
		// if there is at least one cars
		if (!no_car) {
			unsigned int t=static_cast<Event_SignalChangeTrigger*>(E)->time_stamp;
			unsigned int signal_num = static_cast<Event_SignalChangeTrigger*>(E)->signal_num;
			Event_SignalChange * Es;
			for (unsigned int i=0; i<10; i++) {
				Es = new Event_SignalChange;
				Es->time_stamp = t;
				Es->e = SignalChange;
				Es->signal_num = signal_num;
				
				ScheduleEvent(Es);
				
				// assuming that the initial stage will be LLRR
				t=t+static_cast<Event_SignalChangeTrigger*>(E)->left_green_time;
				
				Es = new Event_SignalChange;
				Es->time_stamp = t;
				Es->e = SignalChange;
				Es->signal_num = signal_num;
				
				ScheduleEvent(Es);
				
				t=t+static_cast<Event_SignalChangeTrigger*>(E)->green_time;
			}
//			std::cout << "Scheduling 10 more on signal " << signal_num << std::endl;
		}
		else {
//			std::cout << "Signal change on " << static_cast<Event_SignalChangeTrigger*>(E)->signal_num << " over" << std::endl;
		}
	}
	else {
		std::cerr << "Event not recognized" << std::endl;
	}
}

// Schedule E at time t
void EventHandler::ScheduleEvent(Event* E)
{
	E->time_stamp = E->time_stamp;
	EventQueue.push(E);	
}

void EventHandler::PrintEventInfo(Event* E)
{
	if (E->e==EnterLane) {
		std::cout << "\t\t\tTime: " << E->time_stamp << std::endl;
		std::cout << "\t\t\tType: ENTER LANE" << std::endl;
		std::cout << "\t\t\tLane no: " << static_cast<Event_EnterLane*>(E)->lane_num<< std::endl;
		std::cout << "\t\t\tDir: " << static_cast<Event_EnterLane*>(E)->dir <<  std::endl;
		std::cout << std::endl;
	}
	else if (E->e==ExitLane) {
		std::cout << "\t\t\t\t\t\tTime: " << E->time_stamp << std::endl;
		std::cout << "\t\t\t\t\t\tType: EXIT LANE" << std::endl;
		std::cout << "\t\t\t\t\t\tLane no: " << static_cast<Event_ExitLane*>(E)->lane_num << std::endl;
		std::cout << "\t\t\t\t\t\tDir: " << static_cast<Event_ExitLane*>(E)->dir <<  std::endl;
		std::cout << std::endl;
	}
	else if (E->e==SignalCheck) {
		std::cout << "\t\t\t\t\t\t\t\t\tTime: " << E->time_stamp << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\tType: SIGNAL CHECK" << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\tSignal no: " << static_cast<Event_SignalCheck*>(E)->signal_num << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\tDir: " << static_cast<Event_SignalCheck*>(E)->dir <<  std::endl;
		std::cout << std::endl;
	}
	else if (E->e==SignalChange) {
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tTime: " << E->time_stamp << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tType: SIGNAL CHANGE" << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tLights pre-change: ";
		for (unsigned int i=0; i < (*Sptr)[static_cast<Event_SignalChange*>(E)->signal_num].light.size(); i++) {
			std::cout << (*Sptr)[static_cast<Event_SignalChange*>(E)->signal_num].light[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tSignal no.: " << static_cast<Event_SignalChange*>(E)->signal_num << std::endl;
		std::cout << std::endl;
	}
}
