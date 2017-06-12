/*
+-----------------------------------------------------------------------------+
| *******************  Specific Header of modul PC_INI.C  ******************* |
+-----------------------------------------------------------------------------+
|                      Copyright 2002 by KW Software GmbH                     |
|                                                                             |
|                      KW Software GmbH                                       |
|                      Lagesche Str. 32                                       |
|                      D-32657 Lemgo                                          |
|                      Germany                                                |
|                                                                             |
+-----------------------------------------------------------------------------+
|Author: Thorsten Niemann   , started at: November, 28th, 1997                |
+-----------------------------------------------------------------------------+
*/
#ifndef PC_INI_H
#define PC_INI_H

/* type definitions */
typedef struct pciniDevice
{  union fd
   {  char  *pszMem;       /* pointer to memory (string with '\0' termination) */
      char  *pszFileName;  /* pointer to file name */
   } fd;
   int   hFile;            /* internal use: file handle */
   int   nDeviceOffset;    /* internal use: device offset */
   char  cDevice;          /* device specification 'F' or 'M'*/
} pciniDevice;               

typedef struct pciniSectInfo
{  int      nSectOffset;
   size_t   nSectSize;
} pciniSectInfo;

/*************************************************
** [section]
** key=string           ;comment
** key=nextString       ;nextComment
** key="C:\PCOS\BOOT"
*************************************************/

/* sections: */
#define sectMemory            "Memory"
#define sectSchedule          "Schedule"
#define sectCommunication     "Communication"
#define sectMisc              "Misc"
#define sectFirmware          "Firmware"
#define sectIODriver          "IODriver"                              

/* keys: */
#define keyIECApplCode        "IECApplCode"        /* in [Memory] */
#define keyFlags              "Flags"              /* in [Memory] */
#define keySystemFlags        "SystemFlags"        /* in [Memory] */
#define keySharedMemoryFlags  "SharedMemoryFlags"  /* in [Memory] */
#define keyWarmStartCheck     "WarmStartCheck"     /* in [Memory] */ 
#define keyRetentiveFlags     "RetentiveFlags"     /* in [Memory] */ 
#define keyInputs             "Inputs"             /* in [Memory] */
#define keyOutputs            "Outputs"            /* in [Memory] */
#define keySystemTick         "SystemTick"         /* in [Schedule] */
#define keyMinTimeSlice       "MinTimeSlice"       /* in [Schedule] */
#define keyV24Port1           "V24Port1"           /* in [Communication] */
#define keySocketBacklog      "SocketBacklog"      /* in [Communication] */
#define keyFirmwareString     "FirmwareString"     /* in [Firmware] */
#define keyFBmaxNumber        "FBmaxNumber"        /* in [Misc] */
#define keyFileDir            "FileDir"            /* in [Misc] */

/* several defines */
#define pciniMaxLine     128
#define pciniMaxSect     80
#define PCINI_NO_HIT     0
#define PCINI_HIT        1
#define PCINI_OK         0
#define PCINI_ERR       -1
#define DEVICE_MEM      'M'
#define DEVICE_FILE     'F'

#ifdef __cplusplus
extern "C" {
#endif

/* prototypes: */
int pciniGetEntry(const char *szSect, const char *szKey, char *szBuffer, size_t nBuffSize, pciniDevice *pDevice);
int pciniGetDecEntry(char *szSect, char *szKey, ULONG *ulEntry, pciniDevice *pDevice);
int pciniGetSectInfo(char *szSect, pciniDevice *pDevice, pciniSectInfo *pSectInfo);
int pciniCopySect(char *szBuffer, size_t nBuffSize, pciniDevice *pDevice, pciniSectInfo *pSectInfo);

#ifdef __cplusplus
}
#endif

#endif
