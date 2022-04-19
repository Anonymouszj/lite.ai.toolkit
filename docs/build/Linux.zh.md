# Linux 入門教程

* [編譯 Lite.Ai.ToolKit](#lite.ai.toolkit-linuxzh-Self-Compile)
* [在自己的專案中使用 Lite.Ai.ToolKit](#lite.ai.toolkit-linuxzh-Sample-Project)

## 編譯 Lite.Ai.ToolKit
<div id="lite.ai.toolkit-linuxzh-Self-Compile"></div>

在 Linux 之下，編譯 lite.ai.toolkit 共有三個步驟
1. 編譯 OpenCV
2. 下載 Onnx Runtime 動態庫
3. 編譯 Lite.Ai.ToolKit
### 1. OpenCV
下載原始碼
```
mkdir ~/library
wget https://github.com/opencv/opencv/archive/refs/tags/4.5.2.zip
unzip 4.5.2.zip
```
編譯安裝
```
cd opencv-4.5.2
mkdir build && cd build
cmake .. && make -j8
sudo make install
sudo ldconfig -v
```
查詢已安裝的 opencv 版本
```
opencv_version
> 4.5.2
```
**經過測試目前不支援最新版 v4.5.5**
### 2. ONNX Runtime
下載官方建構的動態庫(不需要再從原始碼進行編譯)
```
cd ~/library
wget https://github.com/microsoft/onnxruntime/releases/download/v1.7.0/onnxruntime-linux-x64-1.7.0.tgz
tar zxvf onnxruntime-linux-x64-1.7.0.tgz
```
可以看到 lib 下面有編譯好的動態庫
```
ls onnxruntime-linux-x64-1.7.0/lib
> libonnxruntime.so  libonnxruntime.so.1.7.0
```

### 3. Lite.Ai.ToolKit
下載原始碼
```
cd ~/library
git clone https://github.com/DefTruth/lite.ai.toolkit.git
```
* 複製自行編譯的 opencv 動態庫到 lib/linux 之下
* 複製官方編譯的 onnxruntime 動態庫到 lib/linux 之下
```
cd lite.ai.toolkit
cp ~/library/opencv-4.5.2/build/lib/* lib/linux
cp ~/library/onnxruntime-linux-x64-1.7.0/lib/* lib/linux
```
編譯 lite.ai.toolkit
```
sh ./build.sh
```
執行測試範例
```
cd build/lite.ai.toolkit/bin
./lite_yolov5
```
以下為輸出 LOG，如果執行成功，偵測結果會被存在 logs 之下
```
LITEORT_DEBUG LogId: ../../../hub/onnx/cv/yolov5s.onnx
=============== Input-Dims ==============
input_node_dims: 1
input_node_dims: 3
input_node_dims: 640
input_node_dims: 640
=============== Output-Dims ==============
Output: 0 Name: pred Dim: 0 :1
Output: 0 Name: pred Dim: 1 :25200
Output: 0 Name: pred Dim: 2 :85
Output: 1 Name: output2 Dim: 0 :1
Output: 1 Name: output2 Dim: 1 :3
Output: 1 Name: output2 Dim: 2 :80
Output: 1 Name: output2 Dim: 3 :80
Output: 1 Name: output2 Dim: 4 :85
Output: 2 Name: output3 Dim: 0 :1
Output: 2 Name: output3 Dim: 1 :3
Output: 2 Name: output3 Dim: 2 :40
Output: 2 Name: output3 Dim: 3 :40
Output: 2 Name: output3 Dim: 4 :85
Output: 3 Name: output4 Dim: 0 :1
Output: 3 Name: output4 Dim: 1 :3
Output: 3 Name: output4 Dim: 2 :20
Output: 3 Name: output4 Dim: 3 :20
Output: 3 Name: output4 Dim: 4 :85
========================================
detected num_anchors: 25200
generate_bboxes num: 39
ONNXRuntime Version Detected Boxes Num: 4
```
## 在自己的案例中使用 Lite.Ai.ToolKit
<div id="lite.ai.toolkit-linuxzh-Sample-Project"></div>

## opencv使用注意问题
如果需要opencv支持mp4格式的视频编解码，还需要在编译opencv时，开启-DWITH_FFMPEG=ON支持，并先安装ffmpeg，安装方法可以参考README.md文档中的说明。

TODO
