//
// Created by YanJun Qiu on 2021/3/18.
//

#ifndef LITEHUB_ORT_ORT_CORE_H
#define LITEHUB_ORT_ORT_CORE_H

#include "__ort_core.h"
#include "ort_handler.h"
#include "ort_types.h"

// namespace cv2 = cv;

namespace ortcv {
  class FSANet;
  class PFLD;
  class SSRNet;
  class UltraFace;
  class ChineseOCR;
  class ChineseOCRAngleNet;
  class ChineseOCRLiteLSTM;
  class ChineseOCRDBNet;
  class YoloV4;
  class YoloV5;
  class AgeGoogleNet;
  class GenderGoogleNet;
  class ArcFaceResNet;
  class EmotionFerPlus;
  class VGG16Age;

  namespace reid {}
  namespace track {}
  namespace faceid {}
}

namespace ortnlp {
  class TextCNN;
  class ChineseBert;
}

namespace ortcv { using core::BasicOrtHandler; using core::BasicMultiOrtHandler; }
namespace ortnlp { using core::BasicOrtHandler; using core::BasicMultiOrtHandler; }
namespace ortasr { using core::BasicOrtHandler; using core::BasicMultiOrtHandler; }
#endif //LITEHUB_ORT_ORT_CORE_H
