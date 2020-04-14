#include "numatable.h"
#include <QDebug>

NumaTable::NumaTable()
{

}

void NumaTable::set_table_treeWidget(QTreeWidget *widget){
    treeWidget = widget;
}

int NumaTable::get_nocEntrys_count(int numa){
    return numaEntrys[numa].get_nocEntrys_count();
}

void NumaTable::get_start_msg(int numa, int node, Mqtt_msg *pmsg){
    NumaEntry *numaEntry = &numaEntrys[numa];
    numaEntry->get_start_msg( node, pmsg);
}

void NumaTable::table_clear(){
    int numa = 0;
    for(numa = 0; numa < numa_count; numa++){
        NumaEntry *numaEntry = &numaEntrys[numa];
          treeWidget->removeItemWidget(numaEntry->get_widgetItem(), 0);
          numaEntry->rem_all_noc_node();

    }
    numa_count = 0;
}

void NumaTable::table_add_numa(int numa){
    if(numa_count < 7){
       NumaEntry *numaEntry = &numaEntrys[numa];
       numaEntry->set_name(QString("numa%1").arg(numa));
       treeWidget->addTopLevelItem(numaEntry->get_widgetItem());

        numa_count++;
    }
}

void NumaTable::table_add_cpu(int numa, Mqtt_msg *msg){
    numaEntrys[numa].add_noc_cpu(msg);

}

void NumaTable::table_add_mem(int numa, Mqtt_msg *msg){
    numaEntrys[numa].add_noc_mem(msg);

}

/////

NumaEntry::NumaEntry() {};

void NumaEntry::set_name(QString name_arg){
    if(!registered){
        name = name_arg;
        widgetItem.setText(0, name_arg);
        registered = 1;
    }
}

QString NumaEntry::get_name(){
    return name;
}

QTreeWidgetItem *NumaEntry::get_widgetItem()
{
    return &widgetItem;
}

QTreeWidgetItem * NumaEntry::get_nocWidgetItem(int index){
    return nocEntrys[index].get_widgetItem();
}
void NumaEntry::set_type(int){

}
QString NumaEntry::get_type(){

}

int NumaEntry::get_nocEntrys_count(){
    return noc_node_count;
}
void NumaEntry::get_start_msg(int node, Mqtt_msg *pmsg){
    nocEntrys[node].get_start_msg(pmsg);

}

void NumaEntry::add_noc_cpu( Mqtt_msg *pmsg){
    if(noc_node_count < 25){

        NocNodeEntry *nocEntry = &nocEntrys[noc_node_count];
        nocEntry->setup(&widgetItem, numa_core_count, pmsg);
        noc_node_count++;
        numa_core_count++;
    }
}

void NumaEntry::add_noc_mem( Mqtt_msg *pmsg){
    if(noc_node_count < 25){

        NocNodeEntry *nocEntry = &nocEntrys[noc_node_count];
        nocEntry->setup(&widgetItem, numa_mem_count, pmsg);
        noc_node_count++;
        numa_mem_count++;
    }
}

void NumaEntry::rem_all_noc_node(){
    int index;
    for(index = 0; index < noc_node_count; index++){
        NocNodeEntry *nocEntry = &nocEntrys[index];
        nocEntry->clear(&widgetItem);

    }
    noc_node_count = 0;
    numa_core_count = 0;
    numa_mem_count = 0;
}


////

NocNodeEntry::NocNodeEntry() {};

void NocNodeEntry::setup(QTreeWidgetItem *pParent, int InstanceId_arg, Mqtt_msg *pmsg){
    qDebug() << "noc->setup   source " << pmsg->msg_source << " value " << pmsg->msg_data[0].value;
    instanceId = InstanceId_arg;
    type = pmsg->msg_node_type;
    randId = pmsg->msg_data[0].value;
    if(type == MSG_NODE_TYPE_CPU){
        name = QString("core%1").arg(instanceId );
        qDebug() << "noc->setup   source " << name.toStdString().c_str() << " randID " << randId;
    }
    else if(type == MSG_NODE_TYPE_MC){
        name = QString("mem%1").arg(instanceId );
        qDebug() << "noc->setup   source " << name.toStdString().c_str() << " randID " << randId;
        pa_base = pmsg->msg_data[2].value;
        pa_size = pmsg->msg_data[1].value;
        qDebug() << "   base " << pa_base << " size " << pa_size;
    }

    widgetItem.setText(0, name);
    pParent->addChild(&widgetItem);


}

void NocNodeEntry::clear(QTreeWidgetItem *pParent){
    pParent->removeChild(&widgetItem);
}

void NocNodeEntry::get_start_msg(Mqtt_msg *msg){
    msg->msg_type = MSG_TYPE_START;
    strcpy(msg->msg_source, name.toStdString().c_str() );
    msg->msg_source_len = name.size();
    msg->msg_node_type = MSG_NODE_TYPE_BIOS;
    msg->msg_data_len = 1;
    msg->msg_data[0].key = KVD_TYPE_RADOM;
    msg->msg_data[0].value = randId;
    if(type == MSG_NODE_TYPE_MC){
        msg->msg_data[1].key = KVD_TYPE_PADD_SIZE;
        msg->msg_data[1].value = pa_size;
        msg->msg_data[2].key = KVD_TYPE_PADD_START;
        msg->msg_data[2].value = pa_base;
        msg->msg_data_len = 3;
    }
    msg->msg_len = MSG_LEN_FULL;
    qDebug() << "node  get_start_msg " << name.toStdString().c_str() << " randID" << randId;
}

QTreeWidgetItem *NocNodeEntry::get_widgetItem(){
    return &widgetItem;
}


QString NocNodeEntry::get_name(){

}

QString NocNodeEntry::get_type(){

}
