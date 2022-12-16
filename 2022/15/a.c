#include "beacons.h"

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  list_t* sensors = list();
  list_t* beacons = list();

  grid_t* map = grid(8000000, 2000000);

  int sx, sy, bx, by;
  while(fscanf(fptr, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sx, &sy, &bx, &by) == 4) {
    append(sensors, point_at(sx + map->offset, sy + map->offset));
    append(beacons, point_at(bx + map->offset, by + map->offset));
  }
  fclose(fptr);

  // printf("%d sensor readings\n", sensors->len);

  int d, f;
  size_t m, n;
  for(size_t i = 0; i < sensors->len; i++) {
    d = distance(sensors->point[i]->x, sensors->point[i]->y, beacons->point[i]->x, beacons->point[i]->y);

    // printf("Sensor %zu (%d, %d) => (%d, %d) [%d]\n", (i + 1), sensors->point[i]->x, sensors->point[i]->y, beacons->point[i]->x, beacons->point[i]->y, d);

    sx = (sensors->point[i]->x - d);
    sy = (sensors->point[i]->y - d);
    bx = (sensors->point[i]->x + d);
    by = (sensors->point[i]->y + d);
    
    for(n = sy; n <= by; n++) {
      for(m = sx; m <= bx; m++) {
        // printf("n=%zu, m=%zu\n", n, m);
        f = distance(sensors->point[i]->x, sensors->point[i]->y, m, n); 
        if(f <= d) {
          mark_point(map, m, n);
        }
      }
    }
  }
  free_list(sensors);
  free_list(beacons);

  // for(size_t j = 0; j < map->dim; j++) {
  //   for(size_t i = 0; i < map->dim; i++) {
  //     char c = (map->values[c2ind(map, i, j)] == 1) ? '#' : '.';
  //     printf("%c", c);
  //   }
  //   printf("\n");
  // }

  int total = 0;
  for(size_t i = 0; i < map->dim; i++) {
    total += map->values[c2ind(map, (i + map->offset), (2000000 + map->offset))];
  }

  printf("Total covered positions: %d\n", total);
  return 0;
}
