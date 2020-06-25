#include "minishow.h"
#include "ui_minishow.h"
#include "playershow.h"

MiniShow::MiniShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiniShow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去除边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    MoveNote=false;
}

MiniShow::~MiniShow()
{
    delete ui;
}

void MiniShow::setPlayShow(PlayerShow* p){
    this->playshow = p;
}

Ui::MiniShow* MiniShow::getUi(){
    return this->ui;
}

//关闭按钮
void MiniShow::on_pushButton_clicked()
{
    this->close();
    playshow->close();
}
//最大化
void MiniShow::on_pushButton_3_clicked()
{
    this->hide();
    playshow->show();
}

//最小化到任务栏
void MiniShow::on_pushButton_2_clicked()
{
    this->hide();
    playshow->buttonHide();
}

void MiniShow::mousePressEvent(QMouseEvent *e)//鼠标事件
{
    //获取鼠标点击偏移
    Moveoffset=e->globalPos()-pos();
    //设置拖动标志为true
    MoveNote=true;
}
void MiniShow::mouseMoveEvent(QMouseEvent *e)
{
    //如果标志为true，则实时更新窗口位置
    if(MoveNote)
    move(e->globalPos()-Moveoffset);
}
void MiniShow::mouseReleaseEvent(QMouseEvent *e)
{
    //如果标志为true，更新窗口位置
    if(MoveNote)
    move(e->globalPos()-Moveoffset);
    MoveNote=false;
}
