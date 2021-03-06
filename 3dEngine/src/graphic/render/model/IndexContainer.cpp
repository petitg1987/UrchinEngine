#include <cstring>
#include <cstdlib>

#include <graphic/render/model/IndexContainer.h>

namespace urchin {

    IndexContainer::IndexContainer(std::size_t indicesCount, const uint32_t* ptr, bool hasPrimitiveRestartIndex) :
            indicesCount(indicesCount),
            bHasPrimitiveRestartIndex(hasPrimitiveRestartIndex) {
        this->ptr = (uint32_t*)malloc(getBufferSize());
        std::memcpy(this->ptr, ptr, getBufferSize());
    }

    IndexContainer::IndexContainer(const IndexContainer& src) :
            indicesCount(src.indicesCount),
            bHasPrimitiveRestartIndex(src.bHasPrimitiveRestartIndex) {
        this->ptr = (uint32_t*)malloc(getBufferSize());
        std::memcpy(this->ptr, src.ptr, getBufferSize());
    }

    IndexContainer::IndexContainer(IndexContainer&& src) noexcept :
            indicesCount(src.indicesCount),
            ptr(src.ptr),
            bHasPrimitiveRestartIndex(src.bHasPrimitiveRestartIndex) {
        src.ptr = nullptr;
    }

    IndexContainer::~IndexContainer() {
        free(ptr);
    }

    const uint32_t* IndexContainer::getIndices() const {
        return ptr;
    }

    /**
     * @return Number of indices
     */
    std::size_t IndexContainer::getIndicesCount() const {
        return indicesCount;
    }

    /**
     * @return Memory size of one index
     */
    std::size_t IndexContainer::getIndicesSize() {
        return sizeof(uint32_t);
    }

    /**
     * @return Memory size of all the indices
     */
    std::size_t IndexContainer::getBufferSize() const {
        return getIndicesSize() * indicesCount;
    }

    bool IndexContainer::hasPrimitiveRestartIndex() const {
        return bHasPrimitiveRestartIndex;
    }

}
