#ifndef URCHINENGINE_SHADERVARIABLE_H
#define URCHINENGINE_SHADERVARIABLE_H

#include <string>
#include <memory>

#include "graphic/shader/model/Shader.h"

namespace urchin {

    class ShaderVar {
        public:
            ShaderVar();
            ShaderVar(const std::unique_ptr<Shader> &, std::string &&);

            int getVariableLocation() const;

        private:
            int variableLocation;
    };

}

#endif