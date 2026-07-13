#ifndef MODELEXTRACTOR_H
#define MODELEXTRACTOR_H

#include <QObject>
#include <QString>

class ModelExtractor : public QObject
{
    Q_OBJECT

public:
    explicit ModelExtractor(QObject *parent = nullptr);

    // Copies the model from assets/resources to a writable location.
    bool extractModels();

    // Returns the writable directory containing the model files.
    QString modelDirectory() const;

    // Individual model paths
    QString encoderParam() const;
    QString encoderBin() const;

    QString decoderParam() const;
    QString decoderBin() const;

    QString joinerParam() const;
    QString joinerBin() const;

    QString tokensFile() const;

signals:
    void extractionFinished();
    void extractionFailed(const QString &reason);

private:

    bool copyFile(const QString &resourcePath,
                  const QString &destinationPath);

    QString m_modelDirectory;
};

#endif // MODELEXTRACTOR_H