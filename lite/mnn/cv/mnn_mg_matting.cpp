//
// Created by DefTruth on 2021/12/5.
//

#include "mnn_mg_matting.h"

using mnncv::MNNMGMatting;

MNNMGMatting::MNNMGMatting(
    const std::string &_mnn_path, unsigned int _num_threads
) : log_id(_mnn_path.data()),
    mnn_path(_mnn_path.data()),
    num_threads(_num_threads)
{
  initialize_interpreter();
  initialize_pretreat();
}

MNNMGMatting::~MNNMGMatting()
{
  mnn_interpreter->releaseModel();
  if (mnn_session)
    mnn_interpreter->releaseSession(mnn_session);
}

void MNNMGMatting::initialize_pretreat()
{
  pretreat = std::shared_ptr<MNN::CV::ImageProcess>(
      MNN::CV::ImageProcess::create(
          MNN::CV::BGR,
          MNN::CV::RGB,
          mean_vals, 3,
          norm_vals, 3
      )
  );
}

void MNNMGMatting::initialize_interpreter()
{
  mnn_interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(mnn_path));
  // 2. init schedule_config
  schedule_config.numThread = (int) num_threads;
  MNN::BackendConfig backend_config;
  backend_config.precision = MNN::BackendConfig::Precision_High; // default Precision_High
  schedule_config.backendConfig = &backend_config;
  // 3. create session
  mnn_session = mnn_interpreter->createSession(schedule_config);
  // 4. init input tensor
  image_tensor = mnn_interpreter->getSessionInput(mnn_session, "image");
  mask_tensor = mnn_interpreter->getSessionInput(mnn_session, "mask");
  dimension_type = image_tensor->getDimensionType(); // CAFFE(NCHW)
}

void MNNMGMatting::transform(const cv::Mat &mat, const cv::Mat &mask)
{
  auto padded_mat = this->padding(mat); // 0-255 int8
  auto padded_mask = this->padding(mask); // 0-1.0 float32

  // update input tensor and resize Session
  mnn_interpreter->resizeTensor(image_tensor, {1, 3, dynamic_input_height, dynamic_input_width});
  mnn_interpreter->resizeSession(mnn_session);
  mnn_interpreter->resizeTensor(mask_tensor, {1, 1, dynamic_input_height, dynamic_input_width});
  mnn_interpreter->resizeSession(mnn_session);

  // push data into image tensor
  pretreat->convert(padded_mat.data, dynamic_input_width, dynamic_input_height,
                    padded_mat.step[0], image_tensor);

  // push data into mask tensor
  auto tmp_host_nchw_tensor = new MNN::Tensor(mask_tensor, MNN::Tensor::CAFFE); // tmp
  std::memcpy(tmp_host_nchw_tensor->host<float>(), padded_mask.data,
              dynamic_input_mask_size * sizeof(float));
  mask_tensor->copyFromHostTensor(tmp_host_nchw_tensor);

  delete tmp_host_nchw_tensor;
}

cv::Mat MNNMGMatting::padding(const cv::Mat &unpad_mat)
{
  const unsigned int h = unpad_mat.rows;
  const unsigned int w = unpad_mat.cols;

  // aligned
  if (h % align_val == 0 && w % align_val == 0)
  {
    unsigned int target_h = h + 2 * align_val;
    unsigned int target_w = w + 2 * align_val;
    cv::Mat pad_mat(target_h, target_w, unpad_mat.type());

    cv::copyMakeBorder(unpad_mat, pad_mat, align_val, align_val,
                       align_val, align_val, cv::BORDER_REFLECT);
    return pad_mat;
  } // un-aligned
  else
  {
    // align & padding
    unsigned int align_h = align_val * ((h - 1) / align_val + 1);
    unsigned int align_w = align_val * ((w - 1) / align_val + 1);
    unsigned int pad_h = align_h - h; // >= 0
    unsigned int pad_w = align_w - w; // >= 0
    unsigned int target_h = h + align_val + (pad_h + align_val);
    unsigned int target_w = w + align_val + (pad_w + align_val);

    cv::Mat pad_mat(target_h, target_w, unpad_mat.type());

    cv::copyMakeBorder(unpad_mat, pad_mat, align_val, pad_h + align_val,
                       align_val, pad_w + align_val, cv::BORDER_REFLECT);
    return pad_mat;
  }
}

void MNNMGMatting::update_guidance_mask(cv::Mat &mask, unsigned int guidance_threshold)
{
  if (mask.type() != CV_32FC1) mask.convertTo(mask, CV_32FC1);
  if (mask.isContinuous())
  {
    const unsigned int h = mask.rows;
    const unsigned int w = mask.cols;
    const unsigned int data_size = h * w * 1;
    float *mutable_data_ptr = (float *) mask.data;
    float guidance_threshold_ = (float) guidance_threshold;
    for (unsigned int i = 0; i < data_size; ++i)
    {
      if (mutable_data_ptr[i] >= guidance_threshold_)
        mutable_data_ptr[i] = 1.0f;
      else
        mutable_data_ptr[i] = 0.0f;
    }
  } //
  else
  {
    float guidance_threshold_ = (float) guidance_threshold;
    for (unsigned int i = 0; i < mask.rows; ++i)
    {
      float *p = mask.ptr<float>(i);
      for (unsigned int j = 0; j < mask.cols; ++j)
      {
        if (p[j] >= guidance_threshold_)
          p[j] = 1.0;
        else
          p[j] = 0.;
      }
    }
  }
}

