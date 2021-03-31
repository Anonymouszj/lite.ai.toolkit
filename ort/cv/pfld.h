//
// Created by YanJun Qiu on 2021/3/14.
//

#ifndef LITEHUB_ORT_CV_PFLD_H
#define LITEHUB_ORT_CV_PFLD_H

#include "ort/core/ort_core.h"

namespace ortcv {

  class PFLD : public BasicOrtHandler {
  private:
    static constexpr const float mean_val = 0.f;
    static constexpr const float scale_val = 1.0f / 255.0f;

  public:

    PFLD(const std::string &_onnx_path, unsigned int _num_threads = 1) :
        BasicOrtHandler(_onnx_path, _num_threads) {};

    ~PFLD() {}; // override

    // un-copyable
  private:
    ort::Value transform(const cv::Mat &mat);

  public:
    void detect(const cv::Mat &mat, types::Landmarksf &landmarksf);

  };
}

#endif //LITEHUB_ORT_CV_PFLD_H
