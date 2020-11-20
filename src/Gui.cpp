

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_time.h"
#include "sokol_glue.h"

// #include ""
#include "imgui.h"
#include "sokol_imgui.h"


#include "Gui.hpp"

namespace sq
{
    void Gui::init()
    {
          simgui_desc_t simgui_desc = { };
            simgui_setup(&simgui_desc);  
    }

} // namespace sq
