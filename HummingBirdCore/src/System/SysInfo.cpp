//
// Created by Kasper de Bruin on 27/12/2023.
//

//
#include <PCH/pch.h>
#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

//IOKit
#include <IOKit/IOKitLib.h>
#include <IOKit/ps/IOPSKeys.h>
#include <IOKit/ps/IOPowerSources.h>

//disk
#include <sys/mount.h>

//network
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <netinet/in.h>

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

      connectionHandle = 0;
    }

    DataType SysInfo::getKeyInfo(std::string keyString) {
      // create a new smc struct and set the key and data fields
      SMCParamStruct inputStruct = SMCParamStruct();
      inputStruct.key = HummingBirdCore::Utils::stringToFourCharCode(keyString);
      inputStruct.data8 = kSMCGetKeyInfo;

      SMCParamStruct readResult = callSMC(inputStruct);

      return {readResult.keyInfo.dataType, static_cast<UInt32>(readResult.keyInfo.dataSize)};
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
      CFRelease(psDict);

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
      CFRelease(psDict);
      return cycles;
    }

    ///OWN FUNCTIONS
    //Battery
    int SysInfo::getBatteryCharge() {
      CFTypeRef info = IOPSCopyPowerSourcesInfo();
      if (!info) {
        return -1;
      }

      CFArrayRef sources = IOPSCopyPowerSourcesList(info);
      if (!sources || CFArrayGetCount(sources) == 0) {
        if (sources) {
          CFRelease(sources);
        }
        CFRelease(info);
        return -1;
      }

      CFDictionaryRef batteryInfo = IOPSGetPowerSourceDescription(info, CFArrayGetValueAtIndex(sources, 0));
      if (!batteryInfo) {
        CFRelease(sources);
        CFRelease(info);
        return -1;
      }

      CFNumberRef capacityRef = (CFNumberRef)CFDictionaryGetValue(batteryInfo, CFSTR(kIOPSCurrentCapacityKey));
      CFNumberRef maxCapacityRef = (CFNumberRef)CFDictionaryGetValue(batteryInfo, CFSTR(kIOPSMaxCapacityKey));

      if (!capacityRef || !maxCapacityRef) {
        CFRelease(sources);
        CFRelease(info);
        return -1; // Early return if capacity references are not available
      }

      int currentCapacity, maxCapacity;
      CFNumberGetValue(capacityRef, kCFNumberIntType, &currentCapacity);
      CFNumberGetValue(maxCapacityRef, kCFNumberIntType, &maxCapacity);

      int batteryPercentage = (int)((double)currentCapacity / (double)maxCapacity * 100);

      CFRelease(sources); // Release sources
      CFRelease(info); // Release info

      return batteryPercentage;
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
          //get link speed in mbps by using iokit
          data.speed = -1;

        //DISAQBLED FOR NOW
          /* TODO:Get the network services using ScNetwork services and iokit.

          SCPreferencesRef prefs = SCPreferencesCreate(NULL, CFSTR("NetworkInterfaces"), NULL);
          bool succes = true;
          if (!prefs) {
          std::cerr << "Failed to create SCPreferencesRef" << std::endl;
          succes = false;
          }
          if (succes) {
            CFArrayRef networkServices = SCNetworkServiceCopyAll(prefs);
            if (!networkServices) {
              std::cerr << "Failed to copy network services" << std::endl;
              CFRelease(prefs);
              succes = false;
            }

            if (succes) {
              CORE_ERROR("COUNT IS: {0}", CFArrayGetCount(networkServices));
              for (CFIndex i = 0; i < CFArrayGetCount(networkServices); ++i) {
                SCNetworkServiceRef service = (SCNetworkServiceRef) CFArrayGetValueAtIndex(networkServices, i);
                SCNetworkInterfaceRef interface = SCNetworkServiceGetInterface(service);
                if (interface) {
                  CFStringRef interfaceName = SCNetworkInterfaceGetBSDName(interface);
                  if (interfaceName) {
                    if (CFStringCompare(interfaceName, CFSTR("en0"), 0) == kCFCompareEqualTo) {
                      CFStringRef interfaceType = SCNetworkInterfaceGetInterfaceType(interface);
                      CFStringRef interfaceName = SCNetworkInterfaceGetLocalizedDisplayName(interface);
                      auto interfaceHardwareAddress = SCNetworkInterfaceGetHardwareAddressString(interface);
                      auto linkMax = SCNetworkInterfaceGetLinkSpeed(interface);

                      if (interfaceType && interfaceName) {
                        std::cout << "Interface Type: " << CFStringGetCStringPtr(interfaceType, kCFStringEncodingUTF8)
                                  << ", Name: " << CFStringGetCStringPtr(interfaceName, kCFStringEncodingUTF8) << std::endl;
                      } else {
                        std::cout << "Could not determine interface details." << std::endl;
                      }
                    }
                  }
                }
              }
              CFRelease(networkServices);
              CFRelease(prefs);
            }
          }
          */

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
              }
            }

            resolvConf.close();
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