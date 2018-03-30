#pragma once
#include "lisp/common.h"

namespace craft {
namespace lisp {
namespace library
{

	class Buffer
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Buffer);
	protected:
		std::vector<uint8_t> _data;

	public:
		Buffer(instance<int64_t> size);
		instance<int64_t> size();
		instance<int64_t> at(instance<int64_t> i);
		void set(instance<int64_t> i, instance<int64_t> d);
		std::vector<uint8_t>& data(); 
		instance<Buffer> join(instance<Buffer> other);
	};

	class List
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::List);
	protected:
		std::vector<instance<>> _data;

	public:

		instance<int64_t> size();
		instance<> at(instance<int64_t> i);
		void insert(instance<int64_t> i, instance<> v);
		void erase(instance<int64_t> i);
		void reverse();
		void push(instance<> i);
		instance<> pop();
		instance<List> slice(instance<int64_t> i, instance<int64_t> j);
		std::vector<instance<>>& data();
	};

	class Map
		: public virtual types::Object
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Map);
	private:
		struct cmpByInstanceSemantics {
			inline bool operator()(const instance<>& a, const instance<>& b) const {
				if (a.typeId() != b.typeId()) return a.typeId() < b.typeId();

				if (a.typeId().isType<std::string>())
					return *a.asType<std::string>() < *b.asType<std::string>();
				else if (a.typeId().isType<int64_t>())
					return *a.asType<int64_t>() < *b.asType<int64_t>();
				else return a.get() < b.get();
			}
		};
	protected:
		std::map<instance<>, instance<>, cmpByInstanceSemantics> _data;

	public:

		instance<int64_t> size();

		instance<> at(instance<> i);

		void insert(instance<> i, instance<> v);
		void erase(instance<> i);
		std::map<instance<>, instance<>, cmpByInstanceSemantics>& data();
		std::vector<instance<>> keys();
		std::vector<instance<>> values();
	};
}}}