#ifndef BYTE_H_
#define BYTE_H_

union byte {
	unsigned char ubyte;
	signed char sbyte;
	struct {
		unsigned char b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1;
	} bits;
};

#endif