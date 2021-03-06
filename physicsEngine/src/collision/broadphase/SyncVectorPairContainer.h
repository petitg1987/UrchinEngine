#pragma once

#include <mutex>

#include <collision/broadphase/VectorPairContainer.h>

namespace urchin {

    /**
     * Thread safe pair container. Pair can be added/removed by physics thread while pairs are read from another thread.
     */
    class SyncVectorPairContainer : public VectorPairContainer {
        public:
            ~SyncVectorPairContainer() override = default;

            void addOverlappingPair(AbstractBody*, AbstractBody*) override;
            void removeOverlappingPair(AbstractBody*, AbstractBody*) override;
            void removeOverlappingPairs(AbstractBody*) override;

            const std::vector<std::unique_ptr<OverlappingPair>>& getOverlappingPairs() const override;
            std::vector<OverlappingPair> retrieveCopyOverlappingPairs() const override;

        private:
            mutable std::mutex pairMutex;
    };

}
