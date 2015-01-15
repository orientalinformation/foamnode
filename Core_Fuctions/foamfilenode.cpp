#include "foamfilenode.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>

FoamFileNode::FoamFileNode()
{
    this->_parent = NULL;
    this->_isList = false;
    this->_nodeType = DATA_NODE;
    this->_semicolonTerminated=false;
}

FoamFileNode::FoamFileNode(QString name, QString value, int nt)
{
    this->_name = name;
    this->_value = value;
    this->_nodeType = nt;
    this->_parent = NULL;
    this->_isList = false;
    this->_semicolonTerminated=false;
}

FoamFileNode::FoamFileNode(QString name, QList<FoamFileNode *> childs, QString value, int nt)
{
    this->_name = name;
    this->_childs = childs;
    this->_value = value;
    this->_nodeType = nt;
    this->_parent = NULL;
    this->_isList = false;
    this->_semicolonTerminated=false;
}

FoamFileNode::FoamFileNode(FoamFileNode *clone)
{
    this->_name = clone->name();
    this->_value = clone->value();
    this->_nodeType = clone->nodeType();
    this->_parent = clone->parent();
    this->_isList =clone->isList();
    this->_index = clone->index();
    this->_inlineComment = clone->inlineComment();

    this->_childs = QList<FoamFileNode*>();
    foreach (FoamFileNode *child, clone->childs()) {
        this->addChildNode(new FoamFileNode(child));
    }
}

QString FoamFileNode::name()
{
    return this->_name;
}

QString FoamFileNode::value()
{
    return this->_value;
}

int FoamFileNode::nodeType()
{
    return this->_nodeType;
}

QString FoamFileNode::inlineComment()
{
    return this->_inlineComment;
}

void FoamFileNode::setName(QString name)
{
    this->_name = name;
}

void FoamFileNode::setValue(QString value)
{
    this->_value = value;
}

FoamFileNode *FoamFileNode::addChildNode(FoamFileNode *node)
{
    if (node!=0) {
        node->setParent(this);
        node->setIndex(this->childsCount()+1);
        this->_childs.append(node);
    }
    return this;
}

FoamFileNode *FoamFileNode::addChildAfter(FoamFileNode *node, FoamFileNode *childNode)
{
    if (node!=0 && childNode!=0) {
        if (node->index()==this->childsCount()) return this->addChildNode(childNode);

        childNode->setParent(this);
        this->_childs.insert(node->index(),childNode);
        this->reindex();
    }
    return this;
}

FoamFileNode *FoamFileNode::addChildBefore(FoamFileNode *node, FoamFileNode *childNode)
{
    if (node!=0 && childNode!=0) {
        childNode->setParent(this);
        this->_childs.insert(node->index()-1,childNode);
        this->reindex();
    }
    return this;
}

void FoamFileNode::setParent(FoamFileNode *parent)
{
    this->_parent = parent;
}

void FoamFileNode::setList(bool isList)
{
    this->_isList = isList;
}

void FoamFileNode::setIndex(int index)
{
    this->_index = index;
}

void FoamFileNode::setNodeType(int nt)
{
    this->_nodeType = nt;
}

void FoamFileNode::setInlineComment(QString cm)
{
    this->_inlineComment = cm;
}

void FoamFileNode::remove()
{
    this->parent()->removeChild(this);
}

void FoamFileNode::reindex()
{
    for (int i=1; i<=this->_childs.count(); i++) {
        this->_childs.at(i-1)->setIndex(i);
    }
}

void FoamFileNode::removeChild(FoamFileNode *child)
{
    this->_childs.removeOne(child);
}

void FoamFileNode::removeAllChild()
{
    while (this->hasChilds())
        this->removeChild(this->childAt(1));
}

void FoamFileNode::setSemicolonTerminated(bool semicolonTerminated)
{
    this->_semicolonTerminated = semicolonTerminated;
}

bool FoamFileNode::isList()
{
    return this->_isList;
}

int FoamFileNode::childsCount()
{
    return this->childs().length();
}

int FoamFileNode::index()
{
    return this->_index;
}

FoamFileNode *FoamFileNode::childAt(int index)
{
    return this->_childs.at(index-1);
}

