#include <List>
#include "DayCalendar_t.h"
#include "Meeting_t.h"
#include "MeetingWithLocation_t.h"
using namespace std;

DayCalendar_t::DayCalendar_t(){ 												//CTOR
	m_meetingsList = *(new list<Meeting_t*>());
}

DayCalendar_t::~DayCalendar_t(){												//DTOR
	for (list<Meeting_t*>::const_iterator it = m_meetingsList.begin(); it != m_meetingsList.end(); ++it) {
		delete(&(*it));
	}
	delete(&m_meetingsList);
}
DayCalendar_t::DayCalendar_t(const DayCalendar_t& pr){}							//Copy CTOR

const DayCalendar_t& DayCalendar_t::operator=(const DayCalendar_t& pr){			//Operator =
	if (this == &pr)  // check for self-assignment by comparing the addresses
		return *this;
	m_meetingsList = pr.m_meetingsList;
	return *this;
}

void DayCalendar_t::insertMeeting(Meeting_t* const meeting){
	bool canAdd = true;
	for (list<Meeting_t*>::const_iterator ci = m_meetingsList.begin(); ci != m_meetingsList.end(); ++ci)
		if (*((Meeting_t*)*ci) == *meeting){
			canAdd = false;
			break;
		}
	if (canAdd){
		// Add the new meeting
		m_meetingsList.push_back(meeting);
		// Sort all meetings
		m_meetingsList.sort([](const Meeting_t* first, const Meeting_t* second) { return ((*first).getStartTime() < (*second).getStartTime()); });
	}
	else {
		throw "(ERROR) Can\'t add meeting. It intersects with an existing meeting.";
	}
}

void DayCalendar_t::insertMeeting(float startTime, float endTime, string& subject){
	if (startTime > endTime){
		throw "(ERROR) Bad meeting times. Start time must be lower than end time.";
	}
	Meeting_t * m = new Meeting_t(startTime, endTime, subject);
	insertMeeting(m);
}

void DayCalendar_t::deleteMeeting(Meeting_t* const meeting){
	m_meetingsList.remove(meeting);
}

void DayCalendar_t::deleteMeeting(float startTime){
	Meeting_t* m = searchForMeeting(startTime);
	if (m != NULL){
		m_meetingsList.remove(m);
	} 
	else {
		throw "(ERROR) Trying to delete a non-existing meeting.";
	}
}

Meeting_t* const DayCalendar_t::searchForMeeting(float startTime) const {
	for (list<Meeting_t*>::const_iterator it = m_meetingsList.begin(); it != m_meetingsList.end(); ++it)
		if ((*((Meeting_t*)*it)).getStartTime() == startTime)
			return ((Meeting_t*)*it);
	return NULL;
}

ostream& operator<<(ostream& os, const DayCalendar_t &calendar){
	list<Meeting_t*> l = calendar.m_meetingsList;
	for (list<Meeting_t*>::iterator it = l.begin(); it != l.end(); it++) {
		Meeting_t m = *((Meeting_t*)*it);
		os << m;
		if (MeetingWithLocation_t * ml = dynamic_cast<MeetingWithLocation_t*>(*it)) {
			os << ' ' + (*ml).getLocation();
		}
		os << '\n';
	}
	os << '\n';
	return os;
}


