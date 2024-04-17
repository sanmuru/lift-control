#pragma once

// exports.h

struct LiftStatus {
	int floor;
	enum { IDLE, ACUP, DEUP, ACDN, DEDN, OPDR, CLDR } condition;
	int load;
};

enum GuestDirection { Up = 1, Down = 2, Both = Up | Down };

extern "C"
{
	__declspec(dllexport) void* CreateBuilding(int iTall);
	__declspec(dllexport) void DisposeBuilding(void* pBuilding);
	__declspec(dllexport) void* AddLift(void* building, int iAtFloor, bool (*fpFilter)(int), int iCapacity);

	__declspec(dllexport) LiftStatus GetLiftStatus(void* pLift);

	__declspec(dllexport) void TimePassing(void* pBuilding, int iTime);

	__declspec(dllexport) void GuestRequest(void* pBuilding, int iAtFloor, GuestDirection iDirection);
	__declspec(dllexport) bool GuestEnter(void* pLift, int iWeight);
	__declspec(dllexport) void GuestOrder(void* pLift, int iToFloor);
}