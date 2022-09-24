#pragma once

#include "gaunlet/editor/workspace/FramebufferRenderPipeline.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/scene/camera/Camera.h"
#include "gaunlet/scene/components/BasicComponents.h"
#include "gaunlet/prefab/object-renderers/skybox-renderer/SkyboxRenderer.h"
#include "dem-editor/graphics/renderer/TerrainRenderer.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"

namespace DemEditor {

    class TerrainRenderPipeline : public gaunlet::Editor::FramebufferRenderPipeline {

    public:

        explicit TerrainRenderPipeline(unsigned int uniformBufferBindingPointOffset = 0);
        void run(const gaunlet::Core::Ref<gaunlet::Scene::Scene>& scene, const gaunlet::Core::Ref<gaunlet::Scene::Camera>& camera, const gaunlet::Core::Ref<gaunlet::Scene::DirectionalLightComponent>& directionalLight, const gaunlet::Core::Ref<gaunlet::Scene::SkyboxComponent>& skybox) override;
        void resize(unsigned int width, unsigned int height) override;
        const gaunlet::Core::Ref<gaunlet::Graphics::Texture>& getRenderedTexture() override;
        int readFramebuffer(unsigned int attachmentIndex, unsigned int x, unsigned int y) override;
        static unsigned int getUniformBufferCount();

    protected:

        static const unsigned int SceneFramebufferAttachmentIndex = 0;
        static const unsigned int SceneEntityIdFramebufferAttachmentIndex = 1;
        static const unsigned int UIEntityIdFramebufferAttachmentIndex = 2;
        static const unsigned int TerrainPositionFramebufferAttachmentIndex = 3;

        void startScene(const gaunlet::Core::Ref<gaunlet::Scene::Scene>& scene, const gaunlet::Core::Ref<gaunlet::Scene::Camera>& camera, const gaunlet::Core::Ref<gaunlet::Scene::DirectionalLightComponent>& directionalLight);
        void drawScene(const gaunlet::Core::Ref<gaunlet::Scene::Scene>& scene);
        void drawUI(const gaunlet::Core::Ref<gaunlet::Scene::Scene>& scene);
        void drawSkybox(const gaunlet::Core::Ref<gaunlet::Scene::SkyboxComponent>& skybox);

        virtual void submitScenePlanes(const gaunlet::Core::Ref<gaunlet::Scene::Scene>& scene);
        virtual void renderSkybox(const gaunlet::Core::Ref<gaunlet::Scene::SkyboxComponent>& skybox);

    protected:

        gaunlet::Core::Ref<gaunlet::Graphics::UniformBuffer> m_scenePropertiesUniformBuffer = nullptr;

        gaunlet::Prefab::ObjectRenderers::SkyboxRenderer m_skyboxRenderer;
        TerrainRenderer m_terrainRenderer;

    private:

        void prepareShaders(unsigned int uniformBufferBindingPointOffset);

        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;

    };

}