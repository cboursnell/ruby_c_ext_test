require 'simplecov'
require 'coveralls'

SimpleCov.formatter = SimpleCov::Formatter::MultiFormatter[
  SimpleCov::Formatter::HTMLFormatter,
  Coveralls::SimpleCov::Formatter
]
SimpleCov.start

require 'test/unit'
begin; require 'turn/autorun'; rescue LoadError; end
require 'shoulda-context'
require 'mytest.so'
# include MyTest

Turn.config.format = :pretty
Turn.config.trace = 5
