#include "viewTreeItem.h"
#include <QStringList>
/*
 * DicomDir�����Զ���Item
 * ��DicomDirTreeModel���MVģʽ
 * Ԥ������ͼ���ܽ�ʹ���������ʵ��
 */

ViewTreeItem::ViewTreeItem(const QVector<QVariant> &data, ViewTreeItem *parent)
{
	parentItem = parent;
	itemData = data;
}

ViewTreeItem::~ViewTreeItem()
{
	qDeleteAll(childItems);
}
/*
 * ���ص�number���ӽڵ�
 */
ViewTreeItem *ViewTreeItem::child(int number)
{
	return childItems.value(number);
}
/*
 * �����ӽڵ������
 */
int ViewTreeItem::childCount() const
{
	return childItems.count();
}
/*
 * ��ǰ�ڵ����ֵܽڵ��е�����
 */
int ViewTreeItem::childNumber() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<ViewTreeItem*>(this));

	return 0;
}
/*
 * ���ص�ǰ�ڵ��м�������
 */
int ViewTreeItem::columnCount() const
{
	return itemData.count();
}
/*
 *���ص�ǰ�ڵ������еĵ�column������
 */
QVariant ViewTreeItem::data(int column) const
{
	return itemData.value(column);
}
/*
 * �ڵ�ǰ�ڵ���ӽڵ��б�ĵ�positionλ�ò���count��ӵ��columns�������е��ӽڵ�
 * ע��ղ��ȥ���ӽڵ��ǿյ�
 */
bool ViewTreeItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		QVector<QVariant> data(columns);
		ViewTreeItem *item = new ViewTreeItem(data, this);
		childItems.insert(position, item);
	}

	return true;
}
/*
 * �ڵ�ǰ�ڵ�������е�positionλ�ò���columns������
 */
bool ViewTreeItem::insertColumns(int position, int columns)
{
	if (position < 0 || position > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.insert(position, QVariant());

	foreach(ViewTreeItem *child, childItems)
		child->insertColumns(position, columns);

	return true;
}
/*
 * ���ص�ǰ�ڵ�ĸ��ڵ�
 */
ViewTreeItem *ViewTreeItem::parent()
{
	return parentItem;
}
/*
 * ɾ����ǰ�ڵ���ӽڵ��б�Ĵ�positionλ�ÿ�ʼ��count���ӽڵ�
 */
bool ViewTreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete childItems.takeAt(position);

	return true;
}
/*
 * ɾ����ǰ�ڵ������д�position��ʼ��columns��
 */
bool ViewTreeItem::removeColumns(int position, int columns)
{
	if (position < 0 || position + columns > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.remove(position);

	foreach(ViewTreeItem *child, childItems)
		child->removeColumns(position, columns);

	return true;
}
/*
 * ����ǰ�ڵ������е�column�д�д������value
 */
bool ViewTreeItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= itemData.size())
		return false;

	itemData[column] = value;
	return true;
}
