require 'rake/testtask'
require 'rake/extensiontask'

RbConfig::CONFIG['CFLAGS'] = ''

Rake::ExtensionTask.new('mytest')

Rake::TestTask.new do |t|
  t.libs << 'test'
end

desc "Run tests"
task :default => :test