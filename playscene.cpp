#include "playscene.h"
#include<QDebug>
#include<QPixmap>
#include<QMenuBar>
#include<QAction>
#include<QPainter>
#include<mypushbutton.h>
#include<QTimer>
#include<QLabel>
#include<QRect>
#include<mycoin.h>
#include<dataconfig.h>
#include<QPropertyAnimation>
#include<QSound>
//PlayScene::PlayScene(QWidget *parent)
//    : QMainWindow{parent}
//{

//}
PlayScene::PlayScene(int index){
    this->index=index;
    qDebug()<<"用户选择的是:"<<this->index;
    QSound * backSound=new QSound(":/res/BackButtonSound.wav");
    QSound * conflipSound=new QSound(":/res/ConFlipSound.wav");
    QSound * winSound=new QSound(":/res/LevelWinSound.wav");


    this->setFixedSize(320,580);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("翻金币");
    QMenuBar *bar=this->menuBar();
    this->setMenuBar(bar);

    bar->setNativeMenuBar(false);
    QMenu * startMenu=bar->addMenu("开始");
    QAction * quitAction=startMenu->addAction("退出");
    connect(quitAction,&QAction::triggered,[=](){this->close();});


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

    QLabel * label=new QLabel;
    label->setParent(this);
    QString str=QString("leave: %1").arg(this->index);
    label->setText(str);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    label->setGeometry(QRect(30,this->height()-50,120,50));

    dataConfig config;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            this->gameArray[i][j]=config.mData[this->index][i][j];
        }
    }

    QLabel * winLabel=new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())/2,-tmpPix.height());

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            QLabel * lb=new QLabel;
            QPixmap pix;
            pix.load(":/res/BoardNode.png");
            lb->setGeometry(0,0,pix.width(),pix.height());
            lb->setPixmap(pix);
            lb->setParent(this);
            lb->move(57+i*pix.width(),200+j*pix.height());

            QString str;
            if(this->gameArray[i][j]==1){
                str=":/res/Coin0001.png";
            }else{
                str=":/res/Coin0008.png";

            }
            MyCoin * coin=new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*pix.width(),204+j*pix.height());
            coin->posx=i;
            coin->posy=j;
            coin->flag=this->gameArray[i][j];

            coinBtn[i][j]=coin;


            connect(coin,&MyCoin::clicked,[=](){
                conflipSound->play();
                for(int i=0;i<4;i++){
                    for(int j=0;j<4;j++){
                        coinBtn[i][j]->isWin=true;
                    }
                }

               coin->changeFlag();
               this->gameArray[i][j]=this->gameArray[i][j]==1?0:1;

               QTimer::singleShot(300,this,[=](){
                   if(coin->posx+1<=3){
                       coinBtn[coin->posx+1][coin->posy]->changeFlag();
                       this->gameArray[coin->posx+1][coin->posy]=this->gameArray[coin->posx+1][coin->posy]==1?0:1;

                   }
                   if(coin->posx-1>=0){
                       coinBtn[coin->posx-1][coin->posy]->changeFlag();
                       this->gameArray[coin->posx-1][coin->posy]=this->gameArray[coin->posx-1][coin->posy]==1?0:1;

                   }
                   if(coin->posy-1>=0){
                       coinBtn[coin->posx][coin->posy-1]->changeFlag();
                       this->gameArray[coin->posx][coin->posy-1]=this->gameArray[coin->posx][coin->posy-1]==1?0:1;

                   }
                   if(coin->posy+1<=3){
                       coinBtn[coin->posx][coin->posy+1]->changeFlag();
                       this->gameArray[coin->posx][coin->posy+1]=this->gameArray[coin->posx][coin->posy+1]==1?0:1;

                   }
                   for(int i=0;i<4;i++){
                       for(int j=0;j<4;j++){
                           coinBtn[i][j]->isWin=false;
                       }
                   }


                   isWin=true;
                   for(int i=0;i<4;i++){
                       for(int j=0;j<4;j++){
                           if(coinBtn[i][j]->flag==false){
                               isWin=false;
                               break;
                           }
                       }
                   }
                   if(isWin){
                       qDebug()<<"you are win!!";
                       winSound->play();

                       for(int i=0;i<4;i++){
                           for(int j=0;j<4;j++){
                               coinBtn[i][j]->isWin=true;
                           }
                       }

                       QPropertyAnimation * animation=new QPropertyAnimation(winLabel,"geometry");
                       animation->setDuration(1000);
                       animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                       animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                       animation->setEasingCurve(QEasingCurve::OutBounce);
                       animation->start(QAbstractAnimation::DeleteWhenStopped);

                   }

               });


            });
        }
    }
}
void PlayScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()/2,pix.height()/2);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);

}

