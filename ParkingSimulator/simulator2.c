#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

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

char* randomPlate(){
  
  static const char upperCase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYX";
  static const char num[] = "0123456789";
  const size_t alphSize = sizeof(upperCase)-1;
  const size_t numSize = sizeof(num)-1;
  int size = 7;
  char *randPlate = NULL;
  //char *result; 

  randPlate = malloc(size * sizeof(int));

  for(int t=0; t<3; t++){
    int randIndex = (double)rand()/RAND_MAX*alphSize;
    randPlate[t] = upperCase[randIndex];
  }

  randPlate[3] = ' ';
  
  for(int t=4; t<7; t++){
    int randIndex = (double)rand()/RAND_MAX*numSize;
    randPlate[t] = num[randIndex];
  }
  
  //free(randPlate);
  return randPlate;
}

Car* randomCar(){
  int size = 1; //size of a Car struct
  Car *randCar = NULL;
  int permit;

  permit = rand()%(2);
  randCar = malloc(size * sizeof(Car));
  initializeCar(randCar, randomPlate(), permit);

  //free(randCar);
  return randCar;
}

int main() {
  srand(time(NULL));
  Car *cars[50];
  ParkingLot *lot;
  int lotSize = 5;
  int randLot;

  lot = malloc(lotSize * sizeof(ParkingLot));

  //generate 50 cars and display them
  for(int i=0; i<50; i++){
    cars[i] = randomCar();
    printf("Car %7s with permit %d \n", *cars[i]->plateNumber, cars[i]->permit);
  }

  
  //initialize 5 parkinglots
  initializeLot(&lot[0], 1, 5, 4, 12);
  initializeLot(&lot[1], 2, 10, 5, 14);
  initializeLot(&lot[2], 3, 15, 6, 16);
  initializeLot(&lot[3], 4, 20, 7, 18);
  initializeLot(&lot[4], 5, 25, 8, 20);

  printf("\n");
  
  //display the 5 parkinglots
  for(int i=0; i<lotSize; i++){
    printf("Parking Lot #%d - rate = $%4.2f, capacity %d, current cars %d\n", lot[i].lotNumber, lot[i].hourlyRate, lot[i].capacity, lot[i].currentCarCount);
    }

  printf("\n");

  //randomly park cars in different lots and increment arrival time of the next car by 5 mins.
  int counter = 0;
  int i;
  int m = 0;
  int h = 6;
  for(i=0; i<50; i++){
    randLot = rand()%(5);
    m = m+5;
    if(m==60){
      counter++;
      m=m+(i*5)-(60*counter);
      h++;
    }
    carEnters(&lot[randLot], cars[i], h, m);
  }

  printf("\n");
  
  //display the 5 parkinglots again
  for(int i=0; i<lotSize; i++){
    printf("Parking Lot #%d - rate = $%4.2f, capacity %d, current cars %d\n", lot[i].lotNumber, lot[i].hourlyRate, lot[i].capacity, lot[i].currentCarCount);
  }

  printf("\n");

  //all cars leave the lots
  int h2 = 11;
  int m2 = 0;
  int counter2 = 0;
  for(i=0; i<50; i++){
    if(cars[i]->lotParkedIn>0){
      m2 = m2+5;
      if(m2==60){
	counter2++;
	m2=m2+(i*5)-(60*counter2);
	h2++;
      }
      carLeaves(&lot[cars[i]->lotParkedIn-1], cars[i], h2, m2);
    }
  }

  printf("\n");
  
  //display the 5 parkinglots again
  for(int i=0; i<lotSize; i++){
    printf("Parking Lot #%d - rate = $%4.2f, capacity %d, current cars %d\n", lot[i].lotNumber, lot[i].hourlyRate, lot[i].capacity, lot[i].currentCarCount);
  }

  printf("\n");


  // Display the total revenue
  printf("Total revenue of Lot 1 is $%4.2f\n", lot[0].revenue);
  printf("Total revenue of Lot 2 is $%4.2f\n", lot[1].revenue);
  printf("Total revenue of Lot 3 is $%4.2f\n", lot[2].revenue);
  printf("Total revenue of Lot 4 is $%4.2f\n", lot[3].revenue);
  printf("Total revenue of Lot 5 is $%4.2f\n", lot[4].revenue);

  free(lot);
  return 0;
}
