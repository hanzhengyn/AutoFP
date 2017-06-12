Attribute VB_Name = "Module1"
Global Const X_AXIS = 0
Global Const Y_AXIS = 1
Global Const Z_AXIS = 2
Global Const U_AXIS = 3
'/*+-----------------------------------------------------------------------------+
'| ******************** Settings' Limit  *************************** |
'+-----------------------------------------------------------------------------+

'#ifdef PCI1244
Global Const MAX_RATE = 150
Global Const MIN_RATE = 0
Global Const MAX_R = 4096
Global Const MIN_R = 0
Global Const MAX_V = 32767
Global Const MIN_V = 1
Global Const MAX_D = 65535
Global Const MIN_D = 1
Global Const MAX_A = 65535
Global Const MIN_A = 1
Global Const MAX_K = 65535
Global Const MIN_K = 1
Global Const MAX_PULSE = 268435455
Global Const MIN_PULSE = 0
Global Const MAX_POSITION = 268435455
'#Else
'Global Const MAX_RATE = 500
'Global Const MIN_RATE = 1
'Global Const MAX_R = 8000000
'Global Const MIN_R = 16000
'Global Const MAX_V = 8000
'Global Const MIN_V = 1
'Global Const MAX_D = 8000
'Global Const MIN_D = 1
'Global Const MAX_A = 8000
'Global Const MIN_A = 1
'Global Const MAX_K = 65535
'Global Const MIN_K = 1
'Global Const MAX_PULSE = 268435455
'Global Const MIN_PULSE = 0
'Global Const MAX_POSITION = 2147483647
'#End If

Global Const Adv_UNKNOWN = &HFF

' ADAM Motion Series
Global Const Adv_ADAM_MOTION = &H0
Global Const Adv_ADAM5202 = (Adv_ADAM_MOTION + 0)
Global Const Adv_ADAM5240 = (Adv_ADAM_MOTION + 1)
Global Const Adv_APAX5202 = (Adv_ADAM_MOTION + 2)

' IO Motion Series
Global Const Adv_IO_MOTION = &H20
Global Const Adv_PCI1240 = (Adv_IO_MOTION + 0)
Global Const Adv_PCM3240 = (Adv_IO_MOTION + 1)
Global Const Adv_PCI1202 = (Adv_IO_MOTION + 2)
Global Const Adv_AMAX2050 = (Adv_IO_MOTION + 3)
Global Const Adv_PCM3202 = (Adv_IO_MOTION + 4)
Global Const Adv_PCI1220 = (Adv_IO_MOTION + 5)
Global Const Adv_PCI1244 = (Adv_IO_MOTION + 6)
Global Const Adv_PCI1245 = Adv_IO_MOTION + 7
Global Const Adv_PCI1265 = Adv_IO_MOTION + 8
Global Const Adv_PCI1245E = Adv_IO_MOTION + 9
Global Const Adv_PCI1285 = Adv_IO_MOTION + 10
Global Const Adv_PCI1285E = Adv_IO_MOTION + 11
Global Const Adv_PCI1245V = Adv_IO_MOTION + 12
Global Const Adv_PCI1245L = Adv_IO_MOTION + 13
Global Const Adv_PCI1245S = Adv_IO_MOTION + 14

'IO Slave Series
Global Const Adv_AMAX_SLAVE = &H40
Global Const Adv_AMAX2210 = (Adv_AMAX_SLAVE + 0)
Global Const Adv_AMAX2240 = (Adv_AMAX_SLAVE + 1)
Global Const Adv_AMAX2710 = (Adv_AMAX_SLAVE + 2)
Global Const Adv_AMAX2754 = (Adv_AMAX_SLAVE + 3)
Global Const Adv_AMAX2730 = (Adv_AMAX_SLAVE + 4)
Global Const Adv_AMAX2756 = (Adv_AMAX_SLAVE + 5)
Global Const Adv_AMAX2752 = (Adv_AMAX_SLAVE + 6)

Global Const Adv_EtherCAT = &H60
Global Const Adv_ADAM5000 = (Adv_EtherCAT + 1)
Global Const Adv_PCI1203 = (Adv_EtherCAT + 2)

Global Const FT_Dev_ID = 0
Global Const PAR_Dev_ID = 101
Global Const CFG_Dev_ID = 201

Global Const FT_Ax_ID = 301
Global Const PAR_Ax_ID = 401
Global Const CFG_Ax_ID = 501

Global Const FT_Gp_ID = 601
Global Const PAR_Gp_ID = 701
Global Const CFG_Gp_ID = 801
'///////////////////////////////////////////////////////////////////////////////
'// Device Feature
'///////////////////////////////////////////////////////////////////////////////
Global Const FT_DevIpoTypeMap = (FT_Dev_ID + 0)
Global Const FT_DevAxesCount = (FT_Dev_ID + 1)
Global Const FT_DevFunctionMap = (FT_Dev_ID + 2)
Global Const FT_DevOverflowCntr = (FT_Dev_ID + 3)
'Master device config
Global Const FT_MasCyclicCnt_R0 = (FT_Dev_ID + 4)
Global Const FT_MasCyclicCnt_R1 = (FT_Dev_ID + 5)

'//DAQ function
Global Const FT_DaqDiMaxChan = (FT_Dev_ID + 50)
Global Const FT_DaqDoMaxChan = (FT_Dev_ID + 51)
Global Const FT_DaqAiRangeMap = (FT_Dev_ID + 52)
Global Const FT_DaqAoRangeMap = (FT_Dev_ID + 53)
Global Const FT_DaqAiMaxSingleChan = (FT_Dev_ID + 54)
Global Const FT_DaqAiMaxDiffChan = (FT_Dev_ID + 55)
Global Const FT_DaqAiResolution = (FT_Dev_ID + 56)
Global Const FT_DaqAoMaxChan = (FT_Dev_ID + 57)
Global Const FT_DaqAoResolution = (FT_Dev_ID + 58)
'///////////////////////////////////////////////////////////////////////////////
'// Device Config
'///////////////////////////////////////////////////////////////////////////////
Global Const CFG_DevBoardID = (CFG_Dev_ID + 0)
Global Const CFG_DevRingID = (CFG_Dev_ID + 1)
Global Const CFG_DevBaseAddress = (CFG_Dev_ID + 2)
Global Const CFG_DevInterrupt = (CFG_Dev_ID + 3)
Global Const CFG_DevBusNumber = (CFG_Dev_ID + 4)
Global Const CFG_DevSlotNumber = (CFG_Dev_ID + 5)
Global Const CFG_DevDriverVersion = (CFG_Dev_ID + 6)
Global Const CFG_DevDllVersion = (CFG_Dev_ID + 7)
Global Const CFG_DevComQualityContiErr = (CFG_Dev_ID + 8)
Global Const CFG_DevComQualityErrRate = (CFG_Dev_ID + 9)
Global Const CFG_DevComWdgMode = (CFG_Dev_ID + 10)
Global Const CFG_DevFwMemory = (CFG_Dev_ID + 11)
Global Const CFG_DevEmgReact = (CFG_Dev_ID + 12)
Global Const CFG_DevFwVersion = (CFG_Dev_ID + 13)
Global Const CFG_DevBelongsTo = (CFG_Dev_ID + 14)
Global Const CFG_DevMasterDev = (CFG_Dev_ID + 15)
Global Const CFG_DevSlaveDevs = (CFG_Dev_ID + 16)

'DAQ function
Global Const CFG_DaqAiChanType = (CFG_Dev_ID + 50)
Global Const CFG_DaqAiRanges = (CFG_Dev_ID + 51)
Global Const CFG_DaqMaiRanges = (CFG_Dev_ID + 52)
Global Const CFG_DaqAoRanges_C0 = (CFG_Dev_ID + 53)
Global Const CFG_DaqAoRanges_C1 = (CFG_Dev_ID + 54)
Global Const CFG_DaqAoRanges_C2 = (CFG_Dev_ID + 55)
Global Const CFG_DaqAoRanges_C3 = (CFG_Dev_ID + 56)

'Master device config
Global Const CFG_MasBaudRate_R0 = (CFG_Dev_ID + 31)
Global Const CFG_MasBaudRate_R1 = (CFG_Dev_ID + 32)
Global Const CFG_MasComErrRate_R0 = (CFG_Dev_ID + 33)
Global Const CFG_MasComErrRate_R1 = (CFG_Dev_ID + 34)
Global Const CFG_MasComContiErr_R0 = (CFG_Dev_ID + 35)
Global Const CFG_MasComContiErr_R1 = (CFG_Dev_ID + 36)
Global Const CFG_MasIoComContiErr_R0 = (CFG_Dev_ID + 37)
Global Const CFG_MasIoComContiErr_R1 = (CFG_Dev_ID + 38)
Global Const CFG_MasDataComContiErr_R0 = (CFG_Dev_ID + 39)
Global Const CFG_MasDataComContiErr_R1 = (CFG_Dev_ID + 40)


'///////////////////////////////////////////////////////////////////////////////
'// Axis Feature
'///////////////////////////////////////////////////////////////////////////////
Global Const FT_AxFunctionMap = (FT_Ax_ID + 0)
Global Const FT_AxMaxVel = (FT_Ax_ID + 1)
Global Const FT_AxMaxAcc = (FT_Ax_ID + 2)
Global Const FT_AxMaxDec = (FT_Ax_ID + 3)
Global Const FT_AxMaxJerk = (FT_Ax_ID + 4)
Global Const FT_AxPulseInMap = (FT_Ax_ID + 5)
Global Const FT_AxPulseInModeMap = (FT_Ax_ID + 6)
Global Const FT_AxPulseOutMap = (FT_Ax_ID + 7)
Global Const FT_AxPulseOutModeMap = (FT_Ax_ID + 8)
Global Const FT_AxAlmMap = (FT_Ax_ID + 9)
Global Const FT_AxInpMap = (FT_Ax_ID + 10)
Global Const FT_AxErcMap = (FT_Ax_ID + 11)
Global Const FT_AxErcEnableModeMap = (FT_Ax_ID + 12)
Global Const FT_AxErcOnTimeMap = (FT_Ax_ID + 13)
Global Const FT_AxErcOffTimeMap = (FT_Ax_ID + 14)
Global Const FT_AxSdMap = (FT_Ax_ID + 15)
Global Const FT_AxElMap = (FT_Ax_ID + 16)
Global Const FT_AxSwMelMap = (FT_Ax_ID + 17)
Global Const FT_AxSwPelMap = (FT_Ax_ID + 18)
Global Const FT_AxHomeMap = (FT_Ax_ID + 19)
Global Const FT_AxBackLashMap = (FT_Ax_ID + 20)
Global Const FT_AxVibrationMap = (FT_Ax_ID + 21)
Global Const FT_AxAlarmMap = (FT_Ax_ID + 22)
'///////////////////////////////////////////////////////////////////////////////
'// Axis Parameter
'///////////////////////////////////////////////////////////////////////////////
Global Const PAR_AxVelLow = (PAR_Ax_ID + 0)
Global Const PAR_AxVelHigh = (PAR_Ax_ID + 1)
Global Const PAR_AxAcc = (PAR_Ax_ID + 2)
Global Const PAR_AxDec = (PAR_Ax_ID + 3)
Global Const PAR_AxJerk = (PAR_Ax_ID + 4)
Global Const PAR_AxHomeExMode = (PAR_Ax_ID + 5)
Global Const PAR_AxHomeExSwitchMode = (PAR_Ax_ID + 6)
Global Const PAR_AxHomeCrossDistance = (PAR_Ax_ID + 7)
Global Const PAR_AxJerkL = (PAR_Ax_ID + 8)
  
