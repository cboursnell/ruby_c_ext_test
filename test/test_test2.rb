#!/usr/bin/env  ruby

require 'helper'

class TestMyTest2 < Test::Unit::TestCase

  context 'my_test_ruby' do

    setup do
      # kmer_size=7, bucket_size=1000, bucket_count=4, cutoff=20
      @test = MyTest.new(7, 100, 4, 20)
    end

    teardown do
      File.delete("output.1.fastq") if File.exist?("output.1.fastq")
      File.delete("output.2.fastq") if File.exist?("output.2.fastq")
    end

    should 'respond to ' do
      assert_respond_to(@test, :add_reads)
      # assert_respond_to(@test, :run) # deprecated
      assert_respond_to(@test, :kmer_size)
    end

    should 'add reads to object' do
      @test.add_reads("test/head.left.fastq", "test/head.right.fastq")
      assert @test.left
      assert @test.right
    end

    should 'raise error if reads not found' do
      assert_raise IOError do
        @test.add_reads("test/not_left.fastq", "test/not_right.fastq")
      end
    end

    # deprecated
    # should 'run on reads' do
    #   @test.add_reads("test/head.left.fastq", "test/head.right.fastq")
    #   @test.run_all("output", 3)
    #   assert File.exist?("output.1.fastq")
    #   assert File.exist?("output.2.fastq")
    # end

    should 'get the kmer size' do
      assert_equal 7, @test.kmer_size
    end

    should 'run the c method' do
      left = "test/head.left.fastq"
      right = "test/head.right.fastq"
      left = File.expand_path(left)
      right = File.expand_path(right)
      @test.run(left, right)
    end
  end
end
