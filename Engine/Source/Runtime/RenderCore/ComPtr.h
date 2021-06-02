// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <type_traits>
#include <comdef.h>

template <typename T>
class ComPtrRefBase
{
public:
    typedef typename T::InterfaceType InterfaceType;

    operator IUnknown** () const throw()
    {
        static_assert(__is_base_of(IUnknown, InterfaceType), "Invalid cast: InterfaceType does not derive from IUnknown");
        return reinterpret_cast<IUnknown**>(_ptr->ReleaseAndGetAddressOf());
    }

protected:
    T* _ptr;
};

template <typename T>
class ComPtrRef : public ComPtrRefBase<T>
{
    using Super = ComPtrRefBase<T>;
    using InterfaceType = typename Super::InterfaceType;
public:
    ComPtrRef(T* ptr) throw()
    {
        this->_ptr = ptr;
    }

    operator void** () const throw()
    {
        return reinterpret_cast<void**>(this->_ptr->ReleaseAndGetAddressOf());
    }

    operator T* () throw()
    {
        *this->_ptr = nullptr;
        return this->_ptr;
    }

    operator InterfaceType** () throw()
    {
        return this->_ptr->ReleaseAndGetAddressOf();
    }

    InterfaceType* operator *() throw()
    {
        return this->_ptr->Get();
    }

    InterfaceType* const* GetAddressOf() const throw()
    {
        return this->_ptr->GetAddressOf();
    }

    InterfaceType** ReleaseAndGetAddressOf() throw()
    {
        return this->_ptr->ReleaseAndGetAddressOf();
    }
};

template <typename T>
class ComPtr
{
public:
    typedef T InterfaceType;

protected:
    InterfaceType* _ptr;
    template<class U> friend class ComPtr;

    void InternalAddRef() const throw()
    {
        if (_ptr != nullptr)
        {
            _ptr->AddRef();
        }
    }

    unsigned long InternalRelease() throw()
    {
        unsigned long ref = 0;
        T* temp = _ptr;

        if (temp != nullptr)
        {
            _ptr = nullptr;
            ref = temp->Release();
        }

        return ref;
    }

public:
    ComPtr() throw() : _ptr(nullptr)
    {
    }

    ComPtr(decltype(__nullptr)) throw() : _ptr(nullptr)
    {
    }

    template<class U>
    ComPtr(U* other) throw() : _ptr(other)
    {
        InternalAddRef();
    }

    ComPtr(const ComPtr& other) throw() : _ptr(other._ptr)
    {
        InternalAddRef();
    }

    template<class U>
    ComPtr(const ComPtr<U>& other, typename std::enable_if_t<std::is_base_of_v<T, U>, void*>* = 0) throw() :
        _ptr(other._ptr)
    {
        InternalAddRef();
    }

    ComPtr(ComPtr&& other) throw() : _ptr(nullptr)
    {
        if (this != reinterpret_cast<ComPtr*>(&reinterpret_cast<unsigned char&>(other)))
        {
            Swap(other);
        }
    }

    template<class U>
    ComPtr(ComPtr<U>&& other, typename std::enable_if_t<std::is_base_of_v<T, U>, void*>* = 0) throw() :
        _ptr(other._ptr)
    {
        other._ptr = nullptr;
    }

    ~ComPtr() throw()
    {
        InternalRelease();
    }

    ComPtr& operator=(decltype(__nullptr)) throw()
    {
        InternalRelease();
        return *this;
    }

    ComPtr& operator=(T* other) throw()
    {
        if (_ptr != other)
        {
            ComPtr(other).Swap(*this);
        }
        return *this;
    }

    template <typename U>
    ComPtr& operator=(U* other) throw()
    {
        ComPtr(other).Swap(*this);
        return *this;
    }

    ComPtr& operator=(const ComPtr& other) throw()
    {
        if (_ptr != other._ptr)
        {
            ComPtr(other).Swap(*this);
        }
        return *this;
    }

    template<class U>
    ComPtr& operator=(const ComPtr<U>& other) throw()
    {
        ComPtr(other).Swap(*this);
        return *this;
    }

