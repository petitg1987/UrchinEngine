#pragma once

#include <string>

#include <resources/model/ConstMeshes.h>
#include <loader/Loader.h>

namespace urchin {

    class LoaderUrchinMesh : public Loader<ConstMeshes> {
        public:
            ~LoaderUrchinMesh() override = default;

            ConstMeshes* loadFromFile(const std::string&, const std::map<std::string, std::string>&) override;
    };

}
