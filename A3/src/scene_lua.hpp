#ifndef SCENE_LUA_HPP
#define SCENE_LUA_HPP

#include <string>
#include "scene.hpp"

SceneNode* import_lua(const std::string& filename);

#endif
