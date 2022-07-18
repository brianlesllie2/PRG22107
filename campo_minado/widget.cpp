#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    srand(time(NULL));

    gameLayout = new QGridLayout;
    setLayout(gameLayout);

    //inicializacao
    botoesDesativados = 0;
    dificuldade = 0;
    fimDeJogo = false;

    bandeiras = new QLabel;
    bandeiras->setText("Bandeiras: 10");
    gameLayout->addWidget(bandeiras, 0, 0);

    resetButton = new QPushButton("Reset");
    connect(resetButton, &QPushButton::clicked,  this, [&](){reset();} );
    gameLayout->addWidget(resetButton, 1, 0);

    difficulties = new QComboBox;
    difficulties->addItem("Facil");
    difficulties->addItem("Medio");
    difficulties->addItem("Dificil");
    connect(difficulties, SIGNAL(currentIndexChanged(int)), this, SLOT(mudaDificuldade()));
    gameLayout->addWidget(difficulties, 2, 0);

    criaBotoes(8, 10, 50);
    geraBombas();
    atribuiNumeroBombaAdj();
}

void Widget::criaBotoes(int n, int m, int size){
    numDeLinhas = n;
    numDeColunas = m;
    botoes = new MyButton**[numDeLinhas];
    valorBotao = new QString*[numDeLinhas];
    for(int i=0; i<numDeLinhas; ++i){
        botoes[i] = new MyButton*[numDeColunas];
        valorBotao[i] = new QString[numDeColunas];
    }
    for(int i=0; i<numDeLinhas; ++i){
        for(int j=0; j<numDeColunas; ++j){
            valorBotao[i][j] = '-';
        }
    }

    switch (dificuldade) {
        case 0:
            numDeBombas = 10;
            numDeBandeiras = 10;
            break;
        case 1:
            numDeBombas = 40;
            numDeBandeiras = 40;
            break;
        case 2:
            numDeBombas = 99;
            numDeBandeiras = 99;
            break;
    }

    for(int i=0; i<numDeLinhas; ++i){    //gera a matriz de botoes
        for(int j=0; j<numDeColunas; ++j){
            botoes[i][j] = new MyButton(this);
            botoes[i][j]->setMinimumSize(size, size);
            botoes[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gameLayout->addWidget(botoes[i][j], i+1, j+1);
            connect(botoes[i][j], &QPushButton::clicked,  this, [&](){mostraValorBotao();} );    //conecta clique esquerdo
            connect(botoes[i][j], SIGNAL(rightClick()), this, SLOT(rightButtonClicked())); //conecta clique direito
            botoes[i][j]->setStyleSheet("background-color:powderblue");    //cor dos botoes
        }
    }
}

void Widget::geraBombas(){
    int nBombas = numDeBombas;
    while(nBombas > 0){
        int x = rand() % numDeLinhas;
        int y = rand() % numDeColunas;
        if (valorBotao[x][y] == 'X'){
            continue;
        }
        valorBotao[x][y] = 'X';
        //botoes[x][y]->setText(valorBotao[x][y]);   //debug para ver se esta atribuindo valor
        --nBombas;
    }
}

void Widget::atribuiNumeroBombaAdj(){
    for(int i=0; i<numDeLinhas; ++i){
        for(int j=0; j<numDeColunas; ++j){
            if(valorBotao[i][j] == 'X'){
                continue;
            }
            int bombaAdjacente = 0;
            for(int x=i-1; x<i+2; ++x){
                for(int y=j-1; y<j+2; ++y){
                    if(x<0 || x>numDeLinhas-1 || y<0 || y>numDeColunas-1){
                        continue;
                    }
                    if(valorBotao[x][y] == "X"){
                        ++bombaAdjacente;
                    }
                }
            }
            if(bombaAdjacente != 0){
                valorBotao[i][j] = QString::number(bombaAdjacente);
                //botoes[i][j]->setText(valorBotao[i][j]);   //debug para ver se esta atribuindo valor
            }
        }
    }
}

void Widget::destroiBotoes(){
    for(int i=0; i<numDeLinhas; ++i){
        for(int j=0; j<numDeColunas; ++j){
            delete botoes[i][j];
        }
    }
}

void Widget::reset(){
    fimDeJogo = false;
    botoesDesativados = 0;

    if(dificuldade == 0){
        destroiBotoes();
        criaBotoes(8, 10, 50);
        geraBombas();
        atribuiNumeroBombaAdj();
        bandeiras->setText("Bandeiras: 10");
    }
    else if(dificuldade == 1){
        destroiBotoes();
        criaBotoes(14, 18, 40);
        geraBombas();
        atribuiNumeroBombaAdj();
        bandeiras->setText("Bandeiras: 40");
    }
    else{
        destroiBotoes();
        criaBotoes(20, 24, 30);
        geraBombas();
        atribuiNumeroBombaAdj();
        bandeiras->setText("Bandeiras: 99");
    }
}

void Widget::mostraValorBotao(){  //mostra o valor que esta no botao
    if(fimDeJogo){
        return;
    }

    int x, y;
    QPushButton* tmp = (QPushButton*)sender();
    if(tmp->styleSheet()=="background-color:orange"){
        return;
    }
    for(int i=0; i<numDeLinhas; i++){    //define o botao clicado em termos de x,y
        for(int j=0; j<numDeColunas; j++){
            if(botoes[i][j] == tmp){
                x = i;
                y = j;
                break;
            }
        }
    }
    if(valorBotao[x][y] == "X"){    //se clicar na bomba
        bandeiras->setText("Voce perdeu!");
        cliqueNaBomba();
    }

    else if(valorBotao[x][y] == '-'){   //abrir recursivamente caso clique em um espaco sem bomba
        botaoSemBombaAdj(x,y);
        if( botoesDesativados == ((numDeLinhas*numDeColunas) - numDeBombas) ){
            bandeiras->setText("Vitoria!");
            vitoria();
        }
    }
    else{   //caso seja um espaco com bomba ao redor
        botoes[x][y]->setText(valorBotao[x][y]);
        botoes[x][y] ->setStyleSheet("color:black");
        botoes[x][y] ->setStyleSheet("background-color:pink");
        botoes[x][y]->setEnabled(false);
        botoesDesativados++;
        if( botoesDesativados == ((numDeLinhas*numDeColunas) - numDeBombas) ){
            bandeiras->setText("Vitoria!");
            vitoria();
        }
    }
}

void Widget::cliqueNaBomba(){
    fimDeJogo = true;

    for(int i=0; i<numDeLinhas; ++i){
        for(int j=0; j<numDeColunas; ++j){
            if(valorBotao[i][j] == "X"){
                botoes[i][j]->setStyleSheet("background-color:red");
                botoes[i][j]->setText(valorBotao[i][j]);
            }
        }
    }

}

void Widget::botaoSemBombaAdj(int i, int j){    //metodo para abrir recursivamente
    if(i<0 || i>numDeLinhas-1 || j<0 || j>numDeColunas-1 || valorBotao[i][j] == "X" || !botoes[i][j]->isEnabled()){
        return;
    }
    if(valorBotao[i][j] == '-'){
        botoes[i][j]->setStyleSheet("background-color:white");
        botoes[i][j]->setEnabled(false);
        botoesDesativados++;
        for(int x=i-1; x<i+2; x++){
            for(int y=j-1; y<j+2; y++){
               botaoSemBombaAdj(x,y);
            }
        }
    }
    if(valorBotao[i][j] != '-'){
        botoes[i][j]->setText(valorBotao[i][j]);
        botoes[i][j] ->setStyleSheet("color:black");
        botoes[i][j] ->setStyleSheet("background-color:pink");
        botoes[i][j]->setEnabled(false);
        botoesDesativados++;
        return;
    }
}

void Widget::vitoria(){
    fimDeJogo = true;
}

void Widget::mudaDificuldade(){
    dificuldade = difficulties->currentIndex();
    reset();
}

void Widget::rightButtonClicked(){
    if(fimDeJogo){
        return;
    }
    if(numDeBandeiras==0){
        return;
    }
    QPushButton* tmp = (QPushButton*)sender();
    if(tmp->styleSheet()=="background-color:orange"){
       tmp->setStyleSheet("background-color:powderblue");
       numDeBandeiras++;
       bandeiras->setText("Bandeiras: " + QString::number(numDeBandeiras));
    }
    else{
       tmp->setStyleSheet("background-color:orange");
       numDeBandeiras--;
       bandeiras->setText("Bandeiras: " + QString::number(numDeBandeiras));
    }
}

Widget::~Widget()
{

}
