#!/usr/bin/env  ruby

require 'helper'

class TestMyTest < Test::Unit::TestCase

  context 'my_test' do

    setup do
      @test = MyTest.new(7, 100, 4)
    end

    teardown do
    end

    should 'add read to cmsketch' do
      assert_equal 1, @test.add("ACGTACGTACGT")
    end

    should 'get the count of a kmer' do
      @test.add("GTCGAGGCCGTCAGGCAT")
      assert_equal 1, @test.get("GTCAGAG") # TCTGACG
    end

    should 'hash a kmer' do
      assert_equal 70, @test.hashing("AAAAAAC", 0)
      assert_equal 2, @test.hashing("ACGTGCA", 0)
      assert_equal 95, @test.hashing("TTTCCCA", 0)
    end
  end
end
