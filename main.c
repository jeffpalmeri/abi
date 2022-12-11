/* 
 * Goal: Make my own binary spec for a single byte,
 * and write some test cases for getting values from it.
 * And learn about bitwise operations and things like
 * masking in the process.
 *
 * Spec:
 * Ok so one byte has a max decimal value of 255.
 * So some example bytes could look like this
 * - 11111111 == 255
 * - 10010010 == 146
 * - 00011000 == 24
 *
 *   Most significant bit will equal gender: 0 = female, 1 = male.
 *   Next two bits will represent the version... 00 = version 1, 01 = version 2, 10 = version 3, 11 = version 4
 *   Next bit will represent if active or not... so 0 = inactive, 1 = active
 *   Next four bits will represent some sort of table of values, where the bits have a different meaning
 *   based on the "version" from earlier in the byte... So I'll make up a table now:
 *        
 *        
 *            1     2     3     4
 *   0000     0     0     0    100
 *   0001     0     0    100   200
 *   0010     0    100   150   250
 *   0011     0    125   175   340    
 *   0100     5    125   180   365
 *   0101    10    150   200   365
 *   0110    10    150   375   375
 *   0111    10    150   400   400
 *   1000    15    150   450   1000
 *   1001    15    150   450   1150
 *   1010    15    150   500   1250
 *   1011    20    155   550   5000
 *   1100    100   200   1560  9800
 *   1101    150   250   2000  12100
 *   1110    230   330   6000  23500
 *   1111    300   500   18000 235550
 *
 *   Alright so some example robots would look like this:
 *    11111111 == 255 == male, version 4, active, 235550 gigahertz
 *    10010010 == 146 == male, version 1, active, 0 gighertz
 *    00011000 == 24  == female, version 1, active, 15 gigahertz
 **/

/*
 * AND masking use cases:
 * 1.) AND bitfags with a mask that has a single bit set to 1. If the result is 0, then the bit at that position is OFF. Otherwise, it's on.
 *     So this techniuqe can tell you the value of a single bit.
 *
 * 2.) AND biflags with a mask. If the result is EQUAL to the bit mask, then we know exactly which bits in the flags are 1 (because we know which bits were 1 in the mask we used).
 *     This also seems to mean that anything masked with itself does not change.
 *
 * 3.) Masking with all 1s will not change the original value... So we can use this fact to turn OFF bits where ever we want with a mask that has 0 set for whichever bits we want to turn off.
 *     So If we wanted to turn the most significant bit off in a byte, we could AND mask wth 01111111, and set the bitfield to the result.
 **/

/*
 * OR masking use cases:
 * 1.) Turning ON single bits. This is basically the opposite of the 3rd bullet point above about AND masks. By ORing some bitflags with a mask where everything is 0 except for
 *     the bits we want to turn on, the result will be the same as the original bitflags with all of the specified bits in the mask turned on.
 *
 * 2.) Just another use case of the first point, but ORing something with a bitmask of all 1s will turn on all bits in the original. So this teqnique can be used for turning on or potentially
 *     resetting some bitflags.
 **/

/*
 * XOR masking use cases:
 * 1.) You can invert a set of bits by masking with a mask of all 1s. One use of masking like this is in cryptography, the significance of which being that if you treat an XOR mask as a "key",
 *     then you can encrypt some value with it by XORing with the mask, and then you can decrypt by XORing again with the exact same mask to get the exact same original value.
 * */

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

struct RobotHeader
{
  int gender;
  int version;
  int active;
  int gigahertz;
};

struct RobotHeader parseByte(unsigned char byte) {
  struct RobotHeader hdr = {
    byte >> 7,
    // 01100000 >> 6 == 000000X11
    byte >> 5 & 0b00000011,
    byte >> 4 & 0b00000001,
    byte & 0b00001111,
  };

  return hdr;
}

int getRobotGender(unsigned char byte) {
  // 1.) Since this is only one byte, no need to shift first. So just mask it so that only the first bit is left, and see if result is 0 or not.
  
  char mask = 0x80;
  unsigned char res = byte & mask;
  printf("The byte we're decoding is %d\n or 0x%x\n", byte, byte);
  printf("The value after masking for the gender byte: %d, which in hex is 0x%x\n", res, res);

  if (res == 0) {
    return 0;
  } else {
    return 1;
  }
}

int main() {
  // char myByteArray[] = {1, 2, 3};
  // char *myByteArray2[] = {"a" "b", "c"};
  // char myByteArray3[] = {0x00, 0x01, 0x11};
  // int *myByteArray4[] = {0x00, 0x01, 0x11};
  
  char bytes[] = {0xFF, 0x92, 0x18};
  // char bytes2[] = {0b10101011, 0x92, 0x18};
  const uint8_t firstByte = bytes[0];

  printf("The value of this byte in decimal and hexidecimal is %d and 0x%x\n", firstByte, firstByte);
  int gender = getRobotGender(firstByte);
  printf("The first robots gender is: %d\n", gender);
  // printf("The second robots gender is: %d\n", getRobotGender(bytes[1]));
  // printf("The third robots gender is: %d\n", getRobotGender(bytes[2]));
  
  struct RobotHeader hdr1 = parseByte(firstByte);
  printf("Gender: %d\n", hdr1.gender);
  printf("Version: %d\n", hdr1.version);
  printf("Active: %d\n", hdr1.active);
  printf("Gigahertz: %d\n", hdr1.gigahertz);
  assert(hdr1.gender == 1);
  assert(hdr1.version == 3);
  assert(hdr1.active == 1);
  assert(hdr1.gigahertz == 15);

  // 10010010
  // gender == 1 == 1
  // version == 00 == 0
  // active == 1 == 1
  // gigahertz == 0010 == 2
  struct RobotHeader hdr2 = parseByte(bytes[1]);
  printf("Gender: %d\n", hdr2.gender);
  printf("Version: %d\n", hdr2.version);
  printf("Active: %d\n", hdr2.active);
  printf("Gigahertz: %d\n", hdr2.gigahertz);
  assert(hdr2.gender == 1);
  assert(hdr2.version == 0);
  assert(hdr2.active == 1);
  assert(hdr2.gigahertz == 2);
  
  // 00011000
  // gender == 0 == 0
  // version == 00 == 0
  // active == 1 == 1
  // gigahertz == 1000 == 8
  struct RobotHeader hdr3 = parseByte(bytes[2]);
  printf("Gender: %d\n", hdr3.gender);
  printf("Version: %d\n", hdr3.version);
  printf("Active: %d\n", hdr3.active);
  printf("Gigahertz: %d\n", hdr3.gigahertz);
  assert(hdr3.gender == 0);
  assert(hdr3.version == 0);
  assert(hdr3.active == 1);
  assert(hdr3.gigahertz == 8);

}
