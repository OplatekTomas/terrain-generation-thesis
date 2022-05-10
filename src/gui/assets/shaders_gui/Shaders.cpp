//
// Created by tomas on 05.04.22.
//

#include <assets/shaders_gui/Shaders.h>

const char *GUIShaders::lightning_vs =
#include <shaders_gui/include/lightning/lightning_vs.shader.out>
;

const char *GUIShaders::lightning_fs =
#include <shaders_gui/include/lightning/lightning_fs.shader.out>
;

const char *GUIShaders::ssao_fs =
#include <shaders_gui/include/ssao/ssao_fs.shader.out>
;

const char *GUIShaders::blur_fs =
#include <shaders_gui/include/ssao/blur_fs.shader.out>
;

const char *GUIShaders::skybox_fs =
#include <shaders_gui/include/skybox/skybox_fs.shader.out>
;

const char *GUIShaders::skybox_vs =
#include <shaders_gui/include/skybox/skybox_vs.shader.out>
;
