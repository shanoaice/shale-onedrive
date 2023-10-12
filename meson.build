project('shale-onedrive', 'cpp', default_options: 'cpp_std=c++20')

cmake = import('cmake')

yyjson_sub = cmake.subproject('yyjson')

# sdbus_cpp_sub = cmake.subproject('sdbus-cpp')

# workaround for broken rocksdb pkg-config
rocksdb_opt = cmake.subproject_options()
rocksdb_opt.add_cmake_defines(
    {
        'WITH_ALL_TESTS': false,
        'WITH_BENCHMARK_TOOLS': false,
    },
)
rocksdb_sub = cmake.subproject('rocksdb')

boost_dep = dependency('boost', modules: ['program_options'])
fuse_dep = dependency('fuse3')
cpr_dep = dependency('cpr')
simdjson_dep = dependency('simdjson')
yyjson_dep = yyjson_sub.dependency('yyjson')
# sdbus_cpp_dep = sdbus_cpp_sub.dependency('sdbus-c++')
rocksdb_dep = rocksdb_sub.dependency('rocksdb')
mimalloc_dep = dependency('mimalloc')
cli11_dep = dependency('cli11')

deplist = [
    boost_dep,
    fuse_dep,
    cpr_dep,
    simdjson_dep,
    yyjson_dep,
    rocksdb_dep,
    mimalloc_dep,
    cli11_dep,
]

sources = [
    'src/constants.h',
    'src/auth.h',
    'src/auth.cpp',
    'src/onedrive.h',
    'src/onedrive.cpp',
    'src/main.cpp',
]

executable('shale', sources, dependencies: deplist)
