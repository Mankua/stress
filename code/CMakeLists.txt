CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( Stress )

# 3 d s   m a x
 
# 3ds Max 2010 | Visual Studio 9.0 | 32 Bits | 64 Bits 
# 3ds Max 2011 | Visual Studio 9.0 | 32 Bits | 64 Bits 
# 3ds Max 2012 | Visual Studio 9.0 | 32 Bits | 64 Bits 
# 3ds Max 2013 | 
                                                      
# Define the Project Name.
#  Projects that are built with the same IDE (ie. Visual Studio 2005) will be
#  added to the same Solution. For this reason all project need to have 
#  unique names.

SET ( PROJECT_NAME_MAX_2010 "StressMax2010" )
SET ( PROJECT_NAME_MAX_2011 "StressMax2011" )
SET ( PROJECT_NAME_MAX_2012 "StressMax2012" )
SET ( PROJECT_NAME_MAX_2013 "StressMax2013" )

# Define the header libraries for each SDK

SET ( MAXSDK_2010_HEADERS_DIR "c:/Libraries/maxsdk/12/include" )
SET ( MAXSDK_2011_HEADERS_DIR "c:/Libraries/maxsdk/13/include" )
SET ( MAXSDK_2012_HEADERS_DIR "c:/Libraries/maxsdk/14/include" )
SET ( MAXSDK_2013_HEADERS_DIR "c:/Libraries/maxsdk/15/include" )

# Define the library files for each SDK, and platform. 64 bits lib path will replace
# 32 bits lib path.

# 32 BITS 

SET ( MAXSDK_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/lib" )
SET ( MAXSDK_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/lib" )
SET ( MAXSDK_2012_LIBRARY_DIR "c:/Libraries/maxsdk/14/lib" )
SET ( MAXSDK_2013_LIBRARY_DIR "c:/Libraries/maxsdk/15/lib" )

SET ( PROGRAM_FILES "c:\\Program Files (x86)" )

IF ( CMAKE_CL_64 ) # 64 BITS

	SET ( MAXSDK_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/x64/lib" )
	SET ( MAXSDK_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/x64/lib" )
	SET ( MAXSDK_2012_LIBRARY_DIR "c:/Libraries/maxsdk/14/x64/lib" )
	SET ( MAXSDK_2013_LIBRARY_DIR "c:/Libraries/maxsdk/15/x64/lib" )
	
	SET ( PROGRAM_FILES "c:\\Program Files" )
	
ENDIF ()

# Add the source files.

SET ( SOURCE_FILES
	"../Plugin.cpp"
	"../Texmap.cpp"
	"../TexShade.cpp"
)

# Add the header files.

SET ( HEADER_FILES
	"../resource.h"
	"../Texmap.h"
)

# Add the resource and definition file.

SET ( RESOURCE_FILES
	"../MankuaLogo.bmp"
	"../stresslogo.bmp"
	"../Texmap.rc"
	"../Plugin.def"
)

# Define the max libraries.

SET( MAX_LIBS  "mnmath.lib"
			   "poly.lib"
			   "paramblk2.lib"
			   "core.lib"
			   "geom.lib"
			   "mesh.lib"
			   "maxutil.lib"
			   "bmm.lib"
			   "maxscrpt.lib"
			   "comctl32.lib"
)

IF ( MSVC90 )

	#ADD_SUBDIRECTORY( Max2010 )
	#ADD_SUBDIRECTORY( Max2011 )
	ADD_SUBDIRECTORY( Max2012 )
	#ADD_SUBDIRECTORY( Max2013 )
	
ENDIF ( MSVC90 )
