#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget *parent = nullptr);
    ~MyTextEdit();

signals:
    void emitSend();


public slots:
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MYTEXTEDIT_H
