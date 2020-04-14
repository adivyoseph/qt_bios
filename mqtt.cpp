#include "mqtt.h"
#include <QDebug>



Mqtt::Mqtt()
{
    host = "localhost";
    mosquitto_lib_init();
    mosq = mosquitto_new(nullptr, true, this);
    if(!mosq){
        qDebug() <<  "Error: mosquitto_new failed";

    }
}

Mqtt::~Mqtt(){
    int rc;
    if(connect_state){
         mosquitto_loop_stop(mosq, true);
         //
         rc = mosquitto_unsubscribe_v5(mosq, nullptr,  "numa0/#",  nullptr);
         rc =mosquitto_disconnect(mosq);
         qDebug() <<  "mosquitto_disconnect rc " << rc;
         //state = 0;
    }


}
void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg, const mosquitto_property *properties){
    Mqtt * pthis = (Mqtt * ) obj;
    size_t len = (size_t) msg->payloadlen;
    struct Mqtt_msg *myMsg = new Mqtt_msg;
    memcpy((void *) myMsg, msg->payload, len);

    qDebug() << "message_callback " << msg->topic << "len " << msg->payloadlen << " msgtype " << myMsg->msg_type;
    //only get here when connected
    switch(myMsg->msg_type){
    case MSG_TYPE_REGISTER:
        qDebug() << "   signal_register";
        qDebug() << "   source " << myMsg->msg_source << " randID " << myMsg->msg_data[0].value;
            emit pthis->mqtt_nodeRegister_event(myMsg);
        break;


    }

}


void Mqtt::mqtt_init(void *mwindow){
    pMainWindow = mwindow;
    int rc;
    int mid = 5;
    qDebug() <<  "mqtt_init";

    //mosquitto_connect_v5_callback_set(mosq, my_connect_callback);
    //mosquitto_disconnect_v5_callback_set(mosq, my_disconnect_callback);
    mosquitto_message_v5_callback_set(mosq, my_message_callback);
    rc = mosquitto_connect_bind_v5(mosq, (const char *) host.c_str(), port, host.size(), (const char *) host.c_str(), nullptr);
    if(rc>0){
        qDebug() <<  "Error: mosquitto_connect_bind_v5 " << rc;
    }
    else{
        qDebug() <<  "mosquitto_connect_bind_v5";
        rc = mosquitto_subscribe_v5(mosq, nullptr,  "numa0/#", 0, MQTT_SUB_OPT_NO_LOCAL, nullptr);
        //rc = mosquitto_subscribe_v5(mosq, nullptr,  topicLevel.toStdString().c_str(), 0, 0, nullptr);

        qDebug() <<  "subscribe_v5" << rc;
        mosquitto_loop_start(mosq);
        connect_state =1;

    }
}

void Mqtt::mqtt_send_msg(QString topic, Mqtt_msg *msg){
    int mid = 5;
    int rc =1;
    if(msg){
        qDebug() <<  "mqtt_send_start  " << msg->msg_source;
        if(connect_state)
        {
            rc = mosquitto_publish_v5(mosq, &mid, topic.toStdString().c_str(), msg->msg_len, msg, 0, 0, nullptr);
            qDebug() <<  "mqtt_pub rc " << rc;
        }
    }
    else {
        qDebug() <<  "mqtt_send_msg NULL msg " << topic.toStdString().c_str() ;
    }
}


void Mqtt::mqtt_send_reset(){
    QString topic = "numa0/bios/reset";
    struct Mqtt_msg msg;
    msg.msg_type = MSG_TYPE_RESET;
    strcpy(msg.msg_source, "numa0" );
    msg.msg_source_len = 6;
    msg.msg_len = MSG_LEN_SHORT;

    int mid = 5;
    int rc =1;
    qDebug() <<  "mqtt_send_reset  ";
    if(connect_state)
    {
        rc = mosquitto_publish_v5(mosq, &mid, topic.toStdString().c_str(), msg.msg_len, &msg, 0, 0, nullptr);
        qDebug() <<  "mqtt_pub rc " << rc;
    }


}


