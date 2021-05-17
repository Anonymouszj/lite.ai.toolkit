//
// Created by DefTruth on 2021/3/28.
//

#include "ort_utils.h"

//*************************************** ortcv::utils **********************************************//
// reference: https://github.com/DefTruth/headpose-fsanet-pytorch/blob/master/src/utils.py
void ortcv::utils::draw_axis_inplace(cv::Mat &mat_inplace,
                                     const types::EulerAngles &euler_angles,
                                     float size, int thickness)
{
  if (!euler_angles.flag) return;

  const float pitch = euler_angles.pitch * _PI / 180.f;
  const float yaw = -euler_angles.yaw * _PI / 180.f;
  const float roll = euler_angles.roll * _PI / 180.f;

  const float height = static_cast<float>(mat_inplace.rows);
  const float width = static_cast<float>(mat_inplace.cols);

  const int tdx = static_cast<int>(width / 2.0f);
  const int tdy = static_cast<int>(height / 2.0f);

  // X-Axis pointing to right. drawn in red
  const int x1 = static_cast<int>(size * std::cosf(yaw) * std::cosf(roll)) + tdx;
  const int y1 = static_cast<int>(
                     size * (std::cosf(pitch) * std::sinf(roll)
                             + std::cosf(roll) * std::sinf(pitch) * std::sinf(yaw))
                 ) + tdy;
  // Y-Axis | drawn in green
  const int x2 = static_cast<int>(-size * std::cosf(yaw) * std::sinf(roll)) + tdx;
  const int y2 = static_cast<int>(
                     size * (std::cosf(pitch) * std::cosf(roll)
                             - std::sinf(pitch) * std::sinf(yaw) * std::sinf(roll))
                 ) + tdy;
  // Z-Axis (out of the screen) drawn in blue
  const int x3 = static_cast<int>(size * std::sinf(yaw)) + tdx;
  const int y3 = static_cast<int>(-size * std::cosf(yaw) * std::sinf(pitch)) + tdy;

  cv::line(mat_inplace, cv::Point2i(tdx, tdy), cv::Point2i(x1, y1), cv::Scalar(0, 0, 255), thickness);
  cv::line(mat_inplace, cv::Point2i(tdx, tdy), cv::Point2i(x2, y2), cv::Scalar(0, 255, 0), thickness);
  cv::line(mat_inplace, cv::Point2i(tdx, tdy), cv::Point2i(x3, y3), cv::Scalar(255, 0, 0), thickness);
}

cv::Mat ortcv::utils::draw_axis(const cv::Mat &mat,
                                const types::EulerAngles &euler_angles,
                                float size, int thickness)
{
  if (!euler_angles.flag) return mat;

  cv::Mat mat_copy = mat.clone();
  const float pitch = euler_angles.pitch * _PI / 180.f;
  const float yaw = -euler_angles.yaw * _PI / 180.f;
  const float roll = euler_angles.roll * _PI / 180.f;

  const float height = static_cast<float>(mat_copy.rows);
  const float width = static_cast<float>(mat_copy.cols);

  const int tdx = static_cast<int>(width / 2.0f);
  const int tdy = static_cast<int>(height / 2.0f);

  // X-Axis pointing to right. drawn in red
  const int x1 = static_cast<int>(size * std::cosf(yaw) * std::cosf(roll)) + tdx;
  const int y1 = static_cast<int>(
                     size * (std::cosf(pitch) * std::sinf(roll)
                             + std::cosf(roll) * std::sinf(pitch) * std::sinf(yaw))
                 ) + tdy;
  // Y-Axis | drawn in green
  const int x2 = static_cast<int>(-size * std::cosf(yaw) * std::sinf(roll)) + tdx;
  const int y2 = static_cast<int>(
                     size * (std::cosf(pitch) * std::cosf(roll)
                             - std::sinf(pitch) * std::sinf(yaw) * std::sinf(roll))
                 ) + tdy;
  // Z-Axis (out of the screen) drawn in blue
  const int x3 = static_cast<int>(size * std::sinf(yaw)) + tdx;
  const int y3 = static_cast<int>(-size * std::cosf(yaw) * std::sinf(pitch)) + tdy;

  cv::line(mat_copy, cv::Point2i(tdx, tdy), cv::Point2i(x1, y1), cv::Scalar(0, 0, 255), thickness);
  cv::line(mat_copy, cv::Point2i(tdx, tdy), cv::Point2i(x2, y2), cv::Scalar(0, 255, 0), thickness);
  cv::line(mat_copy, cv::Point2i(tdx, tdy), cv::Point2i(x3, y3), cv::Scalar(255, 0, 0), thickness);

  return mat_copy;
}

