#ifndef _COMIDAQ_API_H_
#define _COMIDAQ_API_H_

// Device ID definition // 
#define COMI_CP101	0xC101
#define COMI_CP201 	0xC201
#define COMI_CP301 	0xC301
#define COMI_CP302	0xC302
#define COMI_CP401	0xC401
#define COMI_CP501	0xC501
#define COMI_SD101	0xB101
#define COMI_SD102	0xB102
#define COMI_SD103	0xB103
#define COMI_SD104	0xB104
#define COMI_SD201	0xB201
#define COMI_SD202	0xB202
#define COMI_SD203	0xB203
#define COMI_SD301	0xB301
#define COMI_SD401	0xB401
#define COMI_SD402	0xB402
#define COMI_SD403	0xB403
#define COMI_SD404	0xB404
#define COMI_SD414	0xB414
#define COMI_SD424	0xB424
#define COMI_SD501	0xB501
#define COMI_SD502	0xB502
#define COMI_LX101	0xA101
#define COMI_LX102	0xA102
#define COMI_LX103	0xA103
#define COMI_LX201	0xA201
#define COMI_LX202	0xA202
#define COMI_LX203	0xA203
#define COMI_LX301	0xA301
#define COMI_LX401	0xA401
#define COMI_LX402	0xA402
#define COMI_ST101	0xD101
#define COMI_ST201	0xD201
#define COMI_ST202	0xD202
#define COMI_ST203	0xD203
#define COMI_ST301	0xD301
#define COMI_ST401	0xD401
#define COMI_ST402	0xD402
#define COMI_MU101	0xE101
#define COMI_MU201	0xE201
#define COMI_MU301	0xE301
#define COMI_MU401	0xE401
#define COMI_MU402	0xE402
#define COMI_MU403	0xE403
#define COMI_MU501	0xE501
#define COMI_MU701	0xE701
#define MB_DAC101	0x0101
#define MB_DAC201	0x0201
#define MB_DAC301	0x0301
#define MB_DAC401	0x0401
#define MB_DAC501	0x0501
#define MB_DAC601	0x0601

typedef enum {
	cmAI_DIFF=0,
	cmAI_SINGLE=1
}TCdAiInputType; // A/I Scan Transfer Method
	
typedef enum {
	cmTRS_SINGLE=1,
	cmTRS_BLOCK=2
}TCdAiScanTrs; // A/I Scan Transfer Method

typedef enum {
	cmTS_NONE=0,
	cmTS_ANALOG=1,
	cmTS_DIGITAL=2
}TCdAiEventSrc; // A/I Scan Trigger Event Source

typedef enum {
	cmTE_POSITIVE=0,
	cmTE_NEGATIVE=1
}TCdEdgeType; // A/I Scan Trigger Event Source

typedef enum {
	cmDI_ONLY=0,
	cmDI_DO=1,
	cmDO_DI=2,
	cmDO_ONLY=3
}TCdDioUsage;

typedef enum _TCdBool{ cmdFALSE, cmdTRUE }TCdBool;

typedef enum {
	cmCMODE0=0, 
	cmCMODE1=1,
	cmCMODE2=2, 
	cmCMODE3=3, 
	cmCMODE4=4, 
	cmCMODE5=5
}TCnt16Mode;

typedef enum {
	cmCOUNTER_LATCH=0,
	cmREAD_LOAD_MSB=1,
	cmREAD_LOAD_LSB=2,
	cmREAD_LOAD_WORD=3
}TCdRwMode;

typedef enum {
	cmBINARY=0, /* BCD counter                */
	cmBCD=1 /* Binary counter             */
}TBcdBin;

typedef enum {
	cmENCODER_1X=0, 
	cmENCODER_2X=1,
	cmENCODER_4X=2, 
}TEncMode;

#endif
