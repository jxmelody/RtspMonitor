#ifndef USBCAMTHREAD_H
#define USBCAMTHREAD_H

#include <QThread>
#include <QPainter>
#include <QMutex>
#include "opencv2/core.hpp"
#include "opencv2/core/persistence.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


class UsbCamThread : public QThread
{
    Q_OBJECT

public:
    UsbCamThread(QObject *parent = 0);
    ~UsbCamThread(){};
    void run();
    //暂停线程
    void pause();
    //继续线程
    void resume();
    void setDevice(int device);
signals:
    void getUsbCamImage(const QImage &image);
private:
    int m_device;
    QMutex m_playMutex;

};

#endif // USBCAMTHREAD_H
