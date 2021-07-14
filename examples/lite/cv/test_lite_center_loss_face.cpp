//
// Created by DefTruth on 2021/7/14.
//

#include "lite/lite.h"

static void test_default()
{
  std::string onnx_path = "../../../hub/onnx/cv/CenterLossFace_epoch_100.onnx";
  std::string test_img_path0 = "../../../examples/lite/resources/test_lite_faceid_0.png";
  std::string test_img_path1 = "../../../examples/lite/resources/test_lite_faceid_1.png";

  lite::cv::faceid::CenterLossFace *center_loss_face = new lite::cv::faceid::CenterLossFace(onnx_path);

  lite::cv::types::FaceContent face_content0, face_content1;
  cv::Mat img_bgr0 = cv::imread(test_img_path0);
  cv::Mat img_bgr1 = cv::imread(test_img_path1);
  center_loss_face->detect(img_bgr0, face_content0);
  center_loss_face->detect(img_bgr1, face_content1);

  if (face_content0.flag && face_content1.flag)
  {
    float sim = lite::cv::utils::math::cosine_similarity<float>(
        face_content0.embedding, face_content1.embedding);
    std::cout << "Default Version Detected Sim: " << sim << std::endl;
  }

  delete center_loss_face;
}

static void test_onnxruntime()
{
  std::string onnx_path = "../../../hub/onnx/cv/CenterLossFace_epoch_100.onnx";
  std::string test_img_path0 = "../../../examples/lite/resources/test_lite_faceid_0.png";
  std::string test_img_path1 = "../../../examples/lite/resources/test_lite_faceid_2.png";

  lite::onnxruntime::cv::faceid::CenterLossFace *center_loss_face =
      new lite::onnxruntime::cv::faceid::CenterLossFace(onnx_path);

  lite::onnxruntime::cv::types::FaceContent face_content0, face_content1;
  cv::Mat img_bgr0 = cv::imread(test_img_path0);
  cv::Mat img_bgr1 = cv::imread(test_img_path1);
  center_loss_face->detect(img_bgr0, face_content0);
  center_loss_face->detect(img_bgr1, face_content1);

  if (face_content0.flag && face_content1.flag)
  {
    float sim = lite::onnxruntime::cv::utils::math::cosine_similarity<float>(
        face_content0.embedding, face_content1.embedding);
    std::cout << "ONNXRuntime Version Detected Sim: " << sim << std::endl;
  }

  delete center_loss_face;
}

static void test_mnn()
{

}

static void test_ncnn()
{

}

static void test_tnn()
{

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
