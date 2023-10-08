project('shale-onedrive', 'cpp')

boostdep = dependency('boost', modules : ['program_options'])
fusedep = dependency('fuse3')
cprdep = dependency('cpr')
simdjsondep = dependency('simdjson')

deplist = [boostdep, fusedep, cprdep, simdjsondep]

sources = ['src/main.cpp', 'src/auth.cpp', 'src/constants.h']

executable('shale', sources, dependencies : deplist)