QString FoamFileNode::path()
{
    if (this->parent()!=NULL)
        return this->parent()->path() + '/' + this->name();
    else
        return this->name();
}

bool FoamFileNode::semicolonTerminated()
{
    return this->_semicolonTerminated;
}

bool FoamFileNode::hasChilds()
{
    return this->_childs.length() > 0;
}

FoamFileNodes FoamFileNode::childs()
{
    return this->_childs;
}

FoamFileNode *FoamFileNode::parent()
{
    return this->_parent;
}

void FoamFileNode::loadFromFile(QString filename)
{
    QFile inputFile(filename);
    QString content("");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
          QString line = in.readLine();
          content.append(line + "\n");
       }
       inputFile.close();
    }
    this->loadFromString(content);
}

void FoamFileNode::loadFromString(QString foamContent)
{
    QRegExp inlineCommentExpr("//.*");
    QRegExp multiLinesCommentExpr("/\\*.*");
    QRegExp endMultiLinesCommentExpr("\\*/");
    QRegExp sepExpr("\\s+");

    QTextStream stream(new QString(foamContent));
    QStringList tokens;

    bool inBlockComment = false;

    QString line = stream.readLine();
    while ( !line.isNull() ) {
        if (inBlockComment) {
            int endBcommentPos = endMultiLinesCommentExpr.indexIn(line, 0);
            if (endBcommentPos >= 0) {
                endBcommentPos += 2;
                QString partialBlockComment = line.left(endBcommentPos);
                inBlockComment = false;
                tokens.last().append("\n"+partialBlockComment);
                line = line.right(line.length()-partialBlockComment.length());
                line.trimmed();
                if (line.length() == 0)
                    line = stream.readLine();
                continue;
            } else {
                tokens.last().append("\n"+line);
            }
        } else {
            QString lineComment;
            QString blockComment;

            int commentPos = inlineCommentExpr.indexIn(line,0);
            if (commentPos >= 0) {
                lineComment = line.mid(commentPos);
                line.chop(lineComment.length());
                line = line.trimmed();
                if (line.length()==0)
                    lineComment = "\n"+lineComment;
            }

            int blockCommentPos = multiLinesCommentExpr.indexIn(line, 0);
            if (blockCommentPos >= 0) {
                if (lineComment.length()>0)
                    line += lineComment;
                blockComment = line.mid(blockCommentPos);
                line = line.mid(0,blockCommentPos);
                inBlockComment = true;
            }

            if (line.length() > 0)
                tokens.append(line.split(sepExpr, QString::SkipEmptyParts));

            if (inBlockComment)
                tokens.append(blockComment);

            if (!inBlockComment && commentPos >=0)
                tokens.append(lineComment);
        }

        line = stream.readLine();
    }

    this->parse(&tokens, this);
}

QString FoamFileNode::saveToString()
{
    return this->format(this).join("\n");
}

void FoamFileNode::saveToFile(QString filename)
{
    QFile outputFile(filename);
    QStringList content( this->format(this) );
    if (outputFile.open(QIODevice::WriteOnly))
    {
       QTextStream out(&outputFile);
       foreach (QString line, content) {
           line += "\n";
           out << line.toStdString().c_str();
       }
       outputFile.close();
    }
}



