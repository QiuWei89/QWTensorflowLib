#
# Be sure to run `pod lib lint QWTensorflowLib.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'QWTensorflowLib'
  s.version          = '0.1.0'
  s.summary          = 'QWTensorflowLib on iOS'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
                    Tensorflow lib on iOS using pod
                       DESC

  s.homepage         = 'https://github.com/QiuWei89/QWTensorflow'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'QiuWei89' => 'qiuwei@yy.com' }
  s.source           = { :git => 'https://github.com/QiuWei89/QWTensorflowLib.git', :tag => s.version.to_s }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.platform = :ios
  s.ios.deployment_target = '10.0'

  s.source_files = 'QWTensorflowLib/Classes/**/*'
  s.preserve_paths = 'QWTensorflowLib/lib/**/*'
  s.vendored_libraries = 'QWTensorflowLib/lib/libprotobuf-lite.a','QWTensorflowLib/lib/libprotobuf.a','QWTensorflowLib/lib/libtensorflow-core.a','QWTensorflowLib/lib/libnsync.a'

  s.libraries = 'protobuf-lite', 'protobuf', 'tensorflow-core', 'nsync'

  s.frameworks = 'Accelerate'

  s.pod_target_xcconfig = {"HEADER_SEARCH_PATHS" => "$(inherited) '$(PODS_TARGET_SRCROOT)/QWTensorflowLib/lib' '$(PODS_TARGET_SRCROOT)/QWTensorflowLib/lib/tensorflow/contrib/makefile/downloads' '$(PODS_TARGET_SRCROOT)/QWTensorflowLib/lib/tensorflow/contrib/makefile/downloads/eigen' '$(PODS_TARGET_SRCROOT)/QWTensorflowLib/lib/tensorflow/contrib/makefile/downloads/protobuf/src' '$(PODS_TARGET_SRCROOT)/QWTensorflowLib/lib/tensorflow/contrib/makefile/gen/proto' '$(PODS_TARGET_SRCROOT)/QWTensorflowLib/lib/tensorflow/contrib/makefile/downloads/nsync/public' '$(PODS_TARGET_SRCROOT)/QWTensorflowLib/lib/tensorflow/contrib/makefile/downloads/absl'", 'VALID_ARCHS[sdk=iphonesimulator*]' => ''}

  s.user_target_xcconfig = {"OTHER_LDFLAGS" => ['$(inherited)', '$(PODS_ROOT)/QWTensorflowLib/QWTensorflowLib/lib/libnsync.a', '-force_load', '$(PODS_ROOT)/QWTensorflowLib/QWTensorflowLib/lib/libtensorflow-core.a', '$(PODS_ROOT)/QWTensorflowLib/QWTensorflowLib/lib/libprotobuf-lite.a', '$(PODS_ROOT)/QWTensorflowLib/QWTensorflowLib/lib/libprotobuf.a']}

  s.library = 'c++'

  # s.resource_bundles = {
  #   'QWTensorflowLib' => ['QWTensorflowLib/Assets/*.png']
  # }
  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.frameworks = 'UIKit', 'MapKit'
  # s.dependency 'AFNetworking', '~> 2.3'
end