'///////////////////////////////////////////////////////////////////////////////
'// Axis Config
'///////////////////////////////////////////////////////////////////////////////
Global Const CFG_AxPPU = (CFG_Ax_ID + 50)
Global Const CFG_AxPhyID = (CFG_Ax_ID + 51)
Global Const CFG_AxMaxVel = (CFG_Ax_ID + 52)
Global Const CFG_AxMaxAcc = (CFG_Ax_ID + 53)
Global Const CFG_AxMaxDec = (CFG_Ax_ID + 54)
Global Const CFG_AxMaxJerk = (CFG_Ax_ID + 55)
Global Const CFG_AxPulseInMode = (CFG_Ax_ID + 56)
Global Const CFG_AxPulseInLogic = (CFG_Ax_ID + 57)
Global Const CFG_AxPulseInSource = (CFG_Ax_ID + 58)
Global Const CFG_AxPulseOutMode = (CFG_Ax_ID + 59)
Global Const CFG_AxAlmEnable = (CFG_Ax_ID + 60)
Global Const CFG_AxAlmLogic = (CFG_Ax_ID + 61)
Global Const CFG_AxAlmReact = (CFG_Ax_ID + 62)
Global Const CFG_AxInpEnable = (CFG_Ax_ID + 63)
Global Const CFG_AxInpLogic = (CFG_Ax_ID + 64)
Global Const CFG_AxErcLogic = (CFG_Ax_ID + 65)
Global Const CFG_AxErcOnTime = (CFG_Ax_ID + 66)
Global Const CFG_AxErcOffTime = (CFG_Ax_ID + 67)
Global Const CFG_AxErcEnableMode = (CFG_Ax_ID + 68)
Global Const CFG_AxSdEnable = (CFG_Ax_ID + 69)
Global Const CFG_AxSdLogic = (CFG_Ax_ID + 70)
Global Const CFG_AxSdReact = (CFG_Ax_ID + 71)
Global Const CFG_AxSdLatch = (CFG_Ax_ID + 72)
Global Const CFG_AxElEnable = (CFG_Ax_ID + 73)
Global Const CFG_AxElLogic = (CFG_Ax_ID + 74)
Global Const CFG_AxElReact = (CFG_Ax_ID + 75)
Global Const CFG_AxSwMelEnable = (CFG_Ax_ID + 76)
Global Const CFG_AxSwPelEnable = (CFG_Ax_ID + 77)
Global Const CFG_AxSwMelReact = (CFG_Ax_ID + 78)
Global Const CFG_AxSwPelReact = (CFG_Ax_ID + 79)
Global Const CFG_AxSwMelValue = (CFG_Ax_ID + 80)
Global Const CFG_AxSwPelValue = (CFG_Ax_ID + 81)
Global Const CFG_AxHomeMode = (CFG_Ax_ID + 82)
Global Const CFG_AxHomeDir = (PAR_Ax_ID + 83)
Global Const CFG_AxHomeSwitchMode = (PAR_Ax_ID + 84)
Global Const CFG_AxHomePosition = (PAR_Ax_ID + 85)
Global Const CFG_AxHomeCrossDistance = (PAR_Ax_ID + 86)
Global Const CFG_AxOrgEnable = (CFG_Ax_ID + 87)
Global Const CFG_AxOrgLogic = (CFG_Ax_ID + 88)
Global Const CFG_AxEzEnable = (CFG_Ax_ID + 89)
Global Const CFG_AxEzLogic = (CFG_Ax_ID + 90)
Global Const CFG_AxEzCount = (CFG_Ax_ID + 91)
Global Const CFG_AxBacklashEnable = (CFG_Ax_ID + 92)
Global Const CFG_AxBacklashPulses = (CFG_Ax_ID + 93)
Global Const CFG_AxVibrationEnable = (CFG_Ax_ID + 94)
Global Const CFG_AxVibrationReverseTime = (CFG_Ax_ID + 95)
Global Const CFG_AxVibrationForwardTime = (CFG_Ax_ID + 96)
Global Const CFG_AxPositionLagEn = (CFG_Ax_ID + 97)
Global Const CFG_AxMaxPositionLag = (CFG_Ax_ID + 98)
Global Const CFG_AxEmgReaction = (CFG_Ax_ID + 99)
Global Const CFG_AxLatchLogic = (CFG_Ax_ID + 100)
Global Const CFG_AxHomeResetEnable = (CFG_Ax_ID + 101)
Global Const CFG_AxCmpSrc = (CFG_Ax_ID + 102)
Global Const CFG_AxCmpMethod = (CFG_Ax_ID + 103)
Global Const CFG_AxCmpPulseMode = (CFG_Ax_ID + 104)
Global Const CFG_AxCmpPulseLogic = (CFG_Ax_ID + 105)
Global Const CFG_AxCmpPulseWidth = (CFG_Ax_ID + 106)
Global Const CFG_AxCmpEnable = (CFG_Ax_ID + 107)
Global Const CFG_AxRange = (CFG_Ax_ID + 108)
Global Const CFG_AxGenDoEnable = (CFG_Ax_ID + 109)
Global Const CFG_AxExtMasterSrc = (CFG_Ax_ID + 110)
Global Const CFG_AxExtSelEnable = (CFG_Ax_ID + 111)
Global Const CFG_AxExtPulseNum = (CFG_Ax_ID + 112)
Global Const CFG_AxOrgLatch = (CFG_Ax_ID + 113)
Global Const CFG_AxOutLogic = (CFG_Ax_ID + 114)
Global Const CFG_AxDirLogic = (CFG_Ax_ID + 115)
Global Const CFG_AxExtPulseInMode = (CFG_Ax_ID + 116)
Global Const CFG_AxExtPresetNum = (CFG_Ax_ID + 117)
Global Const CFG_AxErcOffTmEnable = (CFG_Ax_ID + 118)
Global Const CFG_AxAuxOutEnable = (CFG_Ax_ID + 119)
Global Const CFG_AxAuxOutTime = (CFG_Ax_ID + 120)
Global Const CFG_AxLatchBufEnable = (CFG_Ax_ID + 146)

'Add by yanli.zhu 15.01.27
Global Const CFG_CH_ID = 1500
Global Const CFG_CH_DaqDiInvertEnable = CFG_CH_ID + &H0
Global Const CFG_CH_DaqDiLowFilter = CFG_CH_ID + &H1
Global Const CFG_CH_DaqDiHighFilter = CFG_CH_ID + &H2
Global Const CFG_CH_DaqDoFsvEnable = CFG_CH_ID + &H3
Global Const CFG_CH_DaqAoRange = CFG_CH_ID + &H4
Global Const CFG_CH_DaqAiRange = CFG_CH_ID + &H5
Global Const CFG_CH_DaqAiEnable = CFG_CH_ID + &H6
Global Const CFG_CH_DaqAiIntegrationTime = CFG_CH_ID + &H7
   
'///////////////////////////////////////////////////////////////////////////////
'// Group Parameter
'///////////////////////////////////////////////////////////////////////////////
Global Const PAR_GpVelLow = (PAR_Gp_ID + 0)
Global Const PAR_GpVelHigh = (PAR_Gp_ID + 1)
Global Const PAR_GpAcc = (PAR_Gp_ID + 2)
Global Const PAR_GpDec = (PAR_Gp_ID + 3)
Global Const PAR_GpJerk = (PAR_Gp_ID + 4)
Global Const PAR_GpGroupID = (PAR_Gp_ID + 5)
Global Const PAR_GpJerkL = (PAR_Gp_ID + 6)
Global Const PAR_GpIsPathDec = (PAR_Gp_ID + 7)
Global Const PAR_GpRefPlane = (PAR_Gp_ID + 8)
'///////////////////////////////////////////////////////////////////////////////
'// Group Config
'///////////////////////////////////////////////////////////////////////////////
Global Const CFG_GpMaxVel = (CFG_Gp_ID + 0)
Global Const CFG_GpMaxAcc = (CFG_Gp_ID + 1)
Global Const CFG_GpMaxDec = (CFG_Gp_ID + 2)
Global Const CFG_GpMaxJerk = (CFG_Gp_ID + 3)
Global Const CFG_GpPPU = (CFG_Gp_ID + 4)
Global Const CFG_GpAxesInGroup = (CFG_Gp_ID + 5)
Global Const CFG_GpGroupID = (CFG_Gp_ID + 6)

