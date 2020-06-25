#include "playershow.h"
#include "ui_playershow.h"
#include "ui_minishow.h"
#include <QDebug>
PlayerShow::PlayerShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerShow)
{
    ui->setupUi(this);
    qDebug()<<1;
    init();//初始化
}
PlayerShow::~PlayerShow()
{
    delete MusicPlayer;
    delete MusicList;
    delete ui;
}
void PlayerShow::init()//各控件初始化
{
    //设置应用图标
    this->setWindowIcon(QIcon(":/new/prefix1/miku.png"));
    //初始化mini窗口
    minishow = new MiniShow();
    minishow->setPlayShow(this);
    miniUi = minishow->getUi();//迷你窗口的Ui指针
    //初始化播放类
    MusicPlayer=new QMediaPlayer();
    MusicList=new QMediaPlaylist();
    Search_AddTable=true;
    //设置播放模式：循环
    MusicList->setPlaybackMode(QMediaPlaylist::Loop);
    MusicPlayer->setPlaylist(MusicList);
    //设置音量50
    MusicPlayer->setVolume(50);
    //初始化系统托盘图标
    SystemIcon=new QSystemTrayIcon(this);
    SystemIcon->setIcon(QIcon(":/new/prefix1/miku_icon.png"));
    //移动和播放标志都设置为false
    MoveNote=false;
    playnote=false;
    //设置样式，无边框，背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);//去除边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    //this->ui->frame->setAttribute(Qt::WA_TranslucentBackground);
    //初始化窗口各控件
    ui->listWidget_2->setModelColumn(2);
    ui->horizontalSlider_2->setRange(0,100);
    ui->horizontalSlider_2->setValue(50);
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->pushButton_6->setIcon(QIcon(":/new/prefix1/img/stop.png"));
    ui->pushButton_4->setIcon(QIcon(":/new/prefix1/img/search.png"));

    miniUi->horizontalSlider_2->setRange(0,100);
    miniUi->horizontalSlider_2->setValue(50);
    miniUi->pushButton_6->setIcon(QIcon(":/new/prefix1/img/stop.png"));
    //连接相应按钮和控件和槽函数
    connect(ui->pushButton,&QPushButton::clicked,this,&QMainWindow::close);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&PlayerShow::buttonMini);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&PlayerShow::musicSearch);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&PlayerShow::buttonLast);
    connect(ui->pushButton_6,&QPushButton::clicked,this,&PlayerShow::buttonPlay);
    connect(ui->pushButton_7,&QPushButton::clicked,this,&PlayerShow::buttonNext);
    connect(ui->pushButton_8,&QPushButton::clicked,this,&PlayerShow::getmusicFiles);
    connect(ui->pushButton_9,&QPushButton::clicked,this,&PlayerShow::getmusicDir);
    connect(ui->pushButton_13,&QPushButton::clicked,this,&PlayerShow::addTable);

    //迷你窗口的上一首，暂停，下一首按钮
    connect(miniUi->pushButton_5,&QPushButton::clicked,this,&PlayerShow::buttonLast);
    connect(miniUi->pushButton_6,&QPushButton::clicked,this,&PlayerShow::buttonPlay);
    connect(miniUi->pushButton_7,&QPushButton::clicked,this,&PlayerShow::buttonNext);

    //音乐播放时自动更新主窗口进度条
    connect(ui->horizontalSlider_2,&QSlider::valueChanged,this,&PlayerShow::setVolume);
    connect(MusicPlayer,&QMediaPlayer::positionChanged,ui->horizontalSlider,&QSlider::setValue);
    connect(MusicPlayer,&QMediaPlayer::durationChanged,this,&PlayerShow::musicTime);

    //音乐播放时自动更新迷你窗口进度条
    connect(miniUi->horizontalSlider_2,&QSlider::valueChanged,this,&PlayerShow::setVolume);
    connect(MusicPlayer,&QMediaPlayer::positionChanged,miniUi->horizontalSlider,&QSlider::setValue);

    //点击播放进度条事件
    connect(ui->horizontalSlider,&QSlider::sliderPressed,this,&PlayerShow::preMusicPosition);
    connect(ui->horizontalSlider,&QSlider::sliderMoved,this,&PlayerShow::getMusicPosition);
    connect(ui->horizontalSlider,&QSlider::sliderReleased,this,&PlayerShow::setMusicPosition);
    //点击迷你窗口播放进度条事件
    connect(miniUi->horizontalSlider,&QSlider::sliderPressed,this,&PlayerShow::preMusicPosition);
    connect(miniUi->horizontalSlider,&QSlider::sliderMoved,this,&PlayerShow::getMusicPosition);
    connect(miniUi->horizontalSlider,&QSlider::sliderReleased,this,&PlayerShow::setMusicPosition);

    //任务栏图标事件
    connect(SystemIcon,&QSystemTrayIcon::activated,this,&PlayerShow::windowShow);
    //最小化
    connect(ui->pushButton_2,&QPushButton::clicked,this,&PlayerShow::buttonHide);
    //更新进度条
    connect(MusicPlayer,&QMediaPlayer::metaDataAvailableChanged,this,&PlayerShow::updateMusicInfo);
    //初始化菜单
    Menu1=new QMenu;
    //Menu1->setStyleSheet("font: 17pt '宋体';");
    QAction *act11=new QAction("delete",this);
    act11->setIcon(QIcon(":/new/prefix1/img/stop.png"));
    act11->setFont(QFont("MS Shell Dlg 2"));
    Menu1->addAction(act11);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(act11,&QAction::triggered,this,&PlayerShow::action_delete);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested,this,&PlayerShow::listMenu1);

    Menu2=new QMenu;
    //Menu2->setStyleSheet("font: 15pt '宋体';");
    QAction *act21=new QAction("deleteno",this);
    Menu2->addAction(act21);
    ui->listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(act21,&QAction::triggered,this,&PlayerShow::action2_delete);
    connect(ui->listWidget_2, &QListWidget::customContextMenuRequested,this,&PlayerShow::listMenu2);
    //各个控件的帮助tip
    ui->pushButton->setToolTip(QString("Close"));
    ui->pushButton_2->setToolTip(QString("Hide"));
    ui->pushButton_3->setToolTip(QString("Mini"));
    ui->pushButton_4->setToolTip(QString("Search/Add"));
    ui->pushButton_5->setToolTip(QString("Previous"));
    ui->pushButton_6->setToolTip(QString("Play/Stop"));
    miniUi->pushButton_6->setToolTip(QString("Play/Stop"));
    ui->pushButton_7->setToolTip(QString("Next"));
    ui->pushButton_8->setToolTip(QString("AddMusicFiles"));
    ui->pushButton_9->setToolTip(QString("AddMusicDir"));
    ui->pushButton_13->setToolTip(QString("AddMusicList"));
    //
    //初始化完毕后加载一个默认的歌单
    loadTable();
    loadMusicFromTable(selectedTable);
}
void PlayerShow::listMenu1(QPoint)//歌单菜单
{
    if(ui->listWidget->selectedItems().size()>0)
    {
    selectedTable=ui->listWidget->selectedItems()[0]->text();
    selectedTableId=ui->listWidget->row(ui->listWidget->selectedItems()[0]);
    loadMusicFromTable(selectedTable);//将歌单设置为被选中
    Menu1->exec(QCursor::pos());
    }
}
void PlayerShow::action_delete()//歌单删除
{
    QSqlQuery query;
    query.exec(QString("delete from AllTable where TableName='%1'").arg(selectedTable));//删除歌单标签中该table
    query.exec(QString("drop table %1").arg(selectedTable));//删除标签
    query.exec("select * from AllTable");//查看是否有歌单在AllTable中如果没有，则添加一个默认歌单Local
    if(!query.next())
    {
        query.exec(QString("create table Local (MusicPath varchar,MusicName varchar,primary key(MusicPath,MusicName))"));
        query.exec(QString("insert into AllTable values('Local')"));
        selectedTable="Local";
    }
    else {
        selectedTable=query.value(0).toString();
    }
    loadTable();//重新加载歌单
    loadMusicFromTable(selectedTable);
}
void PlayerShow::listMenu2(QPoint)//歌曲列表菜单
{
    if(ui->listWidget_2->selectedItems().size()>0)
    {
    selectedMusic=ui->listWidget_2->selectedItems()[0]->text();
    selectedMusicId=ui->listWidget_2->row(ui->listWidget_2->selectedItems()[0]);
    Menu2->exec(QCursor::pos());
    }
}
void PlayerShow::action2_delete()
{
    if(musicNumber==0)return;
    QSqlQuery query;
    query.exec(QString("select * from '%1'").arg(selectedTable));
    if(query.seek(selectedMusicId))
    {
    QString path=query.value(0).toString();
    //qDebug()<<"delete"<<path;
    query.exec(QString("delete from '%1' where MusicPath='%2'").arg(selectedTable).arg(path));//删除歌曲
    loadMusicFromTable(selectedTable);
    musicNumber--;
    }
}
void PlayerShow::on_listWidget_clicked(const QModelIndex &index)//歌单列表单击函数
{
    selectedTable=index.data().toString();//歌单设置为被选中
    selectedTableId=index.row();
    loadMusicFromTable(selectedTable);//从该歌单中加载歌曲
}
void PlayerShow::on_listWidget_2_doubleClicked(const QModelIndex &index)//歌曲列表双击播放函数
{
    if(musicNumber==0)return;
    selectedMusic=index.data().toString();//设置为被选中歌曲
    selectedMusicId=index.row()-1;
    MusicList->clear();
    QSqlQuery query;
    query.exec(QString("select * from %1").arg(selectedTable));
    while(query.next())
        MusicList->addMedia(QUrl::fromLocalFile(query.value(0).toString()));//逐一将歌曲加入播放列表
    MusicList->next();
    for(int i=0;i<index.row();i++)//将歌曲跳到被选中的歌曲处
        MusicList->next();
    //MusicList->nextIndex(index.row());
    MusicPlayer->play();
    ui->pushButton_6->setIcon(QIcon(":/new/prefix1/img/play.png"));
    miniUi->pushButton_6->setIcon(QIcon(":/new/prefix1/img/play.png"));
    //query.exec(QString("delete from %1 where MusicName='%2'").arg(selectedTable).arg(selectedMusic));
    playnote=true;
    loadMusicFromTable(selectedTable);
}
void PlayerShow::addTable()//添加歌单
{
    ui->lineEdit->setToolTip("hello");
    ui->pushButton_4->setIcon(QIcon(":/new/prefix1/img/add11.png"));//搜索按钮图标改为加号的图标
    Search_AddTable=false;
}
void PlayerShow::musicSearch()//搜索和添加歌单归为同一个函数
{
    if(Search_AddTable)//true为搜索
    {
        QString searchMsg=ui->lineEdit->text();
        if(searchMsg=="")return;
        QSqlQuery query,queryt;
        query.exec("drop table Search");
        query.exec(QString("create table Search (MusicPath varchar,MusicName varchar,primary key(MusicPath,MusicName))"));
        queryt.exec(QString("select * from %1").arg(selectedTable));
        while (queryt.next()) {
            if(queryt.value(1).toString().contains(searchMsg,Qt::CaseInsensitive))
            {
                query.exec(QString("insert into Search values('%1','%2')").arg(queryt.value(0).toString()).arg(queryt.value(1).toString()));
            }
        }
        selectedTable="Search";
        loadMusicFromTable(selectedTable);
    }
    else {
        ui->pushButton_4->setIcon(QIcon(":/new/prefix1/img/search.png"));
        Search_AddTable=true;
        QString temp=ui->lineEdit->text();
        if(temp==""||temp.length()>6){
            QMessageBox(QMessageBox::NoIcon,"Errors","The list name couldn't be null or longer than 6 chars.").exec();
            return;
        }
        temp.replace(" ","");
        //qDebug()<<temp;
        selectedTable=temp;
        selectedTableId=ui->listWidget->count();
        QSqlQuery query;
        query.exec(QString("insert into AllTable values('%1')").arg(temp));
        loadTable();
        loadMusicFromTable(selectedTable);
    }
}
void PlayerShow::loadTable()//加载数据库中的歌单
{
    ui->listWidget->clear();//清除歌单列表所有项
    //MusicList->addMedia(QUrl::fromLocalFile("E:\\CloudMusic\\Valentin - A Little Story.mp3"));
    QSqlQuery query;
    query.exec("select * from AllTable");
    if(!query.next())return;//无歌单则返回
    selectedTable=query.value(0).toString();
    selectedTableId=0;
    for(int i=0;query.seek(i);i++)
    {
        QString table=query.value(0).toString();
        query.exec(QString("create table %1 (MusicPath varchar,MusicName varchar,primary key(MusicPath,MusicName))").arg(table));
        //qDebug()<<i<<"***"<<table;
        query.exec("select * from AllTable");
    }
    //qDebug()<<"*********";
    query.exec("select * from AllTable");
    query.next();
    do
    {
        QListWidgetItem * temp=new QListWidgetItem;
        temp->setText(query.value(0).toString());
        temp->setIcon(QIcon(":/new/prefix1/img/music.png"));//设置图标
        ui->listWidget->addItem(temp);
    }
    while(query.next());
}
void PlayerShow::loadMusicFromTable(QString tab)//从数据库歌单中添加歌曲到列表
{
    QSqlQuery query;
    query.exec(QString("select * from %1").arg(tab));//选择
    ui->listWidget_2->clear();//清除歌曲列表
    musicNumber=0;//设置歌曲数目为0
    if(!query.next())//无歌曲添加一个item显示没有歌曲
    {
        QListWidgetItem* item=new QListWidgetItem;
        item->setText(QString("歌单\"%1\"没有歌曲").arg(tab));
        ui->listWidget_2->addItem(item);
        return;
    }
    do//有歌曲逐一添加歌曲
    {
        QListWidgetItem* item=new QListWidgetItem;
        QString temp=query.value(1).toString();
        temp.replace(".mp3","");
        item->setText(temp);
        ui->listWidget_2->addItem(item);
        musicNumber++;
    }
    while (query.next());
}

