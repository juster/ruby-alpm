require 'test/unit'
require 'alpm'

# Keep in mind alpm is automatically initialized when the module is loaded.

class InitRelease < Test::Unit::TestCase
  def test_extra_release
    Alpm.release

    # Releasing alpm while it is un-initialized will raise an error.
    begin
      Alpm.release
      assert false, 'Calling release twice should have thrown an error'
    rescue AlpmError => pmerr
      assert_equal pmerr.to_s, 'library not initialized'
    ensure
      Alpm.init # re-initialize it for other tests
    end
  end
  
  def test_extra_init
    begin
      Alpm.init
      assert false, 'Calling an extra init should have thrown an error'
    rescue AlpmError => pmerr
      assert_equal pmerr.to_s, 'library already initialized'
    end
  end

  def test_release_init
    Alpm.release
    Alpm.init
    assert true, 'Release and init work for expected use'
  end
end