cv::Mat ortcv::utils::draw_landmarks(const cv::Mat &mat, types::Landmarks &landmarks)
{
  if (landmarks.points.empty() || !landmarks.flag) return mat;
  cv::Mat mat_copy = mat.clone();
  for (const auto &point: landmarks.points)
    cv::circle(mat_copy, point, 2, cv::Scalar(0, 255, 0), -1);
  return mat_copy;
}

void ortcv::utils::draw_landmarks_inplace(cv::Mat &mat, types::Landmarks &landmarks)
{
  if (landmarks.points.empty() || !landmarks.flag) return;
  for (const auto &point: landmarks.points)
    cv::circle(mat, point, 2, cv::Scalar(0, 255, 0), -1);
}

void ortcv::utils::draw_boxes_inplace(cv::Mat &mat_inplace, const std::vector<types::Boxf> &boxes)
{
  if (boxes.empty()) return;
  for (const auto &box: boxes)
  {
    if (box.flag)
    {
      cv::rectangle(mat_inplace, box.rect(), cv::Scalar(255, 255, 0), 2);
      if (box.label_text)
      {
        std::string label_text(box.label_text);
        label_text = label_text + ":" + std::to_string(box.score).substr(0, 4);
        cv::putText(mat_inplace, label_text, box.tl(), cv::FONT_HERSHEY_SIMPLEX,
                    0.6f, cv::Scalar(0, 255, 0), 2);

      }
    }
  }
}

cv::Mat ortcv::utils::draw_boxes(const cv::Mat &mat, const std::vector<types::Boxf> &boxes)
{
  if (boxes.empty()) return mat;
  cv::Mat canva = mat.clone();
  for (const auto &box: boxes)
  {
    if (box.flag)
    {
      cv::rectangle(canva, box.rect(), cv::Scalar(255, 255, 0), 2);
      if (box.label_text)
      {
        std::string label_text(box.label_text);
        label_text = label_text + ":" + std::to_string(box.score).substr(0, 4);
        cv::putText(canva, label_text, box.tl(), cv::FONT_HERSHEY_SIMPLEX,
                    0.6f, cv::Scalar(0, 255, 0), 2);

      }
    }
  }
  return canva;
}

cv::Mat ortcv::utils::draw_age(const cv::Mat &mat, types::Age &age)
{
  if (!age.flag) return mat;
  cv::Mat canva = mat.clone();
  const unsigned int offset = static_cast<unsigned int>(
      0.1f * static_cast<float>(mat.rows));
  std::string age_text = "Age:" + std::to_string(age.age).substr(0, 4);
  std::string interval_text = "Interval" + std::to_string(age.age_interval[0])
                              + "_" + std::to_string(age.age_interval[1]);
  std::string interval_prob = "Prob:" + std::to_string(age.interval_prob).substr(0, 4);
  cv::putText(canva, age_text, cv::Point2i(10, offset),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 255, 0), 2);
  cv::putText(canva, interval_text, cv::Point2i(10, offset * 2),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(255, 0, 0), 2);
  cv::putText(canva, interval_prob, cv::Point2i(10, offset * 3),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 0, 255), 2);
  return canva;
}

void ortcv::utils::draw_age_inplace(cv::Mat &mat_inplace, types::Age &age)
{
  if (!age.flag) return;
  const unsigned int offset = static_cast<unsigned int>(
      0.1f * static_cast<float>(mat_inplace.rows));
  std::string age_text = "Age:" + std::to_string(age.age).substr(0, 4);
  std::string interval_text = "Interval" + std::to_string(age.age_interval[0])
                              + "_" + std::to_string(age.age_interval[1]);
  std::string interval_prob = "Prob:" + std::to_string(age.interval_prob).substr(0, 4);
  cv::putText(mat_inplace, age_text, cv::Point2i(10, offset),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 255, 0), 2);
  cv::putText(mat_inplace, interval_text, cv::Point2i(10, offset * 2),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(255, 0, 0), 2);
  cv::putText(mat_inplace, interval_prob, cv::Point2i(10, offset * 3),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 0, 255), 2);
}

