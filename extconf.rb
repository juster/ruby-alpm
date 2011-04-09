require 'mkmf'
unless have_library( 'alpm', 'alpm_initialize' )
  fail 'Pacman/alpm must be installed in order to compile this module'
end
dir_config 'alpm'
create_makefile 'alpm'
