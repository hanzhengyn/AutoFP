Attribute VB_Name = "Module2"
Option Explicit

Global Const MAX_DEVICES = 100

Global m_DeviceNum As Long
Global m_Description As Long
Global m_GetModule As Boolean
'static HINSTANCE ghInstance;

Global dwDriverID As Long

Global Const REG_SERVICE_ADVMOT_KEY = "SYSTEM\CurrentControlSet\Services\ADVANTECH\AdvMotion Device\"

Global Const DEVICE_NUM_LEN = 3
Global Const MODULE_NUM_LEN = 3
Global Const MAX_COM_MODULES = 1023
Global Const MAX_CAN_MODULES = 1023
Global Const EXE_NAME_MAX_SIZE = 128
Global Const MAX_DLL_NUM = 30

Global szFileName As String         '// .INI full name
Global szDriverName As String        '// Driver name
Global szDeviceName As String                      '// Device name
Global szModuleName As String                      '// Module name
Global szEntry As String
Global szBuf As String

'---------------------------------------------------------------
'-   注册表   Api   常数...
'---------------------------------------------------------------
'   Reg   Data   Types...
Const REG_SZ = 1                                                         '   Unicode空终结字符串
Const REG_EXPAND_SZ = 2                                           '   Unicode空终结字符串
Const REG_DWORD = 4                                                   '   32-bit   数字

'   注册表创建类型值...
Const REG_OPTION_NON_VOLATILE = 0                     '   当系统重新启动时，关键字被保留

'   注册表关键字安全选项...
Const READ_CONTROL = &H20000
Const KEY_QUERY_VALUE = &H1
Const KEY_SET_VALUE = &H2
Const KEY_CREATE_SUB_KEY = &H4
Const KEY_ENUMERATE_SUB_KEYS = &H8
Const KEY_NOTIFY = &H10
Const KEY_CREATE_LINK = &H20
Const KEY_READ = KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + READ_CONTROL
Const KEY_WRITE = KEY_SET_VALUE + KEY_CREATE_SUB_KEY + READ_CONTROL
Const KEY_EXECUTE = KEY_READ
Const KEY_ALL_ACCESS = KEY_QUERY_VALUE + KEY_SET_VALUE + _
                                              KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + _
                                              KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
                                          
'   注册表关键字根类型...
Const HKEY_CLASSES_ROOT = &H80000000
Const HKEY_CURRENT_USER = &H80000001
Const HKEY_LOCAL_MACHINE = &H80000002
Const HKEY_USERS = &H80000003
Const HKEY_PERFORMANCE_DATA = &H80000004

'   返回值...
Const ERROR_NONE = 0
Const ERROR_BADKEY = 2
Const ERROR_ACCESS_DENIED = 8
Const ERROR_SUCCESS = 0

Type DEVLIST
    dwDeviceNum As Long
    szDeviceName(0 To 49) As Byte
    nNumOfSubdevices As Integer
End Type

Type FILETIME
    dwLowDateTime As Long
    dwHighDateTime As Long
End Type

'---------------------------------------------------------------
'-   注册表安全属性类型...
'---------------------------------------------------------------
Private Type SECURITY_ATTRIBUTES
        nLength   As Long
        lpSecurityDescriptor   As Long
        bInheritHandle   As Boolean
End Type

'---------------------------------------------------------------
'-注册表   API   声明...
'---------------------------------------------------------------
Declare Function RegCloseKey Lib "advapi32.dll" (ByVal hKey As Long) As Long

Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, _
            ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, _
            ByRef lpSecurityAttributes As SECURITY_ATTRIBUTES, ByRef phkResult As Long, ByRef lpdwDisposition As Long) As Long
            
Declare Function RegOpenKeyEx Lib "advapi32.dll" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, _
            ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
            
Declare Function RegQueryValueEx Lib "advapi32.dll" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, _
            ByVal lpReserved As Long, ByRef lpType As Long, ByVal lpData As String, ByRef lpcbData As Long) As Long
            
Declare Function RegSetValueEx Lib "advapi32.dll" Alias "RegSetValueExA" (ByVal hKey As Long, ByVal lpValueName As String, _
            ByVal Reserved As Long, ByVal dwType As Long, ByVal lpData As String, ByVal cbData As Long) As Long
            
'Declare Function RegEnumKeyEx Lib "advapi32.dll" Alias "RegEnumKeyExA" (ByVal hKey As Long, _
'            ByVal dwIndex As Long, ByVal lpName As String, lpcbName As Long, lpReserved As Long, ByVal lpClass As String, _
'            lpcbClass As Long, lpftLastWriteTime As FILETIME) As Long

Declare Function RegEnumKeyEx Lib "advapi32.dll" Alias "RegEnumKeyExA" (ByVal hKey As Long, _
            ByVal dwIndex As Long, ByVal lpName As String, lpcbName As Long, ByVal lpReserved As Long, _
            ByVal lpClass As String, lpcbClass As Long, lpftLastWriteTime As FILETIME) As Long
            
