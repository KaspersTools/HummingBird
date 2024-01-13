//
// Created by Kasper de Bruin on 11/01/2024.
//

#ifndef HUMMINGBIRD_PLISTUTIL_H
#define HUMMINGBIRD_PLISTUTIL_H

#include <PCH/pch.h>

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

#include <Utils/UUID.h>

//Kasper coding at 3:00 AM so no idea what this doe

namespace HummingBirdCore::Utils::PlistUtil {
  enum PlistType {
    PlistTypeString,
    PlistTypeInteger,
    PlistTypeReal,
    PlistTypeBoolean,
    PlistTypeDate,
    PlistTypeData,
    PlistTypeArray,
    PlistTypeDictionary,
    PlistTypeNone//UNKOWN
  };

  class PlistNode {
public:
    //<key>Month</key>
    //<integer>1</integer>
    //<key>Day</key>
    //<integer>1</integer>
    //<key>Hour</key>
    //<integer>1</integer>
    //<key>Minute</key>
    //<integer>1</integer>
    //<key>Weekday</key>
    //<integer>3</integer>
    struct Date {
  public:
      int month;
      int day;
      int hour;
      int minute;
      int weekday;

      static bool Identify(const PlistNode &node) {
        if (node.children.size() != 5) {
          CORE_TRACE("Date node has wrong amount of children");
          return false;
        }
        CORE_TRACE("Date node has correct amount of children");
        return true;
      }
    };

    //todo: make this private
public:
    std::string key;
    std::variant<std::string, int, float, bool, Date> placeholder;
    std::optional<std::variant<std::string, int, float, bool, Date>> value;
    bool required = false;

    std::map<std::string, PlistNode> children = {};

    PlistType type = PlistTypeNone;
    PlistType parentType = PlistTypeNone;

public:
    PlistNode() {}
    PlistNode(std::string keyName, std::variant<std::string, int, float, bool, Date> nodeValue, PlistType type, PlistType parentType)
        : key(keyName), value(nodeValue), type(type), parentType(parentType) {
    }

    PlistNode(std::string keyName,
              std::variant<std::string, int, float, bool, Date> nodeValue,
              PlistType type,
              PlistType parentType,
              std::vector<PlistNode> children,
              std::variant<std::string, int, float, bool, Date> placeholder,
              bool required = false)
        : key(keyName), value(nodeValue), type(type), parentType(parentType), placeholder(placeholder), required(required) {
        for (auto &child: children) {
          addChild(child);
        }
    }

    PlistNode(std::string keyName,
              PlistType type,
              PlistType parentType,
              std::vector<PlistNode> children,
              std::variant<std::string, int, float, bool, Date> placeholder,
              bool required = false)
        : key(keyName), type(type), parentType(parentType), placeholder(placeholder), required(required) {
      for (auto &child: children) {
        addChild(child);
      }
    }

    void addChild(PlistNode &node) {
      node.parentType = type;
      children.insert(std::pair<std::string, PlistNode>(node.key, node));
    }

    std::variant<std::string, int, float, bool, Date> &getValue() {
      if (value.has_value()) {
        return value.value();
      }

      return placeholder;
    }

    void setValue(std::variant<std::string, int, float, bool, Date> &value) {
      PlistNode::value = value;
    }

    void setValue(std::string &value) {
      PlistNode::value = value;
    }

    void setValue(int &value) {
      PlistNode::value = value;
    }

    void setValue(float &value) {
      PlistNode::value = value;
    }

    void setValue(bool &value) {
      PlistNode::value = value;
    }

    void setValue(Date &value) {
      PlistNode::value = value;
    }

    std::string getDescription() const {
      std::string desc = "";
      desc += "Key: " + key + "\n";
      desc += "Type: " + std::to_string(type) + "\n";
      desc += "ParentType: " + std::to_string(parentType) + "\n";
      desc += "Required: " + std::to_string(required) + "\n";
      desc += "Value: " + std::to_string(value.has_value()) + "\n";
      desc += "Children: " + std::to_string(children.size()) + "\n";
      return desc;
    }
  };


  class Plist {
public:
    explicit Plist() {
    }

    ~Plist() {
    }

public:
    ///STRUCTURE

