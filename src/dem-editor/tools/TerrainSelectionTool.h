#pragma once

#include "gaunlet/prefab/editor-tools/EntitySelectionTool.h"
#include "dem-editor/graphics/components/terrain/PlaneComponent.h"
#include "dem-editor/graphics/components/terrain/HeightmapComponent.h"
#include "dem-editor/graphics/render-pipeline/extensions/TerrainLocationExtension.h"

namespace DemEditor {

    class TerrainSelectionTool : public gaunlet::Prefab::EditorTools::EntitySelectionTool {

    protected:

        bool isHoveringTerrain() {

            auto renderPanel = getWorkspace()->getHoveredRenderPanel();
            if (!renderPanel || !renderPanel->getRenderPipeline()->hasExtension<TerrainLocationExtension>()) {
                return false;
            }

            auto plane = mousePickTaggedEntity<PlaneComponent>(renderPanel, gaunlet::Prefab::RenderPipelineExtensions::EntitySelectionExtension::EntityLayer::SceneLayer);
            if (!plane || !plane.hasComponent<HeightmapComponent>()) {
                return false;
            }

            return true;

        }

        gaunlet::Scene::Entity mousePickTerrain() {

            selectHoveredRenderPanelWithExtension<TerrainLocationExtension>();

            if (!m_renderPanel) {
                return {};
            }

            auto plane = mousePickTaggedEntity<PlaneComponent>(
                m_renderPanel,
                gaunlet::Prefab::RenderPipelineExtensions::EntitySelectionExtension::EntityLayer::SceneLayer
            );

            if (plane && plane.hasComponent<HeightmapComponent>()) {
                return plane;
            }

            return {};

        }

        glm::vec4 mousePickTerrainLocation(gaunlet::Editor::RenderPanel* renderPanel) {

            unsigned int pixelPositionX = renderPanel->getMousePositionX() * gaunlet::Core::Window::getCurrentInstance()->getDPI();
            unsigned int pixelPositionY = renderPanel->getMousePositionYInverted() * gaunlet::Core::Window::getCurrentInstance()->getDPI();

            return renderPanel->getRenderPipeline()
                ->getExtension<TerrainLocationExtension>()
                ->mousePickTerrainLocation(pixelPositionX, pixelPositionY);

        }

    };

}