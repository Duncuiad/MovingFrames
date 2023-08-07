#pragma once

#include "Serializable.h"
#include "Serializer.h"

#include <type_traits>
#include <utility>
#include <vector>

template <typename ElemT, bool IsDynamic = false>
class Array : public Serializable
{
public:
    void Serialize(Serializer& aSerializer) override;
    int Count() const;
    ElemT& PushBack(const ElemT& anElement);
    ElemT& GetFirst();
    const ElemT& GetFirst() const;
    ElemT& GetLast();
    const ElemT& GetLast() const;
    const ElemT* GetBuffer() const;

    // Adapter types and methods
    using Iterator = std::vector<ElemT>::iterator;
    using ConstIterator = std::vector<ElemT>::const_iterator;

    template <class... ValT>
    decltype(auto) EmplaceBack(ValT&&... someVals);
    template <typename IterT>
    decltype(auto) Insert(IterT aPos, const ElemT& aVal);
    template <typename IterT>
    decltype(auto) Insert(IterT aPos, ElemT&& aVal);
    void RemoveAt(int anIndex);
    void RemoveAll();
    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    Iterator end() noexcept;
    ConstIterator end() const noexcept;
    decltype(auto) rbegin() noexcept;
    decltype(auto) rbegin() const noexcept;
    decltype(auto) rend() noexcept;
    decltype(auto) rend() const noexcept;
    ElemT& operator[](size_t idx);
    const ElemT& operator[](size_t idx) const;

private:
    std::vector<ElemT> myElements;
};

template <typename ElemT>
using ArrayDynamic = Array<ElemT, true>;

template <typename ElemT, bool IsDynamic>
inline void Array<ElemT, IsDynamic>::Serialize(Serializer& aSerializer)
{
    int count {static_cast<int>(myElements.size())};
    aSerializer.Process("myCount", count);
    myElements.resize(count);

    for (int i = 0; i < count; ++i)
    {
        if constexpr (IsDynamic)
        {
            SerializableDynamic* ptr {Unwrap(myElements[i])};
            aSerializer.Process(std::to_string(i).data(), ptr);
            if (Unwrap(myElements[i]) != ptr)
            {
                myElements[i] = Wrap<ElemT>(ptr);
            }
        }
        else
        {
            aSerializer.Process(std::to_string(i).data(), myElements[i]);
        }
    }
}

template <typename ElemT, bool IsDynamic>
inline int Array<ElemT, IsDynamic>::Count() const
{
    return static_cast<int>(myElements.size());
}

template <typename ElemT, bool IsDynamic>
inline ElemT& Array<ElemT, IsDynamic>::PushBack(const ElemT& anElement)
{
    myElements.push_back(anElement);
    return myElements.back();
}

template <typename ElemT, bool IsDynamic>
inline ElemT& Array<ElemT, IsDynamic>::GetFirst()
{
    return myElements.front();
}

template <typename ElemT, bool IsDynamic>
inline const ElemT& Array<ElemT, IsDynamic>::GetFirst() const
{
    return myElements.front();
}

template <typename ElemT, bool IsDynamic>
inline ElemT& Array<ElemT, IsDynamic>::GetLast()
{
    return myElements.back();
}

template <typename ElemT, bool IsDynamic>
inline const ElemT& Array<ElemT, IsDynamic>::GetLast() const
{
    return myElements.back();
}

template <typename ElemT, bool IsDynamic>
inline const ElemT* Array<ElemT, IsDynamic>::GetBuffer() const
{
    return myElements.data();
}

template <typename ElemT, bool IsDynamic>
template <class... ValT>
decltype(auto) Array<ElemT, IsDynamic>::EmplaceBack(ValT&&... someVals)
{
    myElements.emplace_back(std::forward<ValT>(someVals)...);
}

template <typename ElemT, bool IsDynamic>
template <typename IterT>
decltype(auto) Array<ElemT, IsDynamic>::Insert(IterT aPos, const ElemT& aVal)
{
    return myElements.insert(aPos, aVal);
}

template <typename ElemT, bool IsDynamic>
template <typename IterT>
decltype(auto) Array<ElemT, IsDynamic>::Insert(IterT aPos, ElemT&& aVal)
{
    return myElements.insert(aPos, std::forward<ElemT>(aVal));
}

template <typename ElemT, bool IsDynamic>
inline void Array<ElemT, IsDynamic>::RemoveAt(int anIndex)
{
    myElements.erase(begin() + anIndex);
}

template <typename ElemT, bool IsDynamic>
inline void Array<ElemT, IsDynamic>::RemoveAll()
{
    myElements.clear();
}

template <typename ElemT, bool IsDynamic>
inline ElemT& Array<ElemT, IsDynamic>::operator[](std::size_t idx)
{
    return myElements[idx];
}

template <typename ElemT, bool IsDynamic>
inline const ElemT& Array<ElemT, IsDynamic>::operator[](std::size_t idx) const
{
    return myElements[idx];
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::Iterator Array<ElemT, IsDynamic>::begin() noexcept
{
    return myElements.begin();
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::ConstIterator Array<ElemT, IsDynamic>::begin() const noexcept
{
    return myElements.begin();
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::Iterator Array<ElemT, IsDynamic>::end() noexcept
{
    return myElements.end();
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::ConstIterator Array<ElemT, IsDynamic>::end() const noexcept
{
    return myElements.end();
}

template <typename ElemT, bool IsDynamic>
inline decltype(auto) Array<ElemT, IsDynamic>::rbegin() noexcept
{
    return myElements.rbegin();
}

template <typename ElemT, bool IsDynamic>
inline decltype(auto) Array<ElemT, IsDynamic>::rbegin() const noexcept
{
    return myElements.rbegin();
}

template <typename ElemT, bool IsDynamic>
inline decltype(auto) Array<ElemT, IsDynamic>::rend() noexcept
{
    return myElements.rend();
}

template <typename ElemT, bool IsDynamic>
inline decltype(auto) Array<ElemT, IsDynamic>::rend() const noexcept
{
    return myElements.rend();
}
