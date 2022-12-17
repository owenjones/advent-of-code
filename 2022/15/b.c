#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// #define MAXCOORD 20
#define MAXCOORD 4000000

typedef struct sensor {
  int x, y, range;
} sensor_t;

int is_the_beacon(uint64_t* x, uint64_t* y, sensor_t* sensors, int s, int c) {
  for(size_t i = 0; i < s; i++) {
    if(i == c) continue;
    if(*x < 0 || *x > MAXCOORD || *y < 0 || *y > MAXCOORD) return 0;
    if((abs(*x - sensors[i].x) + abs(*y - sensors[i].y)) <= sensors[i].range) {
      return 0;
    }
  }
  printf("beacon at (%llu, %llu)\n", *x, *y);
  return 1;
}

void find_the_beacon(sensor_t* sensors, int s, uint64_t* x, uint64_t* y) {
  for(size_t i = 0; i < s; i++) {
    *x = sensors[i].x - sensors[i].range - 1;
    *y = sensors[i].y;
    while(*x <= sensors[i].x) {
      if(is_the_beacon(x, y, sensors, s, i)) return;
      (*x)++;
      (*y)--;
    }
    
    *x = sensors[i].x;
    *y = sensors[i].y - sensors[i].range - 1;
    while(*y <= sensors[i].y) {
      if(is_the_beacon(x, y, sensors, s, i)) return;
      (*x)++;
      (*y)++;
    }
    
    *x = sensors[i].x + sensors[i].range + 1;
    *y = sensors[i].y;
    while(*x <= sensors[i].x) {
      if(is_the_beacon(x, y, sensors, s, i)) return;
      (*x)--;
      (*y)++;
    }
    
    *x = sensors[i].x;
    *y = sensors[i].y + sensors[i].range + 1;
    while(*y <= sensors[i].y) {
      if(is_the_beacon(x, y, sensors, s, i)) return;
      (*x)--;
      (*y)--;
    }  
  }
  return;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int s = 0;
  sensor_t sensors[25];

  int ax, ay, bx, by, d;
  while(fscanf(fptr, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &ax, &ay, &bx, &by) == 4) {
    d = abs(ax - bx) + abs(ay - by);
    
    sensor_t sensor;
    sensor.x = ax;
    sensor.y = ay;
    sensor.range = d;
    sensors[s++] = sensor;
  }
  fclose(fptr);
  
  uint64_t x, y;
  find_the_beacon(sensors, s, &x, &y);
  uint64_t frequency = (uint64_t) (x * MAXCOORD) + y;
  printf("Frequency of distress beacon: %llu\n", frequency); // 13029714573243
  return 0;
}
