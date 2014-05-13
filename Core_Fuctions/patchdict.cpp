#include "patchdict.h"

PatchDict::PatchDict()
{
}
void PatchDict::WritePatchDict(QString path)
{
    path.append("/createPatchDict");
    QFile file(QDir::toNativeSeparators(path));
    file.open(QIODevice::WriteOnly);
    QString str_File_New;
    str_File_New  = "/*--------------------------------*- C++ -*----------------------------------*\\\n";
    str_File_New += "| =========                 |                                                 |\n";
    str_File_New += "| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |\n";
    str_File_New += "|  \\\\    /   O peration     | Version:  2.1.0                                 |\n";
    str_File_New += "|   \\\\  /    A nd           | Web:      www.OpenFOAM.org                      |\n";
    str_File_New += "|    \\\\/     M anipulation  |                                                 |\n";
    str_File_New += "\\*---------------------------------------------------------------------------*/\n";
    str_File_New += "FoamFile\n";

    str_File_New += "{\n";
    str_File_New += "    version 2.1;\n";
    str_File_New += "    format ascii;\n";
    str_File_New += "    class dictionary;\n";
    str_File_New += "    location system;\n";
    str_File_New += "    object createPatchDict;\n";
    str_File_New += "}\n";
    str_File_New += "\n";
    str_File_New += "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n";
    str_File_New += "\n";
    str_File_New += "pointSync false;\n";
    str_File_New += "patches\n";
    str_File_New += "(\n";
    //For each boundary
    foreach(Boundary b, boundaries)
    {
        str_File_New += "    {\n";
        str_File_New += "        name " + b.name +";\n";
        str_File_New += "        patchInfo\n";
        str_File_New += "        {\n";
        str_File_New += "            type " + b.patchInfo.type + ";\n";
        str_File_New += "        }\n";
        str_File_New += "        constructFrom patches;\n";
        str_File_New += "        patches (";
        foreach(QString surface, b.patches.surfaces)
        {
            str_File_New += " \"" + surface + ".*\"";
        }

        str_File_New += " );\n";
        str_File_New += "    }\n";
    }
    str_File_New += ");\n";
    str_File_New +="// ************************************************************************* //\n";
    file.write(str_File_New.toAscii().data());
    file.close();
}

void PatchDict::ReadPatchDict(QString path)
{
    QString str_file = "";
    #if defined(Q_OS_WIN)
    {
        path = path.replace("/","\\");
    }
    #endif
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        str_file = file.readAll();

    if(str_file == "")
    {
        return ;
    }
    QStringList lines = str_file.split("\n",QString::SkipEmptyParts);

    for(int i= 0; i< lines.length(); i++)
    {
        if(lines[i].contains("name"))
        {
            Boundary b;
            QStringList list_name = lines[i].trimmed().split(" ", QString::SkipEmptyParts);

            b.name = list_name[1].remove(";").trimmed();

            list_name = lines[i+3].trimmed().split(" ", QString::SkipEmptyParts);

            b.patchInfo.type = list_name[1].remove(";").trimmed();

            list_name  = lines[i + 6].split("(");
            QStringList list_Temp = list_name[1].split(")");
            QStringList list_P = list_Temp[0].split(".*");
            for(int i =0; i< list_P.length()- 1; i++)
            {
                QString temp = list_P[i].trimmed().remove("\"").trimmed();
                b.patches.surfaces.append(temp);
            }
            boundaries.append(b);
        }
    }
    file.close();
}
