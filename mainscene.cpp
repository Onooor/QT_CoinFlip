#include "mainscene.h"
#include "ui_mainscene.h"
#include<QAction>
#include<QPainter>
#include<mypushbutton.h>
#include<QDebug>
#include<QTimer>
#include<QPixmap>
#include<QSound>
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //主场景基本配置
    this->setWindowTitle("翻金币主场景");

    this->setFixedSize(320,580);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    //开始音效准备
    QSound * startSound=new QSound(":/res/TapButtonSound.wav");

    chooseScene=new ChooseLevelScene;


    MyPushButton  *startBtn=new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->move(this->width()/2-startBtn->width()/2,this->height()*0.7);
    startBtn->setParent(this);

    //监听返回信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
        chooseScene->hide();
        this->setGeometry(chooseScene->geometry());
        this->show();
    });

    connect(startBtn,&MyPushButton::clicked,[=](){
        startSound->play();
        startBtn->zoom1();
        startBtn->zoom2();
        QTimer::singleShot(500,this,[=](){
            this->hide();
            chooseScene->setGeometry(this->geometry());
            chooseScene->show();
        });

    });



}
void MainScene::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,320,580,pix);
    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()/2,pix.height()/2);
    painter.drawPixmap(10,30,pix);






}



MainScene::~MainScene()
{
    delete ui;
}

