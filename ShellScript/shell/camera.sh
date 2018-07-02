#! /bin/bash

# 有关文件push后重启一下
adb root
adb remount
adb shell mkdir -p /data/vendor/recognition
adb shell mkdir -p /system/app/CameraRecognitionService/lib/arm
adb shell mkdir -p /system/app/TvCamera
adb push CameraRecognitionService.apk /system/app/CameraRecognitionService/CameraRecognitionService.apk
adb push libcamera_recognition.so /system/app/CameraRecognitionService/lib/arm/libcamera_recognition.so
adb push camera_recognition_daemon /system/vendor/bin/camera_recognition_daemon
adb push TvCamera-debug.apk /system/app/TvCamera/TvCamera.apk
adb reboot

# after adb reboot
adb root
adb remount
# 运行Daemon 在拍照模式下要进入设置查看或者测试笑脸拍照时需要运行
adb shell ./system/vendor/bin/camera_recognition_daen &



# 如果在拍照模式下要进入设置查看或者测试笑脸拍照,那么要去掉selinux的限制
adb shell enforce 0
# 去掉selinux的限制后,拍照模式下拍好一张照片后就不能继续预览了
# 如果要测试拍照是否正常,那么需要这样做
adb shell enforce 1

