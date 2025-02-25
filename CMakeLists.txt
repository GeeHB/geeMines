cmake_minimum_required(VERSION 3.15)
project(geeMines)

include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/geeMines.c
  src/grid.c
  src/board.c
  src/shared/keys.c
  src/shared/menu.c
  # ...
)
# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS
  # ...
)
set(ASSETS_cg
  assets-cg/about.png
  assets-cg/menu.png
  assets-cg/pause.png
)

fxconv_declare_assets(${ASSETS} ${ASSETS_cg} WITH_METADATA)

add_executable(geeMines ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(geeMines PRIVATE -Wall -Wextra -Os -D=DEST_CASIO_CALC -D=NO_CAPTURE)
target_link_libraries(geeMines Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
	generate_g3a(TARGET geeMines OUTPUT "geeMines.g3a"
		NAME "geeMines" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
