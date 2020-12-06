#ifndef URCHINENGINE_VECTORERASER_H
#define URCHINENGINE_VECTORERASER_H

#include <vector>

namespace urchin {

    class VectorEraser {
        public:
            template<class T> static void erase(std::vector<T>&, std::size_t);
            template<class T> static void erase(std::vector<T>&, typename std::vector<T>::iterator);

    };

    #include "VectorEraser.inl"

}

#endif