void PlayerShow::updateMusicInfo()//当播放歌曲改变时更新歌曲信息
{
    static bool count=true;
    if(count)
    {
    QString Info=MusicPlayer->metaData(QStringLiteral("Title")).toString()+" by "//歌曲名字和作者
               +MusicPlayer->metaData(QStringLiteral("Author")).toString();
    selectedMusicId++;
    if(selectedMusicId+1>musicNumber)
        selectedMusicId=0;
    selectedMusic=ui->listWidget_2->item(selectedMusicId)->text();
    //qDebug()<<selectedMusic;
    ui->listWidget_2->item(selectedMusicId)->setSelected(true);
    ui->label_2->setText(Info);
    }
    count=!count;
}

void PlayerShow::buttonMini()
{
    this->hide();
    minishow->show();
}

void PlayerShow::getmusicFiles()//获取歌曲文件
{
    //调用一个系统资源管理器获取文件信息
    QStringList list=QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("SelectMP3"),
                                                   QString("C:/"),QString::fromLocal8Bit("Files(*.mp3)"));
    QSqlQuery query;
    for(int i=0;i<list.size();i++)
    {
        QString MusicName=list[i].split("/").last();//根据’/‘划分出歌曲名字
        query.exec(QString("insert into %1 values('%2','%3')").arg(selectedTable).arg(list[i]).arg(MusicName));
    }
    loadMusicFromTable(selectedTable);
}
void PlayerShow::getmusicDir()//获取歌曲文件夹
{
    //调用一个系统资源管理器来获取一个文件夹
    QString MusicDir= QFileDialog::getExistingDirectory(this,"SelectDir",QString("C:/"));
    QDir temp=QDir(MusicDir);
    //设置过滤器
    QStringList filters;
    filters<<"*.mp3";
    //过滤出mp3文件
    QFileInfoList list=temp.entryInfoList(filters,QDir::AllDirs|QDir::Files);
    QSqlQuery query;
    //将歌曲添加到数据库
    for(int i=2;i<list.size();i++)
    {
        if(list[i].isDir())continue;
        list[i].makeAbsolute();
        QString MusicName=list[i].filePath().split("/").last();
        query.exec(QString("insert into %1 values('%2','%3')").arg(selectedTable).arg(list[i].filePath()).arg(MusicName));
    }
    loadMusicFromTable(selectedTable);
}