'/*
'+-----------------------------------------------------------------------------+
'| ******************** Device Function Map Define *************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const DEV_FUNC_MOT = 1
Global Const DEV_FUNC_DI = 2
Global Const DEV_FUNC_DO = 4
Global Const DEV_FUNC_AI = 8
Global Const DEV_FUNC_AO = 16
Global Const DEV_FUNC_TMR = 32
Global Const DEV_FUNC_CNT = 64
'/*
'+-----------------------------------------------------------------------------+
'| **************** Device Interpolation Type Map Define ********************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const DEV_IPO_LINE_2AX = 1
Global Const DEV_IPO_LINE_3AX = 2
Global Const DEV_IPO_ARC_2AX = 256
Global Const DEV_IPO_ARC_3AX = 512
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Axis Feature Map Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_FUNC_INP = 1
Global Const AX_FUNC_ALM = 2
Global Const AX_FUNC_ERC = 4
Global Const AX_FUNC_SD = 8
Global Const AX_FUNC_EL = 16
Global Const AX_FUNC_SW_EL = 32
Global Const AX_FUNC_ORG = 64
Global Const AX_FUNC_EZ = 128
Global Const AX_FUNC_BACKLASH_CORRECT = 256
Global Const AX_FUNC_SUPPRESS_VIBRATION = 512
Global Const AX_FUNC_HOME = 1024
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis Pulse In Map Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_PI_MODE = 1
Global Const AX_PI_LOGIC = 2
Global Const AX_PI_SOURCE = 4
'/*
'+-----------------------------------------------------------------------------+
'| ******************* Axis Pulse In Mode Map Define ************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_PIM_1XAB = 1
Global Const AX_PIM_2XAB = 2
Global Const AX_PIM_4XAB = 4
Global Const AX_PIM_CWCCW = 8
'/*
'+-----------------------------------------------------------------------------+
'| ******************* Axis Pulse Out Map Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_PO_MODE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************* Axis Pulse Out Mode Map Define ************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_POM_OUT_DIR = 1
Global Const AX_POM_OUT_DIR_OUTNEG = 2
Global Const AX_POM_OUT_DIR_DIRNEG = 4
Global Const AX_POM_OUT_DIR_ALLNEG = 8
Global Const AX_POM_CW_CCW = 16
Global Const AX_POM_CW_CCW_ALLNEG = 32
Global Const AX_POM_AB = 64
Global Const AX_POM_BA = 128
Global Const AX_POM_CW_CCW_OUTNEG = 256
Global Const AX_POM_CW_CCW_DIRNEG = 512
'/*
'+-----------------------------------------------------------------------------+
'| *********************** Axis Alarm Map Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_ALM_EN = 1
Global Const AX_ALM_LOGIC = 2
Global Const AX_ALM_REACT = 4
'/*
'+-----------------------------------------------------------------------------+
'| ********************** Axis InPosition Map Define ************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_INP_EN = 1
Global Const AX_INP_LOGIC = 2
'/*
'+-----------------------------------------------------------------------------+
'| *********************** Axis ERC Map Define ******************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_ERC_EN_MODE = 1
Global Const AX_ERC_LOGIC = 2
Global Const AX_ERC_ON_TIME = 4
Global Const AX_ERC_OFF_TIME = 8
'/*
'+-----------------------------------------------------------------------------+
'| *********************** Axis SD Map Define ******************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_SD_EN = 1
Global Const AX_SD_LOGIC = 2
Global Const AX_SD_REACT = 4
Global Const AX_SD_LATCH = 8
'/*
'+-----------------------------------------------------------------------------+
'| ***************** Axis Hardware Limit Map Define ************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_EL_EN = 1
Global Const AX_EL_LOGIC = 2
Global Const AX_EL_REACT = 4
'/*
'+-----------------------------------------------------------------------------+
'| ************** Axis Software Minus Limit Map Define *********************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_SW_MEL_EN = 1
Global Const AX_SW_MEL_REACT = 2
Global Const AX_SW_MEL_VALUE = 4
'/*
'+-----------------------------------------------------------------------------+
'| ************** Axis Software Plus Limit Map Define ************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_SW_PEL_EN = 1
Global Const AX_SW_PEL_REACT = 2
Global Const AX_SW_PEL_VALUE = 4
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis Home Map Define ********************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_HM_MODE = 1
Global Const AX_HM_ORG_LOGIC = 2
Global Const AX_HM_EZ_LOGIC = 4
'/*
'+-----------------------------------------------------------------------------+
'+-----------------------------------------------------------------------------+
'*/
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Pulse In Mode Define ********************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const AB_1X = 0
Global Const AB_2X = 1
Global Const AB_4X = 2
Global Const I_CW_CCW = 3
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Pulse In Logic Define ******************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const NO_INV_DIR = 0
Global Const INV_DIR = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Pulse In Source Define ******************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const EXTERNAL_FEEBACK = 0
Global Const COMMAND_PULSE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Pulse Out Mode Define ******************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const OUT_DIR = 1
Global Const OUT_DIR_OUT_NEG = 2
Global Const OUT_DIR_DIR_NEG = 4
Global Const OUT_DIR_ALL_NEG = 8
Global Const O_CW_CCW = 16
Global Const CW_CCW_ALL_NEG = 32
Global Const AB_PHASE = 64
Global Const BA_PHASE = 128
Global Const CW_CCW_OUT_NEG = 256
Global Const CW_CCW_DIR_NEG = 512
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Alarm Enable Define ********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ALM_DIS = 0
Global Const ALM_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Alarm Logic Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ALM_ACT_LOW = 0
Global Const ALM_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Alarm React Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ALM_IMMED_STOP = 0
Global Const ALM_DEC_TO_STOP = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Inposition Enable Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const INP_DIS = 0
Global Const INP_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** Inposition Logic Define ****************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const INP_ACT_LOW = 0
Global Const INP_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** SD Enable Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SD_ENABLE = 0
Global Const SD_DISABLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** SD Latch Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SD_LEVEL = 0
Global Const SD_LATCH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** SD Logic Define ****************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SD_ACT_LOW = 0
Global Const SD_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ******************** SD React Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SD_IMMED_STOP = 1
Global Const SD_DEC_AND_STOP = 0
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Hardware Limit Enable Define *************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const HLMT_DIS = 0
Global Const HLMT_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Out Logic Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const OUT_ACT_LOW = 0
Global Const OUT_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Dir Logic Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const DIR_ACT_LOW = 0
Global Const DIR_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Erc Logic Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ERC_DIS = 0
Global Const ERC_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Erc Timer Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ERC_OFF_TIME_DIS = 0
Global Const ERC_OFF_TIME_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Hardware Limit Enable Define *************************** |
'+-----------------------------------------------------------------------------+
'*/
'Global Const HLMT_DIS = 0
'Global Const HLMT_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Hardware Limit Logic Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const HLMT_ACT_LOW = 0
Global Const HLMT_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Hardware Limit React Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const HLMT_IMMED_STOP = 0
Global Const HLMT_DEC_TO_STOP = 1

'/*
'+-----------------------------------------------------------------------------+
'| ****************** IN Disable or Enable Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const IN_DISABLE = 0
Global Const IN_ENABLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Software Limit Enable Define *************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SLMT_DIS = 0
Global Const SLMT_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Software Limit React Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SLMT_IMMED_STOP = 0
Global Const SLMT_DEC_TO_STOP = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Ax General DO¡¡Enable Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const GEN_DO_DISABLE = 0
Global Const GEN_DO_ENABLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Ax External Master Source Define ************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const EXT_MASTER_SRC_AX_0 = 0
Global Const EXT_MASTER_SRC_AX_1 = 1
Global Const EXT_MASTER_SRC_AX_2 = 2
Global Const EXT_MASTER_SRC_AX_3 = 3
Global Const EXT_MASTER_SRC_SELF = 4
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Ax External Drive Mode ************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const EXT_DRIVE_DISABLE = 0
Global Const EXT_DRIVE_JOG = 1
Global Const EXT_DRIVE_MPG = 2
Global Const EXT_DRIVE_MPG_CONT = 3
Global Const EXT_DRIVE_MPG_PRESETP = 4
Global Const EXT_DRIVE_MPG_PRESETN = 5
'/*
'+-----------------------------------------------------------------------------+
'| ********************* ORG Enable Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ORG_DISABLE = 0
Global Const ORG_ENABLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************* ORG Logic Define ************************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const ORG_ACT_LOW = 0
Global Const ORG_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************* ORG Latch Define ************************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const ORG_LEVEL = 1
Global Const ORG_LATCH = 0
'/*
'+-----------------------------------------------------------------------------+
'| ********************** EZ Enable Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const EZ_DISABLE = 0
Global Const EZ_ENABLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************** EZ Logic Define ************************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const EZ_ACT_LOW = 0
Global Const EZ_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************** In1 Enable Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const IN1_DISABLE = 0
Global Const IN1_ENABLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************** IN1 Logic Define ************************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const IN1_ACT_LOW = 0
Global Const IN1_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************** In2 Enable Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const IN2_DISABLE = 0
Global Const IN2_ENABLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************** IN2 Logic Define ************************************ |
'+-----------------------------------------------------------------------------+
'*/
Global Const IN2_ACT_LOW = 0
Global Const IN2_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************** Servo State Define ********************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const SV_OFF = 0
Global Const SV_ON = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************** Motion Axis State Define *************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const STA_AX_DISABLE = 0
Global Const STA_AX_READY = 1
Global Const STA_AX_STOPPING = 2
Global Const STA_AX_ERROR_STOP = 3
Global Const STA_AX_HOMING = 4
Global Const STA_AX_PTP_MOT = 5
Global Const STA_AX_CONTI_MOT = 6
Global Const STA_AX_SYNC_MOT = 7
Global Const STA_AX_EXT_JOG = 8
Global Const STA_AX_EXT_MPG = 9

'/*
'+-----------------------------------------------------------------------------+
'| ********************** Motion Group State Define ************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const STA_GP_DISABLE = 0
Global Const STA_GP_READY = 1
Global Const STA_GP_STOPPING = 2
Global Const STA_GP_ERROR_STOP = 3
Global Const STA_GP_MOTION = 4
Global Const STA_GP_AX_MOTION = 5
Global Const STA_GP_MOTION_PATH = 6
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Veloctiy Move Direction Define ************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const DIR_POSITIVE = 0
Global Const DIR_NEGATIVE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion Home Mode Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AbsSwitch = 0
Global Const LmtSwitch = 1
Global Const RefPulse = 2
Global Const Direct = 3

