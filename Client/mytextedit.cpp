#include "mytextedit.h"
#include <QTextCursor>
#include <QKeyEvent>

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

MyTextEdit::~MyTextEdit()
{

}

void MyTextEdit::keyPressEvent(QKeyEvent *event)
{
    QTextEdit::keyPressEvent(event);
}
