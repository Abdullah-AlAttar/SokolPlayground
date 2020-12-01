project('sokolSample', 'c', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

CXX = meson.get_compiler('cpp')
libs_names = []

plat_ops = { }


if host_machine.system() == 'windows'
  libs_names = [
    # 'GL'
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


sokol_dep = dependency('sokol', fallback : ['sokol', 'sokol_dep'])
imgui_dep = dependency('imgui', fallback : ['imgui', 'imgui_dep'])

deps += [glm_dep, sokol_dep, imgui_dep]


configure_file(input: 'sokol-shdc', output: 'sokol-shdc', copy : true)
configure_file(input: 'sokol-shdc.exe', output: 'sokol-shdc.exe', copy : true)

subdir('src')

