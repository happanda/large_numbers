# C++ source files
set(SOURCE_FILES_LN
${SourcePath}/LongNumberExceptions.h
${SourcePath}/LongNumber.h
${SourcePath}/LongNumber.cpp
${SourcePath}/NumberContainer.h
${SourcePath}/NumberContainer.cpp
)

set(SOURCE_FILES_TEST
${TestPath}/auxiliary.h
${TestPath}/dynamic.h
${TestPath}/input.h
${TestPath}/static.h
${TestPath}/main.cpp
)

# set appropriate source groups
source_group(ln  FILES  ${SOURCE_FILES_LN})
source_group(test  FILES  ${SOURCE_FILES_TEST})

# set all source files
set(SOURCE_FILES
${SOURCE_FILES_LN}
${SOURCE_FILES_TEST}
)