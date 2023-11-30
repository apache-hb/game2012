project('hello', [ 'c', 'cpp' ],
    default_options : [
        'cpp_std=c++20',
        'warning_level=3',
        'default_library=static',
        'werror=true'
    ]
)

glad = subproject('glad').get_variable('glad')
glm = dependency('glm')
glfw = dependency('glfw3')
stb = subproject('stb').get_variable('stb')

# i'd use gltf but im a little short on time
tinyobj = subproject('tinyobj').get_variable('tinyobj')

src = [
    # imgui
    'src/imgui/imgui.cpp',
    'src/imgui/imgui_demo.cpp',
    'src/imgui/imgui_draw.cpp',
    'src/imgui/imgui_widgets.cpp',
    'src/imgui/imgui_tables.cpp',
    'src/imgui/backends/imgui_impl_glfw.cpp',
    'src/imgui/backends/imgui_impl_opengl3.cpp',

    # main
    'src/main.cpp',
    'src/mesh.cpp',
    'src/obj.cpp',
    'src/texture.cpp',
    'src/program.cpp',
    'src/gui.cpp'
]

include_directories = include_directories('src', 'src/simcoe')

executable('hello', src,
    include_directories : include_directories,
    cpp_args : [ '/wd4201' ],
    dependencies : [ glad, glm, glfw, tinyobj, stb ]
)
