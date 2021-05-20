#pragma once
#include <string>
#include <vector>
#include <any>
#include "Variable.h"

class Integer;

const std::string OBJECT_HEAD_OF_LIST = "-1";


struct ObjectProperties {
	std::string name;
	std::any expr;
};

struct Property {
	std::any value;
	std::string key;
	Property* next;
};

class Object : public Variable
{
	public:
		Object(const std::vector<ObjectProperties>& values, Context* ctx);
		Object(const Object& lhs);
		~Object();
		virtual Object& SetContext(Context* context) override;
		virtual Object& SetPostion(Position start, Position end) override;
		bool empty() const;
		void SetValue(std::string key, std::any value);
		std::any GetValue(std::string key);
		Integer HasProperty(std::string key);
		Integer operator==(const Object& rhs) const;
		Integer operator!=(const Object& rhs) const;
		friend std::ostream& operator<<(std::ostream& lhs, const Object& other);
	private:
		Property* head = nullptr;
		void insert(std::string key, std::any value);
		void remove(std::string key);

};

