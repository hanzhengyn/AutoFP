
#ifndef __REGISTRY_H__
#define __REGISTRY_H__

//REG_SERVICE_ADVCOMMOTION_KEY & REG_SERVICE_ADVANTECH are used by SYS driver
#define REG_SERVICE_ADVCOMMOTION_KEY		\
         L"\\REGISTRY\\MACHINE\\SYSTEM\\CurrentControlSet\\Services\\ADVANTECH\\AdvMotion Device\\"

#define REG_SERVICE_ADVANTECH		\
         L"\\REGISTRY\\MACHINE\\SYSTEM\\CurrentControlSet\\Services\\ADVANTECH\\"

//REG_SERVICE_ADVMOT_KEY is used by device dll
#define  REG_SERVICE_ADVMOT_KEY    "SYSTEM\\CurrentControlSet\\Services\\ADVANTECH\\AdvMotion Device\\"

#define  REG_DEV_NAME         	"DevName"
#define  REG_GUID_NAME         	"GUID"
#define  REG_SLOT_NUM         	"SlotNumber"
#define  REG_IO_BASE          	"IOBase"
#define  REG_IO_LEN           	"IOLen"
#define  REG_IRQ              	"Irq"
#define  REG_DEV_NUM          	"DevNumber"
#define  REG_SYS_NAME         	"SYS_NAME"
#define  REG_DLL_NAME         	"DLL_NAME"

//Add for other Corp. [dan.yang 2011.06.08]
#define REG_FLAG_DEVTYPE_KEY   "SYSTEM\\CurrentControlSet\\Services\\ADVANTECH\\AdvMotion Device\\OEM" //YDD 2015.4.10
#define REG_FLAG                "OEM"
#define OEM_1245				"PCI-1245"  //YDD 2015.4.10
#define OEM_1240				"PCI-1240"
#define OEM_1245HP				"PCI-1245HP"  //YDD 2015.4.10
//#define  REG_INSTANCE_NAME       _T("InstanceName")

#endif // __REGISTRY_H__
