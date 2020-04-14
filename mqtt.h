#ifndef MQTT_H
#define MQTT_H

#include <mqtt_protocol.h>
#include <mosquitto.h>
#include <QObject>

typedef struct msg_kvd {
    int key;
    int value;

} t_msg_kvd;



#define MSG_TYPE_RESET          1
#define MSG_TYPE_REGISTER       2
#define MSG_TYPE_START          3

#define MSG_LEN_SHORT      (sizeof(int)*3 + 16)
#define MSG_LEN_DATAONLY   (sizeof(int)*13 + 16)
#define MSG_LEN_FULL       (sizeof(Mqtt_msg))

#define MSG_NODE_TYPE_BIOS  1
#define MSG_NODE_TYPE_CPU   2
#define MSG_NODE_TYPE_MC    3

#define KVD_TYPE_RADOM       1
#define KVD_TYPE_PADD_START  2
#define KVD_TYPE_PADD_SIZE   3

struct Mqtt_msg {
    int msg_type;
    int msg_len;
    int msg_source_len;
    char msg_source[16];
    int msg_node_type;
    int msg_data_len;
    t_msg_kvd msg_data[4];
    int cache_line[8];      //8  x 64bits
};


class Mqtt : public QObject
{
    Q_OBJECT

signals:
   void mqtt_nodeRegister_event(struct Mqtt_msg *);

public:
    Mqtt();
    ~Mqtt();

//mainwindow commands
    void mqtt_init(void *);
    void mqtt_send_reset();
    void mqtt_send_msg(QString topic, Mqtt_msg *msg);

private:
    void * pMainWindow;
    struct mosquitto *mosq;
    int connect_state = 0;
    int port  = 1883;
    std::string host;

};

#endif // MQTT_H