void PlayerShow::setVolume(int value){
    MusicPlayer->setVolume(value);
    ui->horizontalSlider_2->setValue(value);
    miniUi->horizontalSlider_2->setValue(value);
}

void PlayerShow::musicTime(int pt)//更新歌曲的事件
{
    ui->horizontalSlider->setRange(0,pt);//设置歌曲播放条
    miniUi->horizontalSlider->setRange(0,pt);
}
void PlayerShow::preMusicPosition()//歌曲播放条点击响应，将歌曲暂停，设置播放状态为暂停
{
    disconnect(MusicPlayer,&QMediaPlayer::positionChanged,ui->horizontalSlider,&QSlider::setValue);//
    disconnect(MusicPlayer,&QMediaPlayer::positionChanged,miniUi->horizontalSlider,&QSlider::setValue);//
    MusicPlayer->pause();
    ui->pushButton_6->setIcon(QIcon(":/new/prefix1/img/stop.png"));
    miniUi->pushButton_6->setIcon(QIcon(":/new/prefix1/img/stop.png"));
    getMusicPosition(int(MusicPlayer->position()));
}
void PlayerShow::getMusicPosition(int pt)//设置歌曲播放位置
{
    MusicTimeAt=pt;//获取歌曲的播放位置
}
void PlayerShow::setMusicPosition()
{
    //设置歌曲播放位置
    MusicPlayer->setPosition(MusicTimeAt);
    MusicPlayer->play();
    ui->pushButton_6->setIcon(QIcon(":/new/prefix1/img/play.png"));
    miniUi->pushButton_6->setIcon(QIcon(":/new/prefix1/img/play.png"));
    connect(MusicPlayer,&QMediaPlayer::positionChanged,ui->horizontalSlider,&QSlider::setValue);
    connect(MusicPlayer,&QMediaPlayer::positionChanged,miniUi->horizontalSlider,&QSlider::setValue);
}
void PlayerShow::windowShow()//系统托盘图标点击响应函数
{
    this->show();
    SystemIcon->hide();
}
void PlayerShow::buttonHide()//点击显示系统托盘图标
{
    this->hide();
    SystemIcon->show();
}
void PlayerShow::buttonPlay()//播放按钮事件
{
    //if(musicNumber==0)return;
    playnote=!playnote;
    if(playnote)
    {
        MusicPlayer->play();
        if(MusicPlayer->error()!=QMediaPlayer::ResourceError){
        ui->pushButton_6->setIcon(QIcon(":/new/prefix1/img/play.png"));
        miniUi->pushButton_6->setIcon(QIcon(":/new/prefix1/img/play.png"));
        }
        else playnote=!playnote;
    }
    else
    {
        MusicPlayer->pause();
        ui->pushButton_6->setIcon(QIcon(":/new/prefix1/img/stop.png"));
        miniUi->pushButton_6->setIcon(QIcon(":/new/prefix1/img/stop.png"));
    }
}
void PlayerShow::buttonLast()//上一首按钮事件
{
    MusicList->previous();
    selectedMusicId-=2;
    if(selectedMusicId<0)selectedMusicId=0;
}
void PlayerShow::buttonNext()//下一首按钮事件
{
    MusicList->next();
}
void PlayerShow::mousePressEvent(QMouseEvent *e)//鼠标事件
{
    //获取鼠标点击偏移
    Moveoffset=e->globalPos()-pos();
    //设置拖动标志为true
    MoveNote=true;
}
void PlayerShow::mouseMoveEvent(QMouseEvent *e)
{
    //如果标志为true，则实时更新窗口位置
    if(MoveNote)
    move(e->globalPos()-Moveoffset);
}
void PlayerShow::mouseReleaseEvent(QMouseEvent *e)
{
    //如果标志为true，更新窗口位置
    if(MoveNote)
    move(e->globalPos()-Moveoffset);
    MoveNote=false;
}
