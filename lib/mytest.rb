#!/usr/bin/env ruby

require 'mytest/mytest'

class MyTest

  attr_reader :left, :right

  def add_reads left, right
    @left ||= []
    left = File.expand_path(left)
    if File.exist?(left)
      @left << left
    else
      raise IOError.new("File not found: #{left}")
    end
    @right ||= []
    right = File.expand_path(right)
    if File.exist?(right)
      @right << right
    else
      raise IOError.new("File not found: #{right}")
    end
  end

  # runs
  def run_all output, cutoff=20
    if @left and @right
      File.open("#{output}.1.fastq", "w") do |out1|
        File.open("#{output}.2.fastq", "w") do |out2|
          @left.zip(@right).each do |pair|
            left = File.open(pair[0])
            right = File.open(pair[1])
            name1 = left.readline rescue nil
            name2 = right.readline rescue nil
            while name1 != nil and name2 != nil
              seq1 = left.readline
              plus1 = left.readline
              quals1 = left.readline
              seq2 = right.readline
              plus2 = right.readline
              quals2 = right.readline
              #
              if seq1.length > self.kmer_size + 2
                array1 = self.median(seq1)
                median1 = array1.sort[array1.length/2+1]
              else
                median1 = 0
              end

              if seq2.length > self.kmer_size + 2
                array2 = self.median(seq2)
                median2 = array2.sort[array2.length/2+1]
              else
                median2 = 0
              end
              #
              if median1 and median2
                if median1 <= cutoff and median2 <= cutoff
                  self.add(seq1)
                  self.add(seq2)

                  out1.write "#{name1}#{seq1}+\n#{quals1}"
                  out2.write "#{name2}#{seq2}+\n#{quals2}"
                end
              end
              #
              name1 = left.readline rescue nil
              name2 = right.readline rescue nil
            end
          end
        end
      end
    end
  end
end