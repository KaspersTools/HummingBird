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

public:
    std::string key;
    std::variant<std::string, int, float, bool, Date> value;

    std::vector<PlistNode> children = {};
    PlistType type = PlistTypeNone;
    PlistType parentType = PlistTypeNone;

    PlistNode() {}
    PlistNode(std::string keyName, std::variant<std::string, int, float, bool, Date> nodeValue, PlistType type, PlistType parentType)
        : key(keyName), value(nodeValue), type(type), parentType(parentType) {
    }
  };

  class Plist {
public:
    explicit Plist(const std::string &filename) {
      parsePlist(filename);
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

            parent.children.push_back(plnode);
          }
        }
      }

      return true;
    }

    bool parsePlist(const std::string filename) {
      xmlDocPtr doc = xmlReadFile(filename.c_str(), NULL, 0);
      if (doc == NULL) {
        CORE_TRACE("Error: could not parse file " + filename);
        return false;
      }

      xmlNode *root_element = xmlDocGetRootElement(doc);
      if (root_element == NULL) {
        CORE_TRACE("Error: could not parse file " + filename);
        return false;
      }


      std::string currentKey;
      rootNode = PlistNode();
      parseNode(root_element, rootNode, currentKey);

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
          if (childNode.key == "Minute") {
            minute = std::get<int>(childNode.value);
          } else if (childNode.key == "Hour") {
            hour = std::get<int>(childNode.value);
          } else if (childNode.key == "Day") {
            day = std::get<int>(childNode.value);
          } else if (childNode.key == "Month") {
            month = std::get<int>(childNode.value);
          } else if (childNode.key == "Weekday") {
            weekday = std::get<int>(childNode.value);
          };
        }

        node.children.clear();
        node.value = PlistNode::Date{month, day, hour, minute, weekday};
      }

      for (auto &childNode: node.children) {
        if (childNode.children.size() > 0) {
          checkForDateNodes(childNode);
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
        writeNode(node, plistString);
      }

      plistString += "</dict>\n";
      plistString += "</plist>\n";

      CORE_TRACE("SAVING PLIST TO: " + file.path.string());
      CORE_TRACE("SAVING PLIST: \n " + plistString);

      std::ofstream plistFile(file.path);
      plistFile << plistString;
      plistFile.close();
    }

private:
    void writeNode(const PlistNode &node, std::string &plistString) {
      if(node.parentType != PlistTypeArray){
        plistString += "<key>" + node.key + "</key>\n";
      }

      if (node.type == PlistTypeString) {
        plistString += "<string>" + std::get<std::string>(node.value) + "</string>\n";
      } else if (node.type == PlistTypeInteger) {
        plistString += "<integer>" + std::to_string(std::get<int>(node.value)) + "</integer>\n";
      } else if (node.type == PlistTypeReal) {
        plistString += "<real>" + std::to_string(std::get<float>(node.value)) + "</real>\n";
      } else if (node.type == PlistTypeBoolean) {
        plistString += "<" + std::to_string(std::get<bool>(node.value)) + "/>\n";
      } else if (node.type == PlistTypeDate) {
        //        plistString += "<date>" + std::get<std::string>(node.value) + "</date>\n";
      } else if (node.type == PlistTypeData) {
        plistString += "<data>" + std::get<std::string>(node.value) + "</data>\n";
      } else if (node.type == PlistTypeArray) {
        plistString += "<array>\n";
        for (auto &childNode: node.children) {
          writeNode(childNode, plistString);
        }
        plistString += "</array>\n";
      } else if (node.type == PlistTypeDictionary) {
        plistString += "<dict>\n";
        for (auto &childNode: node.children) {
          writeNode(childNode, plistString);
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
