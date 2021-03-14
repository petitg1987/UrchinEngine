template<class BUILDER> TextureFilter::TextureFilter(const TextureFilterBuilder<BUILDER>* textureFilterBuilder):
    isInitialized(false),
    sourceTexture(textureFilterBuilder->getSourceTexture()),
    depthTexture(textureFilterBuilder->getDepthTexture()),
    textureWidth(textureFilterBuilder->getTextureWidth()),
    textureHeight(textureFilterBuilder->getTextureHeight()),
    textureType(textureFilterBuilder->getTextureType()),
    textureNumberLayer(textureFilterBuilder->getTextureNumberLayer()),
    textureFormat(textureFilterBuilder->getTextureFormat()) {

}