project('hello', [ 'c', 'cpp' ],
    default_options : [
        'cpp_std=c++20',
        'warning_level=3',
        'default_library=static',
        'werror=true'
    ]
)

glad = subproject('glad').get_variable('glad')

glfw = dependency('glfw3', fallback : [ 'glfw', 'glfw3' ])

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
    'src/program.cpp'
]

executable('hello', src,
    include_directories : [ 'src' ],
    dependencies : [ glad, glfw ]
)

