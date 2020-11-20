



sokol_shdc = find_program('sokol-shdc', dirs : meson.build_root())

shaders = [
     {
       'name' : 'triangle.glsl',
       'path': 'shaders/triangle.glsl',
     },
     {
       'name' : 'uniforms.glsl',
       'path': 'shaders/uniforms.glsl',
     } 
]

foreach shader : shaders
  shader_target = custom_target(
    shader['name'] ,
    input: [shader['path']],
    output: [shader['name'] + '.h'],
    command: [sokol_shdc, '-i', '@INPUT@', '-o', '@OUTPUT@', '-l', 'glsl330'],
    install: true,
    install_dir: join_paths(meson.project_build_root())
  )
endforeach

build_directory_name = meson.project_build_root().split('/').get(-1)

shaders_includes = include_directories(
  join_paths('../', build_directory_name)
)

executable('sokolsample', 
  sources : ['sokolsample.cpp', 'Gui.cpp', 'Unit.cpp',  imgui_sources],
  dependencies : [deps , sokol_deps],
  include_directories : [imgui_includes, shaders_includes],
  # cpp_args : ['-DSOKOL_IMPL', '-DSOKOL_GLCORE33']
)