    bool parseNode(xmlNode *node, PlistNode &parent, std::string &currentKey) {
      for (xmlNode *currentNode = node; currentNode; currentNode = currentNode->next) {
        if (currentNode->type == XML_ELEMENT_NODE) {
          std::string nodeName = (const char *) currentNode->name;
          if (nodeName == "key") {
            currentKey = (const char *) currentNode->children->content;
          } else {
            int ind = 0;
            std::string itemKey = currentKey;
            if (parent.type == PlistTypeArray) {
              ind = parent.children.size();
            }

            PlistNode plnode = plnode;
            plnode.key = itemKey;
            if (parent.type == PlistTypeArray) {
              plnode.key = std::to_string(ind);
            }

            plnode.parentType = parent.type;


            if (currentKey.empty()) {
              parseNode(currentNode->children, parent, currentKey);
            } else if (nodeName == "string") {
              plnode.type = PlistTypeString;
              plnode.value = (const char *) currentNode->children->content;
            } else if (nodeName == "integer") {
              plnode.type = PlistTypeInteger;
              plnode.value = std::stoi((const char *) currentNode->children->content);
            } else if (nodeName == "real") {
              plnode.type = PlistTypeReal;
              plnode.value = std::stof((const char *) currentNode->children->content);
            } else if (nodeName == "true") {
              plnode.type = PlistTypeBoolean;
              plnode.value = true;
            } else if (nodeName == "false") {
              plnode.type = PlistTypeBoolean;
              plnode.value = false;
            } else if (nodeName == "date") {
              plnode.type = PlistTypeDate;
              plnode.value = (const char *) currentNode->children->content;
            } else if (nodeName == "data") {
              plnode.type = PlistTypeData;
              plnode.value = (const char *) currentNode->children->content;
            } else if (nodeName == "array") {
              plnode.type = PlistTypeArray;
              parseNode(currentNode->children, plnode, currentKey);
            } else if (nodeName == "dict") {
              plnode.type = PlistTypeDictionary;
              parseNode(currentNode->children, plnode, currentKey);
            } else {
              plnode.type = PlistTypeNone;
            }

            parent.addChild(plnode);
          }
        }
      }

      return true;
    }

    bool createNewPlist() {
      parsed = true;
      rootNode = PlistNode();
      createDefaultTypes();
      return true;
    }

    bool parsePlist(const std::string filename) {
      xmlDocPtr doc = xmlReadFile(filename.c_str(), NULL, 0);
      if (doc == NULL) {
        CORE_TRACE("Error: could not parse file " + filename);
        parsed = false;
        return false;
      }

      xmlNode *root_element = xmlDocGetRootElement(doc);
      if (root_element == NULL) {
        CORE_TRACE("Error: could not parse file " + filename);
        parsed = false;
        return false;
      }


      std::string currentKey;
      rootNode = PlistNode();
      createDefaultTypes();

      xmlFreeDoc(doc);
      xmlCleanupParser();

      checkForDateNodes(rootNode);
      parsed = true;
      return true;
    }

    void checkForDateNodes(PlistNode &node) {
      if (PlistNode::Date::Identify(node)) {
        node.type = PlistTypeDate;
        int minute = 0;
        int hour = 0;
        int day = 0;
        int month = 0;
        int weekday = 0;

        for (auto &childNode: node.children) {
          if (childNode.second.key == "Minute") {
            if (childNode.second.value.has_value())
              minute = std::get<int>(childNode.second.value.value());
            else
              minute = 0;
          } else if (childNode.second.key == "Hour") {
            if (childNode.second.value.has_value())
              hour = std::get<int>(childNode.second.value.value());
            else
              hour = 0;
          } else if (childNode.second.key == "Day") {
            if (childNode.second.value.has_value())
              day = std::get<int>(childNode.second.value.value());
            else
              day = 0;
          } else if (childNode.second.key == "Month") {
            if (childNode.second.value.has_value())
              month = std::get<int>(childNode.second.value.value());
            else
              month = 0;
          } else if (childNode.second.key == "Weekday") {
            if (childNode.second.value.has_value())
              weekday = std::get<int>(childNode.second.value.value());
            else
              weekday = 0;
          };
        }

        node.children.clear();
        node.value = PlistNode::Date{month, day, hour, minute, weekday};
      }

      for (auto &childNode: node.children) {
        if (childNode.second.children.size() > 0) {
          checkForDateNodes(childNode.second);
        }
      }
    }

