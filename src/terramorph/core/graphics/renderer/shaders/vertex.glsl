#version 410 core

struct EntityPropertySet {
    uint position;
    float leftSizeFactor;
    float bottomSizeFactor;
    float rightSizeFactor;
    float topSizeFactor;
    uint textureIndex;
};

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

// Uniforms
layout (std140) uniform EntityPropertySets {
    EntityPropertySet entityPropertySets[100];
};

layout (std140) uniform SceneProperties {
    mat4 view;
    mat4 projection;
    vec4 viewport;
    DirectionalLight directionalLight;
};

struct FrustumPlane {
    vec3 normal;
    float distance;
};

struct CameraFrustum {
    FrustumPlane nearPlane;
    FrustumPlane farPlane;
    FrustumPlane leftPlane;
    FrustumPlane rightPlane;
    FrustumPlane bottomPlane;
    FrustumPlane topPlane;
};

layout (std140) uniform TerrainProperties {
    CameraFrustum cameraFrustum;
    float terrainWidth;
    float terrainDepth;
    float maxHeight;
    float triangleSize;
    float heightmapResolution;
    int entityId;
    vec2 stampUvOrigin;
    float stampUvWidth;
    float stampUvHeight;
};

// Vertex attributes
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in uint a_entityIndex;

// Outputs
out vec2 v_textureCoordinates;
flat out uint v_entityIndex;

// Textures
uniform sampler2D heightmap;

void main() {

    v_textureCoordinates = a_textureCoordinates;
    v_entityIndex = a_entityIndex;

    vec4 vertexPosition = a_position;
    float height = texture(heightmap, v_textureCoordinates).x * maxHeight;
    vertexPosition.y = height;

    gl_Position = vertexPosition;

}