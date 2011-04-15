# -*- ruby -*-
Gem::Specification.new do |spec|
  spec.name     = 'alpm'
  spec.version  = '0.01'
  spec.author   = 'Justin Davis'
  spec.email    = 'jrcd83@gmail.com'
  spec.summary  = %Q{Interface to libalpm, ArchLinux's package manager library} #'
  spec.homepage = 'https://github.com/juster/ruby-alpm'
  spec.files    = Dir.glob( 'ext/*.[ch]' )
  spec.test_files = Dir.glob( 'tests/*.rb' )
  spec.extensions = 'ext/extconf.rb'
  spec.description = <<'END_DESC'
Alpm is an acronym that stands for ArchLinux Package Manager.
libalpm was separated from pacman, the Archlinux package
manager program, in an attempt to be more modular. The lower
level functions of pacman are available in libalpm and you
can access them easily in ruby with this gem.
END_DESC
end
