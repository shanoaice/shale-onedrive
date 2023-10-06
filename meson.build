project('shale-onedrive', 'cpp')

boostdep = dependency('boost', modules : ['program_options'])
fusedep = dependency('fuse3')
cprdep = dependency('cpr')

deplist = [boostdep, fusedep, cprdep]

executable('shale', 'src/main.cpp', dependencies : deplist)