require 'test/unit'
require 'alpm'

class AlpmGroupTest < Test::Unit::TestCase
  def test_grouphash
    Alpm.root = '/'
    Alpm.dbpath = '/var/lib/pacman'
    grps = Alpm.localdb.groups
    assert grps.instance_of?( Hash ), 'Group cache is returned as a hash'

    grps.each do |grp, pkgs|
      pkgs.each { |pkg| puts "#{grp} #{pkg.name()}" }
    end
  end
end
