#include "viewTreeItem.h"
#include <QStringList>
/*
 * DicomDir树的自定义Item
 * 与DicomDirTreeModel组成MV模式
 * 预计缩略图功能将使用这个方案实现
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
 * 返回第number个子节点
 */
ViewTreeItem *ViewTreeItem::child(int number)
{
	return childItems.value(number);
}
/*
 * 返回子节点的数量
 */
int ViewTreeItem::childCount() const
{
	return childItems.count();
}
/*
 * 当前节点在兄弟节点中的排名
 */
int ViewTreeItem::childNumber() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<ViewTreeItem*>(this));

	return 0;
}
/*
 * 返回当前节点有几列数据
 */
int ViewTreeItem::columnCount() const
{
	return itemData.count();
}
/*
 *返回当前节点数据行的第column列数据
 */
QVariant ViewTreeItem::data(int column) const
{
	return itemData.value(column);
}
/*
 * 在当前节点的子节点列表的第position位置插入count个拥有columns个数据列的子节点
 * 注意刚插进去的子节点是空的
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
 * 在当前节点的数据行的position位置插入columns个空列
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
 * 返回当前节点的父节点
 */
ViewTreeItem *ViewTreeItem::parent()
{
	return parentItem;
}
/*
 * 删除当前节点的子节点列表的从position位置开始的count个子节点
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
 * 删除当前节点数据行从position开始的columns列
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
 * 给当前节点数据行的column列处写入数据value
 */
bool ViewTreeItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= itemData.size())
		return false;

	itemData[column] = value;
	return true;
}
