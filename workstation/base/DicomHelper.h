#pragma once
#include "BasicDefine.h"
#undef UNICODE
#undef _UNICODE
#define NOMINMAX
#include "dcmtk/config/osconfig.h" 
#include "dcmtk/dcmdata/dctk.h" 
#include "dcmtk/dcmdata/dctag.h"
#include "dcmtk/dcmdata/dcistrmb.h"
#include "dcmtk/dcmdata/dcfilefo.h"

namespace Dicom
{
	std::string	GetTagString(DcmItem* data_item, const DcmTagKey& dcm_tag_key);
	std::string	GetTagArrayString(DcmItem* data_item, const DcmTagKey& dcm_tag_key);
}