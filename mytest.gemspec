Gem::Specification.new do |gem|
  gem.name        = 'mytest'
  gem.version     = '0.2'
  gem.date        = '2014-07-12'
  gem.summary     = "Ruby implementation of digital normalisation"
  gem.description = "See summary"
  gem.authors     = ["Chris Boursnell"]
  gem.email       = 'cmb211@cam.ac.uk'
  gem.files       = `git ls-files`.split("\n")
  gem.executables = "diginorm"
  gem.extensions  = ["ext/mytest/extconf.rb"]
  gem.require_paths = %w( lib ext)
  gem.homepage    = 'http://rubygems.org/gems/mytest'
  gem.license     = 'MIT'

  gem.add_dependency 'trollop', '~> 2.0'

  gem.add_development_dependency 'rake', '~> 10.3', '>= 10.3.2'
  gem.add_development_dependency 'rake-compiler', '~> 0.9', '>= 0.9.2'
  gem.add_development_dependency 'turn', '~> 0.9', '>= 0.9.7'
  gem.add_development_dependency 'simplecov', '~> 0.8', '>= 0.8.2'
  gem.add_development_dependency 'shoulda-context', '~> 1.2', '>= 1.2.1'
  gem.add_development_dependency 'coveralls', '~> 0.7'
end