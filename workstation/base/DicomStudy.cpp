#include "DicomStudy.h"

PTR<DicomStudy> DicomStudy::New()
{
	return PTR<DicomStudy>(new DicomStudy);
}

DicomStudy::DicomStudy()
{
}

DicomStudy::~DicomStudy()
{
}
