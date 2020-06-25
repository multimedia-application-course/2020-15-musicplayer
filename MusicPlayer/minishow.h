#ifndef MINISHOW_H
#define MINISHOW_H

#include <QWidget>

namespace Ui {
class MiniShow;
}
class PlayerShow;

class MiniShow : public QWidget
{
    Q_OBJECT

public:
    explicit MiniShow(QWidget *parent = nullptr);
    ~MiniShow();
    void setPlayShow(PlayerShow *p);
    Ui::MiniShow* getUi();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

protected:
    void mousePressEvent(QMouseEvent *e);//鼠标点击函数
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动的槽函数
    void mouseReleaseEvent(QMouseEvent *e);//鼠标放开的槽函数

private:
    Ui::MiniShow *ui;
    PlayerShow *playshow;
    QPoint Moveoffset;//鼠标移动偏移，实现点击移动
    bool MoveNote;//窗口移动标志
};

#endif // MINISHOW_H
