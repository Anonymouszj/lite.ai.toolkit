//
// Created by YanJun Qiu on 2021/3/14.
//

#include "fsanet.h"

using ortcv::FSANet;

FSANet::FSANet(const std::string &_var_onnx_path, const std::string &_conv_onnx_path,
               unsigned int _num_threads) : var_onnx_path(_var_onnx_path.data()),
                                            conv_onnx_path(_conv_onnx_path.data()),
                                            num_threads(_num_threads) {
  ort_env = ort::Env(ORT_LOGGING_LEVEL_ERROR, "fsanet-onnx");
  // 0. session options
  ort::SessionOptions session_options;
  session_options.SetIntraOpNumThreads(num_threads);
  session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
  session_options.SetLogSeverityLevel(4);
  // 1. session
  ort_var_session = new ort::Session(ort_env, var_onnx_path, session_options);
  ort_conv_session = new ort::Session(ort_env, conv_onnx_path, session_options);
  // 2. input info.
  ort::AllocatorWithDefaultOptions allocator;
  input_name = ort_var_session->GetInputName(0, allocator);
  input_node_names.resize(1);
  input_node_names[0] = input_name;
  // 3. type info.
  ort::TypeInfo type_info = ort_var_session->GetInputTypeInfo(0);
  auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
  input_tensor_size = 1;
  input_node_dims = tensor_info.GetShape();
  for (unsigned int i = 0; i < input_node_dims.size(); ++i)
    input_tensor_size *= input_node_dims.at(i);
  input_tensor_values.resize(input_tensor_size);
#if LITEORT_DEBUG
  for (unsigned int i = 0; i < input_node_dims.size(); ++i)
    std::cout << "input_node_dims: " << input_node_dims.at(i) << "\n";
#endif
}

FSANet::~FSANet() {
  if (ort_var_session)
    delete ort_var_session;
  ort_var_session = nullptr;
  if (ort_conv_session)
    delete ort_conv_session;
  ort_conv_session = nullptr;
}

void FSANet::preprocess(const cv::Mat &mat) {
  cv::Mat canva;
  // 0. padding
  if (use_padding) {
    const int h = mat.rows;
    const int w = mat.cols;
    const int nh = static_cast<int>(
        (static_cast<float>(h) + pad * static_cast<float>(h)));
    const int nw = static_cast<int>(
        (static_cast<float>(w) + pad * static_cast<float>(w)));

    const int nx1 = std::max(0, static_cast<int>((nw - w) / 2));
    const int ny1 = std::max(0, static_cast<int>((nh - h) / 2));
    const int nx2 = std::min(nw, nx1 + w);
    const int ny2 = std::min(nh, ny1 + h);
    canva = cv::Mat(nh, nw, CV_8UC3, cv::Scalar(0, 0, 0));
    mat.copyTo(canva(cv::Rect(nx1, ny1, w, h)));

  } else { mat.copyTo(canva); }

  // 1. resize & normalize
  cv::Mat resize_norm;
  cv::resize(canva, canva, cv::Size(input_width, input_height));
  // reference: https://blog.csdn.net/qq_38701868/article/details/89258565
  canva.convertTo(resize_norm, CV_32FC3, 1.f / 127.5f, -1.0f);

  // 2. convert to tensor.
  std::vector<cv::Mat> channels;
  cv::split(resize_norm, channels);
  std::vector<float> channel_values;
  channel_values.resize(input_height * input_width);
  for (unsigned int i = 0; i < channels.size(); ++i) {
    channel_values.clear();
    channel_values = channels.at(i).reshape(1, 1);
    std::memcpy(input_tensor_values.data() + i * (input_height * input_width),
                channel_values.data(),
                input_height * input_width * sizeof(float)); // CXHXW
  }
}

void FSANet::detect(const cv::Mat &mat, types::EulerAngles &euler_angles) {
  this->preprocess(mat);

  // 1. convert to tensor
  ort::Value input_tensor = ort::Value::CreateTensor<float>(
      memory_info, input_tensor_values.data(),
      input_tensor_size, input_node_dims.data(),
      4
  );
  // 2. inference angles (1,3)
  auto output_var_tensors = ort_var_session->Run(
      ort::RunOptions{nullptr}, input_node_names.data(),
      &input_tensor, 1, output_node_names.data(), 1
  );
  auto output_conv_tensors = ort_conv_session->Run(
      ort::RunOptions{nullptr}, input_node_names.data(),
      &input_tensor, 1, output_node_names.data(), 1
  );
  const float *var_angles = output_var_tensors.front().GetTensorMutableData<float>();
  const float *conv_angles = output_conv_tensors.front().GetTensorMutableData<float>();
  const float mean_yaw = (var_angles[0] + conv_angles[0]) / 2.0f;
  const float mean_pitch = (var_angles[1] + conv_angles[1]) / 2.0f;
  const float mean_roll = (var_angles[2] + conv_angles[2]) / 2.0f;

  euler_angles.yaw = mean_yaw;
  euler_angles.pitch = mean_pitch;
  euler_angles.roll = mean_roll;
}


















































