#include<iostream>
#include<stdlib.h>
#include<time.h>
#include"PriorityQueue.h"
#include"Framework.h"

using namespace std;

class SoftwareGurusBar {
public:
// try with 50 chairs, then try with 40, 60, ...
// in order to find out the optimal profit.
    SoftwareGurusBar()
    : freeChairs(66), profit(0.0),local_beer(0),imported_beer(0),special_beer(0){}
    bool canSeat(unsigned int numberOfPeople);
    void order(unsigned int beerType);
    void leave(unsigned int numberOfPeople);

    unsigned int freeChairs;
    double profit;
    int local_beer,imported_beer,special_beer;
};

SoftwareGurusBar theBar;
SimulationFramework theSimulation;

bool SoftwareGurusBar::canSeat (unsigned int numberOfPeople)
{
// if sufficient room, then seat customers
    cout << "\nTime: " << theSimulation.currentTime <<" minutes"<< ", ";
    cout << " Group of " << numberOfPeople << " customers arrives,";
    if (numberOfPeople < freeChairs) {
        cout << " Group is seated" << endl;
        freeChairs -= numberOfPeople;
    return false;
    }
    else {
    cout << " No room, group leaves" << endl;
    return true;
    }
}

 void SoftwareGurusBar::order (unsigned int beerType)
{

// serve beer
    cout << "\nTime: " << theSimulation.currentTime <<" minutes"<< ", ";
    cout << " serviced order for beer type " << beerType << endl;
// update profit based on this beerType
// add your code here ...
    switch(beerType)
    {
        case 1:
            profit += 2;
            local_beer += 1;
            break;

        case 2:
            profit += 3;
            imported_beer += 1;
            break;

        case 3:
            profit += 4;
            special_beer += 1;
            break;

    }

}

void SoftwareGurusBar::leave (unsigned int numberOfPeople)
{
    cout << "\nTime: " << theSimulation.currentTime <<" minutes"<< ", ";
    cout << " group of size " << numberOfPeople << " leaves" << endl;
    freeChairs += numberOfPeople;
}


class ArriveEvent : public Event {
public:
    ArriveEvent (unsigned int time, unsigned int gs) : Event(time), groupSize(gs) { }
    virtual void processEvent ();
protected:
    unsigned int groupSize;
};

class OrderEvent : public Event {
public:
    OrderEvent (unsigned int time, unsigned int gs) : Event(time), groupSize(gs) { }
    virtual void processEvent ();
protected:
    unsigned int groupSize;
};



void ArriveEvent::processEvent()
{
    if ( theBar.canSeat(groupSize) != 1 )
// place an order within 2 & 10 minutes
    theSimulation.scheduleEvent(
    new OrderEvent (theSimulation.currentTime + randBetween(2,10),
    groupSize));
}

class reOrderEvent : public Event{
public://class of re-orderEvent

  reOrderEvent (unsigned int time, unsigned int gs) : Event(time), groupSize(gs) { }

  virtual void processEvent();

protected:
  unsigned int groupSize;
};


class LeaveEvent : public Event {
public:
    LeaveEvent (unsigned int time, unsigned int gs) : Event(time), groupSize(gs) { }
    virtual void processEvent ()
    {
      theBar.leave(groupSize);
    }
protected:
    unsigned int groupSize;
};


void OrderEvent::processEvent()
{
// each member of the group orders a beer (type 1,2,3)
    for (int i = 0; i < groupSize; i++)
        theBar.order(theSimulation.weightedProbability(15,25,60));

    int t = theSimulation.currentTime + randBetween(30,60);

    int guess=randBetween(1,10);

    if(guess<5)//make a guess to see if they need to reorder or leave
      theSimulation.scheduleEvent(new reOrderEvent(t,groupSize));//schedule a re-order event to see if they need to re-order

    else
    theSimulation.scheduleEvent(new LeaveEvent(t, groupSize));
};

void reOrderEvent::processEvent()
{
cout << "\nTime: " << theSimulation.currentTime << " minutes, the group of " << groupSize << " has reordered for round 1 "<<endl;
for (int i = 0; i < groupSize; i++)//call the order function
    theBar.order(theSimulation.weightedProbability(15,25,60));

int t = theSimulation.currentTime + randBetween(30,60);

int guess = randBetween(1,10);//make a random guess to see if needed re-order for the second round

if(guess > 5){
cout << "\nTime: " << theSimulation.currentTime << " minutes, the group of " << groupSize << " has reordered for round 2 "<<endl;
for(int i = 0; i < groupSize; i++)
theBar.order(theSimulation.weightedProbability(15,25,60));//call the order function
t = theSimulation.currentTime + randBetween(30,60);
}

  theSimulation.scheduleEvent(new LeaveEvent(t, groupSize));
}



int main() {
    cout<<"SoftwareGurus Bar Simulation, testing 73 seats\n";
// load priority queue with initial Arrive Events then run simulation
    unsigned int t = 0;
// load 4 hours (240 minutes) of Arrive Events
    while (t < 240) {
// new group every 2-5 minutes
    t += randBetween(2,5);
// group size ranges from 1 to 5
    theSimulation.scheduleEvent(new ArriveEvent(t, randBetween(1,5)));
    }
// then run simulation and print profits
    theSimulation.run();
    cout << "\n" <<endl;
    cout << "Total profits: " << theBar.profit << endl;
    cout << "Local beer sold: " << theBar.local_beer << endl;
    cout << "Imported beer sold: " << theBar.imported_beer<< endl;
    cout << "Special beer sold: " << theBar.special_beer << endl;


    return 0;
}
