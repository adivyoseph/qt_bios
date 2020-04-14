#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Mqtt myMqtt;
    w.SetMqtt(&myMqtt);
    myMqtt.mqtt_init(&w);
    QObject::connect(&myMqtt, SIGNAL(mqtt_nodeRegister_event(struct Mqtt_msg *)), &w, SLOT(on_register_event(struct Mqtt_msg *)));
    w.show();

    return a.exec();
}
