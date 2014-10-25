# IOS

Add cocos2d/plugin/protocols/proj.ios/PluginProtocol.xcodeproj
Add cocos2d/plugin/plugins/admob/proj.ios/PluginAdmob.xcodeproj


XCode [Target] -> [appname iOS] -> [Build Phases] -> 'Link Binary With Libraries' -> [+]

- MediaPlayer.framework
- GameController.framework
- libPluginProtocol.a
- Foundation.framework
- AdSupport.framework
- libPluginAdmob.a
- ImageIO.framework
- CoreTelephony.framework
- StoreKit.framework
- SystemConfiguration.framework
- MessageUI.framework
- libPluginAdmob.a

XCode [Target] -> [appname iOS] -> [Build Phases] ->  'Search Paths' -> [Header Search Paths] add $(SRCROOT)/../cocos2d/plugin/publish/protocols/include

XCode [Target] -> [appname iOS] -> [Build Phases] -> 'Linking' -> [Other Linker Flags]  add '-ObjC'
