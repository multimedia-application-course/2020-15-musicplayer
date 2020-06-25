#ifndef PLAYERSHOW_H
#define PLAYERSHOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QListWidget>
#include <QFileDialog>
#include <QtSql/QSqlTableModel>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>
#include <QTextCodec>
#include <QMessageBox>
#include "minishow.h"
namespace Ui {
class PlayerShow;
class MiniShow;
}

class PlayerShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerShow(QWidget *parent = nullptr);
    ~PlayerShow();

private:
    void loadTable();//加载本地数据库的table到listwidget
    void loadMusicFromTable(QString tab);//从标签加载音乐
    void init();//各控件初始化
    //void addMusicToDataBase();
    //void addTableToDataBase();
private slots:
    void on_listWidget_clicked(const QModelIndex &index);//单击获取歌单列表
    void on_listWidget_2_doubleClicked(const QModelIndex &index);//歌曲双击播放
    void windowShow();//托盘点击事件
    void musicSearch();//搜索和新建歌单是同一个lineEdit
    void addTable();//添加歌单的槽函数
    void action_delete();//删除歌单得分槽函数
    void action2_delete();//删除歌曲的槽函数
    void updateMusicInfo();//当播放新歌曲的时候响应更新歌名
    void buttonPlay();//播放按钮
    void buttonLast();//上一首
    void buttonNext();//下一首
    void buttonMini();//迷你化槽函数
    void getmusicFiles();//获取歌曲
    void getmusicDir();//获取歌曲目录
    void setVolume(int value);//设置歌曲的音量
    void musicTime(int pt);//更新歌曲时间
    void preMusicPosition();//准备设置播放进度
    void getMusicPosition(int pt);//获取拖动条进度
    void setMusicPosition();//设定播放进度
    void listMenu1(QPoint);//歌单列表的菜单槽函数
    void listMenu2(QPoint);//歌曲列表的菜单槽函数
public slots:
    void buttonHide();//隐藏按钮
protected:
    void mousePressEvent(QMouseEvent *e);//鼠标点击函数
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动的槽函数
    void mouseReleaseEvent(QMouseEvent *e);//鼠标放开的槽函数
private:
    Ui::PlayerShow *ui;//ui
    Ui::MiniShow* miniUi;
    MiniShow *minishow;
    QMediaPlayer *MusicPlayer;//播放类
    QMediaPlaylist *MusicList;//播放列表类
    QSystemTrayIcon * SystemIcon;//系统托盘图标
    QPoint Moveoffset;//鼠标移动偏移，实现点击移动
    bool MoveNote;//窗口移动标志
    int MusicTimeAt;//歌曲播放时间
    QString selectedTable;//被选中的歌单的名字
    int selectedTableId;//被选中歌单的id
    QString selectedMusic;//被选中的歌曲名字
    int selectedMusicId;//被选中的歌曲的id
    bool Search_AddTable;//true为search，false为addtable
    bool playnote;//音乐正在播放的标志
    int musicNumber;//歌单歌曲数目
    //
    QMenu *Menu1;//菜单1
    QMenu *Menu2;//菜单2
    //
};

#endif // PLAYERSHOW_H
