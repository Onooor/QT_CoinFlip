#include "mycoin.h"
#include<QDebug>
#include<QPixmap>
#include<QTimer>

//MyCoin::MyCoin(QWidget *parent)
//    : QPushButton{parent}
//{

//}

MyCoin::MyCoin(QString imgPath){
    QPixmap pix;
    bool ret=pix.load(imgPath);
    if(!ret){
        qDebug()<<"图片加载失败"<<imgPath;
        return;

    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QpushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    timer1=new QTimer(this);
    timer2=new QTimer(this);

    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1.png").arg(this->min++);
        bool ret=pix.load(str);
        if(!ret){
            qDebug()<<"图片加载失败"<<imgPath;
            return;

        }
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QpushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        if(this->min>this->max){
            this->min=1;
            this->isAnimation=false;
            timer1->stop();
        }
    });

    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1.png").arg(this->max--);
        bool ret=pix.load(str);
        if(!ret){
            qDebug()<<"图片加载失败"<<imgPath;
            return;

        }
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QpushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        if(this->max<this->min){
            this->max=8;
            this->isAnimation=false;

            timer2->stop();
        }
    });


}

void MyCoin::changeFlag(){

    if(this->flag){
        timer1->start(30);
        this->isAnimation=true;
        this->flag=false;
    }else{
        this->flag=true;
        this->isAnimation=true;
        timer2->start(30);
    }
}
void MyCoin::mousePressEvent(QMouseEvent *e){
    if(this->isAnimation|| this->isWin){
        return;
    }else{
        QPushButton::mousePressEvent(e);
    }
}

