//
// Created by Robin Grönberg on 04/02/16.
//

#ifndef GRAPHICS_PROJECT_COMPONENTS_H
#define GRAPHICS_PROJECT_COMPONENTS_H

#include "thirdparty/ecs.h"
#include "gfx/Model.h"

namespace engine{
  struct Position : ecs::Property<glm::vec3> {};
  struct Rotation : ecs::Property<glm::vec3> {};
  struct Scale    : ecs::Property<glm::vec3> {};
  struct UVScale  : ecs::Property<glm::vec2> {};
  struct Material{
    glm::vec3 color;
    float metallic;
    float roughness;
    float specular;
  };

  struct RotationVelocity : ecs::Property<glm::vec3> {
    RotationVelocity(float x, float y, float z) : ecs::Property<glm::vec3>(glm::vec3(x,y,z)){}
  };

  class Renderable : public ecs::EntityAlias<Position, Rotation, Scale, gfx::Model>{};
}

#endif //GRAPHICS_PROJECT_COMPONENTS_H
