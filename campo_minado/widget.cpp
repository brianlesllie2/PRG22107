#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    gameLayout = new QGridLayout;
    setLayout(gameLayout);

    //inicializacao
    disabledButtons = 0;
    dificuldade = 0;
    endOfGame = false;

    //
    difficulties = new QComboBox;
    difficulties->addItem("Facil");
    difficulties->addItem("Medio");
    difficulties->addItem("Dificil");
    gameLayout->addWidget(difficulties, 2, 0);

    flags = new QLabel;
    flags->setText("Bandeiras: 10");
    gameLayout->addWidget(flags, 0, 0);

    resetButton = new QPushButton("Reset");
    connect(resetButton, &QPushButton::clicked,  this, [&](){reset();} );
    gameLayout->addWidget(resetButton, 1, 0);

    setButtons(8, 10, 20);
    setBombs();
    setNumbers();
}

void Widget::setButtons(int n, int m, int size){
    buttonRows = n;
    buttonColumns = m;
    buttons = new MyButton**[n];
    buttonText = new QString*[n];
    for(int i=0; i<buttonRows; ++i){
        buttons[i] = new MyButton*[m];
        buttonText[i] = new QString[m];
    }
    for(int i=0; i<buttonRows; ++i){
        for(int j=0; j<m; ++j){
            buttonText[i][j] = '-';
        }
    }

    switch (dificuldade) {
        case 0:
            numberOfBombs = 10;
            numberOfFlags = 10;
            break;
        case 1:
            numberOfBombs = 40;
            numberOfFlags = 40;
            break;
        case 2:
            numberOfBombs = 99;
            numberOfFlags = 99;
            break;
    }

    for(int i=0; i<buttonRows; ++i){
        for(int j=0; j<buttonColumns; ++j){
            buttons[i][j] = new MyButton(this);
            buttons[i][j]->setMinimumSize(size, size);
            buttons[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gameLayout->addWidget(buttons[i][j], i+1, j+1);
        }
    }
}

void Widget::setBombs(){ // bombák beállítása
    int nBombas = numberOfBombs;
    while(nBombas > 0){
        int x = qrand() % buttonRows;
        int y = qrand() % buttonColumns;
        if (buttonText[x][y] == 'X'){
            continue;
        }
        buttonText[x][y] = 'X';
        --nBombas;
    }
}

void Widget::setNumbers(){
    for(int i=0; i<buttonRows; ++i){
        for(int j=0; j<buttonColumns; ++j){
            if(buttonText[i][j] == 'X'){
                continue;
            }
            int bombaAdjacente = 0;
            for(int x=i-1; x<i+2; ++x){
                for(int y=j-1; y<j+2; ++y){
                    if(x<0 || x>buttonRows-1 || y<0 || y>buttonColumns-1){
                        continue;
                    }
                    if(buttonText[x][y] == "X"){
                        ++bombaAdjacente;
                    }
                }
            }
            if(bombaAdjacente != 0){
                buttonText[i][j] = QString::number(bombaAdjacente);
            }
        }
    }
}

void Widget::reset(){
    endOfGame = false;
    disabledButtons = 0;

    if(dificuldade == 0){
        clearButtons();
        setButtons(8, 10, 50);
        setBombs();
        setNumbers();
        flags->setText("Bandeiras: 10");
    }
    else if(dificuldade == 1){
        clearButtons();
        setButtons(14, 18, 40);
        setBombs();
        setNumbers();
        flags->setText("Bandeiras: 40");
    }
    else{
        clearButtons();
        setButtons(20, 24, 30);
        setBombs();
        setNumbers();
        flags->setText("Bandeiras: 99");
    }
}
//possivelmente funcao cagada
void Widget::clearButtons(){
    while(gameLayout->count()){
       for(int i=0; i<gameLayout->count(); ++i){
           QWidget *tempWidget = gameLayout->itemAt(i++)->widget();
           gameLayout->removeWidget(tempWidget);
           delete tempWidget;
       }
    }
    delete buttons;
    delete buttonText;
}

Widget::~Widget()
{

}
