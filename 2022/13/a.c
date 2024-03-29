#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

enum packet_type {
  UNKNOWN_PACKET,
  LIST_PACKET,
  INTEGER_PACKET
};

typedef struct packet {
  enum packet_type type;
  uint8_t value;
  uint8_t children;
  struct packet** child;
} packet_t;

packet_t* make_packet() {
  packet_t* packet = calloc(1, sizeof(packet_t));
  packet->type = UNKNOWN_PACKET;
  packet->value = 0;
  packet->children = 0;
  packet->child = calloc(100, sizeof(packet_t*));
  return packet;
}

void add_child_packet(packet_t* parent, packet_t* add) {
  parent->child[parent->children++] = add;
}

packet_t* parse_packet(char* input) {
  packet_t* packets[100];
  
  int n = 0;
  size_t s = strlen(input);
  char extract[5];
  for(size_t i = 0; i < s; i++) {
    switch(input[i]) {
      case '[':
        packets[n] = make_packet();
        packets[n]->type = LIST_PACKET;
        if(n > 0) add_child_packet(packets[n-1], packets[n]);
        n++;
        break;
        
      case ']':
        n--;
        break;
        
      case ',':
        break;
        
      default:
        if(isdigit(input[i])) {
          size_t j = i;
          while(isdigit(input[j])) j++;
          for(size_t k = 0; k < (j - i); k++) {
            extract[k] = input[(i + k)];
          }
          extract[(j - i)] = '\0';
          int d = atoi(extract);
          i = (j - 1);
          
          packet_t* p = make_packet();
          p->type = INTEGER_PACKET;
          p->value = d;
          add_child_packet(packets[n-1], p);
        }
        break;
    }
  }
  
  return packets[0];
}

void wrap_in_list(packet_t* packet) {
  // to keep original pointer the same we make a new integer child packet,
  // and repurpose the original integer packet as it's parent
  packet_t* child = make_packet();
  child->type = INTEGER_PACKET;
  child->value = packet->value;
  child->children = 0;
  packet->type = LIST_PACKET;
  add_child_packet(packet, child);
}

int compare_packets(packet_t* left, packet_t* right) {
  // 1 = in order, 0 = not in order, -1 = we don't know, continue comparison
  if(left->type == INTEGER_PACKET && right->type == INTEGER_PACKET) {
    if(left->value == right->value) return -1;
    else if(left->value < right->value) {
      return 1;
    } else if(left->value > right->value) {
      return 0;
    }
  } else if(left->type == LIST_PACKET && right->type == LIST_PACKET) {
    size_t l = (left->children > right->children) ? left->children : right->children;
    for(size_t i = 0; i < l; i++) {
      if(i >= left->children) {
        return 1; // left has run out of items
      }
      else if(i >= right->children) {
        return 0; // right has run out of items
      }      
      else {
        int result = compare_packets(left->child[i], right->child[i]);
        if(result < 0) continue;
        else return result;
      }
    }
    return -1;
  } else if(left->type == INTEGER_PACKET && right->type != INTEGER_PACKET) {
    wrap_in_list(left);
    return compare_packets(left, right);
  } else if(left->type != INTEGER_PACKET && right->type == INTEGER_PACKET) {
    wrap_in_list(right);
    return compare_packets(left, right);
  }
  
  printf("Something went wrong with comparison\n");
  exit(1); // something went wrong
}

void free_packets(packet_t* packet) {
  for(size_t i = 0; i < packet->children; i++) {
    free_packets(packet->child[i]);
  }
  free(packet->child);
  free(packet);
}

int in_order(char* l, char* r) {
  packet_t* left = parse_packet(l);
  packet_t* right = parse_packet(r);
  int result = compare_packets(left, right);
  free_packets(left);
  free_packets(right);
  return result;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char *left = NULL, *right = NULL;
  size_t bufl = 0, bufr = 0;
  int cl, cr, pair = 1, total = 0;
  while(1) {
    cl = getline(&left, &bufl, fptr);
    cr = getline(&right, &bufr, fptr);
    if((cl > 0) && (cr > 0)) {
      if(in_order(left, right) == 1) total += pair;
      getline(&left, &bufl, fptr); // clear the newline after a pair
      pair++;
    } else {
      break;
    }
  }
  fclose(fptr);
  free(left);
  free(right);

  printf("Sum of indices already in order: %d\n", total); // 5806
  return 0;
}
