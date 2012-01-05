#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <enet/enet.h>
#include <QObject>
#include <QString>
#include <QWidget>

class NetworkManager : public QObject
{
    Q_OBJECT

private:
    ENetHost *client;
    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    QString ip_addr;
    int port;
    int next_time;
    int keep_running;

public:
    NetworkManager();
    ~NetworkManager();
    int network_init(QString ip_addr = "127.0.0.1", int port=1234);
    void set_key(int key);
    void network_loop();
    void process_packet(ENetEvent * event);
    void set_rand_key(int key);
    void update_state();
    void testFunction();
    void process_key(QKeyEvent * event, int key_status);

public slots:
    void start();

signals:
    void writeText(QString text);
};

#endif // NETWORKMANAGER_H
