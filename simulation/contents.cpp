#include "contents.h"

void Lane::EnterLane(unsigned int Vid)
{
// 	if (contents.empty()) {
		
		// schedule it, may be it should go in process event
//	}
}

void Lane::ExitLane()
{

}

bool Signal::SignalCheck(DirType indir, DirType outdir)
{
	LightType l = light[indir];
	
	// if turning right, always return true
	if ((indir == North and outdir == East) or (indir == East and outdir == South) or (indir == South and outdir == West) or (indir == West and outdir == North)) {
		return true;
	}
	
	if (l==LeftGreen) {
		if ((indir == East and outdir == North) or (indir == South and outdir == East) or (indir == West and outdir == South) or (indir == North and outdir == West)) {
			return true;
		}
	}
	
	else if (l==Green) {
		if (indir == outdir) {
			return true;
		}
	}
	return false;
}

// Emulating the cycle: LLRR -> GGRR -> RRLL -> RRGG -> LLRR
void Signal::SignalChange()
{
	// if empty, init
	if (light.size()==0) {
		light = {LeftGreen, LeftGreen, Red, Red};
	}
	
	else {
		if (light[North]==LeftGreen) {
			light[North] = Green;
			light[South] = Green;
		}
		else if (light[North] == Green) {
			light[North] = Red;
			light[South] = Red;
			light[East] = LeftGreen;
			light[West] = LeftGreen;
		}
		else { // light[North]==Red
			if (light[East] == LeftGreen) {
				light[East] = Green;
				light[West] = Green;
			}
			else if (light[East] == Green) {
				light[North] = LeftGreen;
				light[South] = LeftGreen;
				light[East] = Red;
				light[West] = Red;
			}
		}
	}
}

unsigned int Signal::get_outlane_for_dir(DirType d)
{
	if (d==North) { return neigh_lanes[South]; }
	else if (d==South) { return neigh_lanes[North]; }
	else if (d==East) { return neigh_lanes[West]; }
	else if (d==West) { return neigh_lanes[East]; }
	return 10;
}