#pragma once

typedef uint64_t u64;
typedef	uint32_t u32;
typedef	uint16_t u16;
typedef	signed short s16;
typedef uint8_t u8;
typedef signed char s8;

#pragma pack(push, 1) 

typedef struct 
{
	u8 	head[3];
	u32	totalLen;	// 본 패킷의 총 전송 길이
	u16	cmdType;	// 명령의 종류
    u8 raw[0];
    //u8 data[0];
}PACKET; 

#pragma pack(pop)
