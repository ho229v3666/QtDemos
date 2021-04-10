# QtDemos
English | [Chinese](./README.CN.md)

![windows](https://img.shields.io/badge/Windows-passing-green.svg?style=flat-square)
![linux](https://img.shields.io/badge/Linux-passing-green.svg?style=flat-square)


* This is a project practice about `Qt5`. Some controls and `video player` will be added in the future. Welcome to comment, learn and test.

| Name | Description              |
| ---- | ---------------- |
| [CustomWidgetDemos](./CustomWidgetDemos) | Custom animation controls and examples |
| [MultithreadedDownloader](./MultithreadedDownloader) | Multithreaded Downloader |
| [QmlDemo](./QmlDemo) | QML Learning project |
| [QmlFireworks](./QmlFireworks) | QML Fireworks (Particle System) Demo |

## Development Environment
* Tool Kit : `Qt 5.15.2`
* Complier : `Microsoft Visual C++ 2019` , `GCC 10`

## Build
* Need `Qt5` environment.
```shell
sudo pacman -S qt5  # Arch Linux
```
* Clone the repository locally and compile the project.
```shell
git clone https://github.com/ho229v3666/QtDemos.git
     # or https://gitee.com/ho229/QtDemos.git
cd QtDemos
mkdir build
cd build
qmake ..
make -j
```
## Author
* QQ : 2189684957
* E-mail : <ho229v3666@gmail.com>