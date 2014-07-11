Gem::Specification.new do |gem|
  gem.name        = 'mytest'
  gem.version     = '0.1'
  gem.date        = '2014-04-28'
  gem.summary     = "Test of Ruby and C in a gem"
  gem.description = "See summary"
  gem.authors     = ["Chris Boursnell"]
  gem.email       = 'cmb211@cam.ac.uk'
  gem.files       = `git ls-files`.split("\n")
  gem.require_paths = %w( lib ext)
  gem.homepage    = 'http://rubygems.org/gems/mytest'
  gem.license     = 'MIT'

  gem.add_dependency 'rake'
  gem.add_dependency 'trollop'
  gem.add_dependency 'rake-compiler'

  gem.add_development_dependency 'turn'
  gem.add_development_dependency 'simplecov'
  gem.add_development_dependency 'shoulda-context'
  gem.add_development_dependency 'coveralls', '>= 0.6.7'
end