Global Const MODE1_Abs = 0
Global Const MODE2_Lmt = 1
Global Const MODE3_Ref = 2
Global Const MODE4_Abs_Ref = 3
Global Const MODE5_Abs_NegRef = 4
Global Const MODE6_Lmt_Ref = 5
Global Const MODE7_AbsSearch = 6
Global Const MODE8_LmtSearch = 7
Global Const MODE9_AbsSearch_Ref = 8
Global Const MODE10_AbsSearch_NegRef = 9
Global Const MODE11_LmtSearch_Ref = 10
'/*
'+-----------------------------------------------------------------------------+
'| ****************** Motion Home Direction Mode Define ********************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const PosiDir = 0
Global Const NegDir = 1
Global Const SwitchPosi = 2
Global Const SwitchNeg = 3
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion Home Switch Mode Define ********************** |
'+-----------------------------------------------------------------------------+
'*/
'Global Const LevelOn = 0
'Global Const LevelOff = 1
'Global Const EdgeOn = 2
'Global Const EdgeOff = 3
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion HomeEx Steps Define ********************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const HomeStep0 = 0
Global Const HomeStep1 = 1
Global Const HomeStep2 = 2
Global Const HomeStep3 = 3
Global Const HomeStep4 = 4
Global Const HomeStep5 = 5
Global Const HomeStep6 = 6
'/*
'+-----------------------------------------------------------------------------+
'| ***************** Arc Interpolation Direction Define ********************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const DIR_CW = 0
Global Const DIR_CCW = 1
'/*
'+-----------------------------------------------------------------------------+
'| ***************** Compare Source Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SRC_COMMAND_POSITION = 0
Global Const SRC_ACTUAL_POSITION = 1
'/*
'+-----------------------------------------------------------------------------+
'| ***************** Compare Pulse mode Define ******************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const CMP_PULSE = 0
Global Const CMP_TOGGLE = 1
'/*
'+-----------------------------------------------------------------------------+
'| ***************** Compare Enable Define *********************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const CMP_ENABLE = 1
Global Const CMP_DISABLE = 0
'/*
'+-----------------------------------------------------------------------------+
'| ***************** Compare Pulse Logic Define ****************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const CP_ACT_LOW = 0
Global Const CP_ACT_HIGH = 1
'/*
'+-----------------------------------------------------------------------------+
'| ***************** Compare Pulse Width Define ****************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const CP_5US = 0
Global Const CP_10US = 1
Global Const CP_20US = 2
Global Const CP_50US = 3
Global Const CP_100US = 4
Global Const CP_200US = 5
Global Const CP_500US = 6
Global Const CP_1000US = 7
'/*
'+-----------------------------------------------------------------------------+
'| ************************* Compare Method Define ****************************|
'+-----------------------------------------------------------------------------+
'*/
Global Const MTD_GREATER_POSITION = 0
Global Const MTD_SMALLER_POSITION = 1
Global Const MTD_DIRECTIONLESS = 2
'/*
'+-----------------------------------------------------------------------------+
'| ************************* Event Type Define ******************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_MOTION_DONE = &H80
Global Const AX_COMPARE = &H1E
Global Const EVT_NO_EVENT = &H0
Global Const EVT_AX_MOTION_DONE = &H1
Global Const EVT_AX_COMPARED = &H2
Global Const EVT_AX_VH_START = &H10
Global Const EVT_AX_VH_END = &H20
Global Const EVT_AX_LATCHBUF_DONE = &H40

Global Const EVT_GP_MOTION_DONE = &H2
Global Const EVT_GP1_MOTION_DONE = &H1            ' done event of GroupID 1
Global Const EVT_GP2_MOTION_DONE = &H2            ' done event of GroupID 2
Global Const EVT_GP3_MOTION_DONE = &H4            '// done event of GroupID 3

'Start High Speed
Global Const EVT_GP1_VH_START = &H1
Global Const EVT_GP2_VH_START = &H2
Global Const EVT_GP3_VH_START = &H4
Global Const EVT_GP4_VH_START = &H8


'Start deceleration
Global Const EVT_GP1_VH_END = &H1
Global Const EVT_GP2_VH_END = &H2
Global Const EVT_GP3_VH_END = &H4
Global Const EVT_GP4_VH_END = &H8

Global Const EVT_CANCELED = &H10
Global Const EVT_TIMEOUT = &HFFFF

Enum EVENT_TYPE
              TYPE_DISABLED
              TYPE_X_PULSE
              TYPE_X_GREATER_COMP_NEG
              TYPE_X_LESS_COMP_NEG
              TYPE_X_LESS_COMP_POS
              TYPE_X_GREATER_COMP_POS
              TYPE_X_C_END
              TYPE_X_C_STA
              TYPE_X_D_END

              TYPE_Y_PULSE
              TYPE_Y_GREATER_COMP_NEG
              TYPE_Y_LESS_COMP_NEG
              TYPE_Y_LESS_COMP_POS
              TYPE_Y_GREATER_COMP_POS
              TYPE_Y_C_END
              TYPE_Y_C_STA
              TYPE_Y_D_END

              TYPE_Z_PULSE
              TYPE_Z_GREATER_COMP_NEG
              TYPE_Z_LESS_COMP_NEG
              TYPE_Z_LESS_COMP_POS
              TYPE_Z_GREATER_COMP_POS
              TYPE_Z_C_END
              TYPE_Z_C_STA
              TYPE_Z_D_END

              TYPE_U_PULSE
              TYPE_U_GREATER_COMP_NEG
              TYPE_U_LESS_COMP_NEG
              TYPE_U_LESS_COMP_POS
              TYPE_U_GREATER_COMP_POS
              TYPE_U_C_END
              TYPE_U_C_STA
              TYPE_U_D_END

              TYPE_INTERPOLATION
    
End Enum

Global Const MAX_EVENT_TYPE_NUM = 64   'TYPE_IPO+1

Global Const NodeHd_Type = &HF0
Global Const HdType_Dev = &HF1
Global Const HdType_Axis = &HF2
Global Const HdType_Group = &HF3
Global Const HdType_AxInGp = &HF4
Global Const HdType_MasDev = &HF5

Global Const WR3_Mask = &HF9F             '//use '&'


'//********************************Motion type of Axis OR Group*****************
'//Ptp types:
Global Const Unknown = 0
Global Const SynTPtP = 1
Global Const AsynTPtP = 2
Global Const SynSPtP = 3
Global Const AsynSPtP = 4
'Conti types:
Global Const SynTConti = 5
Global Const AsynTConti = 6
Global Const SynSConti = 7
Global Const AsynSConti = 8
'Home types:
Global Const HomeEX = 9

'Stop types:
Global Const SynTDecStop = 22
Global Const AsynTDecStop = 23
Global Const SynSDecStop = 24
Global Const AsynSDecStop = 25

Global Const EmgStop = 26
'IPO types:
Global Const SynTLineIpo = 27
Global Const AsynTLineIpo = 28
Global Const SynSLineIpo = 29
Global Const AsynSLineIpo = 30
Global Const SynTContiIpo = 31
Global Const AsynTContiIpo = 32
Global Const SynTArcIpo = 33
Global Const AsynTArcIpo = 34
Global Const SynTDirectIpo = 35
Global Const AsynTDirectIpo = 36
Global Const SynSDirectIpo = 37
Global Const AsynSDirectIpo = 38
Global Const SynTDecStopGp = 39
Global Const AsynTDecStopGp = 40
Global Const SynSDecStopGp = 41
Global Const AsynSDecStopGp = 42
Global Const EmgStopGp = 43

Global Const SynSArcIpo = 44
Global Const AsynSArcIpo = 45
Global Const SynSContiIpo = 46
Global Const AsynSContiIpo = 47

'//******************Home Sub Types***************************//

Global Const SearchLevelOnOrgByFixDir = 0
Global Const SearchLevelOffOrgByFixDir = 1
Global Const SearchEdgeOnOrgByFixDir = 2       '//HomeEX_SubType1,2,3,4
Global Const SearchEdgeOffOrgByFixDir = 3
Global Const SearchLevelOnOrgBySwitchDir = 4
Global Const SearchLevelOffOrgBySwitchDir = 5
Global Const SearchEdgeOnOrgBySwitchDir = 6    '//HomeEX_SubType5,6,7,8
Global Const SearchEdgeOffOrgBySwitchDir = 7

Global Const SearchLevelOnLmtBySwitchDir = 8
Global Const SearchLevelOffLmtBySwitchDir = 9
Global Const SearchEdgeOnLmtByFixDir = 10       '//HomeEX_SubType9,10,11,12
Global Const SearchEdgeOnLmtBySwitchDir = 11    '//HomeEX_SubType13,14,15,16
Global Const SearchEdgeOffLmtBySwitchDir = 12
Global Const SearchEz = 13                      '//HomeEX_SubType17,18,19,20

Global Const HomeEX_SubType1 = 1         '//AbsSwitch+PosiDir+EdgeOn+HighActive
Global Const HomeEX_SubType2 = 2         '//AbsSwitch+PosiDir+EdgeOn+LowActive
Global Const HomeEX_SubType3 = 3         '//AbsSwitch+NegDir+EdgeOn+HighActive
Global Const HomeEX_SubType4 = 4         '//AbsSwitch+NegDir+EdgeOn+LowActive
Global Const HomeEX_SubType5 = 5         '//AbsSwitch+SwitchPosi+EdgeOn+HighActive
Global Const HomeEX_SubType6 = 6         '//AbsSwitch+SwitchPosi+EdgeOn+LowActive
Global Const HomeEX_SubType7 = 7         '//AbsSwitch+SwitchNeg+EdgeOn+HighActive
Global Const HomeEX_SubType8 = 8         '//AbsSwitch+SwitchNeg+EdgeOn+LowActive
Global Const HomeEX_SubType9 = 9         '//LimitSwitch+PosiDir+EdgeOn+HighActive
Global Const HomeEX_SubType10 = 10       '//LimitSwitch+PosiDir+EdgeOn+LowActive
Global Const HomeEX_SubType11 = 11       '//LimitSwitch+NegDir+EdgeOn+HighActive
Global Const HomeEX_SubType12 = 12       '//LimitSwitch+NegDir+EdgeOn+LowActive
Global Const HomeEX_SubType13 = 13       '//LimitSwitch+SwitchPosi+EdgeOn+HighActive
Global Const HomeEX_SubType14 = 14       '//LimitSwitch+SwitchPosi+EdgeOn+LowActive
Global Const HomeEX_SubType15 = 15       '//LimitSwitch+SwitchNeg+EdgeOn+HighActive
Global Const HomeEX_SubType16 = 16       '//LimitSwitch+SwitchNeg+EdgeOn+LowActive
Global Const HomeEX_SubType17 = 17       '//RefPulse+PosiDir+EdgeOn+HighActive
Global Const HomeEX_SubType18 = 18       '//RefPulse+PosiDir+EdgeOn+LowActive
Global Const HomeEX_SubType19 = 19       '//RefPulse+NegDir+EdgeOn+HighActive
Global Const HomeEX_SubType20 = 20       '//RefPulse+NegDir+EdgeOn+LowActive
'//******************End of Home Sub Types***************************//
'//**********************************End of Motion type***************************

Global Const Ptp_PosDir = 0
Global Const Ptp_NegDir = 1

Global Const Conti_PosDir = 0
Global Const Conti_NegDir = 1

