//
// Created by DefTruth on 2021/10/7.
//

#ifndef LITE_AI_TOOLKIT_NCNN_CORE_NCNN_CORE_H
#define LITE_AI_TOOLKIT_NCNN_CORE_NCNN_CORE_H

#include "ncnn_config.h"
#include "ncnn_handler.h"
#include "ncnn_types.h"
#include "ncnn_custom.h"

namespace ncnncv
{
  class LITE_EXPORTS NCNNNanoDet;                                // [0] * reference: https://github.com/RangiLyu/nanodet
  class LITE_EXPORTS NCNNNanoDetEfficientNetLite;                // [1] * reference: https://github.com/RangiLyu/nanodet
  class LITE_EXPORTS NCNNNanoDetDepreciated;                     // [2] * reference: https://github.com/RangiLyu/nanodet
  class LITE_EXPORTS NCNNNanoDetEfficientNetLiteDepreciated;     // [3] * reference: https://github.com/RangiLyu/nanodet
  class LITE_EXPORTS NCNNRobustVideoMatting;                     // [4] * reference: https://github.com/PeterL1n/RobustVideoMatting
  class LITE_EXPORTS NCNNYoloX;                                  // [5] * reference: https://github.com/Megvii-BaseDetection/YOLOX
  class LITE_EXPORTS NCNNYOLOP;                                  // [6] * reference: https://github.com/hustvl/YOLOP
  class LITE_EXPORTS NCNNYoloV5;                                 // [7] * reference: https://github.com/ultralytics/yolov5
  class LITE_EXPORTS NCNNYoloX_V_0_1_1;                          // [8] * reference: https://github.com/Megvii-BaseDetection/YOLOX
  class LITE_EXPORTS NCNNYoloR;                                  // [9] * reference: https://github.com/WongKinYiu/yolor
  class LITE_EXPORTS NCNNYoloRssss;                              // [10] * reference: https://github.com/WongKinYiu/yolor
  class LITE_EXPORTS NCNNYoloV5_V_6_0;                           // [11] * reference: https://github.com/ultralytics/yolov5
  class LITE_EXPORTS NCNNYoloV5_V_6_0_P6;                        // [12] * reference: https://github.com/ultralytics/yolov5
  class LITE_EXPORTS NCNNGlintArcFace;                           // [13] * reference: https://github.com/deepinsight/insightface/tree/master/recognition/arcface_torch
  class LITE_EXPORTS NCNNGlintCosFace;                           // [14] * reference: https://github.com/deepinsight/insightface/tree/master/recognition/arcface_torch
  class LITE_EXPORTS NCNNGlintPartialFC;                         // [15] * reference: https://github.com/deepinsight/insightface/tree/master/recognition/partial_fc
  class LITE_EXPORTS NCNNFaceNet;                                // [16] * reference: https://github.com/timesler/facenet-pytorch
  class LITE_EXPORTS NCNNFocalArcFace;                           // [17] * reference: https://github.com/ZhaoJ9014/face.evoLVe.PyTorch
  class LITE_EXPORTS NCNNFocalAsiaArcFace;                       // [18] * reference: https://github.com/ZhaoJ9014/face.evoLVe.PyTorch
  class LITE_EXPORTS NCNNTencentCurricularFace;                  // [19] * reference: https://github.com/Tencent/TFace/tree/master/tasks/distfc
  class LITE_EXPORTS NCNNTencentCifpFace;                        // [20] * reference: https://github.com/Tencent/TFace/tree/master/tasks/cifp
  class LITE_EXPORTS NCNNCenterLossFace;                         // [21] * reference: https://github.com/louis-she/center-loss.pytorch
  class LITE_EXPORTS NCNNSphereFace;                             // [22] * reference: https://github.com/clcarwin/sphereface_pytorch
  class LITE_EXPORTS NCNNMobileFaceNet;                          // [23] * reference: https://github.com/Xiaoccer/MobileFaceNet_Pytorch
  class LITE_EXPORTS NCNNCavaGhostArcFace;                       // [24] * reference: https://github.com/cavalleria/cavaface.pytorch
  class LITE_EXPORTS NCNNCavaCombinedFace;                       // [25] * reference: https://github.com/cavalleria/cavaface.pytorch
  class LITE_EXPORTS NCNNMobileSEFocalFace;                      // [26] * reference: https://github.com/grib0ed0v/face_recognition.pytorch
  class LITE_EXPORTS NCNNUltraFace;                              // [27] * reference: https://github.com/Linzaer/Ultra-Light-Fast-Generic-Face-Detector-1MB
  class LITE_EXPORTS NCNNRetinaFace;                             // [28] * reference: https://github.com/biubug6/Pytorch_Retinaface
  class LITE_EXPORTS NCNNFaceBoxes;                              // [29] * reference: https://github.com/zisianw/FaceBoxes.PyTorch
}

namespace ncnncv
{
  using ncnncore::BasicNCNNHandler;
}

namespace ncnnnlp
{
  using ncnncore::BasicNCNNHandler;
}

namespace ncnnasr
{
  using ncnncore::BasicNCNNHandler;
}

#endif //LITE_AI_TOOLKIT_NCNN_CORE_NCNN_CORE_H
