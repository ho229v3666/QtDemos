# QtDemos
### 项目说明
* 一个关于 `Qt5` 的项目实践，未来会添加一些控件和 `Video Player`。欢迎大家留言评论，参考学习和测试。
* 开发环境 :
  * Qt Version : `Qt 5.15.2`
  * Compiler : `MSVC 2019 (Windows)` , `GCC 10 (Linux)`
* | 名称 | 描述              |
  | ---- | ---------------- |
  | [CustomWidgetDemos](./CustomWidgetDemos) | 一些动画控件和使用示例 |
  | [MultithreadedDownloader](./MultithreadedDownloader) | 多线程下载器 |
  | [QmlDemo](./QmlDemo) | QML 学习项目 |
---------
### 如何编译
* 需要 `Qt5` 环境
```shell
sudo pacman -S qt5
```
* 克隆到仓库到本地并编译项目
```shell
git clone https://gitee.com/ho229/QtDemos.git
cd QtDemos
mkdir build
cd build
qmake ..
make -j
```

### 关于作者
* QQ : 2189684957
* E-mail : <ho229v3666@gmail.com>