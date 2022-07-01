//
// Created by DefTruth on 2022/7/1.
//

#include "lite/lite.h"

static void test_default()
{
  std::string onnx_path = "../../../hub/onnx/cv/face_parsing_512x512.onnx";
  std::string test_img_path = "../../../examples/lite/resources/test_lite_face_parsing.png";
  std::string save_img_path = "../../../logs/test_lite_face_parsing_bisenet.jpg";

  lite::cv::segmentation::FaceParsingBiSeNet *face_parsing_bisenet =
      new lite::cv::segmentation::FaceParsingBiSeNet(onnx_path, 4); // 4 threads

  lite::types::FaceParsingContent content;
  cv::Mat img_bgr = cv::imread(test_img_path);
  face_parsing_bisenet->detect(img_bgr, content);

  if (content.flag)
  {
    if (!content.merge.empty()) cv::imwrite(save_img_path, content.merge);
    std::cout << "Default Version FaceParsingBiSeNet Done!" << std::endl;
  }

  delete face_parsing_bisenet;
}

static void test_onnxruntime()
{
#ifdef ENABLE_ONNXRUNTIME
  std::string onnx_path = "../../../hub/onnx/cv/face_parsing_512x512.onnx";
  std::string test_img_path = "../../../examples/lite/resources/test_lite_face_parsing.png";
  std::string save_img_path = "../../../logs/test_lite_face_parsing_bisenet_onnx.jpg";

  lite::onnxruntime::cv::segmentation::FaceParsingBiSeNet *face_parsing_bisenet =
      new lite::onnxruntime::cv::segmentation::FaceParsingBiSeNet(onnx_path, 4); // 4 threads

  lite::types::FaceParsingContent content;
  cv::Mat img_bgr = cv::imread(test_img_path);
  face_parsing_bisenet->detect(img_bgr, content);

  if (content.flag)
  {
    if (!content.merge.empty()) cv::imwrite(save_img_path, content.merge);
    std::cout << "ONNXRuntime Version FaceParsingBiSeNet Done!" << std::endl;
  }

  delete face_parsing_bisenet;
#endif
}

static void test_mnn()
{
#ifdef ENABLE_MNN
  std::string mnn_path = "../../../hub/mnn/cv/face_parsing_512x512.mnn";
  std::string test_img_path = "../../../examples/lite/resources/test_lite_face_parsing.png";
  std::string save_img_path = "../../../logs/test_lite_face_parsing_bisenet_mnn.jpg";

  lite::mnn::cv::segmentation::FaceParsingBiSeNet *face_parsing_bisenet =
      new lite::mnn::cv::segmentation::FaceParsingBiSeNet(mnn_path, 4); // 4 threads

  lite::types::FaceParsingContent content;
  cv::Mat img_bgr = cv::imread(test_img_path);
  face_parsing_bisenet->detect(img_bgr, content);

  if (content.flag)
  {
    if (!content.merge.empty()) cv::imwrite(save_img_path, content.merge);
    std::cout << "MNN Version FaceParsingBiSeNet Done!" << std::endl;
  }

  delete face_parsing_bisenet;
#endif
}

static void test_ncnn()
{
#ifdef ENABLE_NCNN
#endif
}

static void test_tnn()
{
#ifdef ENABLE_TNN
#endif
}

static void test_lite()
{
  test_default();
  test_onnxruntime();
  test_mnn();
  test_ncnn();
  test_tnn();
}

int main(__unused int argc, __unused char *argv[])
{
  test_lite();
  return 0;
}
