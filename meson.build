project('shale-onedrive', 'cpp', default_options: {'cpp_std': 'c++20'})

cmake = import('cmake')

yyjson_sub = cmake.subproject('yyjson')
sdbus_cpp_sub = cmake.subproject('sdbus-cpp')

boostdep = dependency('boost', modules: ['program_options'])
fusedep = dependency('fuse3')
cprdep = dependency('cpr')
simdjsondep = dependency('simdjson')
yyjsondep = yyjson_sub.dependency('yyjson')
sdbus_cppdep = sdbus_cpp_sub.dependency('sdbus-c++')

deplist = [boostdep, fusedep, cprdep, simdjsondep, yyjsondep, sdbus_cppdep]

sources = ['src/main.cpp', 'src/auth.cpp', 'src/constants.h']

executable('shale', sources, dependencies: deplist)
