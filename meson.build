project('shale-onedrive', 'cpp', default_options: {'cpp_std': 'c++20'})

cmake = import('cmake')

yyjson_sub = cmake.subproject('yyjson')
# sdbus_cpp_sub = cmake.subproject('sdbus-cpp')

boost_dep = dependency('boost', modules: ['program_options'])
fuse_dep = dependency('fuse3')
cpr_dep = dependency('cpr')
simdjson_dep = dependency('simdjson')
yyjson_dep = yyjson_sub.dependency('yyjson')
# sdbus_cpp_dep = sdbus_cpp_sub.dependency('sdbus-c++')
rocksdb_dep = dependency('rocksdb')
mimalloc_dep = dependency('mimalloc')

deplist = [boost_dep, fuse_dep, cpr_dep, simdjson_dep, yyjson_dep, rocksdb_dep, mimalloc_dep]

sources = ['src/main.cpp', 'src/auth.hpp', 'src/constants.h']

executable('shale', sources, dependencies: deplist)
