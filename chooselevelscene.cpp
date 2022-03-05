#include "chooselevelscene.h"
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QPixmap>
#include<QPainter>
#include<mypushbutton.h>
#include<QDebug>
#include<QTimer>
#include<QLabel>
#include<playscene.h>
#include<QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    QMenuBar * bar=menuBar();
    QMenu * startMenu=bar->addMenu("开始");
    QAction * quitAction=startMenu->addAction("退出");
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });
    this->setWindowTitle("选择关卡");
    //开始音效准备
    QSound * chooseSound=new QSound(":/res/TapButtonSound.wav");
    QSound * backSound=new QSound(":/res/BackButtonSound.wav");

    this->setFixedSize(320,580);
   // this->setBaseSize(320,620);
    bar->setNativeMenuBar(false);

    MyPushButton * backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
    connect(backBtn,&MyPushButton::clicked,[=](){
        //qDebug()<<"返回到主场景";
        backSound->play();
        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();

        });
    });

    playScene=NULL;


    for(int i=0;i<20;i++){
        MyPushButton * menuBtn=new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        //menuBtn->setText(QString::number(i+1));
        menuBtn->move(25+(i%4)*70,130+i/4*70);
        QLabel * label=new QLabel;
        label->setParent(this);
        connect(menuBtn,&MyPushButton::clicked,[=](){
       //     qDebug()<<"你选择的是第"<<i+1<<"关.";
            playScene=new PlayScene(i+1);
            chooseSound->play();
            this->hide();
            playScene->setGeometry(this->geometry());
            this->playScene->show();

            connect(this->playScene,&PlayScene::chooseSceneBack,[=](){
                delete this->playScene;
                this->setGeometry(playScene->geometry());
                this->playScene=NULL;

                this->show();

            });

        });

        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
        label->move(25+(i%4)*70,130+i/4*70);
    }


}
void ChooseLevelScene::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/Title.png");
    painter.drawPixmap(this->width()/2-pix.width()/2,30,pix.width(),pix.height(),pix);
}

