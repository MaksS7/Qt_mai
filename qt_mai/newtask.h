#ifndef NEWTASK_H
#define NEWTASK_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class NewTask;
}

class NewTask : public QWidget
{
    Q_OBJECT

public:
    explicit NewTask(QWidget *parent = nullptr);
    ~NewTask();
    int size = 0;
    int sizeVariant = 0;
    QStringList strList;
    QStringList strListVariant;
    QTableWidget *tableMain;
    QString nameTask;
    int loop = 0;

private slots:
    void on_btnNextStep_clicked();
    void on_btnSaveTable_clicked();
    void on_btnNextStepVarint_clicked();
    void on_btnCreatTask_clicked();
    void createTable();

signals:
    void getTable();
    void saveDone();

private:
    Ui::NewTask *ui;
};

#endif // NEWTASK_H
