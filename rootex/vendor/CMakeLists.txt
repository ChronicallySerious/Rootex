file(GLOB_RECURSE VendorSources stub.cpp)
file(GLOB_RECURSE VendorHeaders *.h)

set(ROOTEX_INCLUDES
    ${ROOTEX_INCLUDES}
    ${CMAKE_CURRENT_LIST_DIR}
CACHE INTERNAL "")

set(ROOTEX_HEADERS
    ${ROOTEX_HEADERS}
    ${VendorHeaders}
CACHE INTERNAL "")

set(ROOTEX_SOURCES
    ${ROOTEX_SOURCES}
    ${VendorSources}
CACHE INTERNAL "")

add_subdirectory(Bullet3D)
add_subdirectory(OpenALSoft)
add_subdirectory(DirectXTK)
add_subdirectory(Gainput)
add_subdirectory(ImGui)
add_subdirectory(ImGuiColorTextEdit)
add_subdirectory(ImGuizmo)
add_subdirectory(JSON)
add_subdirectory(Assimp)
add_subdirectory(FreeType)
add_subdirectory(Rlottie)
add_subdirectory(RmlUi)
add_subdirectory(Tracy)
add_subdirectory(ASSAO)
add_subdirectory(alut)
add_subdirectory(Meshoptimizer)
add_subdirectory(Effekseer)
