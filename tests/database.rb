require 'test/unit'
require 'alpm'

class AlpmDBTest < Test::Unit::TestCase
  def test_localdb
    Alpm.root   = '/'
    Alpm.dbpath = '/var/lib/pacman'
    
    localdb = Alpm.localdb
    assert localdb, 'loaded localdb'
    assert_equal 'local', localdb.name, 'name of local database is "local"'

    %W{ url add_url update }.each do |m|
      assert !localdb.respond_to?( m ), 'local dbs do not have #{m} method'
    end

    assert localdb.respond_to?( 'set_pkg_reason' ),
      'local db does have the set_pkg_reason method'

    assert localdb.pkgs.length > 0, 'local database pkgcache seems to work'
  end
end
