#include "Scene/Components/CRenderable.h"
#include "Scene/Scene.h"
#include "Renderer/MeshRenderable.h"
#include "Renderer/Renderer.h"
EFFECTS_NAMESPACE_BEGIN

CRenderable::CRenderable(SceneNode* node)
    :Component(node) {
    mInternal = new MeshRenderable();
    Renderer::instance().addRenderable(mInternal);
}

CRenderable::~CRenderable() {
}

void CRenderable::initialized() {
    mInternal->initialize();
}

void CRenderable::onNodeDirty() {
    mInternal->setTransform(mNode->getWorldMatrix());
}

void CRenderable::setMesh(std::shared_ptr<Mesh> mesh) {
    mInternal->setMesh(mesh);
}

void CRenderable::setMaterial(std::shared_ptr<Material> material) {
    mInternal->setMaterial(material);
}

std::shared_ptr<Mesh> CRenderable::getMesh() {
    return mInternal->getMesh();
}

std::shared_ptr<Material> CRenderable::getMaterial() {
    return mInternal->getMaterial();
}

EFFECTS_NAMESPACE_END