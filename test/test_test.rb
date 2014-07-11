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

    should 'get the count of a kmer before anything is added' do
      assert_equal 0, @test.get("GTCAGAG") # TCTGACG
    end

    should 'get the median count' do
      assert_equal 1, @test.add("TACGTACGTACGTTACGTAC")
      array = @test.median("AACGTACGTACGTAAACGTA")
      assert_equal 1, array.sort[array.length/2+1]

    end

    should 'hash a kmer' do
      assert_equal 70, @test.hashing("AAAAAAC", 0)
      assert_equal 2, @test.hashing("ACGTGCA", 0)
      assert_equal 95, @test.hashing("TTTCCCA", 0)
    end

    should 'hash a kmer with large k' do
      test = MyTest.new(21, 100, 4)
      v = test.hashing("ACGTGCATCGATTCGATCGAT", 0)
    end

    should 'load in lots of reads and get the median' do
      test = MyTest.new(21, 200, 4)
      left = File.join(File.dirname(__FILE__), 'head.left.fastq')
      right = File.join(File.dirname(__FILE__), 'head.right.fastq')
      count=0
      File.open("#{left}").each_line do |line|
        if count % 4 == 1
          # puts line
          array = test.median(line.chomp)
          median = array.sort[array.length/2+1]
          if median < 5
            test.add(line.chomp)
          end
        end
        count += 1
      end
    end
  end
end
