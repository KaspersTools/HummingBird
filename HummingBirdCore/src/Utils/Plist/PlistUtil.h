//
// Created by Kasper de Bruin on 11/01/2024.
//

#ifndef HUMMINGBIRD_PLISTUTIL_H
#define HUMMINGBIRD_PLISTUTIL_H

#include <PCH/pch.h>

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

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

    PlistNode() {}
    PlistNode(std::string keyName, std::variant<std::string, int, float, bool, Date> nodeValue, PlistType type)
        : key(keyName), value(nodeValue), type(type) {}

    std::vector<PlistNode> getNodes() const {
      return children;
    }

    bool const hasChildren() const {
      return !children.empty();
    }
  };

  class Plist {
public:
    std::vector<PlistNode> nodes;
    bool parsed = false;

    ///STRUCTURE

    bool parseNode(xmlNode *node, PlistNode &parent, std::string &currentKey) {
      for (xmlNode *currentNode = node; currentNode; currentNode = currentNode->next) {
        if (currentNode->type == XML_ELEMENT_NODE) {
          std::string nodeName = (const char *) currentNode->name;
          //<key> is identifier plist looks like this:
          //<valuetype(string)(integer)> is the value </valuetype>

          //<key>Label</key>                       //identifier
          //<string>kasperslabel</string>          //value
          //<key>StartInterval</key>               //identifier
          //<array>                                //holder
          //  <key>Day</key>                       //identifier
          //  <integer>1</integer>                 //value

          if (nodeName == "key") {
            currentKey = (const char *) xmlNodeGetContent(currentNode);
          } else if (!currentKey.empty()) {
            PlistType type = PlistTypeNone;

            if (nodeName == "string") {
              type = PlistTypeString;
            } else if (nodeName == "integer") {
              type = PlistTypeInteger;
            } else if (nodeName == "real") {
              type = PlistTypeReal;
            } else if (nodeName == "true" || nodeName == "false") {
              type = PlistTypeBoolean;
              type = PlistTypeBoolean;
            } else if (nodeName == "data") {
              type = PlistTypeData;
            } else if (nodeName == "array") {
              type = PlistTypeArray;
            } else if (nodeName == "dict") {
              type = PlistTypeDictionary;
            }

            PlistNode childNode(currentKey, "", type);

            if (nodeName == "dict" || nodeName == "array") {
              PlistNode childNode(currentKey, "", PlistTypeArray);// Use appropriate type
              std::string newKey;
              parseNode(currentNode->children, childNode, newKey);
              parent.children.push_back(childNode);
              currentKey.clear();
            } else if (currentNode->children && currentNode->children->type == XML_TEXT_NODE) {
              //TODO: CLEAN THIS UP AND ADD MORE TYPES
              if (childNode.type == PlistTypeInteger) {
                childNode.value = std::stoi((const char *) xmlNodeGetContent(currentNode));
              } else if (childNode.type == PlistTypeReal) {
                childNode.value = std::stof((const char *) xmlNodeGetContent(currentNode));
              } else if (childNode.type == PlistTypeBoolean) {
                childNode.value = (nodeName == "true");
              } else if (childNode.type == PlistTypeDate) {
                std::string date = (const char *) xmlNodeGetContent(currentNode);
                childNode.value = PlistNode::Date();
              } else {
                childNode.value = (const char *) xmlNodeGetContent(currentNode);
              }
            }
            parent.children.push_back(childNode);
            currentKey.clear();
          } else if (nodeName == "array" || nodeName == "dict") {
            // For nested <array> or <dict> without a preceding <key>
            PlistNode childNode("", "", PlistTypeArray);// Or PlistTypeDictionary
            std::string newKey;
            parseNode(currentNode->children, childNode, newKey);
            parent.children.push_back(childNode);
          } else {
            parseNode(currentNode->children, parent, currentKey);
          }
        }
      }

      return true;
    }

    bool parsePlist(const std::string &filename) {
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

      PlistNode root;
      std::string currentKey;
      if (!parseNode(root_element, root, currentKey)) {
        CORE_TRACE("Error: could not parse file " + filename);
        return false;
      }

      nodes = root.children;

      xmlFreeDoc(doc);
      xmlCleanupParser();

      for (auto &node: nodes) {
        checkForDateNodes(node);
      }

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

    std::vector<PlistNode> getChildren() const {
      return nodes;
    }

    PlistNode getChild(const std::string &key) const {
      for (auto &node: nodes) {
        if (node.key == key) {
          return node;
        }
      }
      return PlistNode();
    }

    std::variant<std::string, int, float, bool, PlistNode::Date> getValue(const std::string &key) const {
      for (auto &node: nodes) {
        if (node.key == key) {
          return node.value;
        }
      }
      return std::variant<std::string, int, float, bool, PlistNode::Date>();
    }


    std::string getString(const std::string &key) const {
      for (auto &node: nodes) {
        if (node.key == key) {
          return std::get<std::string>(node.value);
        }
      }
      return "";
    }

    int getInt(const std::string &key) const {
      for (auto &node: nodes) {
        if (node.key == key) {
          return std::get<int>(node.value);
        }
      }
      return 0;
    }

    float getFloat(const std::string &key) const {
      for (auto &node: nodes) {
        if (node.key == key) {
          return std::get<float>(node.value);
        }
      }
      return 0;
    }

    bool getBool(const std::string &key) const {
      for (auto &node: nodes) {
        if (node.key == key) {
          return std::get<bool>(node.value);
        }
      }
      return false;
    }

    bool const hasChildren() const {
      return !nodes.empty();
    }

    bool const isParsed() const {
      return parsed;
    }
  };


}// namespace HummingBirdCore::Utils::PlistUtil

#endif//HUMMINGBIRD_PLISTUTIL_H
