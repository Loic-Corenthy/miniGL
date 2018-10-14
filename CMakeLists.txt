cmake_minimum_required (VERSION 3.10)


# Set different project names
set (LOCAL_PROJECT_1 miniGL)


# Define the name of the project
project (${LOCAL_PROJECT_1})


# Display a message with the name of the project
message (STATUS "" )
message (STATUS "==================================================" )
message (STATUS "   Project: " ${LOCAL_PROJECT_1} )
message (STATUS "==================================================" )


# Check that we are not building the project in the directory containing the CMakeLists.txt file
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message ( FATAL_ERROR "Run CMake in a separate build directory to avoid source code contamination." )
endif ()


# Enumerate the local header files for the project 
set (MY_LOCAL_HEADER_FILES_PROJECT_1
	${CMAKE_SOURCE_DIR}/src/Algebra.hpp
	${CMAKE_SOURCE_DIR}/src/Angle.hpp
	${CMAKE_SOURCE_DIR}/src/Application.hpp
	${CMAKE_SOURCE_DIR}/src/AntTweakBarWrapper.hpp
	${CMAKE_SOURCE_DIR}/src/DebugRender.hpp
	${CMAKE_SOURCE_DIR}/src/BaseLight.hpp
	${CMAKE_SOURCE_DIR}/src/BaseBackend.hpp
	${CMAKE_SOURCE_DIR}/src/BackendGLFW.hpp
	${CMAKE_SOURCE_DIR}/src/BackendGLUT.hpp
	${CMAKE_SOURCE_DIR}/src/Billboard.hpp
	${CMAKE_SOURCE_DIR}/src/BillboardList.hpp
	${CMAKE_SOURCE_DIR}/src/CallbacksInterface.hpp
	${CMAKE_SOURCE_DIR}/src/CallbacksRender.hpp
	${CMAKE_SOURCE_DIR}/src/Camera.hpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapFBO.hpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLight.hpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightLighting.hpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/Constants.hpp
	${CMAKE_SOURCE_DIR}/src/CubemapTexture.hpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingNullPass.hpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingDirectionalLightPass.hpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingGeometryPass.hpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingLightPass.hpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingPointLightPass.hpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingSpotLightPass.hpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/Degree.hpp
	${CMAKE_SOURCE_DIR}/src/DirectionalLight.hpp
	${CMAKE_SOURCE_DIR}/src/EngineCommon.hpp
	${CMAKE_SOURCE_DIR}/src/EnumClassCast.hpp
	${CMAKE_SOURCE_DIR}/src/Exceptions.hpp
	${CMAKE_SOURCE_DIR}/src/GBuffer.hpp
	${CMAKE_SOURCE_DIR}/src/GLFXLighting.hpp
	${CMAKE_SOURCE_DIR}/src/GLFXTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/GLUtils.hpp
	${CMAKE_SOURCE_DIR}/src/InstancedLighting.hpp
	${CMAKE_SOURCE_DIR}/src/IntermediateBuffer.hpp
	${CMAKE_SOURCE_DIR}/src/InternalMathType.hpp
	${CMAKE_SOURCE_DIR}/src/IOBuffer.hpp
	${CMAKE_SOURCE_DIR}/src/InstancedLightingTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/Lighting.hpp
	${CMAKE_SOURCE_DIR}/src/LightingBase.hpp
	${CMAKE_SOURCE_DIR}/src/Log.hpp
	${CMAKE_SOURCE_DIR}/src/Matrix.hpp
	${CMAKE_SOURCE_DIR}/src/MeshAOS.hpp
	${CMAKE_SOURCE_DIR}/src/MeshAdjacencies.hpp
	${CMAKE_SOURCE_DIR}/src/MeshBase.hpp
	${CMAKE_SOURCE_DIR}/src/MeshBoneData.hpp
	${CMAKE_SOURCE_DIR}/src/MeshEdge.hpp
	${CMAKE_SOURCE_DIR}/src/MeshFace.hpp
	${CMAKE_SOURCE_DIR}/src/MeshNeighbors.hpp
	${CMAKE_SOURCE_DIR}/src/MeshSOA.hpp
	${CMAKE_SOURCE_DIR}/src/MotionBlur.hpp
	${CMAKE_SOURCE_DIR}/src/MeshAndTransform.hpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMap.hpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMapFBO.hpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMapLighting.hpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMapTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/NullRender.hpp
	${CMAKE_SOURCE_DIR}/src/ParticleSystem.hpp
	${CMAKE_SOURCE_DIR}/src/ParticleSystemRender.hpp
	${CMAKE_SOURCE_DIR}/src/PickingRender.hpp
	${CMAKE_SOURCE_DIR}/src/PickingTexture.hpp
	${CMAKE_SOURCE_DIR}/src/Picking3D.hpp
	${CMAKE_SOURCE_DIR}/src/PointLight.hpp
	${CMAKE_SOURCE_DIR}/src/Program.hpp
	${CMAKE_SOURCE_DIR}/src/ProgramGLFX.hpp
	${CMAKE_SOURCE_DIR}/src/Quaternion.hpp
	${CMAKE_SOURCE_DIR}/src/Radian.hpp
	${CMAKE_SOURCE_DIR}/src/RandomTexture.hpp
	${CMAKE_SOURCE_DIR}/src/RenderingTechniqueBase.hpp
	${CMAKE_SOURCE_DIR}/src/Shader.hpp
	${CMAKE_SOURCE_DIR}/src/ShadowMap.hpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapFBO.hpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLight.hpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightLighting.hpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/ShadowVolumeRender.hpp
	${CMAKE_SOURCE_DIR}/src/ShadowVolumeTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/SilhouetteRender.hpp
	${CMAKE_SOURCE_DIR}/src/SilhouetteTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/SimpleColorRender.hpp
	${CMAKE_SOURCE_DIR}/src/SimpleLightingWithShadow.hpp
	${CMAKE_SOURCE_DIR}/src/Skinning.hpp
	${CMAKE_SOURCE_DIR}/src/SkinningTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/SkyBoxRender.hpp
	${CMAKE_SOURCE_DIR}/src/Skybox.hpp
	${CMAKE_SOURCE_DIR}/src/SpotLight.hpp
	${CMAKE_SOURCE_DIR}/src/SSAORender.hpp
	${CMAKE_SOURCE_DIR}/src/SSAOGeometryPass.hpp
	${CMAKE_SOURCE_DIR}/src/SSAOBlur.hpp
	${CMAKE_SOURCE_DIR}/src/SSAOLighting.hpp
	${CMAKE_SOURCE_DIR}/src/SSAOTechnique.hpp
	${CMAKE_SOURCE_DIR}/src/TessellationLighting.hpp
	${CMAKE_SOURCE_DIR}/src/Tessellation.hpp
	${CMAKE_SOURCE_DIR}/src/TessellationPN.hpp
	${CMAKE_SOURCE_DIR}/src/Texture.hpp
	${CMAKE_SOURCE_DIR}/src/Transform.hpp
	${CMAKE_SOURCE_DIR}/src/Vector.hpp
	${CMAKE_SOURCE_DIR}/src/Vertex.hpp
	${CMAKE_SOURCE_DIR}/src/VertexBoneData.hpp

	# Shaders
	${CMAKE_SOURCE_DIR}/resources/Shaders/Debug.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/Debug.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/Billboard.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/Billboard.geom
	${CMAKE_SOURCE_DIR}/resources/Shaders/Billboard.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/DirectionalLightPass.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/GLFXLighting.glsl
	${CMAKE_SOURCE_DIR}/resources/Shaders/GeometryPass.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/GeometryPass.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/InstancedLighting.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/InstancedLighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/LightPass.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/Lighting.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/Lighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/NullPass.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/NullPass.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/ParticleUpdate.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/ParticleUpdate.geom
	${CMAKE_SOURCE_DIR}/resources/Shaders/ParticleUpdate.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/Picking.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/Picking.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/PointLightPass.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMap.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMap.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLight.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLight.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLightLighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLightLighting.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLight.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLight.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLightLighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLightLighting.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/SimpleColor.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/SimpleColor.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SkyBox.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/SkyBox.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SpotLightPass.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.ctrl
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.eval
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.ctrl
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.eval
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/Skinning.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/Skinning.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/SkinningWithMotionBlur.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SkinningWithMotionBlur.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/MotionBlur.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/MotionBlur.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/Silhouette.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/Silhouette.geom
	${CMAKE_SOURCE_DIR}/resources/Shaders/Silhouette.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowVolume.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowVolume.geom
	${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowVolume.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/Null.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/Null.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMap.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMap.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMapLighting.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMapLighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAO.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAO.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOBlur.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOBlur.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOGeometryPass.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOGeometryPass.frag
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOLighting.vert
	${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOLighting.frag
)


# Enumerate the local source files
set (MY_LOCAL_SOURCE_FILES_PROJECT_1
	${CMAKE_SOURCE_DIR}/src/Application.cpp
	${CMAKE_SOURCE_DIR}/src/AntTweakBarWrapper.cpp
	${CMAKE_SOURCE_DIR}/src/DebugRender.cpp
	${CMAKE_SOURCE_DIR}/src/BaseLight.cpp
	${CMAKE_SOURCE_DIR}/src/BaseBackend.cpp
	${CMAKE_SOURCE_DIR}/src/BackendGLFW.cpp
	${CMAKE_SOURCE_DIR}/src/BackendGLUT.cpp
	${CMAKE_SOURCE_DIR}/src/Billboard.cpp
	${CMAKE_SOURCE_DIR}/src/BillboardList.cpp
	${CMAKE_SOURCE_DIR}/src/CallbacksInterface.cpp
	${CMAKE_SOURCE_DIR}/src/CallbacksRender.cpp
	${CMAKE_SOURCE_DIR}/src/Camera.cpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapFBO.cpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLight.cpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightLighting.cpp
	${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/Constants.cpp
	${CMAKE_SOURCE_DIR}/src/CubemapTexture.cpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingNullPass.cpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingDirectionalLightPass.cpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingGeometryPass.cpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingLightPass.cpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingPointLightPass.cpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingSpotLightPass.cpp
	${CMAKE_SOURCE_DIR}/src/DeferredShadingTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/Degree.cpp
	${CMAKE_SOURCE_DIR}/src/DirectionalLight.cpp
	${CMAKE_SOURCE_DIR}/src/Exceptions.cpp
	${CMAKE_SOURCE_DIR}/src/GBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/GLFXLighting.cpp
	${CMAKE_SOURCE_DIR}/src/GLFXTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/GLUtils.cpp
	${CMAKE_SOURCE_DIR}/src/InstancedLighting.cpp
	${CMAKE_SOURCE_DIR}/src/IntermediateBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/IOBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/InstancedLightingTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/Lighting.cpp
	${CMAKE_SOURCE_DIR}/src/LightingBase.cpp
	${CMAKE_SOURCE_DIR}/src/Log.cpp
	${CMAKE_SOURCE_DIR}/src/Matrix.cpp
	${CMAKE_SOURCE_DIR}/src/MeshAOS.cpp
	${CMAKE_SOURCE_DIR}/src/MeshAdjacencies.cpp
	${CMAKE_SOURCE_DIR}/src/MeshBase.cpp
	${CMAKE_SOURCE_DIR}/src/MeshBoneData.cpp
	${CMAKE_SOURCE_DIR}/src/MeshEdge.cpp
	${CMAKE_SOURCE_DIR}/src/MeshFace.cpp
	${CMAKE_SOURCE_DIR}/src/MeshNeighbors.cpp
	${CMAKE_SOURCE_DIR}/src/MeshSOA.cpp
	${CMAKE_SOURCE_DIR}/src/MeshAndTransform.cpp
	${CMAKE_SOURCE_DIR}/src/MotionBlur.cpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMap.cpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMapFBO.cpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMapLighting.cpp
	${CMAKE_SOURCE_DIR}/src/MultipassShadowMapTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/NullRender.cpp
	${CMAKE_SOURCE_DIR}/src/ParticleSystem.cpp
	${CMAKE_SOURCE_DIR}/src/ParticleSystemRender.cpp
	${CMAKE_SOURCE_DIR}/src/PickingRender.cpp
	${CMAKE_SOURCE_DIR}/src/PickingTexture.cpp
	${CMAKE_SOURCE_DIR}/src/Picking3D.cpp
	${CMAKE_SOURCE_DIR}/src/PointLight.cpp
	${CMAKE_SOURCE_DIR}/src/Program.cpp
	${CMAKE_SOURCE_DIR}/src/ProgramGLFX.cpp
	${CMAKE_SOURCE_DIR}/src/Quaternion.cpp
	${CMAKE_SOURCE_DIR}/src/Radian.cpp
	${CMAKE_SOURCE_DIR}/src/RandomTexture.cpp
	${CMAKE_SOURCE_DIR}/src/RenderingTechniqueBase.cpp
	${CMAKE_SOURCE_DIR}/src/Shader.cpp
	${CMAKE_SOURCE_DIR}/src/ShadowMap.cpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapFBO.cpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLight.cpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightLighting.cpp
	${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/ShadowVolumeRender.cpp
	${CMAKE_SOURCE_DIR}/src/ShadowVolumeTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/SilhouetteRender.cpp
	${CMAKE_SOURCE_DIR}/src/SilhouetteTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/SimpleColorRender.cpp
	${CMAKE_SOURCE_DIR}/src/SimpleLightingWithShadow.cpp
	${CMAKE_SOURCE_DIR}/src/Skinning.cpp
	${CMAKE_SOURCE_DIR}/src/SkinningTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/SkyBoxRender.cpp
	${CMAKE_SOURCE_DIR}/src/Skybox.cpp
	${CMAKE_SOURCE_DIR}/src/SpotLight.cpp
	${CMAKE_SOURCE_DIR}/src/SSAORender.cpp
	${CMAKE_SOURCE_DIR}/src/SSAOGeometryPass.cpp
	${CMAKE_SOURCE_DIR}/src/SSAOBlur.cpp
	${CMAKE_SOURCE_DIR}/src/SSAOLighting.cpp
	${CMAKE_SOURCE_DIR}/src/SSAOTechnique.cpp
	${CMAKE_SOURCE_DIR}/src/TessellationLighting.cpp
	${CMAKE_SOURCE_DIR}/src/Tessellation.cpp
	${CMAKE_SOURCE_DIR}/src/TessellationPN.cpp
	${CMAKE_SOURCE_DIR}/src/Texture.cpp
	${CMAKE_SOURCE_DIR}/src/Transform.cpp
	${CMAKE_SOURCE_DIR}/src/Vector.cpp
	${CMAKE_SOURCE_DIR}/src/Vertex.cpp
	${CMAKE_SOURCE_DIR}/src/VertexBoneData.cpp
	${CMAKE_SOURCE_DIR}/src/main.cpp
)

# Group files per category
source_group ( "Common" FILES ${CMAKE_SOURCE_DIR}/src/Application.hpp
								  ${CMAKE_SOURCE_DIR}/src/Application.cpp
								  ${CMAKE_SOURCE_DIR}/src/CallbacksInterface.hpp
								  ${CMAKE_SOURCE_DIR}/src/CallbacksInterface.cpp
								  ${CMAKE_SOURCE_DIR}/src/Camera.hpp
								  ${CMAKE_SOURCE_DIR}/src/Camera.cpp
								  ${CMAKE_SOURCE_DIR}/src/Constants.hpp
								  ${CMAKE_SOURCE_DIR}/src/Constants.cpp
								  ${CMAKE_SOURCE_DIR}/src/EnumClassCast.hpp
								  ${CMAKE_SOURCE_DIR}/src/Exceptions.hpp
								  ${CMAKE_SOURCE_DIR}/src/Exceptions.cpp
								  ${CMAKE_SOURCE_DIR}/src/main.cpp
								  ${CMAKE_SOURCE_DIR}/src/Program.hpp
								  ${CMAKE_SOURCE_DIR}/src/Program.cpp
								  ${CMAKE_SOURCE_DIR}/src/ProgramGLFX.hpp
								  ${CMAKE_SOURCE_DIR}/src/ProgramGLFX.cpp
								  ${CMAKE_SOURCE_DIR}/src/Shader.hpp
								  ${CMAKE_SOURCE_DIR}/src/Shader.cpp
								  ${CMAKE_SOURCE_DIR}/src/Texture.hpp
								  ${CMAKE_SOURCE_DIR}/src/Texture.cpp
								  ${CMAKE_SOURCE_DIR}/src/Transform.hpp
								  ${CMAKE_SOURCE_DIR}/src/Transform.cpp
								  ${CMAKE_SOURCE_DIR}/src/Vertex.hpp
								  ${CMAKE_SOURCE_DIR}/src/Vertex.cpp
								  ${CMAKE_SOURCE_DIR}/src/BaseBackend.hpp
								  ${CMAKE_SOURCE_DIR}/src/BaseBackend.cpp
								  ${CMAKE_SOURCE_DIR}/src/BackendGLFW.hpp
								  ${CMAKE_SOURCE_DIR}/src/BackendGLFW.cpp
								  ${CMAKE_SOURCE_DIR}/src/BackendGLUT.hpp
								  ${CMAKE_SOURCE_DIR}/src/BackendGLUT.cpp
								  ${CMAKE_SOURCE_DIR}/src/Log.hpp
								  ${CMAKE_SOURCE_DIR}/src/Log.cpp
								  ${CMAKE_SOURCE_DIR}/src/GLUtils.hpp
								  ${CMAKE_SOURCE_DIR}/src/GLUtils.cpp
								  ${CMAKE_SOURCE_DIR}/src/CallbacksRender.hpp
								  ${CMAKE_SOURCE_DIR}/src/CallbacksRender.cpp
								  ${CMAKE_SOURCE_DIR}/src/RenderingTechniqueBase.hpp
								  ${CMAKE_SOURCE_DIR}/src/RenderingTechniqueBase.cpp
								  ${CMAKE_SOURCE_DIR}/src/EngineCommon.hpp)

	# Group source files in different categories
	source_group ( “Debug” FILES ${CMAKE_SOURCE_DIR}/src/Vector.hpp
								${CMAKE_SOURCE_DIR}/src/DebugRender.hpp
								${CMAKE_SOURCE_DIR}/src/DebugRender.cpp
								${CMAKE_SOURCE_DIR}/resources/Shaders/Debug.vert
								${CMAKE_SOURCE_DIR}/resources/Shaders/Debug.frag)



	# Group source files in different categories
	source_group ( "Math" FILES ${CMAKE_SOURCE_DIR}/src/Vector.hpp
								${CMAKE_SOURCE_DIR}/src/Vector.cpp
								${CMAKE_SOURCE_DIR}/src/Matrix.hpp
								${CMAKE_SOURCE_DIR}/src/Matrix.cpp
								${CMAKE_SOURCE_DIR}/src/Quaternion.hpp
								${CMAKE_SOURCE_DIR}/src/Quaternion.cpp
								${CMAKE_SOURCE_DIR}/src/Algebra.hpp
								${CMAKE_SOURCE_DIR}/src/Degree.hpp
								${CMAKE_SOURCE_DIR}/src/Degree.cpp
								${CMAKE_SOURCE_DIR}/src/Radian.hpp
								${CMAKE_SOURCE_DIR}/src/Radian.cpp
								${CMAKE_SOURCE_DIR}/src/Angle.hpp
								${CMAKE_SOURCE_DIR}/src/InternalMathType.hpp)

	source_group ( "Mesh" FILES ${CMAKE_SOURCE_DIR}/src/VertexBoneData.hpp
								${CMAKE_SOURCE_DIR}/src/VertexBoneData.cpp
								${CMAKE_SOURCE_DIR}/src/MeshBoneData.hpp
								${CMAKE_SOURCE_DIR}/src/MeshBoneData.cpp
								${CMAKE_SOURCE_DIR}/src/MeshBase.hpp
								${CMAKE_SOURCE_DIR}/src/MeshBase.cpp
								${CMAKE_SOURCE_DIR}/src/MeshAOS.hpp
								${CMAKE_SOURCE_DIR}/src/MeshAOS.cpp
								${CMAKE_SOURCE_DIR}/src/MeshSOA.hpp
								${CMAKE_SOURCE_DIR}/src/MeshSOA.cpp
								${CMAKE_SOURCE_DIR}/src/MeshAdjacencies.hpp
								${CMAKE_SOURCE_DIR}/src/MeshAdjacencies.cpp
								${CMAKE_SOURCE_DIR}/src/MeshEdge.hpp
								${CMAKE_SOURCE_DIR}/src/MeshEdge.cpp
								${CMAKE_SOURCE_DIR}/src/MeshFace.hpp
								${CMAKE_SOURCE_DIR}/src/MeshFace.cpp
								${CMAKE_SOURCE_DIR}/src/MeshNeighbors.hpp
								${CMAKE_SOURCE_DIR}/src/MeshNeighbors.cpp
								${CMAKE_SOURCE_DIR}/src/MeshAndTransform.hpp
								${CMAKE_SOURCE_DIR}/src/MeshAndTransform.cpp)

	source_group ( "Lights" FILES ${CMAKE_SOURCE_DIR}/src/BaseLight.hpp
								  ${CMAKE_SOURCE_DIR}/src/BaseLight.cpp
								  ${CMAKE_SOURCE_DIR}/src/DirectionalLight.hpp
								  ${CMAKE_SOURCE_DIR}/src/DirectionalLight.cpp
								  ${CMAKE_SOURCE_DIR}/src/PointLight.hpp
								  ${CMAKE_SOURCE_DIR}/src/PointLight.cpp
								  ${CMAKE_SOURCE_DIR}/src/SpotLight.hpp
								  ${CMAKE_SOURCE_DIR}/src/SpotLight.cpp)

	source_group ( "Lighting" FILES ${CMAKE_SOURCE_DIR}/src/LightingBase.hpp
									${CMAKE_SOURCE_DIR}/src/LightingBase.cpp
									${CMAKE_SOURCE_DIR}/src/Lighting.hpp
									${CMAKE_SOURCE_DIR}/src/Lighting.cpp
									${CMAKE_SOURCE_DIR}/src/SimpleLightingWithShadow.hpp
									${CMAKE_SOURCE_DIR}/src/SimpleLightingWithShadow.cpp
									${CMAKE_SOURCE_DIR}/resources/Shaders/Lighting.vert
									${CMAKE_SOURCE_DIR}/resources/Shaders/Lighting.frag)

	source_group ( "Instanced Lighting" FILES ${CMAKE_SOURCE_DIR}/src/InstancedLighting.hpp
											  ${CMAKE_SOURCE_DIR}/src/InstancedLighting.cpp
											  ${CMAKE_SOURCE_DIR}/src/InstancedLightingTechnique.hpp
											  ${CMAKE_SOURCE_DIR}/src/InstancedLightingTechnique.cpp
											  ${CMAKE_SOURCE_DIR}/resources/Shaders/InstancedLighting.vert
											  ${CMAKE_SOURCE_DIR}/resources/Shaders/InstancedLighting.frag)

	source_group ( "Shadow Map" FILES ${CMAKE_SOURCE_DIR}/src/ShadowMap.hpp
									  ${CMAKE_SOURCE_DIR}/src/ShadowMap.cpp
									  ${CMAKE_SOURCE_DIR}/src/ShadowMapFBO.hpp
									  ${CMAKE_SOURCE_DIR}/src/ShadowMapFBO.cpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMap.hpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMap.cpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMapFBO.hpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMapFBO.cpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMapLighting.hpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMapLighting.cpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMapTechnique.hpp
									  ${CMAKE_SOURCE_DIR}/src/MultipassShadowMapTechnique.cpp
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMap.frag
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMap.vert
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMap.frag
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMap.vert
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMapLighting.frag
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/MultipassShadowMapLighting.vert)

	source_group ( "Deferred Shading" FILES ${CMAKE_SOURCE_DIR}/src/DeferredShadingDirectionalLightPass.hpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingDirectionalLightPass.cpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingGeometryPass.hpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingGeometryPass.cpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingNullPass.hpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingNullPass.cpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingLightPass.hpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingLightPass.cpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingPointLightPass.hpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingPointLightPass.cpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingSpotLightPass.hpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingSpotLightPass.cpp
											${CMAKE_SOURCE_DIR}/src/GBuffer.hpp
											${CMAKE_SOURCE_DIR}/src/GBuffer.cpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingTechnique.hpp
											${CMAKE_SOURCE_DIR}/src/DeferredShadingTechnique.cpp
											${CMAKE_SOURCE_DIR}/resources/Shaders/GeometryPass.vert
											${CMAKE_SOURCE_DIR}/resources/Shaders/GeometryPass.frag
											${CMAKE_SOURCE_DIR}/resources/Shaders/NullPass.vert
											${CMAKE_SOURCE_DIR}/resources/Shaders/NullPass.frag
											${CMAKE_SOURCE_DIR}/resources/Shaders/LightPass.vert
											${CMAKE_SOURCE_DIR}/resources/Shaders/DirectionalLightPass.frag
											${CMAKE_SOURCE_DIR}/resources/Shaders/PointLightPass.frag
											${CMAKE_SOURCE_DIR}/resources/Shaders/SpotLightPass.frag)

	source_group ( "Skybox" FILES ${CMAKE_SOURCE_DIR}/src/CubemapTexture.cpp
								  ${CMAKE_SOURCE_DIR}/src/CubemapTexture.hpp
								  ${CMAKE_SOURCE_DIR}/src/SkyBoxRender.cpp
								  ${CMAKE_SOURCE_DIR}/src/SkyBoxRender.hpp
								  ${CMAKE_SOURCE_DIR}/src/Skybox.cpp
								  ${CMAKE_SOURCE_DIR}/src/Skybox.hpp
								  ${CMAKE_SOURCE_DIR}/resources/Shaders/SkyBox.vert
								  ${CMAKE_SOURCE_DIR}/resources/Shaders/SkyBox.frag)

	source_group ( "Billboard" FILES ${CMAKE_SOURCE_DIR}/src/Billboard.cpp
									 ${CMAKE_SOURCE_DIR}/src/Billboard.hpp
									 ${CMAKE_SOURCE_DIR}/src/BillboardList.cpp
									 ${CMAKE_SOURCE_DIR}/src/BillboardList.hpp
									 ${CMAKE_SOURCE_DIR}/resources/Shaders/Billboard.frag
									 ${CMAKE_SOURCE_DIR}/resources/Shaders/Billboard.geom
									 ${CMAKE_SOURCE_DIR}/resources/Shaders/Billboard.vert)

	source_group ( "Particles" FILES ${CMAKE_SOURCE_DIR}/src/Particle.hpp
									 ${CMAKE_SOURCE_DIR}/src/Particle.cpp
									 ${CMAKE_SOURCE_DIR}/src/ParticleSystem.hpp
									 ${CMAKE_SOURCE_DIR}/src/ParticleSystem.cpp
									 ${CMAKE_SOURCE_DIR}/src/ParticleSystemRender.hpp
									 ${CMAKE_SOURCE_DIR}/src/ParticleSystemRender.cpp
									 ${CMAKE_SOURCE_DIR}/src/RandomTexture.hpp
									 ${CMAKE_SOURCE_DIR}/src/RandomTexture.cpp
									 ${CMAKE_SOURCE_DIR}/resources/Shaders/ParticleUpdate.vert
									 ${CMAKE_SOURCE_DIR}/resources/Shaders/ParticleUpdate.geom
									 ${CMAKE_SOURCE_DIR}/resources/Shaders/ParticleUpdate.frag)

	source_group ( "3D Picking" FILES ${CMAKE_SOURCE_DIR}/src/PickingRender.hpp
									  ${CMAKE_SOURCE_DIR}/src/PickingRender.cpp
									  ${CMAKE_SOURCE_DIR}/src/PickingTexture.hpp
									  ${CMAKE_SOURCE_DIR}/src/PickingTexture.cpp
									  ${CMAKE_SOURCE_DIR}/src/SimpleColorRender.hpp
									  ${CMAKE_SOURCE_DIR}/src/SimpleColorRender.cpp
									  ${CMAKE_SOURCE_DIR}/src/Picking3D.hpp
									  ${CMAKE_SOURCE_DIR}/src/Picking3D.cpp
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/Picking.vert
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/Picking.frag
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/SimpleColor.frag
									  ${CMAKE_SOURCE_DIR}/resources/Shaders/SimpleColor.vert)

	source_group ( "Tesselation" FILES ${CMAKE_SOURCE_DIR}/src/TessellationLighting.hpp
									   ${CMAKE_SOURCE_DIR}/src/TessellationLighting.cpp
									   ${CMAKE_SOURCE_DIR}/src/Tessellation.hpp
									   ${CMAKE_SOURCE_DIR}/src/Tessellation.cpp
									   ${CMAKE_SOURCE_DIR}/src/TessellationPN.hpp
									   ${CMAKE_SOURCE_DIR}/src/TessellationPN.cpp
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.ctrl
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.eval
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.frag
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationLighting.vert
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.ctrl
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.eval
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.frag
									   ${CMAKE_SOURCE_DIR}/resources/Shaders/TessellationPNLighting.vert)


	source_group ( "Skeletal animation" FILES   ${CMAKE_SOURCE_DIR}/src/Skinning.hpp
												${CMAKE_SOURCE_DIR}/src/Skinning.cpp
												${CMAKE_SOURCE_DIR}/src/SkinningTechnique.hpp
												${CMAKE_SOURCE_DIR}/src/SkinningTechnique.cpp
												${CMAKE_SOURCE_DIR}/src/MotionBlur.hpp
												${CMAKE_SOURCE_DIR}/src/MotionBlur.cpp
												${CMAKE_SOURCE_DIR}/src/IntermediateBuffer.hpp
												${CMAKE_SOURCE_DIR}/src/IntermediateBuffer.cpp
												${CMAKE_SOURCE_DIR}/resources/Shaders/Skinning.vert
												${CMAKE_SOURCE_DIR}/resources/Shaders/Skinning.frag
												${CMAKE_SOURCE_DIR}/resources/Shaders/SkinningWithMotionBlur.vert
												${CMAKE_SOURCE_DIR}/resources/Shaders/SkinningWithMotionBlur.frag
												${CMAKE_SOURCE_DIR}/resources/Shaders/MotionBlur.vert
												${CMAKE_SOURCE_DIR}/resources/Shaders/MotionBlur.frag)


	source_group ( "Stencil shadow volume" FILES ${CMAKE_SOURCE_DIR}/src/SilhouetteRender.hpp
												 ${CMAKE_SOURCE_DIR}/src/SilhouetteRender.cpp
												 ${CMAKE_SOURCE_DIR}/src/SilhouetteTechnique.hpp
												 ${CMAKE_SOURCE_DIR}/src/SilhouetteTechnique.cpp
												 ${CMAKE_SOURCE_DIR}/src/ShadowVolumeRender.hpp
												 ${CMAKE_SOURCE_DIR}/src/ShadowVolumeRender.cpp
												 ${CMAKE_SOURCE_DIR}/src/ShadowVolumeTechnique.hpp
												 ${CMAKE_SOURCE_DIR}/src/ShadowVolumeTechnique.cpp
												 ${CMAKE_SOURCE_DIR}/src/NullRender.hpp
												 ${CMAKE_SOURCE_DIR}/src/NullRender.cpp
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/Silhouette.vert
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/Silhouette.geom
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/Silhouette.frag
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowVolume.vert
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowVolume.geom
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowVolume.frag
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/Null.vert
												 ${CMAKE_SOURCE_DIR}/resources/Shaders/Null.frag)


	source_group ("Screen Space Ambiant Occlusion" FILES ${CMAKE_SOURCE_DIR}/src/SSAORender.hpp
														 ${CMAKE_SOURCE_DIR}/src/SSAORender.cpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOGeometryPass.hpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOGeometryPass.cpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOBlur.hpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOBlur.cpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOLighting.hpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOLighting.cpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOTechnique.hpp
														 ${CMAKE_SOURCE_DIR}/src/SSAOTechnique.cpp
														 ${CMAKE_SOURCE_DIR}/src/IOBuffer.hpp
														 ${CMAKE_SOURCE_DIR}/src/IOBuffer.cpp
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAO.vert
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAO.frag
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOBlur.vert
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOBlur.frag
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOGeometryPass.vert
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOGeometryPass.frag
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOLighting.vert
														 ${CMAKE_SOURCE_DIR}/resources/Shaders/SSAOLighting.frag)

	source_group ("Shadow Map Directional Light" FILES ${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLight.hpp
													   ${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLight.cpp
													   ${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightLighting.hpp
													   ${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightLighting.cpp
													   ${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightTechnique.hpp
													   ${CMAKE_SOURCE_DIR}/src/ShadowMapDirectionalLightTechnique.cpp
													   ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLight.vert
													   ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLight.frag
													   ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLightLighting.vert
													   ${CMAKE_SOURCE_DIR}/resources/Shaders/ShadowMapDirectionalLightLighting.frag)

	source_group("Cascaded Shadow Map Directional Light" FILES ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapFBO.hpp
															   ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapFBO.cpp
															   ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLight.hpp
															   ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLight.cpp
															   ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightLighting.hpp
															   ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightLighting.cpp
															   ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightTechnique.hpp
															   ${CMAKE_SOURCE_DIR}/src/CascadedShadowMapDirectionalLightTechnique.cpp
															   ${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLight.vert
															   ${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLight.frag
															   ${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLightLighting.vert
															   ${CMAKE_SOURCE_DIR}/resources/Shaders/CascadedShadowMapDirectionalLightLighting.frag)

	source_group("GLFX" FILES ${CMAKE_SOURCE_DIR}/src/ProgramGLFX.hpp
							  ${CMAKE_SOURCE_DIR}/src/ProgramGLFX.cpp
							  ${CMAKE_SOURCE_DIR}/src/GLFXLighting.hpp
							  ${CMAKE_SOURCE_DIR}/src/GLFXLighting.cpp
							  ${CMAKE_SOURCE_DIR}/src/GLFXTechnique.hpp
							  ${CMAKE_SOURCE_DIR}/src/GLFXTechnique.cpp
							  ${CMAKE_SOURCE_DIR}/resources/Shaders/GLFXLighting.glsl)

	source_group ("Ant Tweak Bar" FILES ${CMAKE_SOURCE_DIR}/src/AntTweakBarWrapper.hpp
										${CMAKE_SOURCE_DIR}/src/AntTweakBarWrapper.cpp)


# Set the required c++ version
if (NOT WIN32)
	add_definitions (-std=c++14)
endif ()


if (APPLE)
	add_executable (${LOCAL_PROJECT_1} ${MY_LOCAL_SOURCE_FILES_PROJECT_1} ${MY_LOCAL_HEADER_FILES_PROJECT_1})


	# Find packages using modules provided by cmake
	find_package (ImageMagick COMPONENTS Magick++ REQUIRED)
	find_package (OPENGL REQUIRED)


	# Add local (from the cmake folder in this repo) Findxxx.cmake for other packages
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


	# Look for external libraries not included in the external directory
	find_package (ASSIMP REQUIRED)
	find_package (GLEW REQUIRED)
	find_package (glfw3 3.2 REQUIRED)

	# Define varibales for external libraries added manually
	set (GLFX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glfx/include/gl)
	set (GLFX_LIBRARY ${CMAKE_SOURCE_DIR}/external/glfx/build/src/libglfx_static.a)

	set (ANT_TWEAK_BAR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/AntTweakBar/include)
	set (ANT_TWEAK_BAR_LIBRARY ${CMAKE_SOURCE_DIR}/external/AntTweakBar/lib/libAntTweakBar.dylib)


	# Set include directories for source code and external libraries
	target_include_directories(${LOCAL_PROJECT_1} PUBLIC ${CMAKE_SOURCE_DIR}/src
														 ${ImageMagick_INCLUDE_DIRS}
														 ${ASSIMP_INCLUDE_DIR}
														 ${GLFW3_INCLUDE_DIR}
														 ${GLFX_INCLUDE_DIR}
														 ${ANT_TWEAK_BAR_INCLUDE_DIR}
	)


	# Specify external libraries needed for this project
	target_link_libraries (${LOCAL_PROJECT_1} ${ASSIMP_LIBRARY} 
											  ${GLEW_LIBRARY}
											  ${GLFW3_LIBRARY}
											  ${ImageMagick_LIBRARIES}
											  ${OPENGL_LIBRARY}
											  ${GLFX_LIBRARY}
											  ${ANT_TWEAK_BAR_LIBRARY}
	)


	# Copy resources directory
	ADD_CUSTOM_COMMAND (TARGET ${LOCAL_PROJECT_1} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources "${CMAKE_CURRENT_BINARY_DIR}/resources/")
	message (STATUS "Copied resources from " ${CMAKE_SOURCE_DIR}/resources " to " ${CMAKE_CURRENT_BINARY_DIR}/resources/)


	# Correct wrong path for the Ant Tweak bar library
	add_custom_command (TARGET ${LOCAL_PROJECT_1} POST_BUILD COMMAND
	${CMAKE_INSTALL_NAME_TOOL} -change "../lib/libAntTweakBar.dylib" ${CMAKE_SOURCE_DIR}/external/AntTweakBar/lib/libAntTweakBar.dylib $<TARGET_FILE_DIR:${LOCAL_PROJECT_1}>/${LOCAL_PROJECT_1})


	# Add those option to remove warnings from magick++
	target_compile_definitions (${LOCAL_PROJECT_1} PUBLIC "MAGICKCORE_HDRI_ENABLE=0")
	target_compile_definitions (${LOCAL_PROJECT_1} PUBLIC "MAGICKCORE_QUANTUM_DEPTH=16")


	# Finish by a reminder of the current build configuration for the project
	if (CMAKE_BUILD_TYPE MATCHES "(.*)Release")
		message ( STATUS "=========================")
		message ( STATUS "   Build type: RELEASE   ")
		message ( STATUS "=========================")
	endif ()

	if (CMAKE_BUILD_TYPE MATCHES "(.*)Debug")
		message ( STATUS "=======================")
		message ( STATUS "   Build type: DEBUG   ")
		message ( STATUS "=======================")
	endif ()

endif ()


# Configure a visual studio project to build and run and application
if (WIN32)
	add_executable (${LOCAL_PROJECT_1} ${MY_LOCAL_SOURCE_FILES_PROJECT_1} ${MY_LOCAL_HEADER_FILES_PROJECT_1})

	# Add a directory with the header files to the project
	source_group ("Header Files" FILES ${MY_LOCAL_HEADER_FILES_PROJECT_1})


	set	(ImageMagick_INCLUDE_DIRS CACHE PATH "Image Magick include directory")
	set (ASSIMP_INCLUDE_DIR CACHE PATH "Assimp include directory")
	set (GLEW_INCLUDE_DIR CACHE PATH "Glew library")
	set (GLFW3_INCLUDE_DIR CACHE PATH "glfw3 include directory")
	set (GLFX_INCLUDE_DIR CACHE PATH "glfx include directory")
	set (ANT_TWEAK_BAR_INCLUDE_DIR CACHE PATH "Ant Tweak Bar include directory")

	set (ASSIMP_LIBRARY CACHE FILEPATH "Assimp library")
	set (GLEW_LIBRARY CACHE FILEPATH "Glew library")
	set (GLFW3_LIBRARY CACHE FILEPATH "glfw3 library")
	set (ImageMagick_LIBRARIES CACHE FILEPATH "Image magick library")
	set (OPENGL_LIBRARY "opengl32.lib" CACHE FILEPATH "OpenGL library")
	set (GLFX_LIBRARY CACHE FILEPATH "GLFX library")
	set (ANT_TWEAK_BAR_LIBRARY CACHE FILEPATH "Ant Tweak Bar library")


	# Set include directories for source code and external libraries
	target_include_directories(${LOCAL_PROJECT_1} PUBLIC ${CMAKE_SOURCE_DIR}/src
														 ${ImageMagick_INCLUDE_DIRS}
														 ${ASSIMP_INCLUDE_DIR}
														 ${GLEW__INCLUDE_DIR}
														 ${GLFW3_INCLUDE_DIR}
														 ${GLFX_INCLUDE_DIR}
														 ${ANT_TWEAK_BAR_INCLUDE_DIR}
	)


	# Specify external libraries needed for this project
	target_link_libraries (${LOCAL_PROJECT_1} ${ASSIMP_LIBRARY} 
											  ${GLEW_LIBRARY}
											  ${GLFW3_LIBRARY}
											  ${ImageMagick_LIBRARIES}
											  ${OPENGL_LIBRARY}
											  ${GLFX_LIBRARY}
											  ${ANT_TWEAK_BAR_LIBRARY}
	)


	# Copy resources directory
	ADD_CUSTOM_COMMAND (TARGET ${LOCAL_PROJECT_1} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources "${CMAKE_CURRENT_BINARY_DIR}/resources/")
	message (STATUS "Copied resources from " ${CMAKE_SOURCE_DIR}/resources " to " ${CMAKE_CURRENT_BINARY_DIR}/resources/)


	# Define preprocessor variables
	target_compile_definitions (${LOCAL_PROJECT_1} PUBLIC "_USE_MATH_DEFINES")

	# Add those option to remove warnings from magick++
	target_compile_definitions (${LOCAL_PROJECT_1} PUBLIC "MAGICKCORE_HDRI_ENABLE=0")
	target_compile_definitions (${LOCAL_PROJECT_1} PUBLIC "MAGICKCORE_QUANTUM_DEPTH=16")


	# Set the Runtime Library to static for the tests that are included below
	# Need to do that here instead of in the test specific CMakeLists because of scope. See https://stackoverflow.com/questions/20800166/cmake-compile-with-mt-instead-of-md
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd" )
	set(CMAKE_CXX_FLAGS_RELEASE "/MT")

endif ()

# Add tests using google test
ADD_SUBDIRECTORY(test/)
