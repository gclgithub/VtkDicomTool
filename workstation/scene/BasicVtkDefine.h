#pragma once
#include "workstation/base/BasicDefine.h"
#include "vtkObject.h"
#include "vtkSmartPointer.h"
#include "vtkWeakPointer.h"
#include "vtkNew.h"

#include <vtkRenderWindow.h>
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkOpenGLRenderer.h"
#include "vtkImageData.h"
#include "vtkImageReslice.h"
#include "vtkImageActor.h"
#include "vtkImageMapToColors.h"

#include "vtkCamera.h"
#include "vtkMatrix4x4.h"
#include "vtkProp3D.h"
#include "vtkLookupTable.h"
#include "vtkActor2D.h"

template<typename Type>
using SPTR = vtkSmartPointer<Type>;

template<typename Type>
using WPTR = vtkWeakPointer<Type>;
