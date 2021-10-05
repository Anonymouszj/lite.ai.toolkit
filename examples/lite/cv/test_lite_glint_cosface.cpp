//
// Created by DefTruth on 2021/7/1.
//

#include "lite/lite.h"

static void test_default()
{
  std::string onnx_path = "../../../hub/onnx/cv/glint360k_cosface_r100.onnx";
  std::string test_img_path0 = "../../../examples/lite/resources/test_lite_glint_cosface_0.png";
  std::string test_img_path1 = "../../../examples/lite/resources/test_lite_glint_cosface_1.png";

  lite::cv::faceid::GlintCosFace *glint_cosface = new lite::cv::faceid::GlintCosFace(onnx_path);

  lite::cv::types::FaceContent face_content0, face_content1;
  cv::Mat img_bgr0 = cv::imread(test_img_path0);
  cv::Mat img_bgr1 = cv::imread(test_img_path1);
  glint_cosface->detect(img_bgr0, face_content0);
  glint_cosface->detect(img_bgr1, face_content1);

  if (face_content0.flag && face_content1.flag)
  {
    float sim = lite::cv::utils::math::cosine_similarity<float>(
        face_content0.embedding, face_content1.embedding);
    std::cout << "Default Version Detected Sim: " << sim << std::endl;
  }

  delete glint_cosface;
}

static void test_onnxruntime()
{
#ifdef ENABLE_ONNXRUNTIME
  std::string onnx_path = "../../../hub/onnx/cv/glint360k_cosface_r100.onnx";
  std::string test_img_path0 = "../../../examples/lite/resources/test_lite_glint_cosface_0.png";
  std::string test_img_path1 = "../../../examples/lite/resources/test_lite_glint_cosface_2.png";

  lite::onnxruntime::cv::faceid::GlintCosFace *glint_cosface =
      new lite::onnxruntime::cv::faceid::GlintCosFace(onnx_path);

  lite::onnxruntime::cv::types::FaceContent face_content0, face_content1;
  cv::Mat img_bgr0 = cv::imread(test_img_path0);
  cv::Mat img_bgr1 = cv::imread(test_img_path1);
  glint_cosface->detect(img_bgr0, face_content0);
  glint_cosface->detect(img_bgr1, face_content1);

  if (face_content0.flag && face_content1.flag)
  {
    float sim = lite::onnxruntime::cv::utils::math::cosine_similarity<float>(
        face_content0.embedding, face_content1.embedding);
    std::cout << "ONNXRuntime Version Detected Sim: " << sim << std::endl;
  }

  delete glint_cosface;
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
