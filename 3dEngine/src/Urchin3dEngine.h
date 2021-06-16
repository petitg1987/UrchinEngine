#pragma once

#include <scene/SceneManager.h>
#include <scene/InputDeviceKey.h>

#include <scene/renderer3d/camera/Camera.h>
#include <scene/renderer3d/camera/FpsCamera.h>
#include <scene/renderer3d/camera/FreeCamera.h>
#include <scene/renderer3d/camera/CameraSpaceService.h>
#include <scene/renderer3d/model/Model.h>
#include <scene/renderer3d/lighting/light/sun/SunLight.h>
#include <scene/renderer3d/lighting/light/omnidirectional/OmnidirectionalLight.h>
#include <scene/renderer3d/lighting/light/LightManager.h>
#include <scene/renderer3d/lighting/shadow/ShadowManager.h>
#include <scene/renderer3d/lighting/shadow/light/LightSplitShadowMap.h>
#include <scene/renderer3d/postprocess/antialiasing/AntiAliasingManager.h>
#include <scene/renderer3d/landscape/fog/Fog.h>
#include <scene/renderer3d/landscape/terrain/Terrain.h>
#include <scene/renderer3d/landscape/terrain/TerrainMesh.h>
#include <scene/renderer3d/landscape/terrain/TerrainMaterial.h>
#include <scene/renderer3d/landscape/terrain/grass/TerrainGrass.h>
#include <scene/renderer3d/landscape/water/Water.h>

#include <scene/UI/EventListener.h>
#include <scene/UI/UIRenderer.h>
#include <scene/UI/widget/container/Container.h>
#include <scene/UI/widget/window/Window.h>
#include <scene/UI/widget/button/Button.h>
#include <scene/UI/widget/staticbitmap/StaticBitmap.h>
#include <scene/UI/widget/text/Text.h>
#include <scene/UI/widget/textbox/TextBox.h>
#include <scene/UI/widget/sequence/Sequence.h>
#include <scene/UI/widget/checkbox/CheckBox.h>

#include <i18n/LabelTranslator.h>

#include <graphic/render/shader/config/ShaderConfig.h>

#include <resources/geometry/GeometryModel.h>
#include <resources/geometry/aabbox/AABBoxModel.h>
#include <resources/geometry/capsule/CapsuleModel.h>
#include <resources/geometry/cylinder/CylinderModel.h>
#include <resources/geometry/cone/ConeModel.h>
#include <resources/geometry/frustum/FrustumModel.h>
#include <resources/geometry/obbox/OBBoxModel.h>
#include <resources/geometry/points/PointsModel.h>
#include <resources/geometry/lines/LinesModel.h>
#include <resources/geometry/triangles/TrianglesModel.h>
#include <resources/geometry/sphere/SphereModel.h>
#include <resources/geometry/convexhull/ConvexHullModel.h>
