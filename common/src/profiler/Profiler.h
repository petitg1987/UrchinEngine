#pragma once

#include <memory>
#include <map>
#include <stack>

#include <profiler/ProfilerNode.h>

namespace urchin {

    class Profiler {
        public:
            explicit Profiler(const std::string&);

            static const std::unique_ptr<Profiler>& graphic();
            static const std::unique_ptr<Profiler>& physics();
            static const std::unique_ptr<Profiler>& ai();
            static const std::unique_ptr<Profiler>& sound();

            void startNewProfile(const std::string&);
            void stopProfile(const std::string& nodeName = "");

            void log();

        private:
            bool isEnable;
            std::string instanceName;

            std::unique_ptr<ProfilerNode> profilerRoot;
            ProfilerNode* currentNode;
    };

}
