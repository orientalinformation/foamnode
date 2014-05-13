#include "block_dmesh.h"

Block_Dmesh::Block_Dmesh()
{
    isAutomatic = true;
}
void Block_Dmesh::Set(Vertices v)
{
    vertice = v;
}

Vertices Block_Dmesh::Get_Ver()
{
    return vertice;
}

void Block_Dmesh::Set(BoundMesh b)
{
    boundMesh =b;
}

BoundMesh Block_Dmesh::Get_Bou()
{
    return boundMesh;
}

void Block_Dmesh::Write_Bounding(QString path, QStringList string)
{
    QFile file(QDir::toNativeSeparators(path));
    file.open(QIODevice::WriteOnly);
    QString str_File_New;
    for(int i = 0; i < string.size(); i++)
    {
        str_File_New += string[i] + "\n";
    }
    file.write(str_File_New.toAscii().data());
    file.close();
}

void Block_Dmesh::Write_Block_Dmesh(QString path)
{
    //Pre value for elements
    PointDmesh element;
    element.x = (vertice.v[6].x - vertice.v[0].x)/gen_Bounding.element.x;
    element.y = (vertice.v[6].y - vertice.v[0].y)/gen_Bounding.element.y;
    element.z = (vertice.v[6].z - vertice.v[0].z)/gen_Bounding.element.z;
    //Write values to file
    path.append("/blockMeshDict");
    QFile file(QDir::toNativeSeparators(path));
    file.open(QIODevice::WriteOnly);
    QString str_File_New;
    str_File_New  = "/*--------------------------------*- C++ -*----------------------------------*\\\n";
    str_File_New += "| =========                 |                                                 |\n";
    str_File_New += "| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |\n";
    str_File_New += "|  \\\\    /   O peration     | Version:  2.2.0                                 |\n";
    str_File_New += "|   \\\\  /    A nd           | Web:      www.OpenFOAM.org                      |\n";
    str_File_New += "|    \\\\/     M anipulation  |                                                 |\n";
    str_File_New += "\\*---------------------------------------------------------------------------*/\n";
    str_File_New += "FoamFile\n";

    str_File_New += "{\n";
    str_File_New += "    version     2.0;\n";
    str_File_New += "    format      ascii;\n";
    str_File_New += "    class       dictionary;\n";
    str_File_New += "    object      blockMeshDict;\n";
    str_File_New += "}\n";
    str_File_New += "\n";
    str_File_New += "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n";
    str_File_New += "\n";
    str_File_New += "convertToMeters 1;\n";

     str_File_New +="\n";
     str_File_New +="vertices\n";
     str_File_New +="(\n";
     str_File_New +="   ("+QString::number(vertice.v[0].x)+" "+QString::number(vertice.v[0].y)+" "+QString::number(vertice.v[0].z)+") //0\n";
     str_File_New +="   ("+QString::number(vertice.v[1].x)+" "+QString::number(vertice.v[1].y)+" "+QString::number(vertice.v[1].z)+") //1\n";
     str_File_New +="   ("+QString::number(vertice.v[2].x)+" "+QString::number(vertice.v[2].y)+" "+QString::number(vertice.v[2].z)+") //2\n";
     str_File_New +="   ("+QString::number(vertice.v[3].x)+" "+QString::number(vertice.v[3].y)+" "+QString::number(vertice.v[3].z)+") //3\n";
     str_File_New +="   ("+QString::number(vertice.v[4].x)+" "+QString::number(vertice.v[4].y)+" "+QString::number(vertice.v[4].z)+") //4\n";
     str_File_New +="   ("+QString::number(vertice.v[5].x)+" "+QString::number(vertice.v[5].y)+" "+QString::number(vertice.v[5].z)+") //5\n";
     str_File_New +="   ("+QString::number(vertice.v[6].x)+" "+QString::number(vertice.v[6].y)+" "+QString::number(vertice.v[6].z)+") //6\n";
     str_File_New +="   ("+QString::number(vertice.v[7].x)+" "+QString::number(vertice.v[7].y)+" "+QString::number(vertice.v[7].z)+") //7\n";
     str_File_New +=");\n";
     str_File_New +="\n";
     str_File_New +="edges\n";
     str_File_New +="(\n";
     str_File_New +=");\n";
     str_File_New +="\n";
     str_File_New +="blocks\n";
     str_File_New +="(\n";
     str_File_New +="    hex (0 1 2 3 4 5 6 7) (";
     str_File_New +=QString::number(gen_Bounding.element.x)+" ";
     str_File_New +=QString::number(gen_Bounding.element.y)+" ";
     str_File_New +=QString::number(gen_Bounding.element.z);
     str_File_New +=") simpleGrading (1 1 1)\n";
     str_File_New +=");\n";
     str_File_New +="\n";
     str_File_New +="boundary\n";
     str_File_New +="(\n";
     str_File_New +="    " + boundMesh.bounsType[0].name + "\n";
     str_File_New +="    {\n";
     str_File_New +="        type " + boundMesh.bounsType[0].type + ";\n";
     str_File_New +="        faces\n";
     str_File_New +="        (\n";
     str_File_New +="            (3 7 6 2)\n";
     str_File_New +="        );\n";
     str_File_New +="    }\n";
     str_File_New +="\n";
     str_File_New +="    " + boundMesh.bounsType[1].name + "\n";
     str_File_New +="    {\n";
     str_File_New +="        type " + boundMesh.bounsType[1].type + ";\n";
     str_File_New +="        faces\n";
     str_File_New +="        (\n";
     str_File_New +="            (1 5 4 0)\n";
     str_File_New +="        );\n";
     str_File_New +="    }\n";
     str_File_New +="\n";
     str_File_New +="    " + boundMesh.bounsType[2].name + "\n";
     str_File_New +="    {\n";
     str_File_New +="        type " + boundMesh.bounsType[2].type + ";\n";
     str_File_New +="        faces\n";
     str_File_New +="        (\n";
     str_File_New +="            (0 3 2 1)\n";
     str_File_New +="        );\n";
     str_File_New +="    }\n";
     str_File_New +="\n";
     str_File_New +="    " + boundMesh.bounsType[3].name + "\n";
     str_File_New +="    {\n";
     str_File_New +="        type " + boundMesh.bounsType[3].type + ";\n";
     str_File_New +="        faces\n";
     str_File_New +="        (\n";
     str_File_New +="            (4 5 6 7)\n";
     str_File_New +="        );\n";
     str_File_New +="    }\n";
     str_File_New +="\n";
     str_File_New +="    " + boundMesh.bounsType[4].name + "\n";
     str_File_New +="    {\n";
     str_File_New +="        type " + boundMesh.bounsType[4].type + ";\n";
     str_File_New +="        faces\n";
     str_File_New +="        (\n";
     str_File_New +="            (0 4 7 3)\n";
     str_File_New +="        );\n";
     str_File_New +="    }\n";
     str_File_New +="\n";
     str_File_New +="    " + boundMesh.bounsType[5].name + "\n";
     str_File_New +="    {\n";
     str_File_New +="        type " + boundMesh.bounsType[5].type + ";\n";
     str_File_New +="        faces\n";
     str_File_New +="        (\n";
     str_File_New +="            (2 6 5 1)\n";
     str_File_New +="        );\n";
     str_File_New +="    }\n";
     str_File_New +=");\n";
     str_File_New +="\n";
     str_File_New +="mergePatchPairs\n";
     str_File_New +="(\n";
     str_File_New +=");\n";
     str_File_New +="\n";
     str_File_New +="// ************************************************************************* //\n";

     file.write(str_File_New.toAscii().data());
     file.close();
}

