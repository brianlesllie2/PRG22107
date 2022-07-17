#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMap>
#include <QComboBox>
#include <QLabel>
#include "MyButton.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QGridLayout* gameLayout;
    QComboBox* difficulties;
    QPushButton* resetButton;
    QString** buttonText;   //texto dos botoes ex: bomba, 1, 2
    QLabel* flags;
    MyButton*** buttons;
    int dificuldade;
    int buttonRows, buttonColumns;
    int numberOfBombs;
    int numberOfFlags;
    int disabledButtons;
    bool endOfGame;

    QGridLayout* janela;

    int posicaoAtualX;
    int posicaoAtualY;

    void clearButtons();
    void setButtons(int n, int m, int size);
    void setBombs();
    void setNumbers();
    void bombClicked();
    void win();
    void reset();
    void botaoSemBombaAdj(int i, int j);
    void vitoria();

private slots:
    //void buttonClicked();
    void difficultyChanged();
    void showButton();

    void rightButtonClicked();
};

#endif // WIDGET_H
