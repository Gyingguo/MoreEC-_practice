template<class T>
class RCIPtr {
public:
	RCIPtr(T* realPtr = 0);
	RCIPtr(const RCIPtr& rhs);
	~RCIPtr();

	RCIPtr& operator=(const RCIPtr& rhs);

	const T* operator->() const;
	T* operator->();
	const T& operator*() const;
	T& operator*();

private:
	struct CountHolder: public RCObject {
		~CountHolder() {
			delete pointee;
		}

		T *pointee;
	};

	CountHolder *counter;

	void init();
	void makeCopy();
};

template<class T>
void RCIPtr<T>::init() {
	if (counter->isShareable() == false) {
		T *oldValue = counter->pointer;
		counter = new CountHolder;
		counter->pointee = new T(*oldValue);
	}

	counter->addReference();
}

template<class T>
PCIPtr<T>::RCIPtr(T* realPtr)
: counter(new CountHolder) {
	counter->pointee = realPtr;
	init();
}

template<class T>
RCIPtr<T>::~RCIPtr() {
	counter->removeReference();
}

template<class T>
RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr& rhs) {
	if (counter != rhs.counter) {
		counter->removeReference();
		counter = rhs.counter;
		init();
	}
	return *this;
}

template<class T>
void RCIPtr<T>::makeCopy() {
	if (counter->isShared()) {
		T *oldValue = counter->pointee;
		counter->removeReference();
		counter = new CountHolder;
		counter->pointee = new T(*oldValue);
		counter->addReference();
	}
}

template<class T>
const T* RCIPtr<T>::operator->() const {
	return counter->pointee;
}

template<class T>
T* RCIPtr<T>::operator->() {
	makeCopy();
	return counter->pointee;
}

template<class T>
const T& RCIPtr<T>::operator*() const {
	return *(counter->pointee);
}

template<class T>
T& RCIPtr<T>::operator*() {
	makeCopy();
	return *(counter->pointee);
}

class RCObject {
public: 
	void addReference();
	void removeReference();

	void markUnshareable();
	bool isSharerable() const;

	bool isShared() const;

protected:
	RCObject();
	RCObject(const RCObject& rhs);
	virtual ~RCObject() = 0;

private:
	int refCount;
	bool shareable;
};

class Widget {
public:
	Widget(int size);
	Widget(const Widget& rhs);
	~Widget();
	Widget & operator=(const Widget& rhs);

	// keep consistent with the old api
	void doThis();
	int showThat() const;
};

class RCWidget {
public:
	RCWidget(int size): value(new Widget(size)) {}
	void doThis() {
		value->doThis();
	}
	int showThat() const {
		return value->showThat();
	}

private:
	RCIPtr<Widget> value;
};