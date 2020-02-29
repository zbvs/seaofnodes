#pragma once
#include <json/json.h>
#include "Error.h"
using json = nlohmann::json;

typedef size_t Value;

#define VARIABLE_ID_SHIFT 0x8
#define TO_ID(Loc) Loc << VARIABLE_ID_SHIFT
#define TO_LOC(ID) ID >> VARIABLE_ID_SHIFT
#define ID_MASK (Value)-1 << VARIABLE_ID_SHIFT



class Variable {
public:
	static Variable toVariable(Value value) {
		Variable var;
		var.set_value(value);
		return var;
	}
	
	Value toOriginValue() const { return value_ & ID_MASK; }
	void set_value(Value value) { value_ = value; }
	Value value() { return value_; }
	bool isSameVariableDef(Variable rhs) {
		Value lhs_value = value();
		Value rhs_value = rhs.value();
		return lhs_value == rhs_value;
	}

	bool isSameVariable(Variable rhs) {
		Value lhs_value = toOriginValue();
		Value rhs_value = rhs.toOriginValue();
		return lhs_value == rhs_value;
	}
private:
	Variable() {}
	Value value_;

};


class VariableCounter {
public:
	VariableCounter(Variable visit_variable) : variable_(visit_variable), def_count_(0) {

	}
	~VariableCounter() { if (use_count_stack_.size() != 0)_Errmsg("use_count_stack_ hasn't been fully poped"); }
	Variable variable() { return variable_; }
	int def_count() { return def_count_; }
	void inc_def_count() { def_count_++; set_current_use_count(def_count_); }
	void push_use_count(int count) { use_count_stack_.push_back(count); }
	int pop_use_count() { int result = use_count_stack_.back(); use_count_stack_.pop_back(); return result; }
	int get_use_count() { return use_count_stack_.back(); }
	void set_current_use_count(int count) { if (use_count_stack_.size() == 0)_Errmsg("Invalid var use_count stack size"); use_count_stack_[use_count_stack_.size()-1] = count; }
private:
	Variable variable_;
	int def_count_;
	std::vector<int> use_count_stack_;
};

template<class T>
class CommonVector {
public:
	CommonVector() {}
	CommonVector(size_t size) :vector_(size) {}
	using iterator = typename std::vector<T>::iterator;
	std::vector<T>& vector() { return vector_; }
	void push_back(T element) { vector_.push_back(element); }
	void clear() { vector_.clear(); }
	bool empty() { return vector_.empty(); }
	size_t size() { return vector_.size(); }
	void pop_back() { return vector_.pop_back(); }
	T back() { return vector_.back(); }
	void pop_front() {
		if (size < 0)_Errmsg("pop_front() on invalid size vector");
		remove(vector_.at(0));
	}
	T front() {
		if (size < 0)_Errmsg("front() on invalid size vector");
		return vector_.at(0);
	}
	iterator  insert(iterator position, const T& val) { return vector_.insert(position, val); }
	T operator [](size_t index) { return vector_[index]; }
	T at(const size_t _Pos) { return vector_.at(_Pos); }
	iterator begin() { return vector_.begin(); }
	iterator end() { return vector_.end(); }
	iterator remove(T element) {
		if (!IsContain(element)) {
			_Errmsg("remove(): vector does not element");
		}
		return vector_.erase(std::remove(vector_.begin(), vector_.end(), element), vector_.end());
	}
	void resize(size_t size) { vector_.resize(size); }
	bool IsContain(T element) {
		if (std::find(vector_.begin(), vector_.end(), element) != vector_.end()) {
			return true;
		}
		else {
			return false;
		}
	}
	int GetIndexOf(T target) {
		iterator it = std::find(vector_.begin(), vector_.end(), target);
		if (it == vector_.end())
			return -1;
		return std::distance(vector_.begin(), it);
	}
	void concatenate(CommonVector<T> common_vector) {
		std::vector<T> target_vector = common_vector.vector();
		vector_.insert(vector_.end(), target_vector.begin(), target_vector.end());
	}
	void printVector(std::string msg) {
		std::cout << "printVector " << msg << " " << std::endl;
		for (iterator it = vector_.begin(); it != vector_.end(); it++) {
			T element = (*it);
			element->Print("");
		}
	}


	//T at(){}
private:
	std::vector<T> vector_;
};

