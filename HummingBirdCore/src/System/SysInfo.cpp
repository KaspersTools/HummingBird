//
// Created by Kasper de Bruin on 27/12/2023.
//

//
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/ps/IOPSKeys.h>
#include <IOKit/ps/IOPowerSources.h>

//std
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

//disk
#include <sys/mount.h>
#include <sys/param.h>

//network
#include <SystemConfiguration/SystemConfiguration.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>


#include <SystemConfiguration/SystemConfiguration.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <iostream>
#include <map>
#include <net/if.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#include "SysInfo.h"

namespace HummingbirdCore {
  namespace System {
    void SysInfo::open() {
      // get the smc service
      io_service_t service = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching("AppleSMC"));
      if (service == 0) {
        throw std::runtime_error("Unable to find the SMC service.");
      }

      kern_return_t openingResult = IOServiceOpen(service, mach_task_self_, 0, &connectionHandle);
      if (openingResult != kIOReturnSuccess) {
        throw std::runtime_error("Unable to connect to the SMC.");
      }

      IOObjectRelease(service);
    }

    void SysInfo::close() {
      kern_return_t closeResult = IOServiceClose(connectionHandle);
      if (closeResult != kIOReturnSuccess) {
        throw std::runtime_error("Failed to close the connection to the SMC!");
      }
    }

    DataType SysInfo::getKeyInfo(std::string keyString) {
      // create a new smc struct and set the key and data fields
      SMCParamStruct inputStruct = SMCParamStruct();
      inputStruct.key = HummingBirdCore::Utils::stringToFourCharCode(keyString);
      inputStruct.data8 = kSMCGetKeyInfo;

      SMCParamStruct readResult = callSMC(inputStruct);

      return {readResult.keyInfo.dataType, readResult.keyInfo.dataSize};
    }

    SMCParamStruct SysInfo::callSMC(SMCParamStruct givenStruct,
                                    SMCSelector smcSelector) {
      // create an output struct to save the result
      SMCParamStruct outputStruct = SMCParamStruct();
      size_t outputStructSize = sizeof(outputStruct);
      size_t inputStructSize = sizeof(givenStruct);

      // make the call to the SMC
      kern_return_t smcCallResult = IOConnectCallStructMethod(connectionHandle,
                                                              smcSelector,
                                                              &givenStruct,
                                                              inputStructSize,
                                                              &outputStruct,
                                                              &outputStructSize);

      if (smcCallResult == kIOReturnSuccess && outputStruct.result == kSMCSuccess) {
        return outputStruct;
      } else if (smcCallResult == kIOReturnSuccess && outputStruct.result == kSMCKeyNotFound) {
        throw std::runtime_error("Given SMC-Key was not found!");
      } else if (smcCallResult == kIOReturnNotPrivileged) {
        throw std::runtime_error("Reading this key requires root privileges!");
      } else {
        throw std::runtime_error("An unknown error occurred while reading the SMC-Key!");
      }
    }

    void SysInfo::readKey(smc_key_t smcKey, SMCBytes &result) {
      SMCParamStruct inputStruct = SMCParamStruct();

      inputStruct.key = smcKey.code;
      inputStruct.keyInfo.dataSize = (UInt32) smcKey.info.size;
      inputStruct.data8 = kSMCReadKey;

      SMCParamStruct resultStruct = callSMC(inputStruct);
      std::copy(std::begin(resultStruct.bytes), std::end(resultStruct.bytes), std::begin(result));
    }

    void SysInfo::readKey(const std::string &keyCode, DataType typeInfo, SMCBytes &resultArray) {
      SMCKey key = SMCKey(keyCode, typeInfo);
      readKey(key, resultArray);
    }

    int SysInfo::getCpuTemp() {
      SMCBytes readResult = {0};
      readKey("TC0P", types.SP78, readResult);

      return (unsigned int) readResult[0];
    }

