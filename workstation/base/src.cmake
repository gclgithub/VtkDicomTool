set(dir "${PROJECT_SOURCE_DIR}/workstation/base")

set(base_source
${dir}/json.hpp
${dir}/DicomStudy.cpp
${dir}/DicomSeries.cpp
${dir}/DicomHelper.cpp
${dir}/DicomReadToVtk.cpp
${dir}/FileHelper.cpp
)

set(base_header
${dir}/BasicDefine.h
${dir}/DicomStudy.h
${dir}/DicomSeries.h
${dir}/DicomHelper.h
${dir}/DicomReadToVtk.h
${dir}/FileHelper.h
)

source_group(base FILES ${base_source} ${base_header})
set(src ${src} ${base_source} ${base_header})