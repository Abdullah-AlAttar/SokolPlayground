//------------------------------------------------------------------------------
//  imgui-sapp.c
//
//  Demonstrates Dear ImGui UI rendering via sokol_gfx.h and
//  the utility header sokol_imgui.h
//------------------------------------------------------------------------------

//   # cpp_args : ['-DSOKOL_IMPL', '-DSOKOL_GLCORE33']
// #define SOKOL_IMPL
// #define SOKOL_GLCORE33
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_time.h"
#include "sokol_glue.h"
// #include "triangle.glsl.h"
#include "uniforms.glsl.h"

// #include ""
#include "imgui.h"
// #define SOKOL_IMGUI_IMPL
#include "sokol_imgui.h"

#include "Gui.hpp"

#include <cmath>

static uint64_t last_time = 0;
static bool show_test_window = false;
static bool show_another_window = false;

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
    sq::Gui gui;
} state;


void init(void) {
    // setup sokol-gfx, sokol-time and sokol-imgui
    sg_desc desc = { };
    desc.context = sapp_sgcontext();
    sg_setup(&desc);
    stm_setup();

    sg_shader shd = sg_make_shader(simple_shader_desc());

    /* a vertex buffer with 3 vertices */
    float vertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f,     // bottom left
        0.5f, -0.5f, 0.0f,      // bottom right
        0.0f,  0.5f, 0.0f       // top
    };
    sg_buffer_desc buffer_desc = {};
    buffer_desc.size = sizeof(vertices);
    buffer_desc.content = vertices;
    buffer_desc.label = "triangle_vertices";
    state.bind.vertex_buffers[0] = sg_make_buffer(buffer_desc);
    

    sg_pipeline_desc pipe_desc = {};

    pipe_desc.shader = shd;
    /* if the vertex layout doesn't have gaps, don't need to provide strides and
       offsets */
    sg_layout_desc layout_desc = {};
    layout_desc.attrs[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3;

    pipe_desc.layout = layout_desc;
    pipe_desc.label = "triangle-pipeline";

    state.pip = sg_make_pipeline(pipe_desc);

    // use sokol-imgui with all default-options (we're not doing
    // multi-sampled rendering or using non-default pixel formats)
    state.gui.init();

    // initial clear color
    state.pass_action.colors[0].action = SG_ACTION_CLEAR;
    state.pass_action.colors[0].val[0] = 0.0f;
    state.pass_action.colors[0].val[1] = 0.5f;
    state.pass_action.colors[0].val[2] = 0.7f;
    state.pass_action.colors[0].val[3] = 1.0f;
}

fs_params_t fs_params = {{0.5, 0, 0}};

void frame(void) {
    const int width = sapp_width();
    const int height = sapp_height();
    const double delta_time = stm_sec(stm_laptime(&last_time));
    simgui_new_frame(width, height, delta_time);


    // fs_params.ourColor = { 0.f, greenValue, 0.f, 1.f };
    // fs_params.ourColor[0] = 0.f;
    // fs_params.ourColor[1] = 0.f;
    // fs_params.ourColor[2] = 0.f;
    // fs_params.ourColor[3] = 1.f;


    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    static float f = 0.0f;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
    ImGui::Begin("Hello Imgui", NULL, window_flags);
    ImGui::SetWindowPos({0, 0});
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", &fs_params.ourColor[0]);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window) {
        ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
    }
    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowDemoWindow()
    if (show_test_window) {
        ImGui::SetNextWindowPos(ImVec2(460, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow();
    }
    ImGui::End();

    // the sokol_gfx draw pass
    sg_begin_default_pass(&state.pass_action, width, height);
    sg_apply_pipeline(state.pip);
    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, &fs_params, sizeof(fs_params));

    sg_apply_bindings(&state.bind);
    sg_draw(0, 3, 1);
    simgui_render();
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    simgui_shutdown();
    sg_shutdown();
}

void input(const sapp_event* event) {
    simgui_handle_event(event);
    
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    sapp_desc desc = { };
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = input;
    desc.width = 1024;
    desc.height = 768;
    // desc.gl_force_gles2 = true;
    desc.window_title = "Dear ImGui (sokol-app)";
    desc.ios_keyboard_resizes_canvas = false;
    return desc;
}
