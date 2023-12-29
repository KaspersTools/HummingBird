/**
* TODO: Copyright stuff here
*/

//
// Created by Kasper de Bruin on 27/12/2023.
//
#pragma once
#include "Updatable.h"

#include "Utils/Utils.h"
#include <IOKit/IOKitLib.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <vector>

namespace HummingbirdCore {
  namespace System {

    /**
    * These enum and structs are defined in the Apple PowerManagement project:
    * https://opensource.apple.com/source/PowerManagement/PowerManagement-211/pmconfigd/PrivateLib.c.auto.html
    */
    enum SMCResult : UInt8 {
      kSMCSuccess = 0,
      kSMCError = 1,
      kSMCKeyNotFound = 132
    };

    enum SMCSelector : UInt8 {
      kSMCHandleYPCEvent = 2,
      kSMCReadKey = 5,
      kSMCWriteKey = 6,
      kSMCGetKeyFromIndex = 8,
      kSMCGetKeyInfo = 9
    };

    typedef UInt8 SMCBytes[32];

    typedef struct DataType {
      DataType(const std::string &givenString, UInt32 givenSize) {
        type = HummingBirdCore::Utils::stringToFourCharCode(givenString);
        size = givenSize;
      }

      DataType(FourCharCode givenType, UInt32 givenSize) : type(givenType), size(givenSize) {
      }

      FourCharCode type;
      UInt32 size;
    } data_type_t;

    typedef struct DataTypes {
      DataType FDS = DataType("{fds", 16);
      DataType Flag = DataType("flag", 1);
      DataType FPE2 = DataType("fpe2", 2);
      DataType FLT = DataType("flt", 4);
      DataType SP78 = DataType("sp78", 2);
      DataType UInt8 = DataType("ui8", 1);
      DataType UInt32 = DataType("ui32", 4);
    } data_types_t;

    typedef struct SMCKey {
      SMCKey(const std::string &givenString, data_type_t &typeInfo) : info(typeInfo) {
        code = HummingBirdCore::Utils::stringToFourCharCode(givenString);
      }

      FourCharCode code;
      DataType info;
    } smc_key_t;

    typedef struct SMCVersion {
      unsigned char major = 0;
      unsigned char minor = 0;
      unsigned char build = 0;
      unsigned char reserved = 0;
      unsigned short release = 0;
    } smc_version_t;

    typedef struct SMCLimitData {
      UInt16 version = 0;
      UInt16 length = 0;
      UInt32 cpuPLimit = 0;
      UInt32 gpuPLimit = 0;
      UInt32 memPLimit = 0;
    } smc_limit_data_t;

    typedef struct SMCKeyInfoData {
      IOByteCount dataSize = 0;
      UInt32 dataType = 0;
      UInt8 dataAttributes = 0;
    } smc_key_info_data_t;

    struct SMCParamStruct {

      UInt32 key = 0;

      SMCVersion vers = SMCVersion();

      SMCLimitData pLimitData = SMCLimitData();

      SMCKeyInfoData keyInfo = SMCKeyInfoData();

      UInt8 result = 0;

      UInt8 status = 0;

      UInt8 data8 = 0;

      UInt32 data32 = 0;

      SMCBytes bytes = {0};
    };

    typedef struct CpuTickStruct {
      int userTicks;
      int systemTicks;
      int idleTicks;
      int niceTicks;
    } cpu_tick_t;

    //Own structs
    //Network
    typedef struct NetworkData {
      std::string              name          = "UNKOWN";//1
      std::string              ipv4          = "UNKOWN";//2
      std::string              netmaskipv4   = "UNKOWN";//3
      std::string              broadcastipv4 = "UNKOWN";//4
      std::string              ipv6          = "UNKOWN";//5
      std::string              netmaskipv6   = "UNKOWN";//6
      std::string              broadcastipv6 = "UNKOWN";//7
      std::string              mac           = "UNKOWN";//8
      std::string              gateway       = "UNKOWN";//9
      std::string              status        = "UNKOWN";//10
      std::string              interfaceType = "UNKOWN";//11
      int                      mtu           = -1;      //12
      int                      interfaceIndex= -1;      //13
      unsigned long            speed         = -1;      //speed in Mbit/s 14
      std::vector<std::string> dnsServers    = {};      //15
      bool                     usingDhcp     = false;   //16
    } network_data_t;

    class SysInfo {
  private:
      io_connect_t connectionHandle = 0;
      cpu_tick_t prevCpuTicks;

  public:
      DataTypes types = DataTypes();

      SysInfo() {
        prevCpuTicks = getCpuLoadInfo();
      }

      ~SysInfo() {
        close();
      }

      /**
        * Opens a connection to the SMC driver.
        * @throws runtime_error    Throws a runtime error if the connection can not be made.
        */
      void open();

      /**
        * Closes the connection to the SMC driver.
        * @throws runtime_error    Throws a runtime error if closing the connection failed.
        */
      void close();

      /**
        * Returns the data type of the given key.
        * @param keyString The given key as a string.
        * @return  The data type information for the given key.
        */
      DataType getKeyInfo(std::string keyString);

