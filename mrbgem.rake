MRuby::Gem::Specification.new('mruby-fatfs-mocloudos') do |spec|
  spec.licenses = ['FatFs License', 'MIT']
  spec.authors = ['Monami-ya LLC, Japan.', 'ChaN']

  spec.cc.include_paths << "#{build.root}/src"
end
