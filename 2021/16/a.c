#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

typedef struct packet {
  uint8_t version, type, children;
  uint64_t value;
  struct packet** child;
} packet_t;

void convert_BITS(char* input, char* output) {
  // convert string of input BITS characters into string of output bits
  static char* binary_strings[] = { 
    "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
  };

  uint8_t hex;
  for(size_t i = 0; i < strlen(input); i++) {
    if(input[i] == 10) continue; // skip new lines
    hex = (input[i] < 65) ? (input[i] - 48) : (input[i] - 55);
    strcpy(&output[(i*4)], binary_strings[hex]);
  }
}

void consume_bits(char* bits, uint32_t len, char* consumed) {
  // take `len` characters from the head of the string of bits and place them in a separate string
  if(len > strlen(bits)) {
    printf("Error - trying to decode more bits than possible (want %i have %lu)\n", len, strlen(bits));
    exit(1);
  }
  memcpy(consumed, bits, len);
  consumed[len] = '\0';
  memcpy(bits, &bits[len], (strlen(bits) - len + 1));
}

uint64_t bits2dec(char* bits) {
  // convert string of binary characters into decimal
  uint64_t dec = 0;
  uint8_t b;
  for(size_t i = 0; i < strlen(bits); i++) {
    b = bits[i] - 48;
    dec += b * pow(2, (strlen(bits) - 1 - i));
  }
  return dec;
}

packet_t* make_packet() {
  packet_t* packet = calloc(1, sizeof(packet_t));
  packet->child = calloc(100, sizeof(packet_t*));
  return packet;
}

void free_packet(packet_t* packet) {
  for(size_t i = 0; i < packet->children; i++) free_packet(packet->child[i]);
  free(packet->child);
  free(packet);
}

void append_child(packet_t* parent, packet_t* child) {
  parent->child[parent->children++] = child;
}

packet_t* decode_transmission(char* bits, packet_t* parent, int8_t n_subpackets, uint8_t is_bitstream) {
  packet_t* packet = make_packet();
  char* read = calloc(100000, sizeof(char));

  consume_bits(bits, 3, read);
  packet->version = (uint8_t) bits2dec(read);

  consume_bits(bits, 3, read);
  packet->type = (uint8_t) bits2dec(read);

  if(packet->type == 4) {
    char* value = calloc(100, sizeof(char));
    size_t i = 0;
    while(1) {
      consume_bits(bits, 5, read);
      memcpy(&value[(i++ * 4)], &read[1], 4);
      if(read[0] != '1') break;
    }
    packet->value = (uint8_t) bits2dec(value);
    free(value);
  } else {
    consume_bits(bits, 1, read);
    uint8_t ltid = bits2dec(read);
    
    if(ltid) {
      consume_bits(bits, 11, read);
      uint32_t n_subpackets = (uint32_t) bits2dec(read);
      decode_transmission(bits, packet, (n_subpackets - 1), 0);
    } else {
      consume_bits(bits, 15, read);
      uint32_t n_bits = (uint32_t) bits2dec(read);
      consume_bits(bits, n_bits, read);
      decode_transmission(read, packet, -1, 1);
    }
  }
  free(read);

  if(parent != NULL) append_child(parent, packet);
  
  // continue reading adjacent sub-packets if we need to
  if(n_subpackets > 0) decode_transmission(bits, parent, (n_subpackets - 1), 0);
  if(is_bitstream && strlen(bits) > 0) decode_transmission(bits, parent, 0, 1);

  return packet;
}

uint32_t sum_versions(packet_t* packet) {
  uint32_t total = packet->version;
  if(packet->children > 0) {
    for(size_t i = 0; i < packet->children; i++) total += sum_versions(packet->child[i]);
  }
  return total;
}

int main(void) {
  FILE* fptr;
  if((fptr = fopen("input.txt", "r")) == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* input = NULL;
  size_t bufsize = 0;
  if(getline(&input, &bufsize, fptr) == -1) {
    printf("Error reading input line\n");
    exit(1);
  }
  fclose(fptr);

  char* bits = calloc(4 * (strlen(input) - 1), sizeof(char));
  convert_BITS(input, bits);
  free(input);

  packet_t* packet = decode_transmission(bits, NULL, -1, 0);
  uint32_t packet_versions = sum_versions(packet);
  free(bits);
  free_packet(packet);
  printf("Sum of all version numbers in all packets: %i\n", packet_versions); // 940
  return 0;
}
