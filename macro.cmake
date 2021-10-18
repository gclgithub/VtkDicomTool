# Copy files from source directory to destination directory, substituting any
# variables.  Create destination directory if it does not exist.

macro(configure_files srcDir destDir)
   # message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
#            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY IMMEDIATE)
		else(NOT IS_DIRECTORY ${srcTemplatePath})
			make_directory(${destDir}/${templateFile})
			configure_files(${srcTemplatePath} ${destDir}/${templateFile})
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)


macro(configure_files_selected srcDir files destDir)
  # message(STATUS "Configuring directory ${destDir}")
  make_directory(${destDir})
  file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/${files})
  foreach(templateFile ${templateFiles})
    set(srcTemplatePath ${srcDir}/${templateFile})
    if(NOT IS_DIRECTORY ${srcTemplatePath})
      #            message(STATUS "Configuring file ${templateFile}")
      configure_file(
        ${srcTemplatePath}
        ${destDir}/${templateFile}
        COPYONLY IMMEDIATE)
    endif(NOT IS_DIRECTORY ${srcTemplatePath})
  endforeach(templateFile)
endmacro(configure_files_selected)