#ifndef NUMATABLE_H
#define NUMATABLE_H
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "mqtt.h"


class NocNodeEntry {

public:
    NocNodeEntry();

    void setup(QTreeWidgetItem *pParent, int id, Mqtt_msg *pmsg);
    void clear(QTreeWidgetItem *pParent);
    void get_start_msg(Mqtt_msg *);
    QString get_name();
    QString get_type();
    QTreeWidgetItem *get_widgetItem();

public:
    QTreeWidgetItem widgetItem;
    QString name;
    int type;
    int instanceId;
    int randId;
    //mc
    int pa_base;
    int pa_size;
    //core

};

class NumaEntry {

public:
    NumaEntry();

    void set_name(QString );
    QString get_name();
    void set_type(int);
    QString get_type();

    int get_nocEntrys_count();
    void get_start_msg( int node,  Mqtt_msg *);

    QTreeWidgetItem * get_widgetItem();
    void add_noc_cpu(Mqtt_msg *pmsg);
    void add_noc_mem(Mqtt_msg *pmsg);

    void add_noc_node(int id, Mqtt_msg *pmsg);
    void rem_all_noc_node();
    QTreeWidgetItem * get_nocWidgetItem(int);






private:
    int registered = 0;
    QString name;
    int type;
#define NUMA_TYPE_SOCKET      1
#define NUMA_TYPE_CXL_TYPE1   2
#define NUMA_TYPE_CXL_TYPE2   3
#define NUMA_TYPE_CXL_TYPE3   4
    QTreeWidgetItem widgetItem;
    int noc_node_count = 0;
    int numa_core_count = 0;
    int numa_mem_count = 0;
    NocNodeEntry nocEntrys[16];

};

class NumaTable
{
public:
    NumaTable();
    void set_table_treeWidget(QTreeWidget *widget);

    void table_clear();
    int get_nocEntrys_count(int numa);
    void get_start_msg(int numa,  int node, Mqtt_msg *);


    void table_add_numa(int );
    void table_add_cpu(int numa, Mqtt_msg *);
    void table_add_mem(int numa, Mqtt_msg *);


private:
    int numa_count = 0;

    int mc_count   = 0;
    int cxl_count  = 0;
    NumaEntry numaEntrys[8];
    QTreeWidget *treeWidget;

};

#endif // NUMATABLE_H
