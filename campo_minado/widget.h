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
    QComboBox* difficulties;        //selecao de dificuldades
    QPushButton* resetButton;       //botao para reiniciar o jogo
    QString** valorBotao;           //texto dos botoes ex: bomba, 1, 2
    QLabel* bandeiras;              //indica a quantidade de bombas no jogo
    MyButton*** botoes;             //botoes do jogo
    int dificuldade;                //variavel que controla o tamanho da matriz e quantidade de bombas
    int numDeLinhas, numDeColunas;  //numero de linhas e colunas da matriz
    int numDeBombas;                //quantidade de bombas no jogo
    int numDeBandeiras;             //quantidade de bandeiras no jogo
    int botoesDesativados;          //variavel para controlar os botes clicados
    bool fimDeJogo;

private slots:
    void criaBotoes(int n, int m, int size);    //cria a matriz de botoes
    void geraBombas();                          //define as posicoes das bombas no jogo
    void atribuiNumeroBombaAdj();               //define os numeros que indicam as bombas ao redor
    void destroiBotoes();                       //destroi os botoes do jogo atual e cria novos
    void reset();                               //reinicia o jogo
    void mostraValorBotao();                    //mostra o que tem no botao clicado
    void cliqueNaBomba();                       //mostra as posicoes das bombas e encerra o jogo
    void botaoSemBombaAdj(int i, int j);        //abre os arredores ao clicar em um espaco sem bomba e numero
    void vitoria();                             //encerra o jogo caso acabem os botoes sem bombas
    void mudaDificuldade();                     //altera a dificuldade e reinicia o jogo
    void rightButtonClicked();                  //atribui uma bandeira para a localizacao de bomba
};

#endif // WIDGET_H
