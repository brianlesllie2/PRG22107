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

    difficulties = new QComboBox;
    difficulties->addItem("Facil");
    difficulties->addItem("Medio");
    difficulties->addItem("Dificil");
    connect(difficulties, SIGNAL(currentIndexChanged(int)), this, SLOT(difficultyChanged()));
    gameLayout->addWidget(difficulties, 2, 0);


    flags = new QLabel;
    flags->setText("Bandeiras: 10");
    gameLayout->addWidget(flags, 0, 0);

    resetButton = new QPushButton("Reset");
    connect(resetButton, &QPushButton::clicked,  this, [&](){reset();} );
    gameLayout->addWidget(resetButton, 1, 0);

    setButtons(8, 10, 50);
    setBombs();
    setNumbers();
}

void Widget::setButtons(int n, int m, int size){
    buttonRows = n;
    buttonColumns = m;
    buttons = new MyButton**[buttonRows];
    buttonText = new QString*[buttonRows];
    for(int i=0; i<buttonRows; ++i){
        buttons[i] = new MyButton*[buttonColumns];
        buttonText[i] = new QString[buttonColumns];
    }
    for(int i=0; i<buttonRows; ++i){
        for(int j=0; j<buttonColumns; ++j){
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
            buttons[i][j]->installEventFilter(this);
            connect(buttons[i][j], &QPushButton::clicked,  this, [&](){showButton();} );
            connect(buttons[i][j], SIGNAL(rightClick()), this, SLOT(rightButtonClicked()));

            buttons[i][j]->setStyleSheet("background-color:powderblue");    //teste de cor
        }
    }
}

void Widget::setBombs(){
    int nBombas = numberOfBombs;
    while(nBombas > 0){
        int x = rand() % buttonRows;
        int y = rand() % buttonColumns;
        if (buttonText[x][y] == 'X'){
            continue;
        }
        buttonText[x][y] = 'X';
        //buttons[x][y]->setText(buttonText[x][y]);   //debug para ver se esta atribuindo valor
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
                //buttons[i][j]->setText(buttonText[i][j]);   //debug para ver se esta atribuindo valor
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


void Widget::bombClicked(){
    endOfGame = true;

    for(int i=0; i<buttonRows; ++i){
        for(int j=0; j<buttonColumns; ++j){
            if(buttonText[i][j] == "X"){
                buttons[i][j]->setStyleSheet("background-color:red");
                buttons[i][j]->setText(buttonText[i][j]);
            }
        }
    }

}

void Widget::showButton(){
    if(endOfGame){
        return;
    }

    int x, y;
    QPushButton* tmp = (QPushButton*)sender();
    if(tmp->styleSheet()=="background-color:orange"){
        return;
    }
    for(int i=0; i<buttonRows; i++){
        for(int j=0; j<buttonColumns; j++){
            if(buttons[i][j] == tmp){
                x = i;
                y = j;
                break;
            }
        }
    }
    if(buttonText[x][y] == "X"){    //clicar na bomba
        flags->setText("Voce perdeu!");
        bombClicked();
    }

    else if(buttonText[x][y] == '-'){   //abrir recursivamente
        posicaoAtualX = x;
        posicaoAtualY = y;
        botaoSemBombaAdj(posicaoAtualX,posicaoAtualY);
        if( disabledButtons == ((buttonRows*buttonColumns) - numberOfBombs) ){
            flags->setText("Vitoria!");
            vitoria();
        }
    }
    else{
        buttons[x][y]->setText(buttonText[x][y]);
        buttons[x][y] ->setStyleSheet("color:black");
        buttons[x][y] ->setStyleSheet("background-color:pink");
        buttons[x][y]->setEnabled(false);
        disabledButtons++;
        if( disabledButtons == ((buttonRows*buttonColumns) - numberOfBombs) ){
            flags->setText("Vitoria!");
            vitoria();
        }
    }
}

void Widget::botaoSemBombaAdj(int i, int j){
    if(i<0 || i>buttonRows-1 || j<0 || j>buttonColumns-1 || buttonText[i][j] == "X" || !buttons[i][j]->isEnabled()){
        return;
    }
    if(buttonText[i][j] == '-'){
        buttons[i][j]->setStyleSheet("background-color:white");
        buttons[i][j]->setEnabled(false);
        disabledButtons++;
        for(int x=i-1; x<i+2; x++){
            for(int y=j-1; y<j+2; y++){
               botaoSemBombaAdj(x,y);
            }
        }
    }
    if(buttonText[i][j] != '-'){
        buttons[i][j]->setText(buttonText[i][j]);
        buttons[i][j] ->setStyleSheet("color:black");
        buttons[i][j] ->setStyleSheet("background-color:pink");
        buttons[i][j]->setEnabled(false);
        disabledButtons++;
        return;
    }
}

void Widget::difficultyChanged(){
    dificuldade = difficulties->currentIndex();
    reset();
}

void Widget::clearButtons(){
    for(int i=0; i<buttonRows; ++i){
        for(int j=0; j<buttonColumns; ++j){
            delete buttons[i][j];
        }
    }
}

void Widget::vitoria(){
    endOfGame = true;
}

void Widget::rightButtonClicked(){
    if(endOfGame){
        return;
    }
    if(numberOfFlags==0){
        return;
    }
    QPushButton* tmp = (QPushButton*)sender();
    if(tmp->styleSheet()=="background-color:orange"){
       tmp->setStyleSheet("background-color:powderblue");
       numberOfFlags++;
       flags->setText("Bandeiras: " + QString::number(numberOfFlags));
    }
    else{
       tmp->setStyleSheet("background-color:orange");
       numberOfFlags--;
       flags->setText("Bandeiras: " + QString::number(numberOfFlags));
    }
}

Widget::~Widget()
{

}