cv::Mat ortcv::utils::draw_gender(const cv::Mat &mat, types::Gender &gender)
{
  if (!gender.flag) return mat;
  cv::Mat canva = mat.clone();
  const unsigned int offset = static_cast<unsigned int>(
      0.1f * static_cast<float>(mat.rows));
  std::string gender_text = std::to_string(gender.label) + ":"
                            + gender.text + ":" + std::to_string(gender.score).substr(0, 4);
  cv::putText(canva, gender_text, cv::Point2i(10, offset),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 255, 0), 2);
  return canva;
}

void ortcv::utils::draw_gender_inplace(cv::Mat &mat_inplace, types::Gender &gender)
{
  if (!gender.flag) return;
  const unsigned int offset = static_cast<unsigned int>(
      0.1f * static_cast<float>(mat_inplace.rows));
  std::string gender_text = std::to_string(gender.label) + ":"
                            + gender.text + ":" + std::to_string(gender.score).substr(0, 4);
  cv::putText(mat_inplace, gender_text, cv::Point2i(10, offset),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 255, 0), 2);
}

cv::Mat ortcv::utils::draw_emotion(const cv::Mat &mat, types::Emotions &emotions)
{
  if (!emotions.flag) return mat;
  cv::Mat canva = mat.clone();
  const unsigned int offset = static_cast<unsigned int>(
      0.1f * static_cast<float>(mat.rows));
  std::string emotion_text = std::to_string(emotions.label) + ":"
                             + emotions.text + ":" + std::to_string(emotions.score).substr(0, 4);
  cv::putText(canva, emotion_text, cv::Point2i(10, offset),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 255, 0), 2);
  return canva;
}

void ortcv::utils::draw_emotion_inplace(cv::Mat &mat_inplace, types::Emotions &emotions)
{
  if (!emotions.flag) return;
  const unsigned int offset = static_cast<unsigned int>(
      0.1f * static_cast<float>(mat_inplace.rows));
  std::string emotion_text = std::to_string(emotions.label) + ":"
                             + emotions.text + ":" + std::to_string(emotions.score).substr(0, 4);
  cv::putText(mat_inplace, emotion_text, cv::Point2i(10, offset),
              cv::FONT_HERSHEY_SIMPLEX, 0.6f, cv::Scalar(0, 255, 0), 2);
}

// reference: https://github.com/Linzaer/Ultra-Light-Fast-Generic-Face-Detector-1MB/
//            blob/master/ncnn/src/UltraFace.cpp
void ortcv::utils::hard_nms(std::vector<types::Boxf> &input, std::vector<types::Boxf> &output,
                            float iou_threshold, unsigned int topk)
{
  if (input.empty()) return;
  std::sort(input.begin(), input.end(),
            [](const types::Boxf &a, const types::Boxf &b)
            { return a.score > b.score; });
  const unsigned int box_num = input.size();
  std::vector<int> merged(box_num, 0);

  unsigned int count = 0;
  for (unsigned int i = 0; i < box_num; ++i)
  {
    if (merged[i]) continue;
    std::vector<types::Boxf> buf;

    buf.push_back(input[i]);
    merged[i] = 1;

    for (unsigned int j = i + 1; j < box_num; ++j)
    {
      if (merged[j]) continue;

      float iou = static_cast<float>(input[i].iou_of(input[j]));

      if (iou > iou_threshold)
      {
        merged[j] = 1;
        buf.push_back(input[j]);
      }

    }
    output.push_back(buf[0]);

    // keep top k
    count += 1;
    if (count >= topk)
      break;
  }
}

void ortcv::utils::blending_nms(std::vector<types::Boxf> &input, std::vector<types::Boxf> &output,
                                float iou_threshold, unsigned int topk)
{
  if (input.empty()) return;
  std::sort(input.begin(), input.end(),
            [](const types::Boxf &a, const types::Boxf &b)
            { return a.score > b.score; });
  const unsigned int box_num = input.size();
  std::vector<int> merged(box_num, 0);

  unsigned int count = 0;
  for (unsigned int i = 0; i < box_num; ++i)
  {
    if (merged[i]) continue;
    std::vector<types::Boxf> buf;

    buf.push_back(input[i]);
    merged[i] = 1;

    for (unsigned int j = i + 1; j < box_num; ++j)
    {
      if (merged[j]) continue;

      float iou = static_cast<float>(input[i].iou_of(input[j]));
      if (iou > iou_threshold)
      {
        merged[j] = 1;
        buf.push_back(input[j]);
      }
    }

    float total = 0.f;
    for (unsigned int k = 0; k < buf.size(); ++k)
    {
      total += std::expf(buf[k].score);
    }
    types::Boxf rects;
    for (unsigned int l = 0; l < buf.size(); ++l)
    {
      float rate = std::expf(buf[l].score) / total;
      rects.x1 += buf[l].x1 * rate;
      rects.y1 += buf[l].y1 * rate;
      rects.x2 += buf[l].x2 * rate;
      rects.y2 += buf[l].y2 * rate;
      rects.score += buf[l].score * rate;
    }
    rects.flag = true;
    output.push_back(rects);

    // keep top k
    count += 1;
    if (count >= topk)
      break;
  }
}

