
#include "stdint.h"
#include "Mcal_BigLittle_Endian.h"

uint16_t BigLittleEnd_Swap_2_Bytes(uint16_t X)
{
	return (  ((X << 8) & 0xFF00)\
		    | ((X >> 8) & 0x00FF) );
}


uint32_t BigLittleEnd_Swap_4_Bytes(uint32_t X)
{
	return (  ((X << 24) & 0xFF000000)\
		    | ((X << 8)  & 0x00FF0000)\
		    | ((X >> 8)  & 0x0000FF00)\
		    | ((X >> 24) & 0x000000FF) );
}


uint32_t BigLittleEnd_Swap_8_Bytes(uint64_t X)
{
	return (  ((X << 56) & 0xFF00000000000000)\
			| ((X << 40) & 0x00FF000000000000)\
            | ((X << 24) & 0x0000FF0000000000)\
            | ((X << 8)  & 0x000000FF00000000)\
            | ((X >> 8)  & 0x00000000FF000000)\
            | ((X >> 24) & 0x0000000000FF0000)\
			| ((X >> 40) & 0x000000000000FF00)\
            | ((X >> 56) & 0x00000000000000FF) );
}


void BigLittleEnd_Swap_Char_Array(uint8_t* Char_Array, uint32_t Len)
{
	uint32_t Swap_Pair_Num;
	uint8_t  Char_Buf;
	
	Swap_Pair_Num = Len >> 1;

	for(uint32_t i = 0; i < Swap_Pair_Num; i++)
	{
		Char_Buf 			= Char_Array[i];
		Char_Array[i] 		= Char_Array[Len - i];
		Char_Array[Len - i] = Char_Buf;
	}
}