Global Const Home_PosDir = 0
Global Const Home_NegDir = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion Home Phase Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const Home_Phase_Search_ORG = 0
Global Const Home_Phase_Search_EL = 1
Global Const Home_Phase_Search_EZ = 2
Global Const Home_Phase_CrossDistance = 3
Global Const Home_Phase_ORG_OUTING = 4
Global Const Home_Phase_EL_OUTING = 5
Global Const Home_Phase_EZ_OUTING = 6
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion Home Switch Mode Define ********************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const LevelOn = 0
Global Const LevelOff = 1
Global Const EdgeOn = 2
Global Const EdgeOff = 3
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion Path Cmd Define ****************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const EndPath = 0
Global Const Abs2DLine = 1
Global Const Rel2DLine = 2
Global Const Abs2DArcCW = 3
Global Const Abs2DArcCCW = 4
Global Const Rel2DArcCW = 5
Global Const Rel2DArcCCW = 6
Global Const Abs3DLine = 7
Global Const Rel3DLine = 8
Global Const AbsMultiLine = 9
Global Const RelMultiLine = 10
'Add for pci1245 and pci1265[dan.yang 2011.05.04]// Reserved.
Global Const Abs2DDirect = 11
Global Const Rel2DDirect = 12
Global Const Abs3DDirect = 13
Global Const Rel3DDirect = 14
Global Const Abs4DDirect = 15
Global Const Rel4DDirect = 16
Global Const Abs5DDirect = 17
Global Const Rel5DDirect = 18
Global Const Abs6DDirect = 19
Global Const Rel6DDirect = 20
Global Const Abs3DArcCW = 21
Global Const Rel3DArcCW = 22
Global Const Abs3DArcCCW = 23
Global Const Rel3DArcCCW = 24
Global Const Abs3DSpiralCW = 25
Global Const Rel3DSpiralCW = 26
Global Const Abs3DSpiralCCW = 27
Global Const Rel3DSpiralCCW = 28
Global Const GPDELAY = 29

Global Const Abs4DSpiralCW = 30                   'add by zhaocui.deng 20130402
Global Const Rel4DSpiralCW = 31
Global Const Abs4DSpiralCCW = 32
Global Const Rel4DSpiralCCW = 33
Global Const Abs5DSpiralCW = 34
Global Const Rel5DSpiralCW = 35
Global Const Abs5DSpiralCCW = 36
Global Const Rel5DSpiralCCW = 37
Global Const Abs6DSpiralCW = 38
Global Const Rel6DSpiralCW = 39
Global Const Abs6DSpiralCCW = 40
Global Const Rel6DSpiralCCW = 41
Global Const Abs7DSpiralCW = 42
Global Const Rel7DSpiralCW = 43
Global Const Abs7DSpiralCCW = 44
Global Const Rel7DSpiralCCW = 45
Global Const Abs8DSpiralCW = 46
Global Const Rel8DSpiralCW = 47
Global Const Abs8DSpiralCCW = 48
Global Const Rel8DSpiralCCW = 49



'added by zhaocui.deng 20130502

Global Const Abs2DArcCWAngle = 50
Global Const Rel2DArcCWAngle = 51
Global Const Abs2DArcCCWAngle = 52
Global Const Rel2DArcCCWAngle = 53
Global Const Abs3DArcCWAngle = 54
Global Const Rel3DArcCWAngle = 55
Global Const Abs3DArcCCWAngle = 56
Global Const Rel3DArcCCWAngle = 57
Global Const Abs3DSpiralCWAngle = 58
Global Const Rel3DSpiralCWAngle = 59
Global Const Abs3DSpiralCCWAngle = 60
Global Const Rel3DSpiralCCWAngle = 61
Global Const Abs4DSpiralCWAngle = 62
Global Const Rel4DSpiralCWAngle = 63
Global Const Abs4DSpiralCCWAngle = 64
Global Const Rel4DSpiralCCWAngle = 65
Global Const Abs5DSpiralCWAngle = 66
Global Const Rel5DSpiralCWAngle = 67
Global Const Abs5DSpiralCCWAngle = 68
Global Const Rel5DSpiralCCWAngle = 69
Global Const Abs6DSpiralCWAngle = 70
Global Const Rel6DSpiralCWAngle = 71
Global Const Abs6DSpiralCCWAngle = 72
Global Const Rel6DSpiralCCWAngle = 73
Global Const Abs7DSpiralCWAngle = 74
Global Const Rel7DSpiralCWAngle = 75
Global Const Abs7DSpiralCCWAngle = 76
Global Const Rel7DSpiralCCWAngle = 77
Global Const Abs8DSpiralCWAngle = 78
Global Const Rel8DSpiralCWAngle = 79
Global Const Abs8DSpiralCCWAngle = 80
Global Const Rel8DSpiralCCWAngle = 81
Global Const Abs7DDirect = 82
Global Const Rel7DDirect = 83
Global Const Abs8DDirect = 84
Global Const Rel8DDirect = 85
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion Path Mode Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const BlendingEn = 0
Global Const BlendingDis = 1
'/*
'+-----------------------------------------------------------------------------+
'| ********************* Motion Event Name Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ADV_EVT_NUM = 6
Global Const ADV_EVT_AX_DONE = "EVT_AX_DONE"
Global Const ADV_EVT_GP_DONE = "EVT_GP_DONE"
Global Const ADV_EVT_CANCELED = "EVT_CANCELED"
    

'/*
'+-----------------------------------------------------------------------------+
'| *********************** Axis ERC Map Define ******************************* |
'+-----------------------------------------------------------------------------+
'*/
'Global Const AX_ERC_EN_MODE = 1
'Global Const AX_ERC_LOGIC = 2
'Global Const AX_ERC_ON_TIME = 4
'Global Const AX_ERC_OFF_TIME = 8


'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis ERC On-Time Map Define ************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_ERC_ON_12US = 1
Global Const AX_ERC_ON_102US = 2
Global Const AX_ERC_ON_409US = 4
Global Const AX_ERC_ON_1600US = 8
Global Const AX_ERC_ON_13MS = 16
Global Const AX_ERC_ON_52MS = 32
Global Const AX_ERC_ON_104MS = 64
Global Const AX_ERC_ON_LEVEL = 128

'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis ERC Enable mode Map Define ********************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_ERC_ON_HOME = 1
Global Const AX_ERC_ON_MIO = 2
Global Const AX_ERC_ON_HOME_MIO = 4

'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis ERC Off-Time Map Define ************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_ERC_OFF_0US = 1
Global Const AX_ERC_OFF_12US = 2
Global Const AX_ERC_OFF_1600US = 4
Global Const AX_ERC_OFF_104MS = 8

'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis BACKLASH Map Define ************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_BKSH_EN = 1
Global Const AX_BKSH_VALUE = 2

'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis VIBRATION Map Define ************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const AX_VIBRA_EN = 1
Global Const AX_VIBRA_FWD_VALUE = 2
Global Const AX_VIBRA_REV_VALUE = 4

'/*
'+-----------------------------------------------------------------------------+
'| ***************** ERC Acitve Logic Define ********************************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const ERC_ACT_LOW = 0
Global Const ERC_ACT_HIGH = 1

'//Added by Dean -----------------------------------------------
Global Const ERC_ON_DISABLE = 0
Global Const ERC_ON_HOME = 1
Global Const ERC_ON_MIO = 2
Global Const ERC_ON_HOME_MIO = 3

'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis ERC On-Time Define ****************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ERC_ON_12US = 0
Global Const ERC_ON_102US = 1
Global Const ERC_ON_409US = 2
Global Const ERC_ON_1600US = 3
Global Const ERC_ON_13MS = 4
Global Const ERC_ON_52MS = 5
Global Const ERC_ON_104MS = 6
Global Const ERC_ON_LEVEL = 7

'/*
'+-----------------------------------------------------------------------------+
'| ******************** Axis ERC Off-Time Define ***************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ERC_OFF_0US = 0
Global Const ERC_OFF_12US = 1
Global Const ERC_OFF_1600US = 2
Global Const ERC_OFF_104MS = 3

'/*
'+-----------------------------------------------------------------------------+
'| ***************** ERC on home Define(only for KW Dialog) ****************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const ERC_HOME_DISABLE = 0
Global Const ERC_HOME_ENABLE = 1

'/*
'+-----------------------------------------------------------------------------+
'| ***************** ERC on motion_io Define(only for KW Dialog) ************* |
'+-----------------------------------------------------------------------------+
'*/
Global Const ERC_ERRMIO_DISABLE = 0
Global Const ERC_ERRMIO_ENABLE = 1

'/*
'+-----------------------------------------------------------------------------+
'| ****************** SD  Enable Define *************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SD_DIS = 0
Global Const SD_EN = 1
'/*
'+-----------------------------------------------------------------------------+
'| ****************** SD React Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SD_DEC = 0
Global Const SD_DEC_TO_STOP = 1

'/*
'+-----------------------------------------------------------------------------+
'| ****************** SD Latch Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const SD_LATCH_DIS = 0
Global Const SD_LATCH_EN = 1

'/*
'+-----------------------------------------------------------------------------+
'| ****************** Latch Logic Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const LATCH_ACT_LOW = 0
Global Const LATCH_ACT_HIGH = 1

'/*
'+-----------------------------------------------------------------------------+
'| ****************** Home Reset Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const HOME_RESET_DIS = 0
Global Const HOME_RESET_EN = 1
'+-----------------------------------------------------------------------------+
'| ****************** Backlash enable Define **************************** |
'+-----------------------------------------------------------------------------+
Global Const BKSH_DIS = 0
Global Const BKSH_EN = 1
'+-----------------------------------------------------------------------------+
'| ****************** Backlash enable Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const VIBRA_DIS = 0
Global Const VIBRA_EN = 1

'/*
'+-----------------------------------------------------------------------------+
'| ****************** Baudrate Define **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const BR_AMONET_2500K = 0
Global Const BR_AMONET_5M = &H1
Global Const BR_AMONET_10M = &H2
Global Const BR_AMONET_20M = &H3
Global Const BR_CAN_10K = &H10
Global Const BR_CAN_20K = &H11
Global Const BR_CAN_50K = &H12
Global Const BR_CAN_125K = &H13
Global Const BR_CAN_250K = &H14
Global Const BR_CAN_500K = &H15
Global Const BR_CAN_800K = &H16
Global Const BR_CAN_1000K = &H17

'/*
'+-----------------------------------------------------------------------------+
'| ****************** Master Communication Status **************************** |
'+-----------------------------------------------------------------------------+
'*/
Global Const COM_ST_DISCONNECTED = &H0
Global Const COM_ST_CONNECTED = &H1
Global Const COM_ST_SLAVE_ERROR = &H2
Global Const COM_ST_BUSY = &H3
Global Const COM_ST_ERROR = &H4

Global Const SUCCESS = &H0
Global Const Warning = &H10000000
Global Const FuncError = &H80000000
Global Const CommError = &H80001000
Global Const MotionError = &H80002000
Global Const DaqError = &H80003000
Global Const DevEvtError = &H80004000

