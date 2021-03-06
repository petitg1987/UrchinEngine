#include <QApplication>
#include <QtCore/QStandardPaths>

#include <StateSaveHelper.h>

namespace urchin {

    StateSaveHelper::StateSaveHelper() {
        std::string applicationDirPath = QCoreApplication::applicationDirPath().toStdString();
        std::string propertiesFilePath = applicationDirPath + "/save/states.properties";
        propertyFileHandler = std::make_unique<PropertyFileHandler>(propertiesFilePath);
    }

    void StateSaveHelper::saveState(const std::string& stateId, const std::string& stateValue) {
        std::map<std::string, std::string> properties = propertyFileHandler->loadPropertyFile();
        properties[stateId] = stateValue;

        propertyFileHandler->savePropertyFile(properties);
    }

    std::string StateSaveHelper::retrieveState(const std::string& stateId, const std::string& defaultValue) const {
        std::map<std::string, std::string> properties = propertyFileHandler->loadPropertyFile();
        return MapUtil::getWithDefault(properties, stateId, defaultValue);
    }



}