void MNNMGMatting::detect(const cv::Mat &mat, cv::Mat &mask, types::MattingContent &content,
                          unsigned int guidance_threshold)
{
  if (mat.empty() || mask.empty()) return;
  const unsigned int img_height = mat.rows;
  const unsigned int img_width = mat.cols;
  this->update_dynamic_shape(img_height, img_width);
  this->update_guidance_mask(mask, guidance_threshold); // -> float32 hw1 0~1.0

  // 1. make input tensors, image, mask
  this->transform(mat, mask);
  // 2. inference & run session
  mnn_interpreter->runSession(mnn_session);

  auto output_tensors = mnn_interpreter->getSessionOutputAll(mnn_session);
  // 3. generate matting
  this->generate_matting(output_tensors, mat, content);
}

void MNNMGMatting::generate_matting(
    const std::map<std::string, MNN::Tensor *> &output_tensors,
    const cv::Mat &mat, types::MattingContent &content)
{
  // TODO: add post-process as official python implementation.
  // https://github.com/yucornetto/MGMatting/blob/main/code-base/infer.py
  auto device_alpha_os1_ptr = output_tensors.at("alpha_os1"); // e.g (1,1,h+2*pad_val,w+2*pad_val)
  MNN::Tensor host_alpha_os1_tensor(device_alpha_os1_ptr, device_alpha_os1_ptr->getDimensionType());
  device_alpha_os1_ptr->copyToHostTensor(&host_alpha_os1_tensor);

  const unsigned int h = mat.rows;
  const unsigned int w = mat.cols;

  auto output_dims = host_alpha_os1_tensor.shape();
  const unsigned int out_h = output_dims.at(2);
  const unsigned int out_w = output_dims.at(3);
  float *alpha_os1_ptr = host_alpha_os1_tensor.host<float>();

  cv::Mat pred_alpha_mat(out_h, out_w, CV_32FC1, alpha_os1_ptr);
  content.pha_mat = pred_alpha_mat(cv::Rect(align_val, align_val, w, h)).clone();
  content.fgr_mat = mat.mul(content.pha_mat);
  cv::Mat bgmat(h, w, CV_32FC3, cv::Scalar(153.f, 255.f, 120.f)); // background mat
  cv::Mat rest = 1. - content.pha_mat;
  content.merge_mat = content.fgr_mat + bgmat.mul(rest);

  content.fgr_mat.convertTo(content.fgr_mat, CV_8UC3);
  content.merge_mat.convertTo(content.merge_mat, CV_8UC3);

  content.flag = true;
}

void MNNMGMatting::update_dynamic_shape(unsigned int img_height, unsigned int img_width)
{
  // update dynamic input dims
  unsigned int h = img_height;
  unsigned int w = img_width;
  // update dynamic input dims
  if (h % align_val == 0 && w % align_val == 0)
  {
    // aligned
    dynamic_input_height = h + 2 * align_val;
    dynamic_input_width = w + 2 * align_val;
  } // un-aligned
  else
  {
    // align first
    unsigned int align_h = align_val * ((h - 1) / align_val + 1);
    unsigned int align_w = align_val * ((w - 1) / align_val + 1);
    unsigned int pad_h = align_h - h; // >= 0
    unsigned int pad_w = align_w - w; // >= 0
    dynamic_input_height = h + align_val + (pad_h + align_val);
    dynamic_input_width = w + align_val + (pad_w + align_val);
  }

  dynamic_input_image_size = 1 * 3 * dynamic_input_height * dynamic_input_width;
  dynamic_input_mask_size = 1 * 1 * dynamic_input_height * dynamic_input_width;
}

void MNNMGMatting::print_debug_string()
{
  std::cout << "LITEMNN_DEBUG LogId: " << log_id << "\n";
  std::cout << "=============== Input-Dims ==============\n";
  if (image_tensor) image_tensor->printShape();
  if (mask_tensor) mask_tensor->printShape();
  if (dimension_type == MNN::Tensor::CAFFE)
    std::cout << "Dimension Type: (CAFFE/PyTorch/ONNX)NCHW" << "\n";
  else if (dimension_type == MNN::Tensor::TENSORFLOW)
    std::cout << "Dimension Type: (TENSORFLOW)NHWC" << "\n";
  else if (dimension_type == MNN::Tensor::CAFFE_C4)
    std::cout << "Dimension Type: (CAFFE_C4)NC4HW4" << "\n";
  std::cout << "=============== Output-Dims ==============\n";
  auto tmp_output_map = mnn_interpreter->getSessionOutputAll(mnn_session);
  std::cout << "getSessionOutputAll done!\n";
  for (auto it = tmp_output_map.cbegin(); it != tmp_output_map.cend(); ++it)
  {
    std::cout << "Output: " << it->first << ": ";
    it->second->printShape();
  }
  std::cout << "========================================\n";
}