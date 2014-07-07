#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <splitter.h>
/**
 * Accepted params:
 * * uid
 * * [percentage]
 * * [seed]
 */
static int MAX_HASH_VALUE=4294967295;

uint32_t
ngx_murmur_hash2(u_char *data, int len)
{   
    uint32_t  h, k;
	
	//init seed
    h = 0 ^ len;

	// Mix 4 bytes at a time into the hash

    while (len >= 4) {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        data += 4;
        len -= 4;
    }

	// Handle the last few bytes of the input array
    switch (len) {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= 0x5bd1e995;
    }

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    return h;
}


char* ngx_parts(int abPercentage, uint32_t hashValue)
{

	char* result="A";
    double parts = ceil(100.0 / abPercentage);

    int i;
    uint32_t curPart = 0;
	uint32_t div = (uint32_t) parts;
	uint32_t partHashValue;

	partHashValue = abPercentage * 100.0 * (uint64_t) 0xffffffff / 10000;
	printf ("partHashValue: %u for %u%%\n", partHashValue, abPercentage);
	
	if ( partHashValue < hashValue )
	{
		printf ("\t%u < %u !\n", partHashValue,hashValue);
		return "A";
	}
	return "B";

}

int main (int argc, char **argv)
{
	char *arg = NULL;
	char* argPrefix = NULL;
	int arg_len, len;
	const int maxVal = 4294967295;
    unsigned char* data;
	uint32_t rHash;
	int abPercentage = 0;

	char* abResult = "";

	if (argc < 2)
	{
		printf ("A parameter is required!\n");
		return 0;
	}
	const char* x = argv[1];
	
	if (argc > 2)
	{
		abPercentage = atoi(argv[2]);
	}
	data = (unsigned char*) x;

	if (argv[3] != NULL)
	{
		argPrefix = argv[3];

	    const size_t prefLength  = strlen(argPrefix);
	    const size_t argLength   = strlen(x);
	    const size_t totalLength = argLength + prefLength;

	    char *const strBuf = (char*)malloc(totalLength + 1);
	    if (strBuf == NULL) {
	        fprintf(stderr, "malloc failed\n");
	        exit(EXIT_FAILURE);
	    }

	    strcpy(strBuf, argPrefix);
	    strcpy(strBuf + prefLength, x);

		x = strBuf;
	}
	data = (unsigned char*) x;

	arg_len = strlen(x);
	//printf ("Sending to murmur %s, %i\n", data, arg_len);
	rHash = ngx_murmur_hash2 ( data, arg_len);
	
	char* sFormat = "%u\n";
	double percentage;

	if (abPercentage > 0)
	{
		if (abPercentage > 100) {abPercentage = 100;}

		abResult = ngx_parts(abPercentage, rHash);
		sFormat = "%u %s from %s\n";
	}
	printf (sFormat, rHash, abResult, x );
	return 1;
}
