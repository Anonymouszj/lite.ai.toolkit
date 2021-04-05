//
// Created by DefTruth on 2021/4/3.
//

#ifndef LITEHUB_ORT_CV_FAST_STYLE_TRANSFER_H
#define LITEHUB_ORT_CV_FAST_STYLE_TRANSFER_H

#include "ort/core/ort_core.h"

namespace ortcv
{
  class FastStyleTransfer : public BasicOrtHandler
  {
  public:
    explicit FastStyleTransfer(const std::string &_onnx_path, unsigned int _num_threads = 1) :
        BasicOrtHandler(_onnx_path, _num_threads)
    {};

    ~FastStyleTransfer()
    {};

  private:
    ort::Value transform(const cv::Mat &mat);

  public:
    void detect(const cv::Mat &mat, types::StyleContent &style_content);
  };
}
#endif //LITEHUB_ORT_CV_FAST_STYLE_TRANSFER_H