// reference: https://github.com/ultralytics/yolov5/blob/master/utils/general.py
void ortcv::utils::offset_nms(std::vector<types::Boxf> &input, std::vector<types::Boxf> &output,
                              float iou_threshold, unsigned int topk)
{
  if (input.empty()) return;
  std::sort(input.begin(), input.end(),
            [](const types::Boxf &a, const types::Boxf &b)
            { return a.score > b.score; });
  const unsigned int box_num = input.size();
  std::vector<int> merged(box_num, 0);

  const float offset = 4096.f;
  /** Add offset according to classes.
   * That is, separate the boxes into categories, and each category performs its
   * own NMS operation. The same offset will be used for those predicted to be of
   * the same category. Therefore, the relative positions of boxes of the same
   * category will remain unchanged. Box of different classes will be farther away
   * after offset, because offsets are different. In this way, some overlapping but
   * different categories of entities are not filtered out by the NMS. Very clever!
   */
  for (unsigned int i = 0; i < box_num; ++i)
  {
    input[i].x1 += static_cast<float>(input[i].label) * offset;
    input[i].y1 += static_cast<float>(input[i].label) * offset;
    input[i].x2 += static_cast<float>(input[i].label) * offset;
    input[i].y2 += static_cast<float>(input[i].label) * offset;
  }

  unsigned int count = 0;
  for (unsigned int i = 0; i < box_num; ++i)
  {
    if (merged[i]) continue;
    std::vector<types::Boxf> buf;

    buf.push_back(input[i]);
    merged[i] = 1;

    for (unsigned int j = i + 1; j < box_num; ++j)
    {
      if (merged[j]) continue;

      float iou = static_cast<float>(input[i].iou_of(input[j]));

      if (iou > iou_threshold)
      {
        merged[j] = 1;
        buf.push_back(input[j]);
      }

    }
    output.push_back(buf[0]);

    // keep top k
    count += 1;
    if (count >= topk)
      break;
  }

  /** Substract offset.*/
  if (!output.empty())
  {
    for (unsigned int i = 0; i < output.size(); ++i)
    {
      output[i].x1 -= static_cast<float>(output[i].label) * offset;
      output[i].y1 -= static_cast<float>(output[i].label) * offset;
      output[i].x2 -= static_cast<float>(output[i].label) * offset;
      output[i].y2 -= static_cast<float>(output[i].label) * offset;
    }
  }

}

ort::Value ortcv::utils::transform::create_tensor(const cv::Mat &mat,
                                                  const std::vector<int64_t> &tensor_dims,
                                                  const ort::MemoryInfo &memory_info_handler,
                                                  std::vector<float> &tensor_value_handler,
                                                  unsigned int data_format)
