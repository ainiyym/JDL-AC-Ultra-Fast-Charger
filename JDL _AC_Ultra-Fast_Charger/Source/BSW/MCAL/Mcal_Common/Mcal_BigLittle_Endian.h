#ifndef __MCAL_BIG_LITTLE_ENDIAN_H__
#define __MCAL_BIG_LITTLE_ENDIAN_H__


#define BIGLITTLEEND_SWAP_2_BYTES(X) (X = (  ((X << 8) & 0xFF00)\
									  	   | ((X >> 8) & 0x00FF) ) )
									  
#define BIGLITTLEEND_SWAP_4_BYTES(X) (X = (  ((X << 24) & 0xFF000000)\
								     	   | ((X << 8 ) & 0x00FF0000)\
								     	   | ((X >> 8 ) & 0x0000FF00)\
								      	   | ((X >> 24) & 0x000000FF) ) )
								      
#define BIGLITTLEEND_SWAP_8_BYTES(X) (X = (  ((X << 56) & 0xFF00000000000000)\
									       | ((X << 40) & 0x00FF000000000000)\
                                           | ((X << 24) & 0x0000FF0000000000)\
                                           | ((X << 8 ) & 0x000000FF00000000)\
                                           | ((X >> 8 ) & 0x00000000FF000000)\
                                           | ((X >> 24) & 0x0000000000FF0000)\
									       | ((X >> 40) & 0x000000000000FF00)\
                                           | ((X >> 56) & 0x00000000000000FF) ) )


extern uint16_t BigLittleEnd_Swap_2_Bytes(uint16_t X);
extern uint32_t BigLittleEnd_Swap_4_Bytes(uint32_t X);
extern uint32_t BigLittleEnd_Swap_8_Bytes(uint64_t X);
extern void BigLittleEnd_Swap_Char_Array(uint8_t* Char_Array, uint32_t Len);


#endif