Global Const InvalidDevNumber = (FuncError + 0)
Global Const DevRegDataLost = (FuncError + 1)
Global Const LoadDllFailed = (FuncError + 2)
Global Const GetProcAddrFailed = (FuncError + 3)
Global Const MemAllocateFailed = (FuncError + 4)
Global Const InvalidHandle = (FuncError + 5)
Global Const CreateFileFailed = (FuncError + 6)
Global Const OpenEventFailed = (FuncError + 7)
Global Const EventTimeOut = (FuncError + 8)
Global Const InvalidInputParam = (FuncError + 9)
Global Const PropertyIDNotSupport = (FuncError + 10)
Global Const PropertyIDReadOnly = (FuncError + 11)
Global Const ConnectWinIrqFailed = (FuncError + 12)
Global Const InvalidAxCfgVel = (FuncError + 13)
Global Const InvalidAxCfgAcc = (FuncError + 14)
Global Const InvalidAxCfgDec = (FuncError + 15)
Global Const InvalidAxCfgJerk = (FuncError + 16)
Global Const InvalidAxParVelLow = (FuncError + 17)
Global Const InvalidAxParVelHigh = (FuncError + 18)
Global Const InvalidAxParAcc = (FuncError + 19)
Global Const InvalidAxParDec = (FuncError + 20)
Global Const InvalidAxParJerk = (FuncError + 21)
Global Const InvalidAxPulseInMode = (FuncError + 22)
Global Const InvalidAxPulseOutMode = (FuncError + 23)
Global Const InvalidAxAlarmEn = (FuncError + 24)
Global Const InvalidAxAlarmLogic = (FuncError + 25)
Global Const InvalidAxInPEn = (FuncError + 26)
Global Const InvalidAxInPLogic = (FuncError + 27)
Global Const InvalidAxHLmtEn = (FuncError + 28)
Global Const InvalidAxHLmtLogic = (FuncError + 29)
Global Const InvalidAxHLmtReact = (FuncError + 30)
Global Const InvalidAxSLmtPEn = (FuncError + 31)
Global Const InvalidAxSLmtPReact = (FuncError + 32)
Global Const InvalidAxSLmtPValue = (FuncError + 33)
Global Const InvalidAxSLmtMEn = (FuncError + 34)
Global Const InvalidAxSLmtMReact = (FuncError + 35)
Global Const InvalidAxSLmtMValue = (FuncError + 36)
Global Const InvalidAxOrgLogic = (FuncError + 37)
Global Const InvalidAxOrgEnable = (FuncError + 38)
Global Const InvalidAxEzLogic = (FuncError + 39)
Global Const InvalidAxEzEnable = (FuncError + 40)
Global Const InvalidAxEzCount = (FuncError + 41)
Global Const InvalidAxState = (FuncError + 42)
Global Const InvalidAxInEnable = (FuncError + 43)
Global Const InvalidAxSvOnOff = (FuncError + 44)
Global Const InvalidAxDistance = (FuncError + 45)
Global Const InvalidAxPosition = (FuncError + 46)
Global Const InvalidAxHomeModeKw = (FuncError + 47)
Global Const InvalidAxCntInGp = (FuncError + 48)
Global Const AxInGpNotFound = (FuncError + 49)
Global Const AxIsInOtherGp = (FuncError + 50)
Global Const AxCannotIntoGp = (FuncError + 51)
Global Const GpInDevNotFound = (FuncError + 52)
Global Const InvalidGpCfgVel = (FuncError + 53)
Global Const InvalidGpCfgAcc = (FuncError + 54)
Global Const InvalidGpCfgDec = (FuncError + 55)
Global Const InvalidGpCfgJerk = (FuncError + 56)
Global Const InvalidGpParVelLow = (FuncError + 57)
Global Const InvalidGpParVelHigh = (FuncError + 58)
Global Const InvalidGpParAcc = (FuncError + 59)
Global Const InvalidGpParDec = (FuncError + 60)
Global Const InvalidGpParJerk = (FuncError + 61)
Global Const JerkNotSupport = (FuncError + 62)
Global Const ThreeAxNotSupport = (FuncError + 63)
Global Const DevIpoNotFinished = (FuncError + 64)
Global Const InvalidGpState = (FuncError + 65)
Global Const OpenFileFailed = (FuncError + 66)
Global Const InvalidPathCnt = (FuncError + 67)
Global Const InvalidPathHandle = (FuncError + 68)
Global Const InvalidPath = (FuncError + 69)
Global Const IoctlError = (FuncError + 70)
Global Const AmnetRingUsed = (FuncError + 71)
Global Const DeviceNotOpened = (FuncError + 72)
Global Const InvalidRing = (FuncError + 73)
Global Const InvalidSlaveIP = (FuncError + 74)
Global Const InvalidParameter = (FuncError + 75)
Global Const InvalidGpCenterPosition = (FuncError + 76)
Global Const InvalidGpEndPosition = (FuncError + 77)
Global Const InvalidAddress = (FuncError + 78)
Global Const DeviceDisconnect = (FuncError + 79)
Global Const DataOutBufExceeded = (FuncError + 80)
Global Const SlaveDeviceNotMatch = (FuncError + 81)
Global Const SlaveDeviceError = (FuncError + 82)
Global Const SlaveDeviceUnknow = (FuncError + 83)
Global Const FunctionNotSupport = (FuncError + 84)
Global Const InvalidPhysicalAxis = (FuncError + 85)
Global Const InvalidVelocity = (FuncError + 86)
Global Const InvalidAxPulseInLogic = (FuncError + 87)
Global Const InvalidAxPulseInSource = (FuncError + 88)
Global Const InvalidAxErcLogic = (FuncError + 89)
Global Const InvalidAxErcOnTime = (FuncError + 90)
Global Const InvalidAxErcOffTime = (FuncError + 91)
Global Const InvalidAxErcEnableMode = (FuncError + 92)
Global Const InvalidAxSdEnable = (FuncError + 93)
Global Const InvalidAxSdLogic = (FuncError + 94)
Global Const InvalidAxSdReact = (FuncError + 95)
Global Const InvalidAxSdLatch = (FuncError + 96)
Global Const InvalidAxHomeResetEnable = (FuncError + 97)
Global Const InvalidAxBacklashEnable = (FuncError + 98)
Global Const InvalidAxBacklashPulses = (FuncError + 99)
Global Const InvalidAxVibrationEnable = (FuncError + 100)
Global Const InvalidAxVibrationRevTime = (FuncError + 101)
Global Const InvalidAxVibrationFwdTime = (FuncError + 102)
Global Const InvalidAxAlarmReact = (FuncError + 103)
Global Const InvalidAxLatchLogic = (FuncError + 104)
Global Const InvalidFwMemoryMode = (FuncError + 105)
Global Const InvalidConfigFile = (FuncError + 106)
Global Const InvalidAxEnEvtArraySize = (FuncError + 107)
Global Const InvalidAxEnEvtArray = (FuncError + 108)
Global Const InvalidGpEnEvtArraySize = (FuncError + 109)
Global Const InvalidGpEnEvtArray = (FuncError + 110)
Global Const InvalidIntervalData = (FuncError + 111)
Global Const InvalidEndPosition = (FuncError + 112)
Global Const InvalidAxisSelect = (FuncError + 113)
Global Const InvalidTableSize = (FuncError + 114)
Global Const InvalidGpHandle = (FuncError + 115)
Global Const InvalidCmpSource = (FuncError + 116)
Global Const InvalidCmpMethod = (FuncError + 117)
Global Const InvalidCmpPulseMode = (FuncError + 118)
Global Const InvalidCmpPulseLogic = (FuncError + 119)
Global Const InvalidCmpPulseWidth = (FuncError + 120)
Global Const InvalidPathFunctionID = (FuncError + 121)
Global Const SysBufAllocateFailed = (FuncError + 122)

Global Const SpeedFordFunNotSpported = (FuncError + 123)
Global Const InvalidNormVector = (FuncError + 124)
Global Const InvalidCmpTimeTableCount = (FuncError + 125)
Global Const InvalidCmpTime = (FuncError + 126)
Global Const FWDownLoading = (FuncError + 127)
Global Const FWVersionNotMatch = (FuncError + 128)

    '//AMONet Communication error
Global Const SlaveIOUpdateError = (FuncError + 150)
Global Const NoSlaveDevFound = (FuncError + 151)
Global Const MasterDevNotOpen = (FuncError + 152)
Global Const MasterRingNotOpen = (FuncError + 153)


    '//DAQ function  -----------------------------------
Global Const InvalidDIPort = (FuncError + 200)
Global Const InvalidDOPort = (FuncError + 201)
Global Const InvalidDOValue = (FuncError + 202)

    '//EVT function
Global Const CreateEventFailed = (FuncError + 203)
Global Const CreateThreadFailed = (FuncError + 204)
Global Const InvalidHomeModeEx = (FuncError + 205)
Global Const InvalidDirMode = (FuncError + 206)
Global Const AxHomeMotionFailed = (FuncError + 207)
Global Const ReadFileFailed = (FuncError + 208)
Global Const PathBufIsFull = (FuncError + 209)
Global Const PathBufIsEmpty = (FuncError + 210)
Global Const GetAuthorityFailed = (FuncError + 211)
Global Const GpIDAllocatedFailed = (FuncError + 212)
Global Const FirmWareDown = (FuncError + 213)
Global Const InvalidGpRadius = (FuncError + 214)
Global Const InvalidAxCmd = (FuncError + 215)
Global Const InvalidaxExtDrv = (FuncError + 216)
Global Const InvalidGpMovCmd = (FuncError + 217)
Global Const SpeedCurveNotSupported = (FuncError + 218)
Global Const InvalidCounterNo = (FuncError + 219)
Global Const InvalidPathMoveMode = (FuncError + 220)
Global Const PathSelStartCantRunInSpeedForwareMode = (FuncError + 221)
Global Const InvalidCamTableID = (FuncError + 222)
Global Const InvalidCamPointRange = (FuncError + 223)
Global Const CamTableIsEmpty = (FuncError + 224)
Global Const InvalidPlaneVector = (FuncError + 225)
Global Const MasAxIDSameSlvAxID = (FuncError + 226)
Global Const InvalidGpRefPlane = (FuncError + 227)
Global Const InvalidAxModuleRange = (FuncError + 228)
Global Const DownloadFileFailed = (FuncError + 229)
Global Const InvalidFileLength = (FuncError + 230)
Global Const InvalidCmpCnt = (FuncError + 231)
Global Const JerkExceededMaxValue = (FuncError + 232)
Global Const AbsMotionNotSupport = (FuncError + 233)
Global Const invalidAiRange = (FuncError + 234)
Global Const AIScaleFailed = (FuncError + 235)
Global Const AxInRobot = (FuncError + 236)
Global Const Invalid3DArcFlat = (FuncError + 237)
Global Const InvalidIpoMap = (FuncError + 238)
Global Const AxisNotFound = (FuncError + 240)
Global Const DataSizeNotCorrect = (FuncError + 239)
Global Const InvalidPathVelHigh = (FuncError + 241)