    bool getIsParsed() const {
      return parsed;
    }

    PlistNode &getRootNode() {
      return rootNode;
    }

    void writePlist(const File &file) {
      std::string plistString = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
      plistString += "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
      plistString += "<plist version=\"1.0\">\n";
      plistString += "<dict>\n";

      for (auto &node: rootNode.children) {
        writeNode(node.second, plistString);
      }

      plistString += "</dict>\n";
      plistString += "</plist>\n";

      CORE_TRACE("SAVING to: " + file.getFullPath() + " the plist Content  \n " + plistString);

      if (FileUtils::fileExists(file)) {
        CORE_TRACE("File exists overwriting");
        if (FileUtils::writeToFile(file, plistString)) {
          CORE_TRACE("Plits saved");
          return;
        } else {
          CORE_ERROR("Something went wrong saving the plist");
          return;
        }
      } else {
        //Create the file
        CORE_TRACE("File does not exist creating a new file");
        if (FileUtils::createFile(file)) {
          CORE_TRACE("File created");
          if (FileUtils::writeToFile(file, plistString)) {
            CORE_TRACE("Plits saved");
            return;
          } else {
            CORE_ERROR("Something went wrong saving the plist");
            return;
          }
        } else {
          CORE_ERROR("Something went wrong creating the file");
          return;
        }
      }
    }

public:
    void addCalendarIntervalToRootNode() {
      PlistNode::Date date = PlistNode::Date{-1, -1, -1, -1, -1};
      PlistNode dateNode = PlistNode("StartCalendarInterval", PlistTypeDate, PlistTypeArray, {}, date, true);
      int childCount = rootNode.children["StartCalendarInterval"].children.size();
      rootNode.children["StartCalendarInterval"].children.insert(std::pair<std::string, PlistNode>(std::to_string(childCount), dateNode));
    }

private:
    void createDefaultTypes() {
      PlistNode label = PlistNode("Label", PlistTypeString, PlistTypeDictionary, {}, "LabelPlaceHolder", true);
      rootNode.children["Label"] = label;

      otherTypes();
    }

