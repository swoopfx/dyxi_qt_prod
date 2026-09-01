#ifndef WARDPROFILE_H
#define WARDPROFILE_H

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QUuid>


class WardProfile : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit WardProfile(QObject *parent = nullptr);


    // CRUD & Substitution Operations
    bool createProfile(const QMap<QString, QVariant> &data);
    QMap<QString, QVariant> readProfile() const;
    bool substituteProfile(const QMap<QString, QVariant> &newData);
    void deleteProfile();

    // Filtering & Validation
    bool validateData(const QMap<QString, QVariant> &data) const;
    QMap<QString, QVariant> filterData(const QMap<QString, QVariant> &data) const;


    // State Checks
    bool isProfileActivated() const;
    bool isProfileStored() const;

private:
    QSettings settings;
    const QString groupName = "wardprofile";

    // Expected keys
    const QString keyIsActivated = "isActivated";
    const QString keyAge = "age";
    const QString keyFullName = "fullname";
    const QString keyWardUuid = "ward_uuid";
    const QString keyCurriculumId = "curriculum_id";

};

#endif // WARDPROFILE_H
