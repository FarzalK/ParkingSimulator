#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct {
  int hour;
  int minute;
} Time;

typedef struct {
  char *plateNumber[7];
  char permit;
  Time enteringTime;
  int lotParkedIn;
} Car;

typedef struct {
  int lotNumber;
  double hourlyRate;
  double maxCharge;
  int capacity;
  int currentCarCount;
  double revenue;
} ParkingLot;

// Sets the hours and minutes amount for the given time t based 
// on the specified hours h.  (e.g., 1.25 hours would be 1 hour 
// and 15 minutes)
void setHours(Time *t, double h) {
  t->hour = (int)(h); //get number without the decimal as an int
  t->minute = (int)((h - (int)(h)) * 60); //get decimal value then multiply by 60 to get minute equivalent
}

// Takes two Time objects (not pointers) and computes the difference 
// in time from t1 to t2 and then stores that difference in the diff 
// Time (which must be a pointer)
void difference(Time t1, Time t2, Time *diff) {
  //check if t1.minute>t2.minute then set difference for minute
  if (t2.minute>=t1.minute){
    diff->minute = t2.minute - t1.minute;
  //else increase minute and reduce hour to subract accurately
  } else {
    t2.hour -= 1;
    t2.minute += 60;
    diff->minute = t2.minute - t1.minute;
  }
  //set difference for hour
  diff->hour = t2.hour - t1.hour;
}


// Initialize the car pointed to by c to have the given plate and 
// hasPermit status.  The car should have it’s lotParkedIn set to 
// 0 and enteringTime to be -1 hours and -1 minutes.
void initializeCar(Car *c, char *plate, char hasPermit) {
  //Car car;
  *c->plateNumber = plate;
  c->permit = hasPermit;
  c->lotParkedIn = 0;
  c->enteringTime.hour = -1;
  c->enteringTime.minute = -1;
}


// Initialize the lot pointed to by p to have the given number, 
// capacity, hourly rate and max charge values.  The currentCarCount 
// and revenue should be at 0.
void initializeLot(ParkingLot *p, int num, int cap, double rate, double max) {
  p->lotNumber = num;
  p->capacity = cap;
  p->hourlyRate = rate;
  p->maxCharge = max;
  p->currentCarCount = 0;
  p->revenue = 0;
}

// Print out the parking lot parameters so that it displays as 
// follows:   Parking Lot #2 - rate = $3.00, capacity 6, current cars 5
void printLotInfo(ParkingLot p) {
  printf("Parking Lot #%d - rate = $%4.2f, capacity %d, current cars %d\n", p.lotNumber, p.hourlyRate, p.capacity, p.currentCarCount);
}


// Simulate a car entering the parking lot
// ...
//check if lot is full
//car enters specified parkinglot
//cars entering time is set
//parkinglot's car count increases by 1
void carEnters(ParkingLot *p, Car *c, int h, int m){
  if(p->capacity==p->currentCarCount){
    printf("Car %7s arrives at Lot %d at %d:%02d, but the lot is full.\n", *c->plateNumber, p->lotNumber, h, m);
    printf("Car %7s cannot get in.\n", *c->plateNumber);
  } else {
    c->lotParkedIn = p->lotNumber;  //parked the car
    c->enteringTime.hour = h;      //set hour of arrival
    c->enteringTime.minute = m;    //set minute of arrival
    p->currentCarCount++;          //increase car count in parking lot

    printf("Car %7s enters Lot %d at %d:%02d.\n", *c->plateNumber, p->lotNumber, h, m);
  }
}

// Simulate a car leaving the parking lot
// ...
//checks if car has permit
//if car has permit then parking is not charged 
//else hourly rate is applied and is charged based on time difference
//max charge of parking lot is kept in mind for calculations
//car leaves specified parkinglot
void carLeaves(ParkingLot *p, Car *c, int h, int m){
  Time exitTime;
  Time diff;
  double rev;
  exitTime.hour = h;
  exitTime.minute = m;
  
  if(!c->permit == 1){   //if has permit
    difference(c->enteringTime, exitTime, &diff); //set time difference

    if(diff.minute>0)
      diff.hour++;

    rev = p->hourlyRate*(diff.hour);  //calculate parking cost
    if(rev>p->maxCharge){ //check if there are overages
      rev = p->maxCharge;
    }
    p->revenue += rev;    //add revenue made from the cars alloted time
    c->lotParkedIn = 0;   //car leaves parkinglot
    p->currentCarCount--; //parkinglots car count decreases by 1

    printf("Car %7s leaves Lot %d at %d:%02d paid $%4.2f.\n", *c->plateNumber, p->lotNumber, h, m, rev);
  } else {
    c->lotParkedIn = 0;   //car leaves parkinglot
    p->currentCarCount--; //parkinglots car count decreases by 1
    printf("Car %7s leaves Lot %d at %d:%02d.\n", *c->plateNumber, p->lotNumber, h, m);
  }
}

int main() {
  
 Car car1, car2, car3, car4, car5, car6, car7, car8, car9;
 ParkingLot p1, p2;

 // Set up 9 cars
 initializeCar(&car1, "ABC 123", 0);
 initializeCar(&car2, "ABC 124", 0);
 initializeCar(&car3, "ABD 314", 0);
 initializeCar(&car4, "ADE 901", 0);
 initializeCar(&car5, "AFR 304", 0);
 initializeCar(&car6, "AGD 888", 0);
 initializeCar(&car7, "AAA 111", 0);
 initializeCar(&car8, "ABB 001", 0);
 initializeCar(&car9, "XYZ 678", 1);
 
 // Set up two parking lots
 initializeLot(&p1, 1, 4, 5.5, 20.0);
 initializeLot(&p2, 2, 6, 3.0, 12.0);
 printLotInfo(p1);
 printLotInfo(p2);
 printf("\n");
 
 // Simulate cars entering the lots
 carEnters(&p1, &car1, 7, 15);
 carEnters(&p1, &car2, 7, 25);
 carEnters(&p2, &car3, 8, 0);
 carEnters(&p2, &car4, 8, 10);
 carEnters(&p1, &car5, 8, 15);
 carEnters(&p1, &car6, 8, 20);
 carEnters(&p1, &car7, 8, 30);
 carEnters(&p2, &car7, 8, 32);
 carEnters(&p2, &car8, 8, 50);
 carEnters(&p2, &car9, 8, 55);
 printf("\n");
 printLotInfo(p1);
 printLotInfo(p2);
 printf("\n");
 
 // Simulate cars leaving the lots
 carLeaves(&p2, &car4, 9, 0);
 carLeaves(&p1, &car2, 9, 5);
 carLeaves(&p1, &car6, 10, 0);
 carLeaves(&p1, &car1, 10, 30);
 carLeaves(&p2, &car8, 13, 0);
 carLeaves(&p2, &car9, 15, 15); 
 carEnters(&p1, &car8, 17, 10);
 carLeaves(&p1, &car5, 17, 50);
 carLeaves(&p2, &car7, 18, 0);
 carLeaves(&p2, &car3, 18, 15);
 carLeaves(&p1, &car8, 20, 55);
 printf("\n");
 printLotInfo(p1);
 printLotInfo(p2);
 printf("\n");
 
 // Display the total revenue
 printf("Total revenue of Lot 1 is $%4.2f\n", p1.revenue);
 printf("Total revenue of Lot 2 is $%4.2f\n", p2.revenue);
}
