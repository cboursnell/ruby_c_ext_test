#!/usr/bin/env  ruby

require 'helper'

class TestMyTest < Test::Unit::TestCase

  context 'my_test' do

    setup do

    end

    teardown do
    end

    should 'add two numbers together' do
      assert_equal 10, add(3,7)
    end
  end
end
