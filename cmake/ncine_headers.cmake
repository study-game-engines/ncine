set(HEADERS
	${NCINE_ROOT}/include/ncine/common_defines.h
	${NCINE_ROOT}/include/ncine/common_constants.h
	${NCINE_ROOT}/include/ncine/common_macros.h
	${NCINE_ROOT}/include/ncine/Random.h
	${NCINE_ROOT}/include/ncine/Rect.h
	${NCINE_ROOT}/include/ncine/Color.h
	${NCINE_ROOT}/include/ncine/Colorf.h
	${NCINE_ROOT}/include/ncine/Object.h
	${NCINE_ROOT}/include/ncine/DrawableNode.h
	${NCINE_ROOT}/include/ncine/IAppEventHandler.h
	${NCINE_ROOT}/include/ncine/IInputEventHandler.h
	${NCINE_ROOT}/include/ncine/InputEvents.h
	${NCINE_ROOT}/include/ncine/IInputManager.h
	${NCINE_ROOT}/include/ncine/Keys.h
	${NCINE_ROOT}/include/ncine/Vector2.h
	${NCINE_ROOT}/include/ncine/Vector3.h
	${NCINE_ROOT}/include/ncine/Vector4.h
	${NCINE_ROOT}/include/ncine/Matrix4x4.h
	${NCINE_ROOT}/include/ncine/Quaternion.h
	${NCINE_ROOT}/include/ncine/IIndexer.h
	${NCINE_ROOT}/include/ncine/ILogger.h
	${NCINE_ROOT}/include/ncine/IAudioDevice.h
	${NCINE_ROOT}/include/ncine/IThreadPool.h
	${NCINE_ROOT}/include/ncine/IThreadCommand.h
	${NCINE_ROOT}/include/ncine/IGfxCapabilities.h
	${NCINE_ROOT}/include/ncine/ServiceLocator.h
	${NCINE_ROOT}/include/ncine/DisplayMode.h
	${NCINE_ROOT}/include/ncine/TimeStamp.h
	${NCINE_ROOT}/include/ncine/Timer.h
	${NCINE_ROOT}/include/ncine/Font.h
	${NCINE_ROOT}/include/ncine/IFile.h
	${NCINE_ROOT}/include/ncine/IGfxDevice.h
	${NCINE_ROOT}/include/ncine/Texture.h
	${NCINE_ROOT}/include/ncine/SceneNode.h
	${NCINE_ROOT}/include/ncine/BaseSprite.h
	${NCINE_ROOT}/include/ncine/Sprite.h
	${NCINE_ROOT}/include/ncine/MeshSprite.h
	${NCINE_ROOT}/include/ncine/Application.h
	${NCINE_ROOT}/include/ncine/PCApplication.h
	${NCINE_ROOT}/include/ncine/AppConfiguration.h
	${NCINE_ROOT}/include/ncine/IDebugOverlay.h
	${NCINE_ROOT}/include/ncine/ParticleAffectors.h
	${NCINE_ROOT}/include/ncine/ParticleSystem.h
	${NCINE_ROOT}/include/ncine/ParticleInitializer.h
	${NCINE_ROOT}/include/ncine/TextNode.h
	${NCINE_ROOT}/include/ncine/RectAnimation.h
	${NCINE_ROOT}/include/ncine/AnimatedSprite.h
)

set(NCTL_HEADERS
	${NCINE_ROOT}/include/nctl/algorithms.h
	${NCINE_ROOT}/include/nctl/iterator.h
	${NCINE_ROOT}/include/nctl/type_traits.h
	${NCINE_ROOT}/include/nctl/utility.h
	${NCINE_ROOT}/include/nctl/Array.h
	${NCINE_ROOT}/include/nctl/ArrayIterator.h
	${NCINE_ROOT}/include/nctl/StaticArray.h
	${NCINE_ROOT}/include/nctl/List.h
	${NCINE_ROOT}/include/nctl/ListIterator.h
	${NCINE_ROOT}/include/nctl/String.h
	${NCINE_ROOT}/include/nctl/StringIterator.h
	${NCINE_ROOT}/include/nctl/HashFunctions.h
	${NCINE_ROOT}/include/nctl/HashMap.h
	${NCINE_ROOT}/include/nctl/HashMapIterator.h
	${NCINE_ROOT}/include/nctl/StaticHashMap.h
	${NCINE_ROOT}/include/nctl/StaticHashMapIterator.h
	${NCINE_ROOT}/include/nctl/HashMapList.h
	${NCINE_ROOT}/include/nctl/HashMapListIterator.h
	${NCINE_ROOT}/include/nctl/HashSet.h
	${NCINE_ROOT}/include/nctl/HashSetIterator.h
	${NCINE_ROOT}/include/nctl/StaticHashSet.h
	${NCINE_ROOT}/include/nctl/StaticHashSetIterator.h
	${NCINE_ROOT}/include/nctl/HashSetList.h
	${NCINE_ROOT}/include/nctl/HashSetListIterator.h
	${NCINE_ROOT}/include/nctl/SparseSet.h
	${NCINE_ROOT}/include/nctl/SparseSetIterator.h
	${NCINE_ROOT}/include/nctl/ReverseIterator.h
	${NCINE_ROOT}/include/nctl/Atomic.h
	${NCINE_ROOT}/include/nctl/UniquePtr.h
	${NCINE_ROOT}/include/nctl/SharedPtr.h
)

if(WIN32)
	list(APPEND NCTL_HEADERS ${NCINE_ROOT}/include/nctl/WindowsAtomic.inl)
elseif(APPLE)
	list(APPEND NCTL_HEADERS ${NCINE_ROOT}/include/nctl/StdAtomic.inl)
else()
	list(APPEND NCTL_HEADERS ${NCINE_ROOT}/include/nctl/GccAtomic.inl)
endif()
