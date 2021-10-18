set(dir "${PROJECT_SOURCE_DIR}/workstation/scene")

set(scene_source
${dir}/MultiImageView2.cpp
${dir}/ViewRenderScene.cpp
${dir}/MousePicker.cpp
${dir}/MPRStyle.cpp
${dir}/SceneReslice.cpp
${dir}/SceneCrossLine.cpp
${dir}/SceneMeasureTools.cpp

)

set(scene_header
${dir}/BasicVtkDefine.h
${dir}/ViewType.h
${dir}/MultiImageView2.h
${dir}/ViewRenderScene.h
${dir}/MousePicker.h
${dir}/MPRStyle.h
${dir}/SceneReslice.h
${dir}/SceneCrossLine.h
${dir}/SceneMeasureTools.h
)

source_group(scene FILES ${scene_source} ${scene_header})
set(src ${src} ${scene_source} ${scene_header})