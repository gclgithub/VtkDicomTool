#include "DicomSeries.h"

PTR<DicomSeries> DicomSeries::New()
{
	return PTR<DicomSeries>(new DicomSeries);
}

DicomSeries::DicomSeries()
{
}

DicomSeries::~DicomSeries()
{
}
