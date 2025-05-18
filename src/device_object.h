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
		BUFFER = 0,
		SHADER,
		TEXTURE,
		RENDERBUFFER,
		FRAMEBUFFER,
		VERTEXARRAY,
		TYPE_LAST
	};
	Type get_type() const;
protected:
	typedef uint32_t handle_t;
	DeviceObject();
	virtual ~DeviceObject() = default; //TODO: log if not empty
	void enlist(Type type, handle_t handle);
	void delist(handle_t handle);
private:
	Type type	: 8;

	struct lookup_vector : public std::vector<handle_t> {
		void push(handle_t h);
		void pop(handle_t h);
	private:
		std::unordered_map<handle_t, size_t> table;
	};

	static lookup_vector lookups[TYPE_LAST];

public:
	static void destroy_all();
	static size_t nobjs(Type t = TYPE_LAST);
};

#endif /* DEVICE_OBJECT_H */