    ComPtr& operator=(ComPtr&& other) throw()
    {
        ComPtr(static_cast<ComPtr&&>(other)).Swap(*this);
        return *this;
    }

    template<class U>
    ComPtr& operator=(ComPtr<U>&& other) throw()
    {
        ComPtr(static_cast<ComPtr<U>&&>(other)).Swap(*this);
        return *this;
    }

    void Swap(ComPtr&& r) throw()
    {
        T* tmp = _ptr;
        _ptr = r._ptr;
        r._ptr = tmp;
    }

    void Swap(ComPtr& r) throw()
    {
        T* tmp = _ptr;
        _ptr = r._ptr;
        r._ptr = tmp;
    }

    operator bool() const throw()
    {
        return Get() != nullptr;
    }

    T* Get() const throw()
    {
        return _ptr;
    }

    InterfaceType* operator->() const throw()
    {
        return _ptr;
    }

    ComPtrRef<ComPtr<T>> operator&() throw()
    {
        return ComPtrRef<ComPtr<T>>(this);
    }

    const ComPtrRef<const ComPtr<T>> operator&() const throw()
    {
        return ComPtrRef<const ComPtr<T>>(this);
    }

    T* const* GetAddressOf() const throw()
    {
        return &_ptr;
    }

    T** GetAddressOf() throw()
    {
        return &_ptr;
    }

    T** ReleaseAndGetAddressOf() throw()
    {
        InternalRelease();
        return &_ptr;
    }

    T* Detach() throw()
    {
        T* ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    void Attach(InterfaceType* other) throw()
    {
        if (_ptr != nullptr)
        {
            _ptr->Release();
        }

        _ptr = other;
    }

    unsigned long Reset()
    {
        return InternalRelease();
    }

    // Previously, unsafe behavior could be triggered when 'this' is ComPtr<IInspectable> or ComPtr<IUnknown> and CopyTo is used to copy to another type U. 
    // The user will use operator& to convert the destination into a ComPtrRef, which can then implicit cast to IInspectable** and IUnknown**. 
    // If this overload of CopyTo is not present, it will implicitly cast to IInspectable or IUnknown and match CopyTo(InterfaceType**) instead.
    // A valid polymoprhic downcast requires run-time type checking via QueryInterface, so CopyTo(InterfaceType**) will break type safety.
    // This overload matches ComPtrRef before the implicit cast takes place, preventing the unsafe downcast.
    template <typename U>
    HRESULT CopyTo(ComPtrRef<ComPtr<U>> ptr) const throw()
    {
        return _ptr->QueryInterface(__uuidof(U), ptr);
    }

    HRESULT CopyTo(InterfaceType** ptr) const throw()
    {
        InternalAddRef();
        *ptr = _ptr;
        return 0;
    }

    HRESULT CopyTo(REFIID riid, void** ptr) const throw()
    {
        return _ptr->QueryInterface(riid, ptr);
    }

    template<typename U>
    HRESULT CopyTo(U** ptr) const throw()
    {
        return _ptr->QueryInterface(__uuidof(U), reinterpret_cast<void**>(ptr));
    }

    // query for U interface
    template<typename U>
    HRESULT As(ComPtrRef<ComPtr<U>> p) const throw()
    {
        return _ptr->QueryInterface(__uuidof(U), p);
    }

    // query for U interface
    template<typename U>
    HRESULT As(ComPtr<U>* p) const throw()
    {
        return _ptr->QueryInterface(__uuidof(U), reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
    }

    // query for riid interface and return as IUnknown
    HRESULT AsIID(REFIID riid, ComPtr<IUnknown>* p) const throw()
    {
        return _ptr->QueryInterface(riid, reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
    }
};

#define HR(LogCategory, x)                                              \
if (HRESULT hr = (x); FAILED(hr))                                       \
{                                                                       \
    _com_error com_error(hr);                                           \
    std::wstring msg = com_error.ErrorMessage();                        \
    LogSystem::Log(LogCategory, ELogVerbosity::Fatal, msg);             \
}