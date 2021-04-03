# litehub
`litehub` for onnxruntime/ncnn/mnn/tnn. What you see is what you get !.  
This library integrates some interesting models. I use them as practice cases when learning `mobile inference frameworks`.
Most of the models come from `ONNX-Model-Zoo`, `PytorchHub` and `other open source projects`. All models used will be cited. 
Many thanks to these contributors. To be clear, I do not plan to maintain the library diligently, they are just examples of my practice, 
so I am not responsible for any bugs, nor do I plan to fix some of them. What you see is what you get, and hopefully you get something out of it.

## 1. Dependencies.  
* Mac OS.  
install `opencv` and `onnxruntime` libraries use home-brew.
```shell
  brew update
  brew install opencv
  brew install onnxruntime
```
* Linux & Windows. 
  * TODO
  
## 2. Implementation Plans.
* Models Plan: 
  * Maybe `200+` models from community, includes cv & nlp & asr. Most of them come from the ONNX-Model-Zoo & PytorchHub & other open source projects.
* Inference Engine Plans:
  * Doing:
    * [x] `onnxruntime c++`
  * TODO:
    * [x] `NCNN`
    * [x] `MNN`
    * [x] `TNN`

## 3. Usage
Disclaimer: The following test pictures are from the Internet search, if it has any impact on you, please contact me immediately, I will remove it immediately. 
### 3.1 Usage for ONNXRuntime Interfaces.
More examples can find at [ortcv-examples](https://github.com/DefTruth/litehub/tree/main/examples/ort/cv).
#### 3.1.1 Age detection using [GoogleNet](https://github.com/onnx/models/tree/master/vision/body_analysis/age_gender).
```c++
#include <iostream>
#include <vector>
#include "ort/cv/age_googlenet.h"
#include "ort/core/ort_utils.h"

static void test_ortcv_age_googlenet() {
  std::string onnx_path = "../../../hub/onnx/cv/age_googlenet.onnx";
  std::string test_img_path = "../../../examples/ort/resources/test_ortcv_age_googlenet.jpg";
  std::string save_img_path = "../../../logs/test_ortcv_age_googlenet.jpg";

  ortcv::AgeGoogleNet *age_googlenet = new ortcv::AgeGoogleNet(onnx_path);
  ortcv::types::Age age;
  cv::Mat img_bgr = cv::imread(test_img_path);
  age_googlenet->detect(img_bgr, age);
  ortcv::utils::draw_age_inplace(img_bgr, age);
  cv::imwrite(save_img_path, img_bgr);
  std::cout << "Detected Age: " << age.age << std::endl;
  delete age_googlenet;
}

int main(__unused int argc, __unused char *argv[]) {
  test_ortcv_age_googlenet();
  return 0;
}
```  
The output is:  
![](logs/test_ortcv_age_googlenet.jpg)
#### 3.1.2 Facial Landmarks detection using [PFLD](https://github.com/Hsintao/pfld_106_face_landmarks).
```c++
#include <iostream>
#include <vector>
#include "ort/cv/pfld.h"
#include "ort/core/ort_utils.h"

static void test_ortcv_pfld() {
  std::string onnx_path = "../../../hub/onnx/cv/pfld-106-v3.onnx";
  std::string test_img_path = "../../../examples/ort/resources/test_ortcv_pfld.jpg";
  std::string save_img_path = "../../../logs/test_ortcv_pfld.jpg";
  
  ortcv::PFLD *pfld = new ortcv::PFLD(onnx_path);
  ortcv::types::Landmarks landmarks;
  cv::Mat img_bgr = cv::imread(test_img_path);
  pfld->detect(img_bgr, landmarks);
  ortcv::utils::draw_landmarks_inplace(img_bgr, landmarks);
  cv::imwrite(save_img_path, img_bgr);
  std::cout << "Detected Landmarks Num: " << landmarks.points.size() << std::endl;
  delete pfld;
}

int main(__unused int argc, __unused char *argv[]) {
  test_ortcv_pfld();
  return 0;
}
```   
The output is:  
![](logs/test_ortcv_pfld.jpg)
#### 3.1.3 Face detection using [UltraFace](https://github.com/Linzaer/Ultra-Light-Fast-Generic-Face-Detector-1MB).
```c++
#include <iostream>
#include <vector>
#include "ort/cv/ultraface.h"
#include "ort/core/ort_utils.h"

static void test_ortcv_ultraface() {
  std::string onnx_path = "../../../hub/onnx/cv/ultraface-rfb-640.onnx";
  std::string test_img_path = "../../../examples/ort/resources/test_ortcv_ultraface.jpg";
  std::string save_img_path = "../../../logs/test_ortcv_ultraface.jpg";
  
  ortcv::UltraFace *ultraface = new ortcv::UltraFace(onnx_path);
  std::vector<ortcv::types::Boxf> detected_boxes;
  cv::Mat img_bgr = cv::imread(test_img_path);
  ultraface->detect(img_bgr, detected_boxes);
  ortcv::utils::draw_boxes_inplace(img_bgr, detected_boxes);
  cv::imwrite(save_img_path, img_bgr);
  std::cout << "Detected Face Num: " << detected_boxes.size() << std::endl;
  delete ultraface;
}

int main(__unused int argc, __unused char *argv[]) {
  test_ortcv_ultraface();
  return 0;
}
```
The output is:  
![](logs/test_ortcv_ultraface.jpg)  

#### 3.1.4 Emotion detection using [EmotionFerPlus](https://github.com/onnx/models/blob/master/vision/body_analysis/emotion_ferplus).
```c++
#include <iostream>
#include <vector>
#include "ort/cv/emotion_ferplus.h"
#include "ort/core/ort_utils.h"

static void test_ortcv_emotion_ferplus() {
  std::string onnx_path = "../../../hub/onnx/cv/emotion-ferplus-8.onnx";
  std::string test_img_path = "../../../examples/ort/resources/test_ortcv_emotion_ferplus.jpg";
  std::string save_img_path = "../../../logs/test_ortcv_emotion_ferplus.jpg";
  
  ortcv::EmotionFerPlus *emotion_ferplus = new ortcv::EmotionFerPlus(onnx_path);
  ortcv::types::Emotions emotions;
  cv::Mat img_bgr = cv::imread(test_img_path);
  emotion_ferplus->detect(img_bgr, emotions);
  ortcv::utils::draw_emotion_inplace(img_bgr, emotions);
  cv::imwrite(save_img_path, img_bgr);
  std::cout << "Detected Emotion: " << emotions.text << std::endl;
  delete emotion_ferplus;
}

int main(__unused int argc, __unused char *argv[]) {
  test_ortcv_emotion_ferplus();
  return 0;
}
```
The output is:  
![](logs/test_ortcv_emotion_ferplus.jpg)

### 3.2 Usage for NCNN Interfaces.  
* TODO.  
### 3.3 Usage for MNN Interfaces.
* TODO. 
### 3.4 Usage for TNN Interfaces.
* TODO. 

## 4. Reference & Implementation Details  
The `*` symbol indicates that the C ++ inference interface for the model has been implemented, and more models may be added. 
But I don't guarantee that there will be more models.
### 4.1 ONNXRuntime Inference Engine. 
```c++
namespace ortcv {
  class FSANet;              // [0] * reference: https://github.com/omasaht/headpose-fsanet-pytorch
  class PFLD;                // [1] * reference: https://github.com/Hsintao/pfld_106_face_landmarks
  class UltraFace;           // [2] * reference: https://github.com/Linzaer/Ultra-Light-Fast-Generic-Face-Detector-1MB
  class AgeGoogleNet;        // [3] * reference: https://github.com/onnx/models/tree/master/vision/body_analysis/age_gender
  class GenderGoogleNet;     // [4] * reference: https://github.com/onnx/models/tree/master/vision/body_analysis/age_gender
  class EmotionFerPlus;      // [5] * reference: https://github.com/onnx/models/blob/master/vision/body_analysis/emotion_ferplus
  class VGG16Age;            // [6] reference: https://github.com/onnx/models/tree/master/vision/body_analysis/age_gender
  class SSRNet;              // [7] reference: https://github.com/oukohou/SSR_Net_Pytorch
  class ChineseOCR;          // [8] reference: https://github.com/DayBreak-u/chineseocr_lite
  class ChineseOCRAngleNet;  // [9] reference: https://github.com/DayBreak-u/chineseocr_lite
  class ChineseOCRLiteLSTM;  // [10] reference: https://github.com/DayBreak-u/chineseocr_lite
  class ChineseOCRDBNet;     // [11] reference: https://github.com/DayBreak-u/chineseocr_lite
  class MobileNet;           // [12] reference: https://github.com/onnx/models/blob/master/vision/classification/mobilenet
  class ResNet;              // [13] reference: https://github.com/onnx/models/blob/master/vision/classification/resnet
  class SqueezeNet;          // [14] reference: https://github.com/onnx/models/blob/master/vision/classification/squeezenet
  class ShuffleNetV1;         // [15] reference: https://github.com/onnx/models/blob/master/vision/classification/shufflenet
  class ShuffleNetV2;         // [16] reference: https://github.com/onnx/models/blob/master/vision/classification/shufflenet
  class EfficientNetLite4;    // [17] reference: https://github.com/onnx/models/blob/master/vision/classification/efficientnet-lite4
  class YoloV2;              // [18] reference: https://github.com/onnx/models/blob/master/vision/object_detection_segmentation/yolov2-coco
  class YoloV3;              // [19] reference: https://github.com/onnx/models/blob/master/vision/object_detection_segmentation/yolov3
  class YoloV4;              // [20] reference: https://github.com/onnx/models/blob/master/vision/object_detection_segmentation/yolov4
  class YoloV5;              // [21 reference: https://github.com/ultralytics/yolov5
  class TinyYoloV2;          // [22] reference: https://github.com/onnx/models/blob/master/vision/object_detection_segmentation/tiny-yolov2
  class TinyYoloV3;          // [23] reference: https://github.com/onnx/models/blob/master/vision/object_detection_segmentation/tiny-yolov3
  class SSD;                 // [24] reference: https://github.com/onnx/models/blob/master/vision/object_detection_segmentation/ssd
  class SSDMobileNetV1;      // [25] reference: https://github.com/onnx/models/blob/master/vision/object_detection_segmentation/ssd-mobilenetv1
  class ArcFaceResNet;       // [26] reference: https://github.com/onnx/models/blob/master/vision/body_analysis/arcface
  class FastStyleTransfer;   // [27] reference: https://github.com/onnx/models/blob/master/vision/style_transfer/fast_neural_style
  class SubPixelCNN;         // [28] reference: https://github.com/onnx/models/blob/master/vision/super_resolution/sub_pixel_cnn_2016
  class SiggraphColor;       // [29] reference: https://github.com/richzhang/colorization-pytorch
}

```
### 4.2 NCNN Inference Engine. 
* TODO. 
### 4.3 MNN Inference Engine.
* TODO. 
### 4.4 TNN Inference Engine.
* TODO. 

## 5. Become a Contributor  
### 5.1 ONNXRuntime Inference Engine. 
For the single input and multiple output model, you can inherit [BasicOrthHandler](https://github.com/DefTruth/litehub/blob/main/ort/core/ort_handler.h) and then implement the 
`transform` and `detect` interfaces, Or you can implement a `xxx_model.cpp` completely on your own. 
After submitting MR, I will modify it in the same way that I inherited `BasicOrtHandler` and add it to the library.  
For example: 
* inherit [BasicOrthHandler](https://github.com/DefTruth/litehub/blob/main/ort/core/ort_handler.h).
```c++
#include "ort/core/ort_core.h"

namespace ortcv {
  class AgeGoogleNet : public BasicOrtHandler {
  private:
    const float mean_val[3] = {104.0f, 117.0f, 123.0f};
    const float scale_val[3] = {1.0f, 1.0f, 1.0f};
    const unsigned int age_intervals[8][2] =
        {{0, 2},{4, 6},{8, 12},{15, 20},{25, 32},{38, 43},{48, 53},{60, 100}};

  public:
    AgeGoogleNet(const std::string &_onnx_path, unsigned int _num_threads = 1) :
        BasicOrtHandler(_onnx_path, _num_threads) {};
    ~AgeGoogleNet() {};

  private:
    ort::Value transform(const cv::Mat &mat);

  public:
    void detect(const cv::Mat &mat, types::Age &age);
  };
``` 
* implementations for `transform` and `detect` interfaces.
```c++
#include "age_googlenet.h"
#include "ort/core/ort_utils.h"

using ortcv::AgeGoogleNet;

ort::Value AgeGoogleNet::transform(const cv::Mat &mat) {
  cv::Mat canva = mat.clone();
  cv::resize(canva, canva, cv::Size(input_node_dims.at(3),
                                    input_node_dims.at(2)));
  cv::cvtColor(canva, canva, cv::COLOR_BGR2RGB);
  // (1,3,224,224)
  ortcv::utils::transform::normalize_inplace(canva, mean_val, scale_val); // float32

  return ortcv::utils::transform::mat3f_to_tensor(
      canva, input_node_dims, memory_info_handler,
      input_values_handler, ortcv::utils::transform::CHW);
}

void AgeGoogleNet::detect(const cv::Mat &mat, types::Age &age) {
  if (mat.empty()) return;
  // 1. make input tensor
  ort::Value input_tensor = this->transform(mat);
  // 2. inference
  auto output_tensors = ort_session->Run(
      ort::RunOptions{nullptr}, input_node_names.data(),
      &input_tensor, 1, output_node_names.data(), num_outputs
  );
  ort::Value &age_logits = output_tensors.at(0); // (1,8)
  auto age_dims = output_node_dims.at(0);
  unsigned int interval = 0;
  const unsigned int num_intervals = age_dims.at(1); // 8
  const float *pred_logits = age_logits.GetTensorMutableData<float>();
  auto softmax_probs = ortcv::utils::math::softmax<float>(pred_logits, num_intervals, interval);
  const float pred_age = static_cast<float>(
      age_intervals[interval][0] + age_intervals[interval][1]) / 2.0f;
  age.age = pred_age;
  age.age_interval[0] = age_intervals[interval][0];
  age.age_interval[1] = age_intervals[interval][1];
  age.interval_prob = softmax_probs[interval];
  age.flag = true;
}
```
### 5.2 NCNN Inference Engine.
* TODO.
### 5.3 MNN Inference Engine.
* TODO.
### 5.4 TNN Inference Engine.
* TODO. 
