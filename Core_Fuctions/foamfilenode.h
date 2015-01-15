#ifndef FOAMFILENODE_H
#define FOAMFILENODE_H

#include <QChar>
#include <QList>
#include <QString>

class FoamFileNode
{
public:
    static const int DATA_NODE = 0;
    static const int BLOCK_COMMENT = 1;
    static const int LINE_COMMENT = 2;

    FoamFileNode();
    FoamFileNode(QString name, QString value = "", int nt = DATA_NODE);
    FoamFileNode(QString name, QList<FoamFileNode*> childs, QString value = "", int nt = DATA_NODE);
    FoamFileNode(FoamFileNode *clone);

    // getters
    QString name();
    QString value();
    int nodeType();
    QString inlineComment();
    bool hasChilds();
    QList<FoamFileNode*> childs();
    FoamFileNode* parent();
    bool isList();
    int childsCount();
    int index();
    FoamFileNode* childAt(int index);
    QString path();
    bool semicolonTerminated();

    // setters | modifiers
    void setName(QString name);
    void setValue(QString value);
    void setParent(FoamFileNode * parent);
    void setList(bool isList);
    void setIndex(int index);
    void setNodeType(int nt);
    void setInlineComment(QString cm);
    void remove();
    void reindex();
    void removeChild(FoamFileNode *child);
    void removeAllChild();
    void setSemicolonTerminated(bool semicolonTerminated);
    FoamFileNode *addChildNode(FoamFileNode* node);
    FoamFileNode *addChildAfter(FoamFileNode* node, FoamFileNode* childNode);
    FoamFileNode *addChildBefore(FoamFileNode* node, FoamFileNode* childNode);

    // reader
    void loadFromFile(QString filename);
    void loadFromString(QString foamContent);

    // parser
    FoamFileNode* parse(QStringList * tokens, FoamFileNode* node = 0, bool isList = false);

    // writer | formater
    QString saveToString();
    void saveToFile(QString filename);
    QStringList format(FoamFileNode* node = 0, int level = 0);

    // query utils
    FoamFileNode* get(QString path);
    QList<FoamFileNode*> query(QString q);


private:
    QString _name;
    QString _value;
    QList<FoamFileNode*> _childs;
    FoamFileNode * _parent;
    int _nodeType;
    QString _inlineComment;
    bool _isList;
    int _index;
    bool _semicolonTerminated;

    QString makeSpaces(int n, QChar ch = ' ');
    QString makeTabs(QString name);
    QString tabsToSpaces(QString line);

    FoamFileNode *lastChildNode;
};

typedef QList<FoamFileNode*> FoamFileNodes;
#endif // FOAMFILENODE_H