    std::vector<float> SysInfo::getMemoryUsage() {
      mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
      mach_port_t host = mach_host_self();
      vm_statistics64_data_t stats;

      kern_return_t result = host_statistics64(host, HOST_VM_INFO64, (host_info64_t) &stats, &count);

      if (result != KERN_SUCCESS) {
        throw std::runtime_error("An error occured while getting the memory usage.");
      }

      vm_size_t page_size = vm_kernel_page_size;
      // divide by 1_073_741_824 to get gigabyte
      float freeMem = (double) (stats.free_count * page_size) / (double) 1073741824;
      float activeMem = (double) (stats.active_count * page_size) / (double) 1073741824;
      float inactiveMem = (double) (stats.inactive_count * page_size) / (double) 1073741824;
      float wiredMem = (double) (stats.wire_count * page_size) / (double) 1073741824;
      float compressedMem = (double) (stats.compressor_page_count * page_size) / (double) 1073741824;

      std::vector<float> resultVec;
      resultVec.push_back(freeMem);
      resultVec.push_back(activeMem);
      resultVec.push_back(inactiveMem);
      resultVec.push_back(wiredMem);
      resultVec.push_back(compressedMem);

      return resultVec;
    }

    float SysInfo::getTotalMemory() {
      mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;
      mach_port_t host = mach_host_self();
      host_basic_info_data_t info;

      kern_return_t result = host_info(host, HOST_BASIC_INFO, (host_info_t) &info, &count);

      if (result != KERN_SUCCESS) {
        throw std::runtime_error("An error occured while getting the total memory of the machine.");
      }

      return (double) info.max_mem / (double) 1073741824;
    }

    std::vector<float> SysInfo::getCpuUsage() {
      cpu_tick_t curCpuTicks = getCpuLoadInfo();

      float userDiff = curCpuTicks.userTicks - prevCpuTicks.userTicks;
      float sysDiff = curCpuTicks.systemTicks - prevCpuTicks.systemTicks;
      float idleDiff = curCpuTicks.idleTicks - prevCpuTicks.idleTicks;
      float niceDiff = curCpuTicks.niceTicks - prevCpuTicks.niceTicks;

      float curTotalTicks = userDiff + sysDiff + idleDiff + niceDiff;

      std::vector<float> resultVec;
      resultVec.push_back(userDiff / curTotalTicks * 100);
      resultVec.push_back(sysDiff / curTotalTicks * 100);
      resultVec.push_back(idleDiff / curTotalTicks * 100);
      resultVec.push_back(niceDiff / curTotalTicks * 100);

      prevCpuTicks = curCpuTicks;

      return resultVec;
    }

    int SysInfo::getFanCount() {
      SMCBytes readResult = {0};
      readKey("FNum", types.UInt8, readResult);

      return (unsigned int) readResult[0];
    }

    int SysInfo::getMinFanSpeed(int fanID) {
      SMCBytes readResult = {0};
      std::string keyString = "F" + std::to_string(fanID) + "Mn";

      try {
        // try to raed using the fpe2 type. Does not work for newer Macbooks.
        readKey(keyString, types.FPE2, readResult);
        UInt8 fpeValue[2] = {readResult[0], readResult[1]};
        return HummingBirdCore::Utils::fpe2ToInt(fpeValue);
      } catch (const std::runtime_error &e) {
        // if reading the data using fpe2 fails try using flt type
        readKey(keyString, types.FLT, readResult);
        UInt8 fltValue[4] = {readResult[0], readResult[1], readResult[2], readResult[3]};
        return HummingBirdCore::Utils::fltToInt(fltValue);
      }
    }

