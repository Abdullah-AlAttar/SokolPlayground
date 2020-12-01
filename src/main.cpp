
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "uniforms.glsl.h"

#include "imgui.h"
#include "sokol_imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>

static uint64_t last_time       = 0;
static bool show_test_window    = false;
static bool show_another_window = false;

static struct
{
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
    struct
    {
        float mouse_x = 0;
        float mouse_y = 0;
    } input_state;
} state;

void init(void)
{
    // setup sokol-gfx, sokol-time and sokol-imgui
    sg_desc desc = {};
    desc.context = sapp_sgcontext();
    sg_setup(&desc);
    stm_setup();

    sg_shader shd = sg_make_shader(simple_uniform_shader_desc());

    float vertices[] = {
        // positions         // texture coords
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f // top left
    };
    sg_buffer_desc vertex_buffer_desc = {};
    vertex_buffer_desc.size           = sizeof(vertices);
    vertex_buffer_desc.content        = vertices;
    vertex_buffer_desc.label          = "quad-vertices";
    state.bind.vertex_buffers[0]      = sg_make_buffer(vertex_buffer_desc);

    /* an index buffer with 2 triangles */
    uint16_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    sg_buffer_desc idx_buffer_desc = {};
    idx_buffer_desc.type           = SG_BUFFERTYPE_INDEXBUFFER;
    idx_buffer_desc.size           = sizeof(indices);
    idx_buffer_desc.content        = indices;
    idx_buffer_desc.label          = "quad-indices";

    state.bind.index_buffer = sg_make_buffer(idx_buffer_desc);

    // Pipelines
    sg_pipeline_desc pipe_desc = {};

    pipe_desc.shader     = shd;
    pipe_desc.index_type = SG_INDEXTYPE_UINT16;
    /* if the vertex layout doesn't have gaps, don't need to provide strides and
       offsets */
    sg_layout_desc layout_desc = {};

    layout_desc.attrs[ATTR_vs_uni_position].format = SG_VERTEXFORMAT_FLOAT3;
    pipe_desc.layout                               = layout_desc;
    pipe_desc.label                                = "triangle-pipeline";

    state.pip = sg_make_pipeline(pipe_desc);
    //---------------------------------------------------------

    // use sokol-imgui with all default-options (we're not doing
    // multi-sampled rendering or using non-default pixel formats)
    simgui_desc_t simgui_desc = {};
    simgui_setup(&simgui_desc);

    // initial clear color
    state.pass_action.colors[0].action = SG_ACTION_CLEAR;
    state.pass_action.colors[0].val[0] = 0.0f;
    state.pass_action.colors[0].val[1] = 0.5f;
    state.pass_action.colors[0].val[2] = 0.7f;
    state.pass_action.colors[0].val[3] = 1.0f;
}

fs_params_t fs_params = { { 0.5, 0, 0 } };

void frame(void)
{
    const int width         = sapp_width();
    const int height        = sapp_height();
    const double delta_time = stm_sec(stm_laptime(&last_time));
    simgui_new_frame(width, height, delta_time);

    // fs_params.ourColor = { 0.f, greenValue, 0.f, 1.f };
    // fs_params.ourColor[0] = 0.f;
    // fs_params.ourColor[1] = 0.f;
    // fs_params.ourColor[2] = 0.f;
    // fs_params.ourColor[3] = 1.f;

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in
    // a window automatically called "Debug"
    static float f                = 0.0f;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
    ImGui::Begin("Hello Imgui", NULL, window_flags);
    ImGui::SetWindowPos({ 0, 0 });
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", &fs_params.ourColor[0]);
    if (ImGui::Button("Test Window"))
        show_test_window ^= 1;
    if (ImGui::Button("Another Window"))
        show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
    }
    // 3. Show the ImGui test window. Most of the sample code is in
    // ImGui::ShowDemoWindow()
    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ImVec2(460, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow();
    }
    ImGui::End();

    vs_params_t vs_params = {};

    glm::mat4 transform = glm::mat4(
        1.0f); // make sure to initialize matrix to identity matrix first
    transform = glm::scale(transform, glm::vec3(0.1, 0.1, 0.1));
    transform = glm::translate(
        transform,
        glm::vec3(state.input_state.mouse_x, state.input_state.mouse_y, 0.0f));
    // transform = glm::rotate(transform, (float)stm_now(), glm::vec3(0.0f,
    // 0.0f, 1.0f));
    const float* glm_mat = (const float*)glm::value_ptr(transform);

    for (int i = 0; i < 16; ++i)
        vs_params.transform[i] = glm_mat[i];

    // the sokol_gfx draw pass
    sg_begin_default_pass(&state.pass_action, width, height);
    sg_apply_pipeline(state.pip);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &vs_params,
                      sizeof(vs_params));

    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, &fs_params,
                      sizeof(fs_params));

    sg_apply_bindings(&state.bind);
    sg_draw(0, 6, 1);
    simgui_render();
    sg_end_pass();
    sg_commit();
}

void cleanup(void)
{
    simgui_shutdown();
    sg_shutdown();
}

void input(const sapp_event* event)
{
    simgui_handle_event(event);
    if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT
        and event->type == SAPP_EVENTTYPE_MOUSE_DOWN)
    {
        state.input_state.mouse_x = event->mouse_x / sapp_width() - 0.5f;
        state.input_state.mouse_y
            = ((sapp_height() - event->mouse_y) / sapp_height() - 0.5f);
        // state.input_state.mouse_y = 0;
        std::cout << state.input_state.mouse_x << " "
                  << state.input_state.mouse_y << std::endl;
    }
}

sapp_desc sokol_main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    sapp_desc desc  = {};
    desc.init_cb    = init;
    desc.frame_cb   = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb   = input;
    desc.width      = 1024;
    desc.height     = 768;
    // desc.gl_force_gles2 = true;
    desc.window_title                = "Dear ImGui (sokol-app)";
    desc.ios_keyboard_resizes_canvas = false;
    return desc;
}
