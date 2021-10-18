set(dir "${PROJECT_SOURCE_DIR}/workstation/app")

set(app_source
${dir}/main.cxx
${dir}/mainWindow.cpp
${dir}/mainWindow.ui
${dir}/viewTreeItem.cpp
${dir}/viewTreeModel.cpp

)

set(app_header
${dir}/mainWindow.h
${dir}/viewTreeItem.h
${dir}/viewTreeModel.h
)

source_group(app FILES ${app_source} ${app_header})
set(src ${src} ${app_source} ${app_header})