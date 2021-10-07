//
// Created by DefTruth on 2021/9/20.
//

#include "lite/lite.h"

static void test_default()
{
  std::string onnx_path = "../../../hub/onnx/cv/rvm_mobilenetv3_fp32.onnx";
  std::string video_path = "../../../examples/lite/resources/test_lite_rvm_0.mp4";
  std::string output_path = "../../../logs/test_lite_rvm_0.mp4";

  auto *rvm = new lite::cv::matting::RobustVideoMatting(onnx_path, 16); // 16 threads
  std::vector<lite::types::MattingContent> contents;

  // 1. video matting.
  rvm->detect_video(video_path, output_path, contents, false, 0.4f);

  delete rvm;
}

static void test_onnxruntime()
{
#ifdef ENABLE_ONNXRUNTIME
  std::string onnx_path = "../../../hub/onnx/cv/rvm_mobilenetv3_fp32.onnx";
  std::string video_path = "../../../examples/lite/resources/test_lite_rvm_1.mp4";
  std::string output_path = "../../../logs/test_lite_rvm_1.mp4";

  auto *rvm = new lite::onnxruntime::cv::matting::RobustVideoMatting(onnx_path, 16); // 16 threads
  std::vector<lite::types::MattingContent> contents;

  // 1. video matting.
  rvm->detect_video(video_path, output_path, contents, false, 0.4f);

  delete rvm;
#endif
}

static void test_mnn()
{
#ifdef ENABLE_MNN
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
