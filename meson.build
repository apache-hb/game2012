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

executable('hello', 'src/main.cpp', dependencies : [ glad, glfw ])
