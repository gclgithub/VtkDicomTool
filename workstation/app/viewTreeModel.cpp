#include "viewTreeModel.h"
#include <QtWidgets>
#include "workstation/base/DicomStudy.h"
#include "workstation/base/DicomSeries.h"

/*
 * 构造函数
 * 参数:QStringList &headers 表头
 *		const QString &data  树数据行
 */
viewTreeModel::viewTreeModel(const QStringList &headers, const QString &data, QObject *parent)
	: QAbstractItemModel(parent)
{
	/*
	 * 获取根节点的数据并构造根节点
	 */
	QVector<QVariant> rootData;
	foreach(QString header, headers)
		rootData << header;

	rootItem = new ViewTreeItem(rootData);
	/*
	 * 以此为根节点向下构造树
	 */
	setupModelData(data.split(QString("\n")), rootItem);
}
/*
 * 构造函数2
 * 参数:
 * headers  表头
 * patient  这棵树要存储的病人实例
 */
viewTreeModel::viewTreeModel(const QStringList& headers, const std::map<std::string, PTR<DicomStudy>>& patient_list, QObject* parent)
{
	QVector<QVariant> rootData;
	foreach(QString header, headers)
		rootData << header;

	rootItem = new ViewTreeItem(rootData);
	/*
	* 以此为根节点向下构造树
	*/
	setupModelData(patient_list, rootItem);
}

/*
 * 析构函数
 */
viewTreeModel::~viewTreeModel()
{
	delete rootItem;
}
/*
 *  返回列数
 */
int viewTreeModel::columnCount(const QModelIndex & /* parent */) const
{
	return rootItem->columnCount();
}
/*
 * 用 QModelIndex和role索引指定Item中的数据
 */
QVariant viewTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	ViewTreeItem *item = getItem(index);

	return item->data(index.column());
}
/*
 *
 */
Qt::ItemFlags viewTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
/*
 * 用QModelIndex索引Item
 */
ViewTreeItem *viewTreeModel::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		ViewTreeItem *item = static_cast<ViewTreeItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}
/*
 * 返回指定Index的Item在树中的层次
 */
int viewTreeModel::getLevel(const QModelIndex & index) const
{
	ViewTreeItem * p_dicom_tree_item = this->getItem(index);
	int level = 0;
	while (p_dicom_tree_item->parent() != NULL)
	{//迭代当前节点的父节点来判断该节点是树的第几层
		level++;
		p_dicom_tree_item = p_dicom_tree_item->parent();
	}
	return level;
}

/*
 *
 */
QVariant viewTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}
/*
 *
 */
QModelIndex viewTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();
	//! [5]

	//! [6]
	ViewTreeItem *parentItem = getItem(parent);

	ViewTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}
/*
 * 插入列
 */
bool viewTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = rootItem->insertColumns(position, columns);
	endInsertColumns();

	return success;
}
/*
 * 插入行
 */
bool viewTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	ViewTreeItem *parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

/*
 * 返回某QModelIndex所对应Item的父节点的QModelIndex
 */
QModelIndex viewTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	ViewTreeItem *childItem = getItem(index);
	ViewTreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}
/*
 * 删除列
 */
bool viewTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if (rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}
/*
 * 删除行
 */
bool viewTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	ViewTreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}
/*
 * 行数
 */
int viewTreeModel::rowCount(const QModelIndex &parent) const
{
	ViewTreeItem *parentItem = getItem(parent);

	return parentItem->childCount();
}
/*
 * 给QModelIndex对应的Item设置DATA 
 */
bool viewTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::EditRole)
		return false;

	ViewTreeItem *item = getItem(index);
	bool result = item->setData(index.column(), value);

	if (result)
		emit dataChanged(index, index);

	return result;
}
/*
 * 改表头
 */
bool viewTreeModel::setHeaderData(int section, Qt::Orientation orientation,const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(section, value);

	if (result)
		emit headerDataChanged(orientation, section, section);

	return result;
}
/*
 * 使用缺省数据文件构造树
 */
