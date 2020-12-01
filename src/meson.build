



sokol_shdc = find_program(plat_ops['sokol_shdc_name'], dirs : meson.project_source_root())

shaders = [
     {
       'name' : 'triangle.glsl',
       'path': join_paths(meson.project_source_root(), 'src/shaders/triangle.glsl'),
     },
     {
       'name' : 'uniforms.glsl',
       'path': join_paths(meson.project_source_root(), 'src/shaders/uniforms.glsl'),
     } 
]

foreach shader : shaders
  shader_target = custom_target(
    shader['name'] ,
    input: [shader['path']],
    output: [shader['name'] + '.h'],
    command: [sokol_shdc, '-i', '@INPUT@', '-o', '@OUTPUT@', '-l', plat_ops['sokol_shdc_backend']],
    install: true,
    install_dir: join_paths(meson.project_build_root())
  )
endforeach

executable('sokolsample', 
  include_directories : include_directories(join_paths('..', plat_ops['build_directory_name'], 'src')),
  dependencies : deps,
  sources : ['main.cpp', 'Unit.cpp'],
)

