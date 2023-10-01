project('onedrive-fuse-layered', 'cpp')

cmake = import('cmake')

libcpr_subproject = cmake.subproject('cpr')

boostdep = dependency('boost', modules : ['program_options'])
fusedep = dependency('fuse3')
cprdep = libcpr_subproject.dependency('cpr')

deplist = [boostdep, fusedep, cprdep]

executable('onedrive-fuse-layered', 'src/main.cpp', dependencies : deplist)