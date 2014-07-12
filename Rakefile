require 'rake/testtask'
require 'rake/extensiontask'

RbConfig::CONFIG['CFLAGS'] = ''

Rake::ExtensionTask.new('mytest') do |ext|
  ext.lib_dir = "lib/mytest"
end

Rake::TestTask.new do |t|
  t.libs << 'test'
end

desc "Run tests"
task :default => :test