Global Const HLmtPExceeded = (MotionError + 0)
Global Const HLmtNExceeded = (MotionError + 1)
Global Const SLmtPExceeded = (MotionError + 2)
Global Const SLmtNExceeded = (MotionError + 3)
Global Const AlarmHappened = (MotionError + 4)
Global Const EmgHappened = (MotionError + 5)
Global Const TimeLmtExceeded = (MotionError + 6)
Global Const DistLmtExceeded = (MotionError + 7)
Global Const InvalidPositionOverride = (MotionError + 8)
Global Const OperationErrorHappened = (MotionError + 9)
Global Const SimultaneousStopHappened = (MotionError + 10)
Global Const OverflowInPAPB = (MotionError + 11)
Global Const OverflowInIPO = (MotionError + 12)
Global Const STPHappened = (MotionError + 13)
Global Const SDHappened = (MotionError + 14)
Global Const AxsiNoCmpDataLeft = (MotionError + 15)
Global Const DevEvtTimeOut = (DevEvtError + 1)
Global Const DevNoEvt = (DevEvtError + 2)
    '//*************************************************************
Global Const Warning_AxWasInGp = (Warning + 1)
    '//inconsistent rate of PPU setting
Global Const Warning_GpInconsistRate = (Warning + 2)
Global Const Warning_GpInconsistPPU = (Warning + 3)
Global Const AX_MOTION_IO_RDY = 1
Global Const AX_MOTION_IO_ALM = 2
Global Const AX_MOTION_IO_LMTP = 4
Global Const AX_MOTION_IO_LMTN = 8
Global Const AX_MOTION_IO_ORG = 16
Global Const AX_MOTION_IO_DIR = 32
Global Const AX_MOTION_IO_EMG = 64
Global Const AX_MOTION_IO_PCS = 128
Global Const AX_MOTION_IO_ERC = 256
Global Const AX_MOTION_IO_EZ = 512
Global Const AX_MOTION_IO_CLR = 1024
Global Const AX_MOTION_IO_LTC = 2048
Global Const AX_MOTION_IO_SD = 4096
Global Const AX_MOTION_IO_INP = 8192
Global Const AX_MOTION_IO_SVON = 16384
Global Const AX_MOTION_IO_ALRM = 32768
Global Const AX_MOTION_IO_SLMTP = 65536
Global Const AX_MOTION_IO_SLMTN = 131072
Global Const AX_MOTION_IO_CMP = 262144
Global Const AX_MOTION_IO_CAMDO = 524288

'Add by yanli.zhu 2015.01.26 for ethcat
Global Const DAQ_AI_NEG_10V_TO_10V = &H1
Global Const DAQ_AI_NEG_5V_TO_5V = &H2
Global Const DAQ_AI_NEG_2500MV_TO_2500MV = &H4
Global Const DAQ_AI_NEG_1250MV_TO_1250MV = &H8
Global Const DAQ_AI_NEG_625MV_TO_625MV = &H10
Global Const DAQ_AI_NEG_1V_TO_1V = &H20
Global Const DAQ_AI_NEG_500MV_TO_500MV = &H40
Global Const DAQ_AI_NEG_150MV_TO_150MV = &H80
Global Const DAQ_AI_0MA_TO_20MA = &H10000
Global Const DAQ_AI_4MA_TO_20MA = &H20000
Global Const DAQ_AI_NEG_20MA_TO_20MA = &H40000

'Add by yanli.zhu 2015.01.26 for ethcat
Global Const CFG_DAQ_AI_NEG_10V_TO_10V = &H0
Global Const CFG_DAQ_AI_NEG_5V_TO_5V = &H1
Global Const CFG_DAQ_AI_NEG_2500MV_TO_2500MV = &H2
Global Const CFG_DAQ_AI_NEG_1250MV_TO_1250MV = &H3
Global Const CFG_DAQ_AI_NEG_625MV_TO_625MV = &H4
Global Const CFG_DAQ_AI_NEG_1V_TO_1V = &H5
Global Const CFG_DAQ_AI_NEG_500MV_TO_500MV = &H6
Global Const CFG_DAQ_AI_NEG_150MV_TO_150MV = &H7
Global Const CFG_DAQ_AI_NEG_0_TO_10V = &H8
Global Const CFG_DAQ_AI_NEG_0_TO_500mV = &H9
Global Const CFG_DAQ_AI_0MA_TO_20MA = &H10
Global Const CFG_DAQ_AI_4MA_TO_20MA = &H11
Global Const CFG_DAQ_AI_NEG_20MA_TO_20MA = &H12

Global Const CFG_DAQ_AO_NEG_10V_TO_10V = &H0
Global Const CFG_DAQ_AO_NEG_5V_TO_5V = &H1
Global Const CFG_DAQ_AO_NEG_2500MV_TO_2500MV = &H2
Global Const CFG_DAQ_AO_NEG_1250MV_TO_1250MV = &H3
Global Const CFG_DAQ_AO_NEG_625MV_TO_625MV = &H4
Global Const CFG_DAQ_AO_NEG_0V_TO_10 = &H5
Global Const CFG_DAQ_AO_0MA_TO_20MA = &H10
Global Const CFG_DAQ_AO_4MA_TO_20MA = &H11

Global Const DAQ_AO_NEG_10V_TO_10V = &H1
Global Const DAQ_AO_NEG_5V_TO_5V = &H2
Global Const DAQ_AO_NEG_2500MV_TO_2500MV = &H4
Global Const DAQ_AO_NEG_1250MV_TO_1250MV = &H8
Global Const DAQ_AO_NEG_625MV_TO_625MV = &H10
Global Const DAQ_AO_NEG_0V_TO_10 = &H20
Global Const DAQ_AO_0MA_TO_20MA = &H10000
Global Const DAQ_AO_4MA_TO_20MA = &H20000
'[Dan.yang 2011.06.08]
Declare Function Acm_GetAvailableDevs Lib "ADVMOT.dll" (ByRef DeviceList As DEVLIST, _
    ByVal MaxEntries As Long, ByRef OutEntries As Long) As Long
