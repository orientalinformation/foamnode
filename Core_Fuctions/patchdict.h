#ifndef PATCHDICT_H
#define PATCHDICT_H

#include <QString>
#include <QList>
#include <QFile>
#include <QDir>
#include <QStringList>

struct PatchInfo
{
    QString type;
};
struct Patches
{
    QStringList surfaces;
};
struct Boundary
{
    QString name;
    PatchInfo patchInfo;
    Patches patches;
};
class PatchDict
{
public:
    PatchDict();    
    QList<Boundary> boundaries;    
    void WritePatchDict(QString path);
    void ReadPatchDict(QString path);
};

#endif // PATCHDICT_H
