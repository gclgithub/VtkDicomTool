#pragma once
/*
 * ViewTreeItem.cpp
 * DicomDir树的自定义Item
 * 与DicomDirTreeModel组成MV模式
 * 预计缩略图功能将使用这个方案实现
 */
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QStandardItem>
#include "workstation/base/BasicDefine.h"

class ViewTreeItem:public QStandardItem
{
public:
	explicit ViewTreeItem(const QVector<QVariant> &data, ViewTreeItem *parent = 0);
	~ViewTreeItem();

	ViewTreeItem *child(int number);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	bool insertChildren(int position, int count, int columns);
	bool insertColumns(int position, int columns);
	ViewTreeItem *parent();
	bool removeChildren(int position, int count);
	bool removeColumns(int position, int columns);
	int childNumber() const;
	bool setData(int column, const QVariant &value);

	SetGetMacro(std::string, series_id)
private:
	QList<ViewTreeItem*> childItems;
	ViewTreeItem *parentItem;
public:
	QVector<QVariant> itemData;
};


#endif // TREEITEM_H
