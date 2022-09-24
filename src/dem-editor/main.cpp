#include "dem-editor/Application.h"

#include "dem-editor/graphics/render-pipeline/TerrainRenderPipeline.h"

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("DEM Editor");
    gaunlet::Core::RunLoop runLoop(window);
    DemEditor::Application app;

    runLoop.run(app);

    return 0;

}