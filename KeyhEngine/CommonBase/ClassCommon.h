#pragma once

#define REMOVE_COPY(ClassName) \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete;

#define REMOVE_MOVE(ClassName) \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete;

#define REMOVE_COPY_AND_MOVE(ClassName) \
	REMOVE_COPY(ClassName) \
	REMOVE_MOVE(ClassName)