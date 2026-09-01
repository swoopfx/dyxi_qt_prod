#include "wardprofile.h"
#include <QDebug>

WardProfile::WardProfile(QObject *parent)
    : QObject(parent), settings()
{
}

// Validates that types match constraints and keys exist
bool WardProfile::validateData(const QMap<QString, QVariant> &data) const
{
    if (!data.contains(keyIsActivated) || data.value(keyIsActivated).typeId() != QMetaType::Bool) {
        qWarning() << "Validation failed: Invalid or missing 'isActivated'.";
        return false;
    }

    if (!data.contains(keyAge) || data.value(keyAge).typeId() != QMetaType::Int) {
        qWarning() << "Validation failed: Invalid or missing 'age'.";
        return false;
    }
    int age = data.value(keyAge).toInt();
    if (age < 0 || age > 150) {
        qWarning() << "Validation failed: 'age' out of realistic range.";
        return false;
    }

    if (!data.contains(keyFullName) || data.value(keyFullName).typeId() != QMetaType::QString) {
        qWarning() << "Validation failed: Invalid or missing 'fullname'.";
        return false;
    }
    if (data.value(keyFullName).toString().trimmed().isEmpty()) {
        qWarning() << "Validation failed: 'fullname' cannot be empty.";
        return false;
    }

    if (!data.contains(keyWardUuid)) {
        qWarning() << "Validation failed: Missing 'ward_uuid'.";
        return false;
    }
    // Verifies if the UUID format string is actually valid
    QUuid uuid(data.value(keyWardUuid).toString());
    if (uuid.isNull()) {
        qWarning() << "Validation failed: 'ward_uuid' is not a valid UUID string.";
        return false;
    }

    if (!data.contains(keyCurriculumId)) {
        qWarning() << "Validation failed: Missing 'curriculum_id'.";
        return false;
    }

    return true;
}

// Strips out any unregistered keys to strictly keep target fields
QMap<QString, QVariant> WardProfile::filterData(const QMap<QString, QVariant> &data) const
{
    QMap<QString, QVariant> filtered;

    if (data.contains(keyIsActivated)) filtered[keyIsActivated] = data[keyIsActivated].toBool();
    if (data.contains(keyAge))         filtered[keyAge]         = data[keyAge].toInt();
    if (data.contains(keyFullName))   filtered[keyFullName]   = data[keyFullName].toString().trimmed();
    if (data.contains(keyWardUuid))   filtered[keyWardUuid]   = data[keyWardUuid].toString();
    if (data.contains(keyCurriculumId)) filtered[keyCurriculumId] = data[keyCurriculumId]; // auto value preserved as QVariant

    return filtered;
}

// Creates the profile only if it does not already exist also used to activate th ward on the evice
bool WardProfile::createProfile(const QMap<QString, QVariant> &data)
{
    settings.beginGroup(groupName);
    bool alreadyExists = settings.contains(keyWardUuid);
    settings.endGroup();

    if (alreadyExists) {
        qWarning() << "Profile creation skipped: Profile already exists. Use substituteProfile instead.";
        return false;
    }

    return substituteProfile(data);
}

// Reads the profile values back into a clean QMap
QMap<QString, QVariant> WardProfile::readProfile() const
{
    QMap<QString, QVariant> data;
    // Explicit const_cast needed because beginGroup modifies settings state tracking internally
    QSettings &mutableSettings = const_cast<QSettings&>(settings);

    mutableSettings.beginGroup(groupName);
    if (mutableSettings.contains(keyWardUuid)) {
        data[keyIsActivated]  = mutableSettings.value(keyIsActivated);
        data[keyAge]          = mutableSettings.value(keyAge);
        data[keyFullName]    = mutableSettings.value(keyFullName);
        data[keyWardUuid]    = mutableSettings.value(keyWardUuid);
        data[keyCurriculumId] = mutableSettings.value(keyCurriculumId);
    }
    mutableSettings.endGroup();

    return data;
}

// Clears old values, validates new entries, and substitutes the data completely
bool WardProfile::substituteProfile(const QMap<QString, QVariant> &newData)
{
    QMap<QString, QVariant> filtered = filterData(newData);

    if (!validateData(filtered)) {
        qWarning() << "Profile substitution aborted due to bad input data.";
        return false;
    }

    settings.beginGroup(groupName);
    settings.remove(""); // Wipes out all old keys inside this group context safely

    settings.setValue(keyIsActivated,  filtered[keyIsActivated]);
    settings.setValue(keyAge,          filtered[keyAge]);
    settings.setValue(keyFullName,    filtered[keyFullName]);
    settings.setValue(keyWardUuid,    filtered[keyWardUuid]);
    settings.setValue(keyCurriculumId, filtered[keyCurriculumId]);
    settings.endGroup();

    settings.sync(); // Force writes changes immediately to disk storage
    return true;
}

// Deletes the profile data group entirely
void WardProfile::deleteProfile()
{
    settings.beginGroup(groupName);
    settings.remove("");
    settings.endGroup();
    settings.sync();
}

// Checks if the profile exists on the device by verifying if the group has keys
bool WardProfile::isProfileStored() const
{
    // const_cast needed because beginGroup/endGroup modify internal state tracking
    QSettings &mutableSettings = const_cast<QSettings&>(settings);

    mutableSettings.beginGroup(groupName);
    // childKeys() returns all keys inside the "wardprofile" group
    bool exists = !mutableSettings.childKeys().isEmpty();
    mutableSettings.endGroup();

    return exists;
}

// Checks if the 'isActivated' key exists and is explicitly set to true
bool WardProfile::isProfileActivated() const
{
    // If the profile doesn't even exist on the device, it cannot be active
    if (!isProfileStored()) {
        return false;
    }

    QSettings &mutableSettings = const_cast<QSettings&>(settings);
    mutableSettings.beginGroup(groupName);

    // Reads value as bool; defaults to false if the key is missing or corrupted
    bool activated = mutableSettings.value(keyIsActivated, false).toBool();
    mutableSettings.endGroup();

    return activated;
}
