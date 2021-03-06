#pragma once

#include <UrchinCommon.h>
#include <UrchinMapHandler.h>

#include <controller/AbstractController.h>

namespace urchin {

    class AIController : public AbstractController {
        public:
            AIController();

            const SceneAI& getSceneAI() const;

            const SceneAI& updateSceneAI(std::unique_ptr<NavMeshAgent>);
    };

}
