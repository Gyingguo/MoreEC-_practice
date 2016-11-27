#include "rc-string.hpp"

#include <iostream>
#include <cstring>

using namespace std;

RCObject::RCObject()
: refCount(0), shareable(true) {}

RCObject::RCObject(const RCObject&)
: refCount(0), shareable(true) {}

RCObject& RCObject::operator=(const RCObject&) {
	return *this;
}

RCObject::~RCObject() {}

void RCObject::addReference() {
	++refCount;
}

void RCObject::removeReference() {
	if (--refCount == 0) delete this;
}

void RCObject::markUnshareable() {
	shareable = false;
}

bool RCObject::isShareable() const {
	return shareable;
}

bool RCObject::isShared() const {
	return refCount > 1;
}

void string_rc::StringValue::init(const char *initValue) {
	data = new char[strlen(initValue) + 1];
	strcpy(data, initValue);
}

string_rc::StringValue::StringValue(const char *initValue) {
	init(initValue);
}

string_rc::StringValue::StringValue(const StringValue& rhs) {
	init(rhs.data);
}

string_rc::StringValue::~StringValue() {
	delete [] data;
}

string_rc::string_rc(const char * initValue)
: value(new StringValue(initValue)) {}

const char& string_rc::operator[](int index) const {
	return value->data[index];
}

char& string_rc::operator[](int index) {
	if (value->isShared()) {
		value = new StringValue(value->data);
	}

	value->markUnshareable();

	return value->data[index];
}

int main() {
	
}