throw(std::runtime_error)
{
  const unsigned int rows = mat.rows;
  const unsigned int cols = mat.cols;
  const unsigned int channels = mat.channels();

  cv::Mat mat_ref;
  if (mat.type() != CV_32FC(channels)) mat.convertTo(mat_ref, CV_32FC(channels));
  else mat_ref = mat; // reference only. zero-time cost. support 1/2/3/... channels

  if (tensor_dims.size() != 4) throw std::runtime_error("dims mismatch.");
  if (tensor_dims.at(0) != 1) throw std::runtime_error("batch != 1");

  // CXHXW
  if (data_format == transform::CHW)
  {

    const unsigned int target_channel = tensor_dims.at(1);
    const unsigned int target_height = tensor_dims.at(2);
    const unsigned int target_width = tensor_dims.at(3);
    const unsigned int target_tensor_size = target_channel * target_height * target_width;
    if (target_channel != channels) throw std::runtime_error("channel mismatch.");

    tensor_value_handler.resize(target_tensor_size);

    cv::Mat resize_mat_ref;
    if (target_height != rows || target_width != cols)
      cv::resize(mat_ref, resize_mat_ref, cv::Size(target_width, target_height));
    else resize_mat_ref = mat_ref; // reference only. zero-time cost.

    std::vector<cv::Mat> mat_channels;
    cv::split(resize_mat_ref, mat_channels);
    std::vector<float> channel_values;
    channel_values.resize(target_height * target_width);
    for (unsigned int i = 0; i < channels; ++i)
    {
      channel_values.clear();
      channel_values = mat_channels.at(i).reshape(1, 1); // flatten
      std::memcpy(tensor_value_handler.data() + i * (target_height * target_width),
                  channel_values.data(),
                  target_height * target_width * sizeof(float)); // CXHXW
    }

    return ort::Value::CreateTensor<float>(memory_info_handler, tensor_value_handler.data(),
                                           target_tensor_size, tensor_dims.data(),
                                           tensor_dims.size());
  }

  // HXWXC
  const unsigned int target_channel = tensor_dims.at(3);
  const unsigned int target_height = tensor_dims.at(1);
  const unsigned int target_width = tensor_dims.at(2);
  const unsigned int target_tensor_size = target_channel * target_height * target_width;
  if (target_channel != channels) throw std::runtime_error("channel mismatch!");
  tensor_value_handler.clear();

  cv::Mat resize_mat_ref;
  if (target_height != rows || target_width != cols)
    cv::resize(mat_ref, resize_mat_ref, cv::Size(target_width, target_height));
  else resize_mat_ref = mat_ref; // reference only. zero-time cost.

  tensor_value_handler.assign(resize_mat_ref.data, resize_mat_ref.data + target_tensor_size);

  return ort::Value::CreateTensor<float>(memory_info_handler, tensor_value_handler.data(),
                                         target_tensor_size, tensor_dims.data(),
                                         tensor_dims.size());
}

cv::Mat ortcv::utils::transform::normalize(const cv::Mat &mat, float mean, float scale)
{
  cv::Mat matf;
  if (mat.type() != CV_32FC3) mat.convertTo(matf, CV_32FC3);
  else matf = mat; // reference
  return (matf - mean) * scale;
}

cv::Mat ortcv::utils::transform::normalize(const cv::Mat &mat, const float *mean, const float *scale)
{
  cv::Mat mat_copy;
  if (mat.type() != CV_32FC3) mat.convertTo(mat_copy, CV_32FC3);
  else mat_copy = mat.clone();
  for (unsigned int i = 0; i < mat_copy.rows; ++i)
  {
    cv::Vec3f *p = mat_copy.ptr<cv::Vec3f>(i);
    for (unsigned int j = 0; j < mat_copy.cols; ++j)
    {
      p[j][0] = (p[j][0] - mean[0]) * scale[0];
      p[j][1] = (p[j][1] - mean[1]) * scale[1];
      p[j][2] = (p[j][2] - mean[2]) * scale[2];
    }
  }
  return mat_copy;
}

void ortcv::utils::transform::normalize(const cv::Mat &inmat, cv::Mat &outmat,
                                        float mean, float scale)
{
  outmat = ortcv::utils::transform::normalize(inmat, mean, scale);
}

void ortcv::utils::transform::normalize_inplace(cv::Mat &mat_inplace, float mean, float scale)
{
  if (mat_inplace.type() != CV_32FC3) mat_inplace.convertTo(mat_inplace, CV_32FC3);
  ortcv::utils::transform::normalize(mat_inplace, mat_inplace, mean, scale);
}

void ortcv::utils::transform::normalize_inplace(cv::Mat &mat_inplace, const float *mean, const float *scale)
{
  if (mat_inplace.type() != CV_32FC3) mat_inplace.convertTo(mat_inplace, CV_32FC3);
  for (unsigned int i = 0; i < mat_inplace.rows; ++i)
  {
    cv::Vec3f *p = mat_inplace.ptr<cv::Vec3f>(i);
    for (unsigned int j = 0; j < mat_inplace.cols; ++j)
    {
      p[j][0] = (p[j][0] - mean[0]) * scale[0];
      p[j][1] = (p[j][1] - mean[1]) * scale[1];
      p[j][2] = (p[j][2] - mean[2]) * scale[2];
    }
  }
}


//*************************************** ortcv::utils **********************************************//