    int SysInfo::getMaxFanSpeed(int fanID) {
      SMCBytes readResult = {0};
      std::string keyString = "F" + std::to_string(fanID) + "Mx";

      try {
        // try to raed using the fpe2 type. Does not work for newer Macbooks.
        readKey(keyString, types.FPE2, readResult);
        UInt8 fpeValue[2] = {readResult[0], readResult[1]};
        return HummingBirdCore::Utils::fpe2ToInt(fpeValue);
      } catch (const std::runtime_error &e) {
        // if reading the data using fpe2 fails try using flt type
        readKey(keyString, types.FLT, readResult);
        UInt8 fltValue[4] = {readResult[0], readResult[1], readResult[2], readResult[3]};
        return HummingBirdCore::Utils::fltToInt(fltValue);
      }
    }

    int SysInfo::getCurrentFanSpeed(int fanID) {
      SMCBytes readResult = {0};
      std::string keyString = "F" + std::to_string(fanID) + "Ac";

      try {
        // try to raed using the fpe2 type. Does not work for newer Macbooks.
        readKey(keyString, types.FPE2, readResult);
        UInt8 fpeValue[2] = {readResult[0], readResult[1]};
        return HummingBirdCore::Utils::fpe2ToInt(fpeValue);
      } catch (const std::runtime_error &e) {
        // if reading the data using fpe2 fails try using flt type
        readKey(keyString, types.FLT, readResult);
        UInt8 fltValue[4] = {readResult[0], readResult[1], readResult[2], readResult[3]};
        return HummingBirdCore::Utils::fltToInt(fltValue);
      }
    }

    int SysInfo::getBatteryCount() {
      SMCBytes readResults = {0};
      readKey("BNum", types.UInt8, readResults);

      return (unsigned int) readResults[0];
    }

    bool SysInfo::isChargingBattery() {
      SMCBytes readResults = {0};
      readKey("BSIn", types.UInt8, readResults);

      return (bool) (readResults[0] & (unsigned) 1);
    }

