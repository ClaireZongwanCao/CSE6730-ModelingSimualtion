#include <queue>
#include <vector>


#ifndef CONTENTS_H
#define CONTENTS_H

#define NO_SIGNAL 10

typedef enum {North, South, East, West} DirType;
typedef enum {LeftGreen, Green, Red} LightType;

// class to simulate vehicles
class Vehicle {
public:
	// @@@@Variables@@@@

	// id of the vehicle
	unsigned int id;
	
	// source lane for the vehicle
	unsigned int src;
	
	// destination lane for the vehicle
	unsigned int dest;
	
	// time at which vehicle enters the src lane
	unsigned int time_entry;
	
	// time at which vehicle exits the dest lane
	unsigned int time_exit;
	
	// current lane and direction for the vehicle
	unsigned int curr_lane;
	DirType cur_dir;
};

// class to implement lanes
class Lane {
public:	
	// @@@@Variables@@@@
	
	// lane number
	unsigned int lane_num;
	
	// direction of the queue
	DirType dir;
	
	// information about front and back signal number respectively
	std::vector<unsigned int> neigh_signals;
	
	// ids of the cars in the queue currently
	std::queue<unsigned int> contents;
	
	// @@@@Functions@@@@
	
	// V is entering in lane L in direction dir
	void EnterLane (unsigned int Vid);

	// The first vehicle in lane L is exiting in direction dir
	void ExitLane ();
};

class Signal {
public:
	// @@@@Variables@@@@
	// id for the signal
	unsigned int signal_num;
	
	// if true, schedule a signal check in direction check_dir, initialize to false
	bool schedule_check;
	std::vector<DirType> check_dir;
	
	// information about neighbouring lanes
	// neigh_lanes[dir] is the lane number for incoming lane that is going in the direction dir.
	std::vector<unsigned int> neigh_lanes;

	// Lights for four directions, in order N,S,E,W
	std::vector<LightType> light;
	
	// direction info, given a lane number, which direction to go for that lane
	std::vector<DirType> ref;
	
	// @@@@Functions@@@@

	// For a given direction d, get the lane number that goes towards that direction
	unsigned int get_outlane_for_dir(DirType d);

	// If a car is coming towards indir and going towards outdir, should it be allowed
	bool SignalCheck(DirType indir, DirType outdir);
	
	// depending on the current configuration, switch to the next config
	void SignalChange();
};
#endif // CONTENTS_H