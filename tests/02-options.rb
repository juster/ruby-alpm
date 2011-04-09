require 'test/unit'
require 'alpm'

class AlpmOptTest < Test::Unit::TestCase
  def test_stringopts
    assert_equal( nil, Alpm.root, 'Unset string options are nil' )

    Alpm.root = '/'
    assert_equal( '/', Alpm.root, 'Root option sticks' )

    Alpm.root = ''
    assert_equal( '/', Alpm.root, 'Forward-slash is appended to dirs' )

    Alpm.dbpath = '/var/lib/pacman'
    assert_equal( '/var/lib/pacman/', Alpm.dbpath, 'DBPath option sticks' )
  end

  def test_strlistopts
    assert_equal( [], Alpm.cachedirs, 'Unset stringlist opts are empty' )

    # Forward-slash is not appended for string lists of dirs
    Alpm.cachedirs = [ '/var/cache/pacman/pkg' ]
    assert_equal( [ '/var/cache/pacman/pkg' ], Alpm.cachedirs,
                  'Cachedirs string array set properly' )
    
  end

  def test_boolopts
    # Boolean options are false by default (right?)
    assert_equal( false, Alpm.usesyslog )

    Alpm.usesyslog = true
    assert Alpm.usesyslog, 'Usesyslog seems to stick'
    Alpm.usesyslog = false
    assert ! Alpm.usesyslog, 'Usesyslog was disabled'
  end
end
