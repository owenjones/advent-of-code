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
  char input[100];
} packet_t;

packet_t* make_packet() {
  packet_t* packet = calloc(1, sizeof(packet_t));
  packet->type = UNKNOWN_PACKET;
  packet->value = 0;
  packet->children = 0;
  packet->child = calloc(500, sizeof(packet_t*));
  return packet;
}

void add_child_packet(packet_t* parent, packet_t* add) {
  parent->child[parent->children++] = add;
}

packet_t* parse_packet(char* input) {
  packet_t* packets[500];
  
  int n = 0;
  size_t s = strlen(input);
  char extract[5];
  for(size_t i = 0; i < s; i++) {
    switch(input[i]) {
      case '[':
        packets[n] = make_packet();
        packets[n]->type = LIST_PACKET;
        if(n == 0) strcpy(packets[n]->input, input);
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
          i = j - 1;
          
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

int in_order(packet_t* left, packet_t* right) {
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
        int result = in_order(left->child[i], right->child[i]);
        if(result < 0) continue;
        else return result;
      }
    }
    return -1;
  } else if(left->type == INTEGER_PACKET && right->type != INTEGER_PACKET) {
    wrap_in_list(left);
    return in_order(left, right);
  } else if(left->type != INTEGER_PACKET && right->type == INTEGER_PACKET) {
    wrap_in_list(right);
    return in_order(left, right);
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

void sort_packets(packet_t** packet, size_t packets) {
  for(size_t i = 0; i < (packets - 1); i++) {
    for(size_t j = 0; j < (packets - 1 - i); j++) {
      int order = in_order(packet[j], packet[j+1]);
      if(order == 0) {
        packet_t* temp = packet[j];
        packet[j] = packet[j+1];
        packet[j+1] = temp;
      }
    }
  }
}

int find_index(packet_t** packet, size_t packets, char* input) {
  for(size_t i = 1; i < (packets + 1); i++) {
    if(strcmp(packet[(i - 1)]->input, input) == 0) return i;
  }
  return 0;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  packet_t** packet = calloc(1000, sizeof(packet_t*));
  packet[0] = parse_packet("[[2]]");
  packet[1] = parse_packet("[[6]]");
  size_t packets = 2;

  char *input = NULL;
  size_t bufsize = 0;
  while(getline(&input, &bufsize, fptr) != -1) {
    if(strcmp(input, "\n") == 0) continue;
    packet[packets++] = parse_packet(input);
  }
  fclose(fptr);
  free(input);

  sort_packets(packet, packets);
  int a = find_index(packet, packets, "[[2]]");
  int b = find_index(packet, packets, "[[6]]");
  int total = a * b;
  
  for(size_t i = 0; i < packets; i++) {
    free_packets(packet[i]);
  }
  free(packet);
  
  printf("Decoder key: %d\n", total); // 23600
  return 0;
}
