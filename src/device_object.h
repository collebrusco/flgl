/** 
 * device_object.h 
 * flgl
 * created 05/18/25 by frank collebrusco
 */
#ifndef DEVICE_OBJECT_H
#define DEVICE_OBJECT_H

#include <vector>
#include <unordered_map>

struct DeviceObject {
	enum Type {
		GLBUFFER = 0,
		SHADER,
		TEXTURE,
		RENDERBUFFER,
		FRAMEBUFFER,
		VERTEXARRAY,
		ALBUFFER,
		SOURCE,
		TYPE_LAST,
		TYPE_GLS,
		TYPE_ALS,
		TYPE_NULL
	};
	static constexpr Type TYPE_GL_FIRST = GLBUFFER;
	static constexpr Type TYPE_GL_LAST = VERTEXARRAY;
	static constexpr Type TYPE_AL_FIRST = ALBUFFER;
	static constexpr Type TYPE_AL_LAST = SOURCE;
	Type get_type() const;
protected:
	typedef uint32_t handle_t;
	DeviceObject(Type t);
	virtual ~DeviceObject() = default; //TODO: log if not empty
	void enlist(handle_t handle);
	void delist(handle_t handle);
private:
	Type type	: 8;

	struct lookup_vector : public std::vector<handle_t> {
		void push(handle_t h);
		void pop(handle_t h);
		void clear();
	private:
		std::unordered_map<handle_t, size_t> table;
	};

	static lookup_vector lookups[TYPE_LAST];

public:
	static void destroy_gl();
	static void destroy_al();
	static size_t nobjs(Type t = TYPE_LAST);
};

#endif /* DEVICE_OBJECT_H */
