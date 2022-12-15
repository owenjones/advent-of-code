#include "beacons.h"

int main(void) {
  FILE* fptr;
  if((fptr = fopen("test_input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  list_t* sensors = list();
  list_t* beacons = list();

  // 30000000
  grid_t* map = grid(50);

  int sx, sy, bx, by;
  while(fscanf(fptr, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sx, &sy, &bx, &by) == 4) {
    append(sensors, point_at(sx + map->offset, sy + map->offset));
    append(beacons, point_at(bx + map->offset, by + map->offset));
  }
  fclose(fptr);

  int d;
  size_t j;
  for(size_t i = 0; i < sensors->len; i++) {
    // 1-to-1 mapping between each sensor and beacon
    d = distance(sensors->point[i], beacons->point[i]);

    for(j = 0; j < d; j++) {
      // we know circle with circumference <distance> is clear of beacons
      map->values[c2ind(map, sensors->point[i]->x + j, sensors->point[i]->y)] = 1; // E
      map->values[c2ind(map, sensors->point[i]->x + j, sensors->point[i]->y - j)] = 1; // SE
      map->values[c2ind(map, sensors->point[i]->x, sensors->point[i]->y + j)] = 1; // S
      map->values[c2ind(map, sensors->point[i]->x - j, sensors->point[i]->y + j)] = 1; // SW
      map->values[c2ind(map, sensors->point[i]->x - j, sensors->point[i]->y)] = 1; // W
      map->values[c2ind(map, sensors->point[i]->x - j, sensors->point[i]->y - j)] = 1; // NW
      map->values[c2ind(map, sensors->point[i]->x - j, sensors->point[i]->y)] = 1; // N
      map->values[c2ind(map, sensors->point[i]->x - j, sensors->point[i]->y + j)] = 1; // NE
    }
  }
  free_list(sensors);
  free_list(beacons);

  for(size_t j = 0; j < map->dim; j++) {
    for(size_t i = 0; i < map->dim; i++) {
      char c = (map->values[c2ind(map, i, j)] == 1) ? '#' : '.';
      printf("%c", c);
    }
    printf("\n");
  }

  int total = 0;
  for(size_t i = 0; i < map->dim; i++) {
    total += map->values[c2ind(map, (i + map->offset), (20 + map->offset))];
  }

  printf("Total covered positions: %d\n", total);
  return 0;
}