void viewTreeModel::setupModelData(const QStringList &lines, ViewTreeItem *parent)
{
	QList<ViewTreeItem*> parents;
	QList<int> indentations;
	parents << parent;
	indentations << 0;

	int number = 0;

	while (number < lines.count()) {
		int position = 0;
		while (position < lines[number].length()) {
			if (lines[number].at(position) != ' ')
				break;
			++position;
		}

		QString lineData = lines[number].mid(position).trimmed();

		if (!lineData.isEmpty()) {
			// Read the column data from the rest of the line.
			QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
			QVector<QVariant> columnData;
			for (int column = 0; column < columnStrings.count(); ++column)
				columnData << columnStrings[column];

			if (position > indentations.last()) {
				// The last child of the current parent is now the new parent
				// unless the current parent has no children.

				if (parents.last()->childCount() > 0) {
					parents << parents.last()->child(parents.last()->childCount() - 1);
					indentations << position;
				}
			}
			else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}

			// Append a new item to the current parent's list of children.
			ViewTreeItem *parent = parents.last();
			parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
			for (int column = 0; column < columnData.size(); ++column)
				parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
		}

		++number;
	}
}
/*
 * 使用patient对象构造树
 */
void viewTreeModel::setupModelData(const std::map<std::string, PTR<DicomStudy>>& patient_list, ViewTreeItem * parent)
{
	int index = 0;
	for (auto tmp: patient_list)
	{
		auto study = tmp.second;
		auto ss = study->get_series();
		auto series0 = ss[0];
		
		QVector<QVariant> PatientData;
		PatientData << QString::fromStdString(series0->get_patient_pid());
		PatientData << QString::fromStdString(series0->get_patient_name());
		parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
		for (int column = 0; column < PatientData.size(); ++column)
			parent->child(parent->childCount() - 1)->setData(column, PatientData[column]);

		for (auto series : ss)
		{
			QVector<QVariant> StudyData;
			StudyData << QString::fromStdString(series->get_series_description());
			StudyData << QString::fromStdString(series->get_modality());
			auto ii = parent->child(index)->childCount();
			parent->child(index)->insertChildren(parent->child(index)->childCount(), 1, rootItem->columnCount());
			for (int column = 0; column < StudyData.size(); ++column)
			{
				parent->child(index)->child(parent->child(index)->childCount() - 1)->setData(column, StudyData[column]);
				parent->child(index)->child(parent->child(index)->childCount() - 1)->set_series_id(series->get_series_guid());
			}
			/*for (int j = 0; j < patient.StudyList[i]->SeriesList.size(); j++)
			{
				QVector<QVariant> SeriesData;
				SeriesData << QString::fromStdString(patient.StudyList[i]->SeriesList[j]->SeriseNumber);
				SeriesData << QString::fromStdString(patient.StudyList[i]->SeriesList[j]->Manufacturer);
				parent->child(0)->child(i)->insertChildren(parent->child(0)->child(i)->childCount(), 1, rootItem->columnCount());
				for (int column = 0; column < SeriesData.size(); ++column)
					parent->child(0)->child(i)->child(parent->child(0)->child(i)->childCount() - 1)->setData(column, SeriesData[column]);
				for (int k = 0; k < patient.StudyList[i]->SeriesList[j]->ImageList.size(); k++)
				{
					QVector<QVariant> ImageData;
					QString temp = "Img"; temp.append(QString::fromStdString(std::to_string(k)));
					ImageData << temp;
					ImageData << QString::fromStdString(patient.StudyList[i]->SeriesList[j]->ImageList[k]->ReferencedFileID);
					parent->child(0)->child(i)->child(j)->insertChildren(parent->child(0)->child(i)->child(j)->childCount(), 1, rootItem->columnCount());
					for (int column = 0; column < ImageData.size(); ++column)
						parent->child(0)->child(i)->child(j)->child(parent->child(0)->child(i)->child(j)->childCount() - 1)->setData(column, ImageData[column]);
				}
			}*/
		}
		index++;
	}




	
}