    void otherTypes(){

      PlistNode programArguments = PlistNode("ProgramArguments", PlistTypeArray, PlistTypeDictionary, {}, "ProgramArgumentsPlaceHolder", false);
      rootNode.children["ProgramArguments"] = programArguments;

      PlistNode program = PlistNode("Program", PlistTypeString, PlistTypeDictionary, {}, "ProgramPlaceHolder", false);
      programArguments.children["Program"] = program;

      PlistNode startInterval = PlistNode("StartInterval", PlistTypeInteger, PlistTypeDictionary, {}, -1, false);
      rootNode.children["StartInterval"] = startInterval;

      //Calendar node
      {
        PlistNode startCalendarInterval = PlistNode("StartCalendarInterval", PlistTypeDictionary, PlistTypeDictionary, {}, "StartCalendarIntervalPlaceHolder", false);
        rootNode.children["StartCalendarInterval"] = startCalendarInterval;
      }

      PlistNode watchPaths = PlistNode("WatchPaths", PlistTypeArray, PlistTypeDictionary, {}, "WatchPathsPlaceHolder", false);
      rootNode.children["WatchPaths"] = watchPaths;

      PlistNode queueDirectories = PlistNode("QueueDirectories", PlistTypeArray, PlistTypeDictionary, {}, "QueueDirectoriesPlaceHolder", false);
      rootNode.children["QueueDirectories"] = queueDirectories;

      PlistNode runAtLoad = PlistNode("RunAtLoad", PlistTypeBoolean, PlistTypeDictionary, {}, false, false);
      rootNode.children["RunAtLoad"] = runAtLoad;

      PlistNode keepAlive = PlistNode("KeepAlive", PlistTypeBoolean, PlistTypeDictionary, {}, false, false);
      rootNode.children["KeepAlive"] = keepAlive;

      PlistNode standardInPath = PlistNode("StandardInPath", PlistTypeString, PlistTypeDictionary, {}, "StandardInPathPlaceHolder", false);
      rootNode.children["StandardInPath"] = standardInPath;

      PlistNode standardOutPath = PlistNode("StandardOutPath", PlistTypeString, PlistTypeDictionary, {}, "StandardOutPathPlaceHolder", false);
      rootNode.children["StandardOutPath"] = standardOutPath;

      PlistNode standardErrorPath = PlistNode("StandardErrorPath", PlistTypeString, PlistTypeDictionary, {}, "StandardErrorPathPlaceHolder", false);
      rootNode.children["StandardErrorPath"] = standardErrorPath;

      PlistNode environmentVariables = PlistNode("EnvironmentVariables", PlistTypeDictionary, PlistTypeDictionary, {}, "EnvironmentVariablesPlaceHolder", false);
      rootNode.children["EnvironmentVariables"] = environmentVariables;

      PlistNode umask = PlistNode("Umask", PlistTypeInteger, PlistTypeDictionary, {}, -1, false);
      rootNode.children["Umask"] = umask;

      PlistNode timeout = PlistNode("Timeout", PlistTypeInteger, PlistTypeDictionary, {}, -1, false);
      rootNode.children["Timeout"] = timeout;

      PlistNode exitTimeout = PlistNode("ExitTimeout", PlistTypeInteger, PlistTypeDictionary, {}, -1, false);
      rootNode.children["ExitTimeout"] = exitTimeout;

      PlistNode throttleInterval = PlistNode("ThrottleInterval", PlistTypeInteger, PlistTypeDictionary, {}, -1, false);
      rootNode.children["ThrottleInterval"] = throttleInterval;

      PlistNode initGroups = PlistNode("InitGroups", PlistTypeBoolean, PlistTypeDictionary, {}, false, false);
      rootNode.children["InitGroups"] = initGroups;

      PlistNode startOnMount = PlistNode("StartOnMount", PlistTypeBoolean, PlistTypeDictionary, {}, false, false);
      rootNode.children["StartOnMount"] = startOnMount;

      PlistNode rootDirectory = PlistNode("RootDirectory", PlistTypeString, PlistTypeDictionary, {}, "RootDirectoryPlaceHolder", false);
      rootNode.children["RootDirectory"] = rootDirectory;

      PlistNode workingDirectory = PlistNode("WorkingDirectory", PlistTypeString, PlistTypeDictionary, {}, "WorkingDirectoryPlaceHolder", false);
      rootNode.children["WorkingDirectory"] = workingDirectory;

      PlistNode debug = PlistNode("Debug", PlistTypeBoolean, PlistTypeDictionary, {}, false, false);
      rootNode.children["Debug"] = debug;

      PlistNode limitLoadToHosts = PlistNode("LimitLoadToHosts", PlistTypeArray, PlistTypeDictionary, {}, "LimitLoadToHostsPlaceHolder", false);
      rootNode.children["LimitLoadToHosts"] = limitLoadToHosts;
    }

    void writeNode(const PlistNode &node, std::string &plistString) {
      if (!node.value.has_value()) {
        return;
      }
      if (node.parentType != PlistTypeArray) {
        plistString += "<key>" + node.key + "</key>\n";
      }

      if (node.type == PlistTypeString) {
        plistString += "<string>" + std::get<std::string>(node.value.value()) + "</string>\n";
      } else if (node.type == PlistTypeInteger) {
        plistString += "<integer>" + std::to_string(std::get<int>(node.value.value())) + "</integer>\n";
      } else if (node.type == PlistTypeReal) {
        plistString += "<real>" + std::to_string(std::get<float>(node.value.value())) + "</real>\n";
      } else if (node.type == PlistTypeBoolean) {
        plistString += "<" + std::to_string(std::get<bool>(node.value.value())) + "/>\n";
      } else if (node.type == PlistTypeDate) {
        //        plistString += "<date>" + std::get<std::string>(node.value) + "</date>\n";
      } else if (node.type == PlistTypeData) {
        plistString += "<data>" + std::get<std::string>(node.value.value()) + "</data>\n";
      } else if (node.type == PlistTypeArray) {
        plistString += "<array>\n";
        for (auto &childNode: node.children) {
          writeNode(childNode.second, plistString);
        }
        plistString += "</array>\n";
      } else if (node.type == PlistTypeDictionary) {
        plistString += "<dict>\n";
        for (auto &childNode: node.children) {
          writeNode(childNode.second, plistString);
        }
        plistString += "</dict>\n";
      }
    }


private:
    bool parsed = false;
    PlistNode rootNode;
  };


}// namespace HummingBirdCore::Utils::PlistUtil

#endif//HUMMINGBIRD_PLISTUTIL_H