FoamFileNode* FoamFileNode::parse(QStringList * tokens, FoamFileNode* node, bool isList)
{
    if (node == 0) {
        node = new FoamFileNode();
    }
    bool listContinue = false;
    QStringList values;
    QRegExp parentheseRegexp(".+\\(.+");

    while (!tokens->isEmpty()) {
        QString token(tokens->first());
        tokens->removeFirst();
        values.append(token);

        if (token.startsWith("\n//") ) {
            FoamFileNode *lineComment = new FoamFileNode();
            lineComment->setNodeType(LINE_COMMENT);
            lineComment->setValue(token.mid(1));
            node->addChildNode(lineComment);
            values.clear();
            continue;
        }

        if (token.startsWith("/*")) {
            FoamFileNode *blockComment = new FoamFileNode();
            blockComment->setNodeType(BLOCK_COMMENT);
            blockComment->setValue(token);
            node->addChildNode(blockComment);
            values.clear();
            continue;
        }

        if (token.startsWith("//")) {
            QString inlineComment = token;
            if (lastChildNode != 0)
                lastChildNode->setInlineComment(inlineComment);
            values.removeLast();
            continue;
        }

        if (token.contains(")(")) {
            QString replace = token.replace(")(",") (");
            QStringList elements = replace.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            for (int i = elements.length()-1; i >= 0; i--) {
                tokens->prepend(elements.at(i));
            }
            values.removeLast();
            continue;
        }

        if ( parentheseRegexp.exactMatch(token) && token.length() > 1) {
            QStringList parts = token.split('(');

            if (parts.length() > 1) {
                tokens->prepend(parts.at(1));
            }
            tokens->prepend("(");
            tokens->prepend(parts.at(0));
            values.removeLast();
            continue;
        }

        if (token.startsWith("{")) {
            lastChildNode = new FoamFileNode();
            if (values.length() > 0 && values.first().length() > 0) {
                values.removeLast();
                if (values.length()>0)
                    lastChildNode->setName(values.join(" "));
            } else
                lastChildNode->setName("");
            values.clear();
            if (token.length()>1) {
                token.remove(0,1);
                tokens->prepend(token);
            }
            lastChildNode->setParent(node);
            lastChildNode = parse(tokens, lastChildNode);

            if (lastChildNode != NULL) {
                node->addChildNode(lastChildNode);
            }
        }

        if (token.endsWith('}')) {
            if (token.length()>1) {
                token.chop(1);
                tokens->append(token);
                tokens->append("}");
                values.removeLast();
            } else {
                return node;
            }
        }

        if (token.startsWith("(")) {
            if (token.length()>1) {
                token.remove(0,1);
                tokens->prepend(token);
                tokens->prepend("(");
                values.removeLast();
                continue;
            } else {
                values.removeLast();
                if (values.length()>0) {
                    lastChildNode = new FoamFileNode();
                    lastChildNode->setName(values.first());
                    values.removeFirst();
                    lastChildNode->setList(true);
                    lastChildNode->setParent(node);
                    if (values.length()>0)
                        lastChildNode->setValue(values.join(" "));
                    lastChildNode = parse(tokens, lastChildNode, true);
                    if (lastChildNode != 0) {
                        node->addChildNode(lastChildNode);
                        values.clear();
                    }
                }
            }
        }

        if (token.endsWith(')')) {
            if (token.length()>1) {
                token.chop(1);
                tokens->prepend(")");
                tokens->prepend(token);
                values.removeLast();
            } else {
                if (node->hasChilds()) {
                    return node;
                } else {
                    values.prepend("(");
                    node->setValue(node->value() + " " + values.join(" "));
                    node->setValue(node->value().trimmed());
                    values.clear();
                    listContinue = true;
                }
            }
        }

        if (token.endsWith(']')) {
            if (token.length()>1) {
                token.chop(1);
                tokens->prepend("]");
                tokens->prepend(token);
                values.removeLast();
            }
        }

        if (token.startsWith('[')) {
            if (token.length()>1) {
                token.remove(0,1);
                tokens->prepend(token);
                tokens->prepend("[");
                values.removeLast();
            }
        }

        if (token.endsWith(';')) {
            if (token.length()>1) {
                token.chop(1);
                tokens->prepend(";");
                tokens->prepend(token);
                values.removeLast();
            } else {
                if (!listContinue) {
                    lastChildNode = new FoamFileNode();
                    lastChildNode->setName(values.first());
                    values.removeFirst();
                    if (!values.empty()) {
                        values.removeLast();
                        if (!values.empty()) {
                            lastChildNode->setValue(values.join(" "));
                            values.clear();
                            node->addChildNode(lastChildNode);
                        }
                    }
                } else {
                    return node;
                }

            }
        }
    }
    return 0;
}

