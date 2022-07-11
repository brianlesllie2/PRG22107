#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class MyButton: public QPushButton
{
    Q_OBJECT
public:
    MyButton(QWidget *parent = nullptr);
    ~MyButton();

protected:
    void mousePressEvent(QMouseEvent *e) override;

signals:
    void rightClick();
};

#endif // MYHEADER_H