      /**
        * Makes a call to the SMC.
        * @throws runtime_error    Throws a runtime error if something went wrong reading the key.
        * @param givenStruct       The given struct which is used to call the SMC.
        * @param smcSelector       The given smc selector.
        * @return  Returns the resulting struct of the SMC call
        */
      SMCParamStruct callSMC(SMCParamStruct givenStruct, SMCSelector smcSelector = kSMCHandleYPCEvent);

      /**
        * Reads the data of a SMC-Key
        * @throws runtime_error    Throws a runtime error if something went wrong while reading the key.
        * @param smcKey            The given struct containing the key.
        * @param result            Reference in which the result will be stored.
        */
      void readKey(smc_key_t smcKey, SMCBytes &result);

      /**
        * Reads the data of the key code from the SMC.
        * @param keyCode   The given four-figure key code.
        * @param typeInfo  The type info for reading the key.
        * @param resultArray   The array in which the read data is written.
        */
      void readKey(const std::string &keyCode, DataType typeInfo, SMCBytes &resultArray);

      /**
        * Reads the current ram usage and returns it in a vector.
        * The vector contains 5 values: [free, active, inactive, wired, compressed]
        * The values are in GB
        */
      std::vector<float> getMemoryUsage();

      /**
        * Returns the total amount of ram memory of this machine in gigabyte.
        */
      float getTotalMemory();

      /**
        * Reads the cpu temperature of the CPU_0_PROXIMITY sensor.
        * @return  The temperature of the cpu in degrees celcius.
        */
      int getCpuTemp();

      /**
        * Returns the cpu usage of the user, system, idle and nice in the given float array in this order.
        */
      std::vector<float> getCpuUsage();

      /**
        * Returns the cpu ticks of the user, system idle and nice in a struct.
        */
      cpu_tick_t getCpuLoadInfo() {
        mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
        mach_port_t host = mach_host_self();
        host_cpu_load_info_data_t load_info;

        kern_return_t result = host_statistics(host, HOST_CPU_LOAD_INFO, (host_info_t) &load_info, &count);

        if (result != KERN_SUCCESS) {
          throw std::runtime_error("An error occured while getting the cpu usage.");
        }

        cpu_tick_t cpuTickStruct;

        cpuTickStruct.userTicks = load_info.cpu_ticks[CPU_STATE_USER];
        cpuTickStruct.systemTicks = load_info.cpu_ticks[CPU_STATE_SYSTEM];
        cpuTickStruct.idleTicks = load_info.cpu_ticks[CPU_STATE_IDLE];
        cpuTickStruct.niceTicks = load_info.cpu_ticks[CPU_STATE_NICE];

        return cpuTickStruct;
      }

      /**
        * Returns the number of fans of the machine.
        * @return  The number of fans.
        */
      int getFanCount();

      /**
        * Returns the minimum rounds per minute (rpm) of the fan with the given id.
        * @param fanID The given fan id.
        * @return  The lowest possible rpm.
        */
      int getMinFanSpeed(int fanID);

      /**
        * Returns the maximum rounds per minute (rpm) of the fan with the given id.
        * @param fanID The given fan id.
        * @return  The highest possible rpm
        */
      int getMaxFanSpeed(int fanID);

      /**
        * Returns the current rounds per minute (rpm) of the fan with the given id.
        * @param fanID The given fan id.
        * @return  The current rpm.
        */
      int getCurrentFanSpeed(int fanID);

      /**
        * Returns the number of batteries of the machine.
        * @return  The number of batteries.
        */
      int getBatteryCount();

      /**
        * Returns true if the battery is currently charged. Return false otherwise.
        * @return  Whether the battery is charged.
        */
      bool isChargingBattery();

      /**
        * Returns the health of the battery in percentage.
        */
      float getBatteryHealth();

      /**
       *  Returns the number of already used battery cycles.
       */
      int getBatteryCycles();

      ///OWN FUNCTIONS
      //Battery
      /**
       * @return the current battery charge in percentage.
       * @throws runtime_error    Throws a runtime error if something went wrong while reading the key.
       */
      int getBatteryCharge();

      //Disk Data
      /**
       * @return the number of disks in the system.
       */
      int getDiskCount();

      /**
       * @return the names of the disks in the system.
       */
      std::vector<std::string> getDiskNames();

      /**
         * @return the total disk space of the disk with the given id in GB.
         */
      std::vector<float> getDiskSpaceTotal();

      /**
         * @return the used disk space of the disk with the given id in GB.
         */
      std::vector<float> getDiskSpaceUsed();

      /**
         * @return the free disk space of the disk with the given id in GB.
         */
      std::vector<float> getDiskSpaceFree();

      //Network
      /**
     * @return the network data of the system.
     */

      /**
     * @return the open ports of the system.
     */

      /**
      * @return the network connections of the system.
      */

      /**
       * @return the interfaces of the system
       */
      std::vector<network_data_t> getNetworkInterfaces();

      //Logs
      /**
     * @return the logs of the system.
     */
      std::string getLogs();
    };

  }// namespace System
}// namespace HummingbirdCore