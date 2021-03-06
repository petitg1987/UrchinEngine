#include <filesystem>
#include <UrchinCommon.h>

#include <i18n/LabelTranslator.h>

namespace urchin {

    LabelTranslator::LabelTranslator() :
            FILE_PREFIX("labels_") ,
            FILE_POSTFIX(".properties") {
        std::string uiLabelsLocation = ConfigService::instance()->getStringValue("ui.labelsLocation");

        if (!uiLabelsLocation.empty()) {
            labelsFilesDirectoryName = FileSystem::instance()->getResourcesDirectory() + uiLabelsLocation;
            loadAvailableLanguages();
        }
    }

    void LabelTranslator::loadAvailableLanguages() {
        for (const auto& dir : std::filesystem::directory_iterator(labelsFilesDirectoryName)) {
            std::string filename = dir.path().filename().u8string();
            if (dir.is_regular_file() && filename.size() > FILE_PREFIX.size() + FILE_POSTFIX.size()) {
                std::size_t endLanguagePosition = filename.find(FILE_POSTFIX);
                std::string language = filename.substr(FILE_PREFIX.size(), endLanguagePosition - FILE_PREFIX.size());
                availableLanguages.emplace_back(language);
            }
        }
    }

    void LabelTranslator::checkMissingTranslation() const {
        for(unsigned int i = 0; i < availableLanguages.size(); ++i) {
            std::string firstLanguage = availableLanguages[i];
            auto firstLanguageLabels = retrieveLanguageLabels(firstLanguage);

            std::string secondLanguage = availableLanguages[(i + 1) % availableLanguages.size()];
            auto secondLanguageLabels = retrieveLanguageLabels(secondLanguage);

            for(auto& firstLanguageLabel : firstLanguageLabels) {
                if(secondLanguageLabels.find(firstLanguageLabel.first) == secondLanguageLabels.end()) {
                    logMissingTranslation(secondLanguage, firstLanguageLabel.first);
                    break;
                }
            }
        }
    }

    const std::vector<std::string>& LabelTranslator::getAvailableLanguages() const {
        return availableLanguages;
    }

    std::string LabelTranslator::translate(const std::string& language, const std::string& labelKey) {
        loadLanguageLabels(language);

        auto itFind = loadedLanguageLabels.find(labelKey);
        if(itFind != loadedLanguageLabels.end()) {
            return itFind->second;
        }

        logMissingTranslation(language, labelKey);
        return "??" + labelKey + "??";
    }

    void LabelTranslator::loadLanguageLabels(const std::string& language) {
        if (language != loadedLanguage) {
            if (std::count(availableLanguages.begin(), availableLanguages.end(), language)) {
                loadedLanguageLabels = retrieveLanguageLabels(language);
            } else {
                loadedLanguageLabels.clear();
                logMissingLanguage(language);
            }
            loadedLanguage = language;
        }
    }

    std::map<std::string, std::string> LabelTranslator::retrieveLanguageLabels(const std::string& language) const {
        return PropertyFileHandler(labelsFilesDirectoryName + FILE_PREFIX + language + FILE_POSTFIX).loadPropertyFile();
    }

    void LabelTranslator::logMissingLanguage(const std::string& language) const {
        static bool missingLanguageLogged = false;
        if (!missingLanguageLogged) {
            Logger::instance()->logError("Translated labels for language '" + language + "' not found");
            missingLanguageLogged = true;
        }
    }

    void LabelTranslator::logMissingTranslation(const std::string& language, const std::string& labelKey) const {
        static bool missingTranslationLogged = false;
        if (!missingTranslationLogged) {
            Logger::instance()->logError("Translation in '" + language + "' not found for label key '" + labelKey + "'");
            missingTranslationLogged = true;
        }
    }

}
