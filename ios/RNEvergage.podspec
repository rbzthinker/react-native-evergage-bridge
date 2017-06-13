
Pod::Spec.new do |s|
  s.name         = "RNEvergage"
  s.version      = "0.0.1"
  s.summary      = "RNEvergage"
  s.description  = <<-DESC
                  RNEvergage
                   DESC
  s.homepage     = ""
  s.license      = "MIT"
  # s.license      = { :type => "MIT", :file => "FILE_LICENSE" }
  s.author             = { "author" => "author@domain.cn" }
  s.platform     = :ios, "7.0"
  s.source       = { :git => "https://github.com/author/RNEvergage.git", :tag => "master" }
  s.source_files  = "RNEvergage/**/*.{h,m}"
  s.requires_arc = true


  s.dependency "React"
  #s.dependency "others"

end

  