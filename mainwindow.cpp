#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table.set_table_treeWidget(ui->treeWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::SetMqtt(Mqtt *p_mqtt_arg){
    p_mqtt = p_mqtt_arg;
}

void MainWindow::on_Reset_clicked()
{
    phycal_addr_base = 100;
    table.table_clear();
    p_mqtt->mqtt_send_reset();
    table.table_add_numa(0);

}


void MainWindow::on_register_event(struct Mqtt_msg *msg){
    struct Mqtt_msg myMsg = *msg;
    qDebug() << "on_register_event";
    qDebug() << "   source " << myMsg.msg_source << " randID " << myMsg.msg_data[0].value;
    if(myMsg.msg_node_type == MSG_NODE_TYPE_CPU){
        table.table_add_cpu(0, &myMsg);
    }
    else if(myMsg.msg_node_type == MSG_NODE_TYPE_MC){
        qDebug() << " mc size " << myMsg.msg_data[1].value;

        //add physical address range to setup
        if(myMsg.msg_data_len != 2 ||
                myMsg.msg_data[1].key != KVD_TYPE_PADD_SIZE     ){
                qDebug() << " msg format error";
        }
        myMsg.msg_data[2].key = KVD_TYPE_PADD_START;
        myMsg.msg_data[2].value = phycal_addr_base;
        phycal_addr_base += myMsg.msg_data[1].value; //size
        myMsg.msg_data_len = 3;
        qDebug() << " mc start " << myMsg.msg_data[2].value;
        table.table_add_mem(0, &myMsg);
    }
}

void MainWindow::on_Start_clicked()
{
    QString topic = "numa0/bios/start";
    struct Mqtt_msg msg;
    int child_count;


    child_count = table.get_nocEntrys_count(0);
    for(int node = 0; node < child_count; node++){
        table.get_start_msg(0, node, &msg);
        qDebug() << "on_Start_clicked node " << node ;
        qDebug() << "   source " << msg.msg_source << " randID " << msg.msg_data[0].value;
        p_mqtt->mqtt_send_msg(topic, &msg);
    }

}
