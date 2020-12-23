#include <functional>
#ifdef _WIN32
    #include <windows.h>
    #include <cstdio>
    #include <set>
    #include <intrin.h>
#else
    #include <sys/utsname.h>
    #include <sys/sysinfo.h>
    #include "system/CommandExecutor.h"
    #include "util/TypeConverter.h"
#endif

#include "SystemInfo.h"
#include "util/StringUtil.h"

namespace urchin {

    std::string SystemInfo::retrieveOsInfo() const {
        #ifdef _WIN32
            unsigned long osVersion;
            NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
            OSVERSIONINFOEXW osInfo;
            *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
            if (RtlGetVersion != nullptr) {
                osInfo.dwOSVersionInfoSize = sizeof(osInfo);
                RtlGetVersion(&osInfo);
                osVersion = osInfo.dwMajorVersion;
                return "Windows " + std::to_string(osVersion);
            }
            return "Windows";
        #else
            struct utsname utsName = {};
            if (!uname(&utsName)) {
                return std::string(utsName.sysname) + ", " + std::string(utsName.release) + ", " + std::string(utsName.version);
            }
            return "Linux";
        #endif
    }

    unsigned int SystemInfo::retrieveCpuCores() const {
        #ifdef _WIN32
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return (unsigned int)sysInfo.dwNumberOfProcessors;
        #else
            std::string numCpuCores = CommandExecutor::instance()->execute(R"(grep --max-count=1 "cpu cores" /proc/cpuinfo | cut -d ":" -f 2)");
            return TypeConverter::toUnsignedInt(numCpuCores);
        #endif
    }

    uint64_t SystemInfo::retrieveTotalMemory() const {
        #ifdef _WIN32
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            if (GlobalMemoryStatusEx(&statex)) {
                return (uint64_t)statex.ullTotalPhys;
            }
            return 0L;
        #else
            struct sysinfo sysInfo = {};
            if (!sysinfo(&sysInfo)) {
                return (uint64_t)sysInfo.totalram;
            }
            return 0L;
        #endif
    }

    std::string SystemInfo::retrieveGraphicsCardNames() const {
        std::vector<std::string> graphicsCardNames;
        #ifdef _WIN32
            std::set<std::string> setGraphicsCardNames;
            for(int i = 0; i < 32; i++) {
                DISPLAY_DEVICE displayDevice = {};
                displayDevice.cb = sizeof(displayDevice);
                BOOL result = EnumDisplayDevices(nullptr, i, &displayDevice, EDD_GET_DEVICE_INTERFACE_NAME);
                if(!result) {
                    break;
                }
                setGraphicsCardNames.insert(std::string(displayDevice.DeviceString));
            }
            graphicsCardNames.insert(graphicsCardNames.begin(), setGraphicsCardNames.begin(), setGraphicsCardNames.end());
        #else
            std::string graphicCardNames = CommandExecutor::instance()->execute(R"(lspci | grep -oE "VGA.*" | cut -d ":" -f 2)");
            StringUtil::split(graphicCardNames, '\n', graphicsCardNames);
        #endif

        std::string graphicsCardNamesList;
        for(auto graphicsCardName : graphicsCardNames) {
            StringUtil::trim(graphicsCardName);
            graphicsCardNamesList += graphicsCardName + ", ";
        }
        if(graphicsCardNamesList.size() >= 2) {
            graphicsCardNamesList = graphicsCardNamesList.substr(0, graphicsCardNamesList.size() - 2);
        }
        return graphicsCardNamesList;
    }

    std::string SystemInfo::systemHash() const {
        return retrieveCpuHash() + "-" + std::to_string(std::hash<std::string>{}(retrieveGraphicsCardNames()));
    }

    std::string SystemInfo::retrieveCpuHash() const {
        #ifdef _WIN32
            int cpuInfo[4] = {0, 0, 0, 0};
            __cpuid(cpuInfo, 0);
            unsigned int hash = 0;
            auto* ptr = (uint16_t*)(&cpuInfo[0]);
            for (std::size_t i = 0; i < 8; i++) {
                hash += ptr[i];
            }
            return std::to_string(hash);
        #else
            unsigned int cpuInfo[4] = {0, 0, 0, 0};
            unsigned int ax = 0;
            __asm __volatile (
                "movl %%ebx, %%esi\n\t"
                "cpuid\n\t"
                "xchgl %%ebx, %%esi" :
                "=a" (cpuInfo[0]),
                "=S" (cpuInfo[1]),
                "=c" (cpuInfo[2]),
                "=d" (cpuInfo[3]) :
                "0" (ax)
            );

            unsigned int hash = 0;
            unsigned int* ptr = &cpuInfo[0];
            for (unsigned int i = 0; i < 4; i++) {
                hash += (ptr[i] & (unsigned int)0xFFFF) + (ptr[i] >> 16u);
            }
            return std::to_string(hash);
        #endif
    }

}