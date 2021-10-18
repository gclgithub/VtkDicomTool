#pragma once
#include <QStandardItem>
#include "ViewTreeItem.h"
#include "workstation/base/BasicDefine.h"
/*
 * QAbstractItemModel������
 * �ܹ���default�ļ������ݿ��α��ʼ��
 * "�α�"ָ���ǵ�ǰ������patient����
 * Ϊ�˷����Զ����������͵�һЩ�������,ʵ����һЩ����
 * ����:QAbstractItemModel�ǳ�����,����ʵ�����еĳ��󷽷�
 */
class DicomStudy;
class viewTreeModel:public QAbstractItemModel
{
	Q_OBJECT

public:
	viewTreeModel(const QStringList &headers, const QString &data,QObject *parent = 0);
	viewTreeModel(const QStringList &headers, const std::map<std::string, PTR<DicomStudy>>& patient_list, QObject *parent = 0);
	~viewTreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole) override;
	bool setHeaderData(int section, Qt::Orientation orientation,const QVariant &value, int role = Qt::EditRole) override;
	bool insertColumns(int position, int columns,const QModelIndex &parent = QModelIndex()) override;
	bool removeColumns(int position, int columns,const QModelIndex &parent = QModelIndex()) override;
	bool insertRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
	int getLevel(const QModelIndex &index)const;
	ViewTreeItem*getItem(const QModelIndex &index) const;
private:
	void setupModelData(const QStringList &lines, ViewTreeItem*parent);
	void setupModelData(const std::map<std::string, PTR<DicomStudy>>& patient_list, ViewTreeItem*parent);
	
	ViewTreeItem*rootItem;
};

