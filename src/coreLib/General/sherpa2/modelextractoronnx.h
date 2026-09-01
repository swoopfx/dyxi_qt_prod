#ifndef MODELEXTRACTORONNX_H
#define MODELEXTRACTORONNX_H

#include <QObject>
#include <QString>
#include "sherpa-onnx/csrc/offline-recognizer.h"
#include "sherpa-onnx/csrc/voice-activity-detector.h"

class ModelExtractorOnnx : public QObject
{
    Q_OBJECT
public:
    explicit ModelExtractorOnnx(QObject *parent = nullptr);
    // Copies the model from assets/resources to a writable location.
    bool extractModels();
    void instantiateConfig();

    // Returns the writable directory containing the model files.
    QString modelDirectory() const;

    // return confgration
    sherpa_onnx::VadModelConfig getVadConfig()const;
    sherpa_onnx::OfflineRecognizerConfig  getRecognizerConfig()const;

    // Individual model paths
    QString preprocessOnnx() const;
    QString encoderOnnx() const;

    QString cachedDecodeOnnx() const;
    QString uncachedDecodeOnnx() const;

    QString sileroVad() const;


    QString tokensTxt() const;

signals:
    void extractionFinished();
    void extractionFailed(const QString &reason);

private:
    bool copyFile(const QString &resourcePath,
                  const QString &destinationPath);

    QString m_modelDirectory;
     std::unique_ptr<sherpa_onnx::OfflineRecognizer> m_recognizer;
      // std::unique_ptr<sherpa_onnx::VadModelConfig> vadConfig;
     sherpa_onnx::VadModelConfig vadConfig;
     sherpa_onnx::OfflineRecognizerConfig recognizerConfig;
       // sherpa_onnx::VadModelConfig vadConfig;
};

#endif // MODELEXTRACTORONNX_H
