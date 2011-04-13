require 'test/unit'
require 'alpm'

class AlpmDBTest < Test::Unit::TestCase
  def assert_event ( level, msg )
    assert_equal [ level, msg ], @evts.shift
  end

  def test_logcb
    @evts = []
    Alpm.logcb { |lvl, msg| @evts << [ lvl, msg ] }
    Alpm.root = '/'
    assert_event :debug, %Q{option 'root' = /\n}
    Alpm.dbpath = '/var/lib/pacman'
    assert_event :debug, %Q{option 'dbpath' = /var/lib/pacman/\n}
    assert_event :debug, %Q{option 'lockfile' = /var/lib/pacman/db.lck\n}
  end
end