QString Block_Dmesh::Read_Setting(QString path)
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
        return false;
    }
    QStringList lines = str_file.split("\n",QString::SkipEmptyParts);
    return lines[0];

}

bool Block_Dmesh::Read_Block_Dmesh(QString path)
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
        return false;
    }
    QStringList lines = str_file.split("\n",QString::SkipEmptyParts);

    for(int i= 0; i< lines.length(); i++)
    {
        if(lines[i].contains("vertices"))
        {
            i = i + 2;
            for(int j=0; j< 8; j++)
            {
                QString line = lines[i+j];
                line =line.trimmed();

                line = line.remove("(");
                line = line.remove(")");

                QStringList list = line.split(" ", QString::SkipEmptyParts);

                //add point
                PointDmesh temp;
                temp.x = list[0].toFloat();
                temp.y = list[1].toFloat();
                temp.z = list[2].toFloat();
                vertice.v.append(temp);

                //tang i them 1 dong
                //i= i + 1;
            }
            i = i +8;
            vertice.n = vertice.v.size();
        }
        else if(lines[i].contains("blocks"))
        {
            i = i + 2;
            QString  values = lines[i].split("(", QString::SkipEmptyParts)[2];
            QString  value = values.split(")", QString::SkipEmptyParts)[0];
            QStringList list = value.split(" ", QString::SkipEmptyParts);
            gen_Bounding.element.x = list[0].toFloat();
            gen_Bounding.element.y = list[1].toFloat();
            gen_Bounding.element.z = list[2].toFloat();
            i= i + 1;
        }
        else if(lines[i].contains("boundary"))
        {
            i = i + 2;
            while(!lines[i].contains(");"))
            {

                BounTypeDmesh b;
                b.name =  lines[i].trimmed();
                b.type =  lines[i+2].trimmed().split(" ", QString::SkipEmptyParts)[1];

                FacesDmesh f;
                QString str = lines[i+5].trimmed();
                str = str.remove("(");
                str = str.remove(")");
                QStringList lst = str.split(" ");
                f.points.append(lst[0].toInt());
                f.points.append(lst[1].toInt());
                f.points.append(lst[2].toInt());
                f.points.append(lst[3].toInt());

                f.n = f.points.size();

                b.faces.append(f);
                b.n = b.faces.size();

                boundMesh.bounsType.append(b);
                boundMesh.n = boundMesh.bounsType.size();

                i = i + 8;
            }
        }
    }
    return true;
}
