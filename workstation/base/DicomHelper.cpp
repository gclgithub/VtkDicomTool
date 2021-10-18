#include "DicomHelper.h"

namespace Dicom
{
	std::string GetTagString(DcmItem* data_item, const DcmTagKey& dcm_tag_key)
	{
		OFString ptr;
		if (data_item->findAndGetOFString(dcm_tag_key, ptr).good()) {
			return ptr.c_str();
		}
		return "";
	}
	std::string GetTagArrayString(DcmItem* data_item, const DcmTagKey& dcm_tag_key)
	{
		const char* ptr = nullptr;
		Uint32 length = 0;
		data_item->findAndGetString(dcm_tag_key, ptr, length);
		if (!ptr) return "";
		return ptr;
	}
}