    float SysInfo::getBatteryHealth() {
      // get the
      io_registry_entry_t batteryRegistry = IORegistryEntryFromPath(kIOMainPortDefault, "IOService:/AppleACPIPlatformExpert/SMB0/AppleECSMBusController/AppleSmartBatteryManager/AppleSmartBattery");
      CFMutableDictionaryRef psDict;
      if (IORegistryEntryCreateCFProperties(batteryRegistry, &psDict, kCFAllocatorDefault, 0)) {
        throw std::runtime_error("Something went wrong while reading the battery health.");
      }

      // get the design capacity
      SInt32 designCapacity;
      CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(psDict, CFSTR(kIOPSDesignCapacityKey)), (CFNumberType) 3, &designCapacity);
      // get the current max capacity
      SInt32 maxCapacity;
      CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(psDict, CFSTR("MaxCapacity")), (CFNumberType) 3, &maxCapacity);

      return (float) maxCapacity / (float) designCapacity;
    }

    int SysInfo::getBatteryCycles() {
      // get the
      io_registry_entry_t batteryRegistry = IORegistryEntryFromPath(kIOMainPortDefault, "IOService:/AppleACPIPlatformExpert/SMB0/AppleECSMBusController/AppleSmartBatteryManager/AppleSmartBattery");
      CFMutableDictionaryRef psDict;
      if (IORegistryEntryCreateCFProperties(batteryRegistry, &psDict, kCFAllocatorDefault, 0)) {
        throw std::runtime_error("Something went wrong while reading the battery cycles.");
      }

      SInt32 cycles;
      CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(psDict, CFSTR("CycleCount")), (CFNumberType) 3, &cycles);

      return cycles;
    }

    ///OWN FUNCTIONS
    //Battery
    int SysInfo::getBatteryCharge() {
      CFTypeRef blob = IOPSCopyPowerSourcesInfo();
      CFArrayRef sources = IOPSCopyPowerSourcesList(blob);
      CFDictionaryRef pSource = NULL;
      const void *psValue;
      int numOfSources = CFArrayGetCount(sources);
      for (int i = 0; i < numOfSources; i++) {
        pSource = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sources, i));
        if (!pSource) {
          std::cout << "Error getting battery info" << std::endl;
          return -1;
        }
        psValue = (CFStringRef) CFDictionaryGetValue(pSource, CFSTR(kIOPSNameKey));
        int curCapacity = 0;
        int maxCapacity = 0;
        int percent;
        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSCurrentCapacityKey));
        CFNumberGetValue((CFNumberRef) psValue, kCFNumberSInt32Type, &curCapacity);
        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSMaxCapacityKey));
        CFNumberGetValue((CFNumberRef) psValue, kCFNumberSInt32Type, &maxCapacity);
        percent = (int) ((double) curCapacity / (double) maxCapacity * 100);
        return percent;
      }
      return -1;
    }

    //Get Battery time

    //Disk data
    int SysInfo::getDiskCount() {
      int count = 0;
      struct statfs *mounts;

      int numMounts = getmntinfo(&mounts, MNT_NOWAIT);
      count = numMounts;

      return count;
    }

    std::vector<std::string> SysInfo::getDiskNames() {
      std::vector<std::string> names;
      struct statfs *mounts;
      int numMounts = getmntinfo(&mounts, MNT_NOWAIT);

      for (int i = 0; i < numMounts; ++i) {
        names.push_back(mounts[i].f_mntfromname);
      }

      return names;
    }

    std::vector<float> SysInfo::getDiskSpaceTotal() {
      std::vector<float> totals;
      struct statfs *mounts;
      int numMounts = getmntinfo(&mounts, MNT_NOWAIT);

      for (int i = 0; i < numMounts; ++i) {
        float total = (float) mounts[i].f_blocks * mounts[i].f_bsize / 1073741824.0;// Convert to GB
        totals.push_back(total);
      }

      return totals;
    }

    std::vector<float> SysInfo::getDiskSpaceUsed() {
      std::vector<float> used;
      struct statfs *mounts;
      int numMounts = getmntinfo(&mounts, MNT_NOWAIT);

      for (int i = 0; i < numMounts; ++i) {
        float usedSpace = ((float) mounts[i].f_blocks - mounts[i].f_bfree) * mounts[i].f_bsize / 1073741824.0;// Convert to GB
        used.push_back(usedSpace);
      }

      return used;
    }

    std::vector<float> SysInfo::getDiskSpaceFree() {
      std::vector<float> frees;
      struct statfs *mounts;
      int numMounts = getmntinfo(&mounts, MNT_NOWAIT);

      for (int i = 0; i < numMounts; ++i) {
        float free = (float) mounts[i].f_bfree * mounts[i].f_bsize / 1073741824.0;// Convert to GB
        frees.push_back(free);
      }

      return frees;
    }

    //Network
    std::vector<network_data_t> SysInfo::getNetworkInterfaces() {
      std::vector<network_data_t> networkDataList;
      struct ifaddrs *interfaces = nullptr;

      if (getifaddrs(&interfaces) == -1) {
        std::cerr << "Failed to get network interfaces" << std::endl;
        return {};
      }

      std::map<std::string, network_data_t> interfaceMap;


      for (struct ifaddrs *interface = interfaces; interface != NULL; interface = interface->ifa_next) {
        std::string interfaceName = interface->ifa_name;

        if (interfaceMap.find(interfaceName) == interfaceMap.end()) {
          network_data_t data;
          interfaceMap[interfaceName] = data;
        }

        network_data_t &data = interfaceMap[interfaceName];
        data.name = interfaceName;

        if (interface->ifa_addr != NULL) {
          int family = interface->ifa_addr->sa_family;
          //IPV4
          if (family == AF_INET) {
            data.ipv4 = inet_ntoa(((struct sockaddr_in *) interface->ifa_addr)->sin_addr);
          }
          //IPV6
          if (family == AF_INET6) {
            char ipv6Address[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &((struct sockaddr_in6 *) interface->ifa_addr)->sin6_addr, ipv6Address, sizeof(ipv6Address));
            data.ipv6 = ipv6Address;
          }
          //MAC ADDRESS
          if (family == AF_LINK) {
            struct sockaddr_dl *sdl = (struct sockaddr_dl *) interface->ifa_addr;
            unsigned char *mac = (unsigned char *) LLADDR(sdl);
            char buf[18];
            snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            data.mac = buf;
          }
          //NETMASK ipv4
          if (interface->ifa_netmask != NULL && interface->ifa_addr->sa_family == AF_INET) {// IPv4
            data.netmaskipv4 = inet_ntoa(((struct sockaddr_in *) interface->ifa_netmask)->sin_addr);
          }
          //NETMASK ipv6
          if (interface->ifa_netmask != NULL && interface->ifa_addr->sa_family == AF_INET6) {// IPv6
            char ipv6Address[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &((struct sockaddr_in6 *) interface->ifa_netmask)->sin6_addr, ipv6Address, sizeof(ipv6Address));
            data.netmaskipv6 = ipv6Address;
          }
          //BROADCAST ipv4
          if (interface->ifa_dstaddr != NULL && interface->ifa_addr->sa_family == AF_INET) {// IPv4
            data.broadcastipv4 = inet_ntoa(((struct sockaddr_in *) interface->ifa_dstaddr)->sin_addr);
          }
          //BROADCAST ipv6
          if (interface->ifa_dstaddr != NULL && interface->ifa_addr->sa_family == AF_INET6) {// IPv6
            char ipv6Address[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &((struct sockaddr_in6 *) interface->ifa_dstaddr)->sin6_addr, ipv6Address, sizeof(ipv6Address));
            data.broadcastipv6 = ipv6Address;
          }
          //gateway
          data.gateway = "not implemented";
          //status
          data.status = (interface->ifa_flags & IFF_UP) ? "up" : "down";
          //interface type
          data.interfaceType = "not implemented";
          //mtu
          data.mtu = -1;
          //interfaceindex
          data.interfaceIndex = if_nametoindex(interface->ifa_name);
          //speed
          data.speed = -1;
          //dns servers
          std::ifstream resolvConf("/etc/resolv.conf");
          if (!resolvConf.is_open()) {
            data.dnsServers = {};
            CORE_ERROR("Error opening /etc/resolv.conf for reading dns servers. This file is autogenerated by mac so if its not available there is something wrong");
          } else {
            std::string dnsServerLine;
            while (getline(resolvConf, dnsServerLine)) {
              std::istringstream iss(dnsServerLine);
              std::string token;
              iss >> token;
              if (token == "nameserver") {

//                std::string ns;
//                iss >> ns;
//                data.dnsServers.push_back(ns);
              }
            }
          }
          //using dhcp
          data.usingDhcp = false;
        }
      }


      // Convert map to vector
      for (auto &entry: interfaceMap) {
        networkDataList.push_back(entry.second);
      }

      freeifaddrs(interfaces);
      return networkDataList;
      //      std::vector<network_data_t> networkDataList{};
      //
      //      // Get list of network interfaces from SystemConfiguration
      //      CFArrayRef scInterfaces = SCNetworkInterfaceCopyAll();
      //      std::map<std::string, std::string> scInterfaceTypes;
      //
      //      for (CFIndex i = 0; i < CFArrayGetCount(scInterfaces); i++) {
      //        SCNetworkInterfaceRef scInterface = (SCNetworkInterfaceRef) CFArrayGetValueAtIndex(scInterfaces, i);
      //        CFStringRef scInterfaceType = SCNetworkInterfaceGetInterfaceType(scInterface);
      //        CFStringRef bsdName = SCNetworkInterfaceGetBSDName(scInterface);
      //
      //        if (bsdName != NULL) {
      //          char bsdNameBuffer[256];
      //          char scInterfaceTypeBuffer[256];
      //
      //          if (CFStringGetCString(bsdName, bsdNameBuffer, sizeof(bsdNameBuffer), kCFStringEncodingUTF8) &&
      //              CFStringGetCString(scInterfaceType, scInterfaceTypeBuffer, sizeof(scInterfaceTypeBuffer), kCFStringEncodingUTF8)) {
      //            scInterfaceTypes[std::string(bsdNameBuffer)] = std::string(scInterfaceTypeBuffer);
      //          }
      //        }
      //      }
      //
      //      // Get list of network interfaces
      //      struct ifaddrs *interfaces = nullptr;
      //      if (getifaddrs(&interfaces) == -1) {
      //        std::cerr << "Failed to get network interfaces" << std::endl;
      //        return {};
      //      }
      //      for (struct ifaddrs *interface = interfaces; interface != NULL; interface = interface->ifa_next) {
      //        network_data_t data;
      //        //name
      //        data.name = interface->ifa_name;
      //
      //        //ip
      //        if (interface->ifa_addr != NULL && interface->ifa_addr->sa_family == AF_INET) { // IPv4
      //          data.ip = inet_ntoa(((struct sockaddr_in *)interface->ifa_addr)->sin_addr);
      //        }else{
      //          data.ip = "not ipv4. ipv6 not implemented";
      //        }
      //        //mac
      //        if (interface->ifa_addr != NULL && interface->ifa_addr->sa_family == AF_LINK) { // MAC
      //          struct sockaddr_dl* sdl = (struct sockaddr_dl *)interface->ifa_addr;
      //          unsigned char* mac = (unsigned char*) LLADDR(sdl);
      //          char buf[18];
      //          snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
      //                   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      //          data.mac = buf;
      //        }else{
      //          data.mac = "not a mac address";
      //        }
      //        //netmask
      //        if (interface->ifa_netmask != NULL && interface->ifa_addr->sa_family == AF_INET) { // IPv4
      //          data.netmask = inet_ntoa(((struct sockaddr_in *)interface->ifa_netmask)->sin_addr);
      //        }else{
      //          data.netmask = "not ipv4 for netmask. ipv6 not implemented";
      //        }
      //        //broadcast
      //        if (interface->ifa_dstaddr != NULL && interface->ifa_addr->sa_family == AF_INET) { // IPv4
      //          data.broadcast = inet_ntoa(((struct sockaddr_in *)interface->ifa_dstaddr)->sin_addr);
      //        }else{
      //          data.broadcast = "not ipv4 for broadcast. ipv6 not implemented";
      //        }
      //        //gateway
      //        data.gateway = "not implemented";
      //        //mtu
      //        data.mtu = -1;
      //        //type
      //        auto it = scInterfaceTypes.find(data.name);
      //        if (it != scInterfaceTypes.end()) {
      //          data.interfaceType = it->second;
      //        }else{
      //          data.interfaceType = "unkown";
      //          CORE_WARN("Error getting interface type for interface: {}", data.name);
      //        }
      //        //status
      //        data.status = (interface->ifa_flags & IFF_UP) ? "up" : "down";
      //        //speed
      //        data.speed = -1;
      //        //dns servers
      //        std::ifstream resolvConf("/etc/resolv.conf");
      //        if (!resolvConf.is_open()) {
      //          data.dnsServers = {};
      //          CORE_ERROR("Error opening /etc/resolv.conf for reading dns servers. This file is autogenerated by mac so if its not available there is something wrong");
      //        } else {
      //          std::string dnsServerLine;
      //          while (getline(resolvConf, dnsServerLine)) {
      //            std::istringstream iss(dnsServerLine);
      //            std::string token;
      //            iss >> token;
      //            if (token == "nameserver") {
      //              std::string ns;
      //              iss >> ns;
      //              data.dnsServers.push_back(ns);
      //            }
      //          }
      //        }
      //        resolvConf.close();
      //        //interface index
      //        data.interfaceIndex = if_nametoindex(interface->ifa_name);
      //        networkDataList.push_back(data);
      //      }
      //
      //      CFRelease(scInterfaces);
      //      freeifaddrs(interfaces);
      //      return networkDataList;
      //CHECK IG
      //
      //        //Name
      //        network_data_t data;
      //        data.name = ifa->ifa_name;
      //
      //        // IPV4 Address
      //        if (ifa->ifa_addr->sa_family == AF_INET) {
      //          // IP Address
      //          char ip[INET_ADDRSTRLEN];
      //          inet_ntop(AF_INET, &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr, ip, INET_ADDRSTRLEN);
      //          data.ip = ip;
      //
      //          // Netmask
      //          char netmask[INET_ADDRSTRLEN];
      //          inet_ntop(AF_INET, &((struct sockaddr_in *) ifa->ifa_netmask)->sin_addr, netmask, INET_ADDRSTRLEN);
      //          data.netmask = netmask;
      //
      //          // Broadcast Address
      //          if (ifa->ifa_flags & IFF_BROADCAST) {
      //            char broadcast[INET_ADDRSTRLEN];
      //            inet_ntop(AF_INET, &((struct sockaddr_in *) ifa->ifa_dstaddr)->sin_addr, broadcast, INET_ADDRSTRLEN);
      //            data.broadcast = broadcast;
      //          }
      //        }
      //        // MAC Address
      //        else if (ifa->ifa_addr->sa_family == AF_LINK) {
      //          struct sockaddr_dl *sdl = (struct sockaddr_dl *) ifa->ifa_addr;
      //          if (sdl->sdl_type == IFT_ETHER) {
      //            unsigned char *mac = (unsigned char *) LLADDR(sdl);
      //            char buf[18];
      //            snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
      //                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      //            data.mac = buf;
      //          }
      //        }
      //        data.gateway = "";
      //
      //        data.mtu = 0;
      //
      //        data.status = (ifa->ifa_flags & IFF_UP) ? "up" : "down";
      //        auto it = scInterfaceTypes.find(data.name);
      //        if (it != scInterfaceTypes.end()) {
      //          data.interfaceType = it->second;
      //        } else {
      //          data.interfaceType = "unkown";
      //          CORE_ERROR("Error getting interface type for interface: {}", data.name);
      //        }
      //
      //        data.speed = 0;
      //
      //        std::ifstream resolvConf("/etc/resolv.conf");
      //        if (!resolvConf.is_open()) {
      //          data.dnsServers = {};
      //          CORE_ERROR("Error opening /etc/resolv.conf");
      //        } else {
      //          std::string dnsServerLine;
      //          while (getline(resolvConf, dnsServerLine)) {
      //            std::istringstream iss(dnsServerLine);
      //            std::string token;
      //            iss >> token;
      //            if (token == "nameserver") {
      //              std::string ns;
      //              iss >> ns;
      //              data.dnsServers.push_back(ns);
      //            }
      //          }
      //        }
      //
      //        //interface index
      //        data.interfaceIndex = if_nametoindex(ifa->ifa_name);
      //
      //
      //        std::ifstream dhclientLeaseFile("/var/lib/dhcp/dhclient.leases");
      //        data.usingDhcp = dhclientLeaseFile.good() && (dhclientLeaseFile.peek() != std::ifstream::traits_type::eof());
      //        dhclientLeaseFile.close();
      //
      //        networkDataList.push_back(data);
      //      }
      //
      //      freeifaddrs(ifaddr);
      //      return networkDataList;
    }

    //Logs
    std::string SysInfo::getLogs() {
      std::string log = "";
      std::string line = "";
      std::ifstream file("/var/log/system.log");
      if (file.is_open()) {
        while (getline(file, line)) {
          log += line + "\n";
        }
        file.close();
      }

      return log;
    }

  }// namespace System
}// namespace HummingbirdCore