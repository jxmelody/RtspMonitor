#ifndef MONITORWINDOWMANAGER_H
#define MONITORWINDOWMANAGER_H

#include <QObject>
#include <QGridLayout>
#include <QList>

class MonitorWindow;

#define MAX_ROW 5
#define MAX_COL 5

//���Ӵ��ڹ����������ü��Ӵ��ڵ����в���
class MonitorWindowManager : public QGridLayout
{
	Q_OBJECT

public:
	//��ʼ�����Ӵ��ڲ��֣�������������
	MonitorWindowManager(int row, int column, QWidget *parent);
	~MonitorWindowManager();
	//���ؼ��Ӵ�������
	int numOfWindows() const;
	//���ô������в���
	void setMonitorLayout(int row, int column);
	//����λ�õõ����Ӵ��ڵ�ʵ��ָ��
	MonitorWindow* getItemAtPosition(int row, int column);
    int getAFreeWindow(int& row, int&column );//get a free window to play video
    int setNextWindow(const int& row, const int& column);//set next play window's position
private:
	void remove(int row, int column, bool deleteWidgets);
	void deleteChildWidgets(QLayoutItem *item);

private:
	QWidget* m_parent;
	int m_numOfWindows;
	int m_row;
    int m_column;
    int m_current_row;
    int m_current_col;
    //the "int" data type has 32 bits. We use each bit stands one monitor window:
    //1 means occupied while 0 means available
    int m_used_grid;
};

#endif // MONITORWINDOWMANAGER_H