''''''''''''''''[Dan.yang 2011.06.21 Modified the parameters of functions]''''''''''''''''''''''''''''''''''''''
Declare Function Acm_GetAddress Lib "ADVMOT.dll" (lpVoid As Any) As Long
Declare Function Acm_DevOpen Lib "ADVMOT.dll" (ByVal DeviceNumber As Long, ByRef DeviceHandle As Long) As Long
Declare Function Acm_DevClose Lib "ADVMOT.dll" (ByRef DeviceHandle As Long) As Long
Declare Function Acm_DevReadEEPROM Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal EEPROMAddr As Integer, ByRef readValue As Integer) As Long
Declare Function Acm_DevWriteEEPROM Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal EEPROMAddr As Integer, ByVal WriteValue As Integer) As Long
Declare Function Acm_GetProperty Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal PropertyID As Long, ByRef Buffer As Any, ByRef BufferLength As Long) As Long
Declare Function Acm_GetU32Property Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal PropertyID As Long, ByRef Buffer As Long) As Long
Declare Function Acm_GetF64Property Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal PropertyID As Long, ByRef Buffer As Double) As Long
Declare Function Acm_SetProperty Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal PropertyID As Long, ByRef Buffer As Any, ByVal BufferLength As Long) As Long
Declare Function Acm_SetU32Property Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal PropertyID As Long, ByVal Buffer As Long) As Long
Declare Function Acm_SetF64Property Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal PropertyID As Long, ByVal Buffer As Double) As Long
Declare Function Acm_EnableMotionEvent Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, _
                                    ByRef AxEnableEvtArray As Long, ByRef GpEnableEvtArray As Long, _
                                    ByVal AxArrayElements As Long, ByVal GpArrayElements As Long) As Long
Declare Function Acm_CheckMotionEvent Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, _
                                    ByRef AxEnableEvtArray As Long, ByRef GpEnableEvtArray As Long, _
                                    ByVal AxArrayElements As Long, ByVal GpArrayElements As Long, ByVal Millisecond As Long) As Long

Declare Function Acm_CancelCheckEvent Lib "ADVMOT.dll" (ByVal DeviceHandle As Long) As Long
Declare Function Acm_DevLoadConfig Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ConfigPath As String) As Long
Declare Function Acm_DevFwDownload Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal Data As Long, ByVal DataID As Long) As Long

Declare Function Acm_GetErrorMessage Lib "ADVMOT.dll" (ByVal ErrorCode As Long, ByVal lpszError As String, ByVal nMaxError As Long) As Boolean
                         
'Master device operation
Declare Function Acm_MasStartRing Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer) As Long
Declare Function Acm_MasStopRing Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer) As Long
Declare Function Acm_MasGetComStatus Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer, ByRef pStatus As Integer) As Long
Declare Function Acm_MasGetComCyclicTime Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer, ByRef pTime As Double) As Long
Declare Function Acm_MasGetDataCyclicTime Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer, ByRef DataCyclicTime As Double) As Long
Declare Function Acm_MasGetActiveTable Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer, _
                    ByRef ActiveTableArray As Long, ByRef ArrayElements As Long) As Long
Declare Function Acm_MasGetErrorTable Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer, ByRef ErrorTableArray As Long, ByRef ArrayElements As Long) As Long
Declare Function Acm_MasGetSlaveInfo Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer, ByVal slaveIP As Integer, ByRef pInfo As Long) As Long
Declare Function Acm_MasLogComStatus Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer) As Long
Declare Function Acm_MasTrigOut Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer) As Long
Declare Function Acm_MasGetRingStatus Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ringNo As Integer, ByRef pStatus As Integer) As Long
'Axis operation
Declare Function Acm_AxOpen Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal PhyAxis As Integer, ByRef AxisHandle As Long) As Long
Declare Function Acm_AxClose Lib "ADVMOT.dll" (ByRef AxisHandle As Long) As Long
Declare Function Acm_AxSetErcOn Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal OnOff As Long) As Long
Declare Function Acm_AxResetAlm Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal OnOff As Long) As Long
Declare Function Acm_AxMoveRel Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal Distance As Double) As Long
Declare Function Acm_AxMoveAbs Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal Position As Double) As Long
Declare Function Acm_AxMoveVel Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal Direction As Integer) As Long
Declare Function Acm_AxStopDec Lib "ADVMOT.dll" (ByVal AxisHandle As Long) As Long
Declare Function Acm_AxStopEmg Lib "ADVMOT.dll" (ByVal AxisHandle As Long) As Long
Declare Function Acm_AxHomeEx Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal DirMode As Long) As Long
Declare Function Acm_AxHome Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal HomeMode As Long, ByVal DirMode As Long) As Long
Declare Function Acm_AxSetSvOn Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal OnOff As Long) As Long

'// U32 Acm_AxPauseMotion(U32 AxisHandle);
'// U32 Acm_AxResumeMotion(U32 AxisHandle);
Declare Function Acm_AxChangeVel Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal NewVel As Double) As Long
Declare Function Acm_AxChangePos Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal NewPos As Double) As Long
Declare Function Acm_AxResetError Lib "ADVMOT.dll" (ByVal AxisHandle As Long) As Long
Declare Function Acm_AxGetState Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef State As Integer) As Long
Declare Function Acm_AxGetMotionIO Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef Status As Long) As Long
Declare Function Acm_AxGetMotionStatus Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef Status As Long) As Long
Declare Function Acm_GetLastError Lib "ADVMOT.dll" (ByVal Handle As Long) As Long
Declare Function Acm_AxGetCmdPosition Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef Position As Double) As Long
Declare Function Acm_AxSetCmdPosition Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal Position As Double) As Long
Declare Function Acm_AxGetActualPosition Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef Position As Double) As Long
Declare Function Acm_AxSetActualPosition Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal Position As Double) As Long
Declare Function Acm_AxGetCmdVelocity Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef Velocity As Double) As Long
Declare Function Acm_AxGetLagCounter Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef Position As Double) As Long
Declare Function Acm_AxGetLatchData Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef Position As Double) As Long
Declare Function Acm_AxStartSoftLatch Lib "ADVMOT.dll" (ByVal AxisHandle As Long) As Long
Declare Function Acm_AxSetExtDrive Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal ExtDrvMode As Integer) As Long
Declare Function Acm_AxDoSetBit Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal DoChannel As Integer, ByVal BitData As Byte) As Long
Declare Function Acm_AxDoGetBit Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal DoChannel As Integer, ByRef BitData As Byte) As Long
Declare Function Acm_AxDiGetBit Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal DiChannel As Integer, ByRef BitData As Byte) As Long
Declare Function Acm_AxSimStartSuspendVel Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal DriDir As Integer) As Long
Declare Function Acm_AxSimStartSuspendRel Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal Distance As Double) As Long
Declare Function Acm_AxSimStartSuspendAbs Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal endPoint As Double) As Long
Declare Function Acm_AxSimStart Lib "ADVMOT.dll" (ByVal AxisHandle As Long) As Long
Declare Function Acm_AxSimStop Lib "AVMOT.dll" (ByVal AxisHandle As Long) As Long

'Group operation
Declare Function Acm_GpAddAxis Lib "ADVMOT.dll" (ByRef GpHandle As Long, ByVal AxHandle As Long) As Long
Declare Function Acm_GpRemAxis Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByVal AxisHandle As Long) As Long
Declare Function Acm_GpClose Lib "ADVMOT.dll" (ByRef GroupHandle As Long) As Long
Declare Function Acm_GpGetState Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef State As Integer) As Long
Declare Function Acm_GpResetError Lib "ADVMOT.dll" (ByVal GroupHandle As Long) As Long

Declare Function Acm_GpGetCmdVel Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef CmdVel As Double) As Long
Declare Function Acm_GpMoveLinearRel Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef DistanceArray As Double, ByRef ArrayElements As Long) As Long
Declare Function Acm_GpMoveLinearAbs Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef PositionArray As Double, ByRef ArrayElements As Long) As Long
Declare Function Acm_GpMoveDirectRel Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef DistanceArray As Double, ByRef ArrayElements As Long) As Long
Declare Function Acm_GpMoveDirectAbs Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef PositionArray As Double, ByRef ArrayElements As Long) As Long
Declare Function Acm_GpMoveCircularRel Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef CenterArray As Double, ByRef EndArray As Double, _
                ByRef ArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMoveCircularAbs Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef CenterArray As Double, ByRef EndArray As Double, _
                ByRef ArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMoveCircularRel_3P Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef RefArray As Double, ByRef EndArray As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMoveCircularAbs_3P Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef RefArray As Double, ByRef EndArray As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long


Declare Function Acm_GpMoveCircularRel_Angle Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef RefArray As Double, ByVal ArcAngle As Integer, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMoveArcRel_Angle Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef RefArray As Double, ByVal ArcAngle As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMoveCircularAbs_Angle Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef RefArray As Double, ByVal ArcAngle As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMoveArcAbs_Angle Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef RefArray As Double, ByVal ArcAngle As Integer, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMove3DArcRel Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef CenterArray As Double, ByRef EndArray As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
Declare Function Acm_GpMove3DArcAbs Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                ByRef CenterArray As Double, ByRef NVectorArray As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long

Declare Function Acm_GpMove3DArcRel_V Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                 ByRef CenterArray As Double, ByRef EndArray As Double, ByVal ArcAngle As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
                
Declare Function Acm_GpMove3DArcAbs_V Lib "ADVMOT.dll" (ByVal GroupHandle As Long, _
                 ByRef CenterArray As Double, ByRef NVectorArray As Double, ByVal ArcAngle As Double, ByRef pArrayElements As Long, ByVal Direction As Integer) As Long
                
Declare Function Acm_GpLoadPath Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByVal FilePath As String, ByRef PathHandle As Long, ByRef pTotalCount As Long) As Long
Declare Function Acm_GpUnloadPath Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef PathHandle As Long) As Long
Declare Function Acm_GpMovePath Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByVal PathHandle As Long) As Long
Declare Function Acm_GpAddPath Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByVal MoveCmd As Integer, ByVal MoveMode As Integer, ByVal FH As Double, ByVal FL As Double, _
                   ByRef EndPoint_DataArray As Double, ByRef CenPoint_DataArray As Double, ByRef ArrayElements As Long) As Long
Declare Function Acm_GpResetPath Lib "ADVMOT.dll" (ByRef GroupHandle As Long) As Long
Declare Function Acm_GpGetPathStatus Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByRef pCurIndex As Long, _
                         ByRef pCurCmdFunc As Long, ByRef pRemainCount As Long, ByRef pFreeSpaceCount As Long) As Long
Declare Function Acm_GpStopDec Lib "ADVMOT.dll" (ByVal GroupHandle As Long) As Long
Declare Function Acm_GpStopEmg Lib "ADVMOT.dll" (ByVal GroupHandle As Long) As Long
Declare Function Acm_GpChangeVel Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByVal newVelocity As Double) As Long
Declare Function Acm_GpChangeVelByRate Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByVal Rate As Long) As Long

Declare Function Acm_GpMoveSelPath Lib "ADVMOT.dll" (ByVal GroupHandle As Long, ByVal PathHandle As Long, ByVal StartIndex As Long, ByVal EndIndex As Long, ByVal Repeat As Long) As Long
' DIO
Declare Function Acm_DaqDiGetByte Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal DiPort As Integer, ByRef ByteData As Byte) As Long
Declare Function Acm_DaqDiGetBit Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal DiChannel As Integer, ByRef BitData As Byte) As Long
Declare Function Acm_DaqDoSetByte Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal DoPort As Integer, ByVal ByteData As Byte) As Long
Declare Function Acm_DaqDoSetBit Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal DoChannel As Integer, ByVal BitData As Byte) As Long
Declare Function Acm_DaqDoGetByte Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal DoPort As Integer, ByRef ByteData As Byte) As Long
Declare Function Acm_DaqDoGetBit Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal DoChannel As Integer, ByRef BitData As Byte) As Long

'mining: Compare functions
Declare Function Acm_AxSetCmpAuto Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal Start As Double, ByVal endPoint As Double, ByVal Interval As Double) As Long
Declare Function Acm_AxGetCmpData Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef CmpPosition As Double) As Long
Declare Function Acm_AxSetCmpData Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByVal CmpPosition As Double) As Long
Declare Function Acm_AxSetCmpTable Lib "ADVMOT.dll" (ByVal AxisHandle As Long, ByRef TableArray As Double, ByVal ArrayCount As Long) As Long

'DUJUNLING 2012.11.25
Declare Function Acm_GmOpen Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByRef GMSHandle As Long) As Long
Declare Function Acm_GmClose Lib "ADVMOT.dll" (ByVal GMSHandle As Long) As Long

Declare Function Acm_GmLoadJob Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByVal JobFilePath As String, ByRef ErrorRow As Long) As Long
Declare Function Acm_GmUploadJob Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByRef GCodeBuffer As String, ByRef GCodeLength As Long) As Long

Declare Function Acm_GmCommand Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByVal InputCmd As String) As Long
Declare Function Acm_GmGetCurrentRow Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByRef CurrentRow As Long) As Long
Declare Function Acm_GmGetCurrentCommand Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByRef CurrentCmd As String, ByRef CurrentCmdLength As Long) As Long
Declare Function Acm_GmGetState Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByRef GmsState As Long) As Long
Declare Function Acm_GmGetPattern Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByVal GmsPatternId As Long, ByRef Buffer As Any, ByRef BufferLength As Long) As Long
Declare Function Acm_GmGetError Lib "ADVMOT.dll" (ByVal GMSHandle As Long, ByRef ErrorCode As Long, ByRef LastError As Long) As Long

'Add by yanli.zhu for ADAM5000 15.01.27

Declare Function Acm_LoadENI Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal FilePath As String) As Long
Declare Function Acm_GetChannelProperty Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ChannelID As Long, ByVal ProperyID As Long, ByRef Value As Double) As Long
Declare Function Acm_SetChannelProperty Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal ChannelID As Long, ByVal ProperyID As Long, ByVal Value As Double) As Long
Declare Function Acm_DaqAiGetCurrData Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AiChannel As Integer, ByRef AiData As Single) As Long
Declare Function Acm_DaqAiGetVoltData Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AiChannel As Integer, ByRef AiData As Single) As Long
                         
Declare Function Acm_DaqAiSpanCalibration Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AiChannel As Integer) As Long
Declare Function Acm_DaqAiZeroCalibration Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AiChannel As Integer) As Long
Declare Function Acm_DaqAoGetRawData Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AoChannel As Integer, ByRef AoData As Integer) As Long
Declare Function Acm_DaqAoGetCurrData Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AoChannel As Integer, ByRef AoData As Single) As Long
Declare Function Acm_DaqAoGetVoltData Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AoChannel As Integer, ByRef AoData As Single) As Long
Declare Function Acm_DaqAoSetCurrData Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AoChannel As Integer, ByVal AoData As Single) As Long
Declare Function Acm_DaqAoSetVoltData Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AoChannel As Integer, ByVal AoData As Single) As Long
Declare Function Acm_DaqAoCaliDone Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AoChannel As Integer) As Long
Declare Function Acm_DaqAoSetCaliValue Lib "ADVMOT.dll" (ByVal DeviceHandle As Long, ByVal AoChannel As Integer, ByVal CaliData As Integer) As Long



