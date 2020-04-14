#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mqtt.h"
#include "numatable.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void     SetMqtt(Mqtt *p_mqtt);


public slots:
    void on_register_event(struct Mqtt_msg *);

private slots:
    void on_Reset_clicked();



    void on_Start_clicked();

private:
    Ui::MainWindow *ui;
    Mqtt *p_mqtt;
    NumaTable table;
    int phycal_addr_base = 100;
};

#endif // MAINWINDOW_H