FoamFileNode *FoamFileNode::get(QString path)
{
    QStringList paths = path.split('/',QString::SkipEmptyParts);
    int total = paths.length();
    FoamFileNode* node = this;
    bool inList = false;
    int listIndex = 0;

    for (int i = 0; i < total; ++i) {
        QStringList listPaths;
        QString listPathName;
        int listPathIndex;

        if (paths.at(i).endsWith(')')) {
            listPaths = paths.at(i).split('(');
            listPathName = listPaths.at(0);
            listPathIndex = listPaths.at(1).left(listPaths.at(1).length()-1).toInt();
        }

        foreach (FoamFileNode* child, node->childs()) {
            if (inList) {
                if ( child->index() == listIndex ) {
                    if (i == total - 1)
                        return child;
                    else {
                        inList = false;
                        listIndex =  0;
                        --i;
                        node = child; break;
                    }
                }
            } else if (!paths.at(i).endsWith(')')) {
                if ( child->name() == paths.at(i) ) {
                    if (i == total - 1)
                        return child;
                    else {
                        node = child; break;
                    }
                }
            } else {
                if ( child->name() == listPathName ) {
                    inList = true;
                    listIndex =  listPathIndex;
                    --i;
                    node = child; break;
                }
            }
        }
    }
    return NULL;
}

QList<FoamFileNode *> FoamFileNode::query(QString q)
{
    QStringList paths = q.split('/',QString::SkipEmptyParts);

    QList<FoamFileNode *> results;

    foreach (FoamFileNode* child, this->childs()) {
        QRegExp regexp(paths[0]); regexp.setPatternSyntax(QRegExp::Wildcard);
        int pos = regexp.indexIn(child->name());
        if (pos >= 0) {
            if (paths.length()>1) {
                results.append(child->query( ((QStringList)  paths.mid(1)).join("/") ));
            } else {
                results.append(child);
            }
        }
    }

    return results;
}

QString FoamFileNode::makeSpaces(int n, QChar ch)
{
    QString result("");
    for (int i = 0; i < n; ++i) {
        result.append(ch);
    }
    return result;
}

QString FoamFileNode::makeTabs(QString name)
{
    int length = name.length();
    int count = 6;
    if (length > 20) count = 1;
    else if (length >= 16)  count = 2;
    else if (length >= 12)  count = 3;
    else if (length >= 8)  count = 4;
    else if (length >= 4)  count = 5;
    return this->makeSpaces(count, '\t');
}

QString FoamFileNode::tabsToSpaces(QString line)
{
    QString seperator("");
    int tabsCount = line.count('\t');
    line = line.replace(QRegExp("\\t+"),"\t");
    QStringList parts = line.split('\t');
    QString name = parts.at(0).trimmed();

    //first tab exception
    int reduceSpaces = 4 - name.length() % 4;
    seperator.append(this->makeSpaces(reduceSpaces, ' '));

    tabsCount--;
    seperator.append(this->makeSpaces(tabsCount*4, ' '));
    line = line.replace("\t",seperator);
    return line;
}

QStringList FoamFileNode::format(FoamFileNode *node, int level)
{
    QStringList result;
    if (node == 0) {
        node = this;
    }

    foreach (FoamFileNode* child, node->childs()) {
        if (!child->hasChilds()) {
            if (child->nodeType() == DATA_NODE) {
                if (child->inlineComment().isEmpty())
                    result.append(
                        this->tabsToSpaces( this->makeSpaces(level * 4) + child->name() + this->makeTabs(child->name()) + child->value() + ';')
                    );
                else
                    result.append(
                        this->tabsToSpaces( this->makeSpaces(level * 4) + child->name() + this->makeTabs(child->name()) + child->value() + "; " ) + child->inlineComment()
                    );
            } else {
                result.append(this->makeSpaces(level * 4) + child->value());
            }

            if (level == 0)
                result.append("");

        } else {
            result.append( this->makeSpaces(level * 4) + child->name() );
            result.append( this->makeSpaces( level * 4) + (!child->isList()?"{":"(") );
            result.append( this->format(child, level+1) );
            if (child->inlineComment().isEmpty()) {
                if (_semicolonTerminated) result.append( this->makeSpaces(level * 4) + (!child->isList()?"}":");") );
                else result.append( this->makeSpaces(level * 4) + (!child->isList()?"}":")") );
            } else {
                if (_semicolonTerminated) result.append( this->makeSpaces(level * 4) + (!child->isList()?"}":");")  + " " + child->inlineComment());
                else result.append( this->makeSpaces(level * 4) + (!child->isList()?"}":")")  + " " + child->inlineComment());
            }
            result.append("");
        }
    }

    return result;
}
