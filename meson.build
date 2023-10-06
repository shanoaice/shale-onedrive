project('onedrive-fuse-layered', 'cpp')

boostdep = dependency('boost', modules : ['program_options'])
fusedep = dependency('fuse3')
cprdep = dependency('cpr')

deplist = [boostdep, fusedep, cprdep]

executable('onedrive-fuse-layered', 'src/main.cpp', dependencies : deplist)