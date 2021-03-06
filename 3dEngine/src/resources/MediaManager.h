#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include <UrchinCommon.h>

#include <resources/ResourceManager.h>
#include <resources/model/ConstMeshes.h>
#include <resources/model/ConstAnimation.h>
#include <resources/image/Image.h>
#include <resources/material/Material.h>
#include <resources/font/Font.h>
#include <loader/Loader.h>

namespace urchin {

    /**
     * Find the appropriate loader according to the extension of the file and load the resource
     */
    class MediaManager : public Singleton<MediaManager> {
        public:
            friend class Singleton<MediaManager>;

            template<class T> T* getMedia(const std::string&, const std::map<std::string, std::string>& = {}, const std::string& = "");

        private:
            MediaManager();

            std::map<std::string, std::unique_ptr<LoaderInterface>> loadersRegistry;
    };

    #include "MediaManager.inl"

}
