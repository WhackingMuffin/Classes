#include "DataModel.h"

USING_NS_CC;
DataModel* DataModel::m_pInstance;
DataModel* DataModel::getModel()
{
	// If there isn't an instance of the data model make one
	if (m_pInstance == NULL)
	{
		m_pInstance = new DataModel();
	}
	return m_pInstance;
}


