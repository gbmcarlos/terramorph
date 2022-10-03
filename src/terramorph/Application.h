#pragma once

#include "terramorph/graphics/render-pipeline/TerrainRenderPipeline.h"
#include "terramorph/gui/EntityComponentsPanel.h"
#include "terramorph/tools/terrain-deformation/TerrainElevationTool.h"
#include "terramorph/graphics/components/terrain/HeightmapComponent.h"

#include "pch.h"

namespace terramorph {

    class RadialElevationTerrainBrush : public Brush {

    public:

        RadialElevationTerrainBrush()
            : m_stamp(gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>(ASSETS_PATH"/texture-1.jpeg")) {}

        const char * getFragmentShaderPath() override {
            return ASSETS_PATH"/fragment.glsl";
        }

        const gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D>& getBrushStampTexture() override {
            return m_stamp;
        }

    private:

        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> m_stamp;

    };

    class Application : public gaunlet::Core::Application {

    public:

        Application() : gaunlet::Core::Application() {}

        void onReady() override {

            m_workspace = new gaunlet::Editor::Workspace();

            prepareLayout();
            prepareTools();
            prepareScene();

        }

        void onUpdate(gaunlet::Core::TimeStep timeStep) override {
            m_workspace->update(timeStep);
        }

        void onGuiRender() override {
            m_workspace->render();
        }

        void onEvent(gaunlet::Core::Event &event) override {
            m_workspace->onEvent(event);
        }

    private:
        gaunlet::Editor::Workspace* m_workspace = nullptr;

        void prepareLayout() {

            auto window = gaunlet::Core::Window::getCurrentInstance();
            unsigned int viewportWidth = window->getViewportWidth();
            unsigned int viewportHeight = window->getViewportHeight();

            // Set the docking layout
            m_workspace->setLayoutSpec({
                {
                    {gaunlet::Editor::DockSpacePosition::Left, 0.2f, {"Render Panel"}},
                    {gaunlet::Editor::DockSpacePosition::Down, 0.2f,  0, {"Tools Manager"}},
                    {gaunlet::Editor::DockSpacePosition::Down, 0.4f,  0, {"Entity Components"}},
                    {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar}
                }, viewportWidth, viewportHeight
            });

            // Push the GUI panels
            m_workspace->pushPanel("render-panel", new gaunlet::Prefab::GuiPanels::RenderPanelComponentsPanel, "Render Panel");
            m_workspace->pushPanel("components", new EntityComponentsPanel, "Entity Components");
            m_workspace->pushPanel("tools", new gaunlet::Prefab::GuiPanels::ToolsManagerPanel, "Tools Manager");

            // Prepare the Render Panel
            m_workspace->addRenderPipeline("main", gaunlet::Core::CreateRef<TerrainRenderPipeline>(
                "terrain",
                gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>(
                    glm::vec3(0.8f, 0.8f, 0.8f),
                    glm::vec3(-0.2f, -1.0f, -0.3f),
                    0.5f, 0.7f
                ),
                gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>(gaunlet::Core::CreateRef<gaunlet::Prefab::Skyboxes::SimpleSkyboxCubeMap>())
            ));
            m_workspace->pushPanel(
                "main",
                new gaunlet::Editor::RenderPanel(),
                "Scene",
                "main",
                "main",
                "main"
            );

        }

        void prepareTools() {

            auto terrainElevationTool = gaunlet::Core::CreateRef<TerrainElevationTool>();
            terrainElevationTool->addBrush("radial-elevation", gaunlet::Core::CreateRef<RadialElevationTerrainBrush>());
            terrainElevationTool->activateBrush("radial-elevation");

            m_workspace->addTool("fp-camera-controller", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::FirstPersonCameraController>("main", 300.0f, 0.5f));
            m_workspace->addTool("transformer", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::TransformerTool>());
            m_workspace->addTool("terrain-elevation", terrainElevationTool);
            m_workspace->activateTool("terrain-elevation");

        }

        void prepareScene() {

            auto window = gaunlet::Core::Window::getCurrentInstance();
            unsigned int viewportWidth = window->getViewportWidth();
            unsigned int viewportHeight = window->getViewportHeight();

            // Scene components
            auto mainCamera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, -100000.0f);
            m_workspace->addCamera("main", mainCamera);
            mainCamera->setPosition({0.0f, 420.0f, 1000.0f});
            mainCamera->setRotation(-90.0f, -27.0f);

            m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());

            // Scene entities
            auto& mainScene = m_workspace->getScene("main");

            gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> heightmap = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>(ASSETS_PATH"/heightmap.png");

            auto terrain = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>("terrain");
            terrain.addComponent<PlaneComponent>(
                1000.0f, // Plane size
                150.0f, 0.5f, // Quad subdivision
                25.0f, // Triangle size
                100.0f, // Max height
                mainCamera
            );

            terrain.addComponent<HeightmapComponent>(
                heightmap
            );

        }

    };

}