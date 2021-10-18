set(dir "${PROJECT_SOURCE_DIR}/workstation/io")

set(io_source
${dir}/DataPool.cpp

)

set(io_header
${dir}/DataPool.h
)

source_group(io FILES ${io_source} ${io_header})
set(src ${src} ${io_source} ${io_header})