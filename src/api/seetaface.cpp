#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <seeta/FaceDetector.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/FaceRecognizer.h>
#include <seeta/FaceTracker.h>
#include <seeta/SeetaNetStruct.h>
#include <seeta/SeetaNetForward.h>
#include <seeta/Struct.h>
#include <seeta/Stream.h>
#include <seeta/CFaceInfo.h>
#include <seeta/CStream.h>
#include <seeta/CTrackingFaceInfo.h>
#include <seeta/FaceDatabase.h>
#include <seeta/Struct_cv.h>
// #include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <pybind11/stl_bind.h>
#include <ndarray_converter.h>

PYBIND11_MAKE_OPAQUE(std::vector<SeetaRect>);
PYBIND11_MAKE_OPAQUE(std::vector<SeetaPointF>);
PYBIND11_MAKE_OPAQUE(std::vector<SeetaFaceInfo>);

namespace py = pybind11;

class SeetaImage {
  public:
    SeetaImage(const std::string &path) {
      cv::Mat mat = cv::imread(path.c_str());
      image.reset(new seeta::cv::ImageData(mat));
    }
    SeetaImage(const cv::Mat &mat) {
      image.reset(new seeta::cv::ImageData(mat));
    }
    seeta::cv::ImageData data() const {
      return *(image.get());
    }
    cv::Mat mat_data() const {
      return image->toMat();
    }

  private:
    std::unique_ptr<seeta::cv::ImageData> image;
};

class FaceDetector {
  public:
    FaceDetector(const std::string& model) {
      seeta::ModelSetting FD_model(model, SEETA_DEVICE_CPU, 0);
      detector.reset(new seeta::FaceDetector(FD_model));
      detector->set(seeta::FaceDetector::PROPERTY_VIDEO_STABLE, 1);
    }

    std::vector<SeetaFaceInfo> detect(std::shared_ptr<SeetaImage> image) {
      SeetaFaceInfoArray faces = detector->detect(image->data());
      std::vector<SeetaFaceInfo> result;
      for (int i = 0; i < faces.size; i++) {
        SeetaFaceInfo faceinfo = faces.data[i];
        result.push_back(faceinfo);
      }
      return result;
    }
  private:
    std::unique_ptr<seeta::FaceDetector> detector;
};

// class PointDetector {
//   public:
//     PointDetector(const std::string& model) {
//       detector.reset(new seeta::PointDetector2(model.c_str()));
//     }

//     std::vector<SeetaPointF> detect(std::shared_ptr<SeetaImage> image, SeetaRect rect) {
//       SeetaPointF *points = detector->Detect(image->data(), rect);
//       std::vector<SeetaPointF> result;
//       for (int i = 0; i < detector->LandmarkNum(); i++) {
//         SeetaPointF point = points[i];
//         result.push_back(point);
//       }
//       return result;
//     }
//   private:
//     std::unique_ptr<seeta::PointDetector2> detector;
// };

PYBIND11_MODULE(seetaface, m) {

  NDArrayConverter::init_numpy();

  py::class_<SeetaSize, std::unique_ptr<SeetaSize>>(m, "SeetaSize")
    .def(py::init<>())
    .def_readwrite("width", &SeetaSize::width)
    .def_readwrite("height", &SeetaSize::height);

  py::class_<SeetaRect, std::unique_ptr<SeetaRect>>(m, "SeetaRect")
    .def(py::init<>())
    .def_readwrite("x", &SeetaRect::x)
    .def_readwrite("y", &SeetaRect::y)
    .def_readwrite("width", &SeetaRect::width)
    .def_readwrite("height", &SeetaRect::height);

  py::class_<SeetaRegion, std::unique_ptr<SeetaRegion>>(m, "SeetaRegion")
    .def(py::init<>())
    .def_readwrite("top", &SeetaRegion::top)
    .def_readwrite("bottom", &SeetaRegion::bottom)
    .def_readwrite("left", &SeetaRegion::left)
    .def_readwrite("right", &SeetaRegion::right);

  py::class_<SeetaPointF, std::unique_ptr<SeetaPointF>>(m, "SeetaPoint")
    .def(py::init<>())
    .def_readwrite("x", &SeetaPointF::x)
    .def_readwrite("y", &SeetaPointF::y);

  py::class_<SeetaFaceInfo, std::unique_ptr<SeetaFaceInfo>>(m, "SeetaFaceInfo")
    .def(py::init<>())
    .def_readwrite("pos", &SeetaFaceInfo::pos)
    .def_readwrite("score", &SeetaFaceInfo::score);

 py::class_<SeetaFaceInfoArray, std::unique_ptr<SeetaFaceInfoArray>>(m, "SeetaFaceInfoArray")
    .def(py::init<>())
    .def_readwrite("data", &SeetaFaceInfoArray::data)
    .def_readwrite("size", &SeetaFaceInfoArray::size);

  py::class_<SeetaImage, std::shared_ptr<SeetaImage>>(m, "SeetaImage")
    .def(py::init<const std::string &>())
    .def(py::init<const cv::Mat &>())
    .def("data", &SeetaImage::mat_data);

  py::bind_vector<std::vector<SeetaRect>>(m, "VectorSeetaRect");
  py::bind_vector<std::vector<SeetaPointF>>(m, "VectorSeetaPoint");
  py::bind_vector<std::vector<SeetaFaceInfo>>(m, "VectorSeetaFaceInfo");

  py::class_<FaceDetector, std::unique_ptr<FaceDetector>>(m, "FaceDetector")
    .def(py::init<const std::string&>())
    .def("detect", &FaceDetector::detect);

  // py::class_<PointDetector, std::unique_ptr<PointDetector>>(m, "PointDetector")
  //   .def(py::init<const std::string&>())
  //   .def("detect", &PointDetector::detect);
}
