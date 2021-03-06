#pragma once

#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <map>
#include <regex>

#include <io/data/model/DataContentLine.h>

namespace urchin {

    class DataContentLine {
        public:
            DataContentLine(std::string, std::string, std::map<std::string, std::string>, DataContentLine*);

            static std::unique_ptr<DataContentLine> fromRawContentLine(const std::string&, DataContentLine*, const std::string&);
            static std::string toRawContentLine(DataContentLine&);

            DataContentLine* getParent() const;

            DataContentLine& addChild(std::unique_ptr<DataContentLine>);
            const std::vector<std::unique_ptr<DataContentLine>>& getChildren() const;

            const std::string& getName() const;

            void setValue(const std::string &);
            const std::string& getValue() const;

            void addAttribute(const std::string&, const std::string&);
            const std::map<std::string, std::string>& getAttributes() const;

        private:
            static constexpr char NAME_REGEX[] = "([a-zA-Z]+)";
            static constexpr char ATTRIBUTES_REGEX[] = "\\(?(.*?)\\)?";
            static constexpr char VALUE_REGEX[] = "\"?(.*?)\"?";

            static constexpr char ATTRIBUTES_SEPARATOR = ';';
            static constexpr char ATTRIBUTES_ASSIGN = '=';

            std::string name;
            std::string value;
            std::map<std::string, std::string> attributes;
            DataContentLine* parent;

            std::vector<std::unique_ptr<DataContentLine>> children;
    };

}
