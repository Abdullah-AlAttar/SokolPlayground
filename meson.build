project('sokolSample', 'c', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

c_cmplr = meson.get_compiler('cpp')
libs_names = []

plat_ops = { }


if host_machine.system() == 'windows'
  libs_names = [
    'GL'
  ]
  plat_ops += { 'build_directory_name': meson.project_build_root().split('\\').get(-1) }
  plat_ops += { 'sokol_shdc_name': 'sokol-shdc.exe' }
  plat_ops += { 'sokol_shdc_backend': 'hlsl5' }

elif host_machine.system() == 'linux'
  libs_names = [
    'GL',
    'dl',
    'm',
    'glfw3',
    'X11',
    'xi',
    'Xcursor'
  ]
  plat_ops += { 'build_directory_name': meson.project_build_root().split('/').get(-1) }
  plat_ops += { 'sokol_shdc_name': 'sokol-shdc' }
  plat_ops += { 'sokol_shdc_backend': 'glsl330'}
endif

deps = []

foreach lib : libs_names
  tmp = dependency(lib, required : false)
  if not tmp.found()
    tmp = c_cmplr.find_library(lib)
  endif
  deps += tmp
endforeach

glm_dep = dependency('glm', fallback: ['glm', 'glm_dep'])

deps += glm_dep

sokol_includes = ['./third_party/sokol', './third_party/sokol/util']

sokol_deps = declare_dependency(include_directories : include_directories(sokol_includes))

imgui_sources = files([
  './third_party/imgui/imgui.cpp',
  './third_party/imgui/imgui_draw.cpp',
  './third_party/imgui/imgui_widgets.cpp',
  './third_party/imgui/imgui_demo.cpp'
])

imgui_includes = include_directories('./third_party/imgui')

subdir('src')

